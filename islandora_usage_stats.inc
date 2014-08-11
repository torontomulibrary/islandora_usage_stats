<?php

/**
 * @file
 * Class to deal with tracking item views on islandora site
 */
class IslandoraUsageStats
{

    function __construct($pid)
    {
        $this->pid = $pid;
    }

    /**
     * Performs check to see if user's IP is excluded from tracking
     * Also checks if user is a bot (if enabled)
     *
     * @return boolean
     */
    function IpExcluded()
    {
        $ip_list = explode(' ', variable_get('islandora_usage_stats_ip_list', 'localhost 127.0.0.1'));
        $ip = ip_address();
        
        $bot_exclude = variable_get('islandora_usage_stats_exclude_bots', 1);
        
        if (in_array($ip, $ip_list)) { // check if user's ip is excluded
            return TRUE;
        }
        else if ($bot_exclude) {
            // bots check here
            $user_agent = $_SERVER['HTTP_USER_AGENT'];
            if (1 === preg_match('/bot|rambler|spider|crawl|slurp|curl|^$/i', $user_agent)) {
                return TRUE;
            }
        }
        
        return FALSE;
    }

    /**
     * Retrieves the number of times an item has been viewed from the database
     * and the last time it was viewed
     *
     * @return array
     */
    function getViews()
    {
        $result = db_query("SELECT views, time from {islandora_usage_stats_object_views} WHERE pid = :pid", array(
            ':pid' => $this->pid
        ));
        $record = $result->fetchAssoc();
        if (! $record) {
            return array(
                'views' => 0,
                'time' => ''
            );
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
    function getViewCount()
    {
        $result = db_query("SELECT views, time from {islandora_usage_stats_object_views} WHERE pid = :pid", array(
            ':pid' => $this->pid
        ));
        $record = $result->fetchAssoc();
        if (! $record) {
            return array(
                'views' => 0,
                'time' => ''
            );
        }
        
        // update the last viewed time, don't store it
        /**
         * **********************
         */
        $this->setLastViewedTime();
        /**
         * **********************
         */
        
        return $record;
    }

    /**
     * Increases the datastream download count by one in the database.
     * Count is not increased if user's IP is on exclusion list.
     *
     * @param string $dsid
     *            the datastream id
     * @return NULL string result
     */
    function addDownload($dsid)
    {
        $result = '';
        
        $logging = variable_get('islandora_usage_stats_use_access_log', 1);
        
        if (! $this->IpExcluded()) { // check if user's ip is excluded
            $result = $this->increaseObjectDSDownloadCounter($dsid);
            
            if ($logging) {
                $this->logObjectDownload($dsid);
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
    function getDownloads($dsid)
    {
        $result = db_query("SELECT downloads, time from {islandora_usage_stats_object_ds_downloads} WHERE pid = :pid AND dsid = :dsid", array(
            ':pid' => $this->pid,
            ':dsid' => $dsid
        ));
        $record = $result->fetchAssoc();
        
        if ($record) {
            return $record;
        } else {
            return array(
                'downloads' => 0,
                'time' => NULL
            );
        }
    }

    /**
     * Increases the download count for the given object's datastream ID
     *
     * @param string $dsid
     *            datastream id
     * @return the number of total downloads
     */
    function increaseObjectDSDownloadCounter($dsid)
    {
        $current_info = $this->getDownloads($dsid);
        
        if ($current_info['downloads']) {
            // increase view count by one
            $result = db_update('islandora_usage_stats_object_ds_downloads')->expression('downloads', 'downloads + :downloads', array(
                ':downloads' => 1
            ))
                ->expression('time', REQUEST_TIME)
                ->condition('pid', $this->pid)
                ->condition('dsid', $dsid)
                ->execute();
            
            return $this->getDownloads($dsid);
        } else {
            $result = db_insert('islandora_usage_stats_object_ds_downloads')->fields(array(
                'pid' => $this->pid,
                'dsid' => $dsid,
                'downloads' => 1,
                'time' => REQUEST_TIME
            ))->execute();
            
            return array(
                'downloads' => 1,
                'time' => NULL
            );
        }
    }

    /**
     * Adds an entry into the log table for an access of a fedora object
     *
     * @return unknown
     */
    function logObjectView()
    {
        global $user;
        $ip = ip_address();
        
        $result = db_insert('islandora_usage_stats_object_access_log')->fields(array(
            'pid' => $this->pid,
            'time' => REQUEST_TIME,
            'ip' => $ip,
            'user' => ($user == NULL) ? $user->name : 'Anonymous'
        ))->execute();
        
        return $result;
    }

    /**
     * Add an entry into the log table for a download of a fedora object datastream
     *
     * @return unknown
     */
    function logObjectDownload($dsid)
    {
        global $user;
        $ip = ip_address();
        
        $result = db_insert('islandora_usage_stats_object_ds_access_log')->fields(array(
            'pid' => $this->pid,
            'time' => REQUEST_TIME,
            'ip' => $ip,
            'dsid' => $dsid,
            'user' => ($user == NULL) ? $user->name : 'Anonymous'
        ))->execute();
        
        return $result;
    }

    /**
     * Queries the database access logs to get the last time an ip accessed this item
     *
     * @return NULL string since the unix epoch
     */
    function getLastViewTimeForIp()
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
    function getLastDownloadTimeForIp($dsid)
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
    function getLastViewTimeForVisitor()
    {
        $time = @$_SESSION['islandora_usage_stats'][ip_address()][$this->pid];
        if (! $time) { // user has NOT requested item in this session
            return NULL;
        }
        return $time;
    }

    function updateLastViewTimeForVisitor()
    {
        $_SESSION['islandora_usage_stats'][ip_address()][$this->pid] = REQUEST_TIME;
    }

    function getLastViewedTime()
    {
        $result = db_query("SELECT time from {islandora_usage_stats_object_views} WHERE pid = :pid", array(
            ':pid' => $this->pid
        ));
        
        $record = $result->fetch();
        
        if (! $record) {
            return NULL;
        } else {
            return $record->time;
        }
    }

    /**
     * Performs a check to see if the user's IP is on the exclusion list, and
     * a check to determine the last time they requested to view the object.
     * If the last time the user requested the object falls within the cooldown period,
     * nothing is updated. Otherwise, this function calls dBSetLastViewedTime
     * and updates the time that the user requested the item
     *
     * @return Ambigous <multitype:, multitype:string, multitype:string number >
     */
    function setLastViewedTime()
    {
        $result = array();
        
        $logging = variable_get('islandora_usage_stats_use_access_log', 1);
        
        if (! $this->IpExcluded()) { // check if user's ip is excluded
            
            $lastViewedTime = $this->getLastViewTimeForVisitor();
            
            // user has never viewed item before
            if (! $lastViewedTime) {
                $result = $this->dBSetLastViewedTime();
                $this->updateLastViewTimeForVisitor();
            } else {
                $cooldown_time = variable_get('islandora_usage_stats_cooldown_seconds', 300); // default 5 min
                
                if ($lastViewedTime + ($cooldown_time) < REQUEST_TIME) {
                    $result = $this->dBSetLastViewedTime();
                    $this->updateLastViewTimeForVisitor();
                }
            }
        }
        
        return $result;
    }

    /**
     * Sets the last accessed time of an object in the database if it exists,
     * otherwise creaets a new entry for it view a view count of 1
     * and access time of 0
     *
     * @return multitype:string number
     */
    function dBSetLastViewedTime()
    {
        $result = array();
        
        $current_views = $this->getViews();
        
        if ($current_views['views']) {
            // set last viewed time to now
            $update_result = db_update('islandora_usage_stats_object_views')->expression('time', REQUEST_TIME)
                ->condition('pid', $this->pid)
                ->execute();
            
            $result['timelastviewed'] = REQUEST_TIME;
        } else {
            // insert a new row into the DB for this object
            $insert_result = db_insert('islandora_usage_stats_object_views')->fields(array(
                'pid' => $this->pid,
                'views' => 1,
                'time' => 0
            ))->execute();
            
            $result['timelastviewed'] = 0;
        }
        
        return $result;
    }

    /**
     * Increase ONLY the view count by 1 for the given
     * IslandoraUsageStats object.
     * If the object does not have an
     * entry in the database already, it creates a new one
     *
     * @return number
     */
    function dBIncreaseViewCounter()
    {
        // $result = 0;
        $current_views = $this->getViews();
        
        if ($current_views['views']) {
            // increase view count by one
            $update_result = db_update('islandora_usage_stats_object_views')->expression('views', 'views + :num', array(
                ':num' => 1
            ))
                ->condition('pid', $this->pid)
                ->execute();
            
            $result = $current_views['views'] + 1;
        } else {
            $insert_result = db_insert('islandora_usage_stats_object_views')->fields(array(
                'pid' => $this->pid,
                'views' => 1,
                'time' => 0
            ))->execute();
            
            $result = 1;
        }
        
        return $result;
    }

    /**
     * If a user's IP address is not excluded and a user has not viewed this
     * particular item in the last X seconds (cooldown time), then this function
     * increases an item's view count by 1 and logs the access, if logging is enabled
     *
     * @return Ambigous <number, string>
     */
    function plusOneView()
    {
        $currentViews = $this->getViews();
        $result = $currentViews['views'];
        
        $logging = variable_get('islandora_usage_stats_use_access_log', 1);
        
        if (! $this->IpExcluded()) { // check if user's ip is excluded
            
            $lastViewedTime = $this->getLastViewTimeForVisitor();
            
            // user has never viewed item before
            if (! $lastViewedTime) {
                $result = $this->dBIncreaseViewCounter();
                if ($logging) {
                    $this->logObjectView();
                }
            } else {
                // check if user has requested this item in the past X seconds
                $cooldown_time = variable_get('islandora_usage_stats_cooldown_seconds', 300); // default 5 min
                
                if (($lastViewedTime + $cooldown_time) < REQUEST_TIME) {
                    $result = $this->dBIncreaseViewCounter();
                    if ($logging) {
                        $this->logObjectView();
                    }
                }
            }
        }
        
        return $result;
    }
}
