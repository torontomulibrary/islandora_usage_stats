<?php

/**
 * @file
 * Class to deal with tracking item views on islandora site
 */
class islandora_usage_stats {

  function __construct($pid) {
    $this->pid = $pid;
  }

  /**
   * Performs check to see if user's IP is excluded from tracking, then checks
   * to see if user has requested to view the item in the last X seconds previous.
   * Does not make calls to increase the view count if IP is on exclusion list
   * or if user requested view of the item too recently
   */
  function add_page_count() {
    global $user;
    $result = '';
    
    $ip_list = explode(' ', variable_get('islandora_usage_stats_ip_list', 'localhost 127.0.0.1'));
    $ip = ip_address();
    
    if (!in_array($ip, $ip_list)) { // check if user's ip is excluded
    	
      $usersLastViewedTime = $this->get_last_view_time_for_ip();
      
      // user has never viewed item before
      if (!$usersLastViewedTime) { 
          $result = $this->increase_page_counter();
          $_SESSION['islandora_usage_stats'][ip_address()][$this->pid] = REQUEST_TIME;
      }
      else {
        $cooldown_time = variable_get('islandora_usage_stats_cooldown_seconds', 300); // default 5 min
        
        if ($usersLastViewedTime + ($cooldown_time) < REQUEST_TIME) {
          $result = $this->increase_page_counter();
          $_SESSION['islandora_usage_stats'][ip_address()][$this->pid] = REQUEST_TIME;
        }
      }
    }
    
    return $result;
  }
  
  /**
   * Increases the view count of an item by one
   * 
   * @return database result
   */
  function increase_page_counter() {
    global $user;
    $ip = ip_address();
    
    $result = db_insert('islandora_usage_stats_page_tracking')
                  ->fields(array(
                      'pid' => $this->pid,
                      'time' => REQUEST_TIME,
                      'ip' => $ip,
                      'user' => $user->name,
                  ))
                  ->execute();
    
    return $result;   
  }
  
  /**
   * Retrieves the number of times an item has been viewed from the database
   * and the last time it was viewed
   * 
   * @return array 
   */
  function read_page_count() {
      $result = db_query("SELECT count(*) as count, max(time) as time from {islandora_usage_stats_page_tracking} WHERE pid = :pid", array(':pid' => $this->pid));
      $record = $result->fetch();
      $array['count'] = $record->count;
      $array['time']  = $record->time;
      return $array;
  }

  /**
   * Queries the database to get the last time a user accessed this item before
   * @return NULL|string seconds since the unix epoch
   */
  function get_last_view_time_for_ip() {
  	global $user;
  	$ip = ip_address();
  	$result = db_query("SELECT max(time) from {islandora_usage_stats_page_tracking} WHERE pid = :pid AND ip = :ip", 
  			array(':pid' => $this->pid,
  						':ip' => $ip,
  			)
  	);
  	$lastViewTime = $result->fetchField(0);
  	return $lastViewTime;
  }
  
  /**
   * CURRENTLY UNUSED 
   * 
   * Uses session variables to check if user requested this item before
   * @return NULL|string seconds since the unix epoch
   */
  function get_last_view_time_for_visitor() {
    global $user;
    $time = @$_SESSION['islandora_usage_stats'][ip_address()][$this->pid];
    if (!$time) { // user has NOT requested item in this session
        return NULL;
    }
    return $time;
  }
  
  /**
   * Increases the datastream download count by one in the database.
   * Count is not increased if user's IP is on exclusion list.
   * 
   * @param string $dsid the datastream id
   * @return NULL|string database result
   */
  function add_ds_count($dsid) {
    global $user;
    $ip_list = explode(' ', variable_get('islandora_usage_stats_ip_list', 'localhost 127.0.0.1'));
    $ip = $_SERVER['REMOTE_ADDR'];
    if (!in_array($ip, $ip_list)) {
      $result = db_insert('islandora_usage_stats_ds_tracking')
                  ->fields(array(
                  		'pid' => $this->pid,
                  		'dsid' => $dsid,
                  		'time' => REQUEST_TIME,
                      'ip'   => $ip,
                  		'user' => $user->name,
                  ))
                  ->execute();
    }
    return $result;
  }

  /**
   * Retrieves the number of times a datastream has been downloaded
   * and the time it was last downloaded.
   * 
   * @param string $dsid the datastream id
   * @return array
   */
  function read_ds_count($dsid) {
    $result = db_query("SELECT count(*) as count, max(time) as time from {islandora_usage_stats_ds_tracking} WHERE pid = :pid and dsid = :dsid",
        array(':pid' => $this->pid,
              ':dsid' => $dsid,
        )
    );
    $record = $result->fetch();
    $array['count'] = $record->count;
    $array['time']  = $record->time;
    return $array;
  }

}
