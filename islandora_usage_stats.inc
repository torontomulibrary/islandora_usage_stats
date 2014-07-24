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
        $result = '';
        
        $logging = variable_get('islandora_usage_stats_use_access_log', 1);
        
        if (! $this->ip_excluded()) { // check if user's ip is excluded
                                     
            $lastViewedTime = $this->get_last_view_time_for_visitor();
            
            // user has never viewed item before
            if (! $lastViewedTime) {
                $timelastviewed = $this->get_last_viewed_time();
                $result = $this->increase_object_view_counter();
                $result['timelastviewed'] = $timelastviewed;
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
     * Retrieves the number of times an item has been viewed from the database
     * and the last time it was viewed, BUT ALSO UPDATES THE LAST VIEWED TIME
     * AFTER THE RESULTS ARE RETURNED
     *
     * @return array
     */
    function get_view_count()
    {
        $result = db_query("SELECT views, time from {islandora_usage_stats_object_views} WHERE pid = :pid", array(
            ':pid' => $this->pid
        ));
        $record = $result->fetchAssoc();
        if (!$record) {
            return array('views' => 0, 'time' => '');
        }
        
        // update the last viewed time, don't store it
        /*************************/
        $this->set_last_viewed_time();
        /*************************/
        
        return $record;
    }

    /**
     * Increases the view count of an item and the time last viewed
     *
     * @return number of views
     */
    function increase_object_view_counter()
    {      
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

    
    function increase_object_view_counter_only()
    {
        $result = NULL;
        
        $logging = variable_get('islandora_usage_stats_use_access_log', 1);
        
        if (! $this->ip_excluded()) { // check if user's ip is excluded
             
            $lastViewedTime = $this->get_last_view_time_for_visitor();
        
            // user has never viewed item before
            if (! $lastViewedTime) {
                $result = $this->db_increase_view_counter();
            } else {
                $cooldown_time = variable_get('islandora_usage_stats_cooldown_seconds', 300); // default 5 min
        
                if ($lastViewedTime + ($cooldown_time) < REQUEST_TIME) {
//                     $result = $this->increase_object_view_counter();
                    $result = $this->db_increase_view_counter();
                }
            }
        }
        
        return $result;
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
            'user' => ($user==NULL) ? $user->name : 'Anonymous'
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
            'user' => ($user==NULL) ? $user->name : 'Anonymous'
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
    
    function get_last_viewed_time()
    {
        $result = db_query("SELECT time from {islandora_usage_stats_object_views} WHERE pid = :pid", array(
        	':pid' => $this->pid,
        ));
        
        $record = $result->fetch();
        if (!$record) {
            return NULL;
        }
        else {
            return $record->time;
        }
    }
    
    function set_last_viewed_time()
    {   
        $result = array();
        
        $logging = variable_get('islandora_usage_stats_use_access_log', 1);
        
        if (! $this->ip_excluded()) { // check if user's ip is excluded
             
            $lastViewedTime = $this->get_last_view_time_for_visitor();
        
            // user has never viewed item before
            if (! $lastViewedTime) {
                
                $result = $this->db_set_last_viewed_time($this->pid);
                $this->update_last_view_time_for_visitor();
        
//                 if ($logging) {
//                     $this->log_object_view();
//                 }
            } else {
                $cooldown_time = variable_get('islandora_usage_stats_cooldown_seconds', 300); // default 5 min
        
                if ($lastViewedTime + ($cooldown_time) < REQUEST_TIME) {
                    
//                     $result = $this->increase_object_view_counter();

                    $result = $this->db_set_last_viewed_time($this->pid);
                    $this->update_last_view_time_for_visitor();
        
//                     if ($logging) {
//                         $this->log_object_view();
//                     }
                }
            }
        }
        
        return $result;
    }
    
    function db_set_last_viewed_time($pid) {
        $result = array();
        
        $current_views = $this->get_views();
        
        if ($current_views['views']) {
            // set last viewed time to now
            $update_result = db_update('islandora_usage_stats_object_views')
            ->expression('time', REQUEST_TIME)
            ->condition('pid', $pid)
            ->execute();
        
            $result['timelastviewed'] = REQUEST_TIME;
        
        } else {
            // insert a new row into the DB for this object
            $insert_result = db_insert('islandora_usage_stats_object_views')->fields(array(
                'pid' => $pid,
                'views' => 1,
                'time' => 0,
            ))->execute();
        
            $result['timelastviewed'] = 0;
        }
        
        return $result;
    }
    
    /**
     * This function increases ONLY the view count by 1 for the given
     * islandora_usage_stats object. If the object does not have an
     * entry in the database already, it creates a new one
     * 
     * @return number
     */
    function db_increase_view_counter() {
//         $result = 0;
        
        $current_views = $this->get_views();
        
        if ($current_views['views']) {
            // increase view count by one
            $update_result = db_update('islandora_usage_stats_object_views')
                ->expression('views', 'views + :num', array(':num' => 1))
                ->condition('pid', $this->pid)
                ->execute();
        
            $result = $current_views['views'] + 1;
        
        } else {
            $insert_result = db_insert('islandora_usage_stats_object_views')->fields(array(
                'pid' => $this->pid,
                'views' => 1,
                'time' => 0,
            ))->execute();
        
            $result = 1;
        }
        
        return $result;
    }
    
    function getLastTimeViewed() {
        $result = db_query("SELECT time from {islandora_usage_stats_object_views} WHERE pid = :pid", array(
            ':pid' => $this->pid,
        ));
        
        $record = $result->fetch();
        
        if (!$record) {
            return NULL;
        }
        else {
            return $record->time;
        }
    }
    
    function plusOneView() {
        $currentViews = $this->get_views();
        $result = $currentViews['views'];
        
        $logging = variable_get('islandora_usage_stats_use_access_log', 1);
        
        if (! $this->ip_excluded()) { // check if user's ip is excluded
             
            $lastViewedTime = $this->get_last_view_time_for_visitor();
        
            // user has never viewed item before
            if (! $lastViewedTime) {
                $result = $this->db_increase_view_counter();
                if ($logging) {
                    $this->log_object_view();
                }
            } 
            else {
                // check if user has requested this item in the past X seconds
                $cooldown_time = variable_get('islandora_usage_stats_cooldown_seconds', 300); // default 5 min
        
                if (($lastViewedTime + $cooldown_time) < REQUEST_TIME) {
                    $result = $this->db_increase_view_counter();
                    if ($logging) {
                        $this->log_object_view();
                    }
                }
            }
        }
        
        return $result;
    }
    
    function updateTimeLastViewed() {
   
        $update_result = db_update('islandora_usage_stats_object_views')
            ->expression('time', REQUEST_TIME)
            ->condition('pid', $this->pid)
            ->execute();
    
        return REQUEST_TIME;
    }
}
