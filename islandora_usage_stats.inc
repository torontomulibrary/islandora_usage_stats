<?php

/**
 * @file
 * Class to deal with tracking item views on islandora site
 */
class islandora_usage_stats
{

    function __construct($pid)
    {
        $this->pid = $pid;
    }

    /**
     * Performs check to see if user's IP is excluded from tracking
     * 
     * @return boolean
     */
    function ip_excluded()
    {
        $ip_list = explode(' ', variable_get('islandora_usage_stats_ip_list', 'localhost 127.0.0.1'));
        $ip = ip_address();
        
        if (in_array($ip, $ip_list)) { // check if user's ip is excluded
            return TRUE;
        }
        
        return FALSE;
    }

    /**
     * Checks to see if user has requested to view the item in the last X seconds previous.
     * Does not make calls to increase the view count if IP is on exclusion list
     * or if user requested view of the item too recently
     */
    function add_view()
    {
        global $user;
        $result = '';
        
        $logging = variable_get('islandora_usage_stats_use_access_log', 1);
        
        if (! $this->ip_excluded()) { // check if user's ip is excluded
                                     
            $lastViewedTime = $this->get_last_view_time_for_visitor();
            
            // user has never viewed item before
            if (! $lastViewedTime) {
                $result = $this->increase_object_view_counter();
                $this->update_last_view_time_for_visitor();
                
                if ($logging) {
                    $this->log_object_view();
                }
            } else {
                $cooldown_time = variable_get('islandora_usage_stats_cooldown_seconds', 300); // default 5 min
                
                if ($lastViewedTime + ($cooldown_time) < REQUEST_TIME) {
                    $result = $this->increase_object_view_counter();
                    $this->update_last_view_time_for_visitor();
                    
                    if ($logging) {
                        $this->log_object_view();
                    }
                }
            }
        }
        
        return $result;
    }

    /**
     * Retrieves the number of times an item has been viewed from the database
     * and the last time it was viewed
     *
     * @return array
     */
    function get_views()
    {
        $result = db_query("SELECT views, time from {islandora_usage_stats_object_views} WHERE pid = :pid", array(
            ':pid' => $this->pid
        ));
        $record = $result->fetchAssoc();
        if (!$record) {
            return array('views' => 0, 'time' => '');
        }
        return $record;
    }

    /**
     * Increases the view count of an item by one
     *
     * @return number of views
     */
    function increase_object_view_counter()
    {
        global $user;
        
        $current_views = $this->get_views();
        
        if ($current_views['views']) {
            // increase view count by one
            $result = db_update('islandora_usage_stats_object_views')
                ->expression('views', 'views + :num', array(':num' => 1))
                ->expression('time', REQUEST_TIME)
                ->condition('pid', $this->pid)
                ->execute();
            
            return $current_views['views'] + 1;
            
        } else {
            $result = db_insert('islandora_usage_stats_object_views')->fields(array(
                'pid' => $this->pid,
                'views' => 1,
                'time' => REQUEST_TIME,
            ))->execute();
            
            return 1;
        }
    }

    /**
     * Increases the datastream download count by one in the database.
     * Count is not increased if user's IP is on exclusion list.
     *
     * @param string $dsid
     *            the datastream id
     * @return NULL string result
     */
    function add_download($dsid)
    {
        global $user;
        $result = '';
       
        $logging = variable_get('islandora_usage_stats_use_access_log', 1);
        
        if (! $this->ip_excluded()) { // check if user's ip is excluded
            $result = $this->increase_object_ds_download_counter($dsid);
            
            if ($logging) {
                $this->log_object_download($dsid);
            }
        }
        
        return $result;
    }

