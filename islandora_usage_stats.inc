<?php

/**
 * @file
 * Class to deal with tracking item views on islandora site
 */
class islandora_usage_stats {

  function __construct($pid) {
    $this->pid = $pid;
  }

  function add_page_count() {
    global $user;
    $ip_list = explode(' ', variable_get('islandora_usage_stats_ip_list', 'localhost 127.0.0.1'));
    $ip = $_SERVER['REMOTE_ADDR'];
    if (!in_array($ip, $ip_list)) {
      $result = db_insert('islandora_usage_stats_page_tracking')
      						->fields(array(
      							'pid' => $this->pid,
      						  'time' => REQUEST_TIME,
      							'user' => $user->name,
      						))
      						->execute();
    }
    return $result;
  }

  function read_page_count() {
    $result = db_query("SELECT time FROM {islandora_usage_stats_page_tracking} WHERE pid = :pid", array(':pid' => $this->pid));
		foreach ($result as $record) {
      if ($record->time == '') {
        $array[] = 'Never';
      }
      else {
        $array[] = date('H:i:s Y-m-d', $record->time);
      }
    }
    $array['count'] = count($array);
    return $array;
  }

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
						      		'user' => $user->name,
						      ))
						      ->execute();
    }
    return $result;
  }

  function read_ds_count($dsid) {
    $result = db_query("SELECT time FROM {islandora_usage_stats_ds_tracking} WHERE pid = :pid AND dsid = :dsid",
    		array(':pid' => $this->pid, 
    					':dsid' => $dsid,
    		)
    );
			foreach ($result as $record) {
      if ($record->time == '') {
        $array[] = 'Never';
      }
      else {
        $array[] = date('H:i:s Y-m-d', $record->time);
      }
    }
    $array['count'] = count($array);
    return $array;
  }

}