    /**
     * Retrieves the number of times the object's datastream has been downloaded
     *
     * @param string $dsid
     *            the datastream id
     * @return array
     */
    function get_downloads($dsid)
    {
        $result = db_query("SELECT downloads, time from {islandora_usage_stats_object_ds_downloads} WHERE pid = :pid AND dsid = :dsid", array(
            ':pid' => $this->pid,
            ':dsid' => $dsid
        ));
       $record = $result->fetchAssoc();
              
       if ($record) {
           return $record;
       } else {
           return array('downloads' => 0, 'time' => NULL);
       }
    }

    /**
     * Increases the download count for the given object's datastream ID
     * 
     * @param string $dsid
     *            datastream id
     * @return the number of total downloads
     */
    function increase_object_ds_download_counter($dsid)
    {
        global $user;

        $current_info = $this->get_downloads($dsid);
        
        if ($current_info['downloads']) {
            // increase view count by one
            $result = db_update('islandora_usage_stats_object_ds_downloads')
            ->expression('downloads', 'downloads + :downloads', array(':downloads' => 1))
            ->expression('time', REQUEST_TIME)
            ->condition('pid', $this->pid)
            ->condition('dsid', $dsid)
            ->execute();
            
            return $this->get_downloads($dsid);
        } else {
            $result = db_insert('islandora_usage_stats_object_ds_downloads')->fields(array(
                'pid' => $this->pid,
                'dsid' => $dsid,
                'downloads' => 1,
                'time' => REQUEST_TIME
            ))->execute();
        
            return array('downloads' => 1, 'time' => NULL);
        }
    }

    /**
     * Adds an entry into the log table for an access of a fedora object
     *
     * @return unknown
     */
    function log_object_view()
    {
        global $user;
        $ip = ip_address();
        
        $result = db_insert('islandora_usage_stats_object_access_log')->fields(array(
            'pid' => $this->pid,
            'time' => REQUEST_TIME,
            'ip' => $ip,
            'user' => $user->name
        ))->execute();
        
        return $result;
    }

    /**
     * Add an entry into the log table for a download of a fedora object datastream
     *
     * @return unknown
     */
    function log_object_download($dsid)
    {
        global $user;
        $ip = ip_address();
        
        $result = db_insert('islandora_usage_stats_object_ds_access_log')->fields(array(
            'pid' => $this->pid,
            'time' => REQUEST_TIME,
            'ip' => $ip,
            'dsid' => $dsid,
            'user' => $user->name
        ))->execute();
        
        return $result;
    }

    /**
     * Queries the database access logs to get the last time an ip accessed this item
     *
     * @return NULL string since the unix epoch
     */
    function get_last_view_time_for_ip()
    {
        global $user;
        $ip = ip_address();
        $result = db_query("SELECT max(time) from {islandora_usage_stats_object_access_log} WHERE pid = :pid AND ip = :ip", array(
            ':pid' => $this->pid,
            ':ip' => $ip
        ));
        $lastViewTime = $result->fetchField(0);
        return $lastViewTime;
    }
    
    /**
     * Queries the datastream access log to get the last time an ip downloaded the given datastream ID
     * 
     * @param string $dsid
     * @return string time in seconds since epoch
     */
    function get_last_download_time_for_ip($dsid)
    {
        global $user;
        $ip = ip_address();
        $result = db_query("SELECT max(time) as time from {islandora_usage_stats_object_ds_access_log} WHERE pid = :pid AND dsid = :dsid AND ip = :ip", array(
            ':pid' => $this->pid,
            ':dsid' => $dsid,
            ':ip' => $ip
        ));
        $record = $result->fetch();
        return $record->time;
    }

    /**
     * Uses session variables to check if user requested this item before
     *
     * @return NULL string since the unix epoch
     */
    function get_last_view_time_for_visitor()
    {
        global $user;
        $time = @$_SESSION['islandora_usage_stats'][ip_address()][$this->pid];
        if (! $time) { // user has NOT requested item in this session
            return NULL;
        }
        return $time;
    }
    
    function update_last_view_time_for_visitor()
    {
        $_SESSION['islandora_usage_stats'][ip_address()][$this->pid] = REQUEST_TIME;
    }
}
