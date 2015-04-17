<?php

/**
 * @file
 * Class file for IslandoraUsageStats.
 */

/**
 * Class to deal with tracking item views on an Islandora site.
 */
class IslandoraUsageStats {

  /**
   * Constructor, populates PID and PID ID.
   */
  public function __construct($pid) {
    module_load_include('inc', 'islandora_usage_stats', 'includes/db');
    $this->pid = $pid;
    $this->pid_id = islandora_usage_stats_get_pid_id($pid);
  }

  /**
   * Performs check to see if user's IP is excluded from tracking.
   *
   * Also checks if user is a bot if enabled.
   *
   * @return bool
   *   TRUE if excluded from tracking, FALSE otherwise.
   */
  public function IpExcluded() {
    $ip_list = explode(' ', variable_get('islandora_usage_stats_ip_list', 'localhost 127.0.0.1'));
    $ip = ip_address();

    $bot_exclude = variable_get('islandora_usage_stats_exclude_bots', 1);
    // Check if user's IP is excluded.
    if (in_array($ip, $ip_list)) {
      return TRUE;
    }
    elseif ($bot_exclude) {
      // Bots check here.
      $user_agent = $_SERVER['HTTP_USER_AGENT'];
      if (1 === preg_match('/bot|rambler|spider|crawl|slurp|curl|^$/i', $user_agent)) {
        return TRUE;
      }
    }

    return FALSE;
  }

  /**
   * Retrieves the number of times and last time an item was viewed.
   *
   * @return array
   *   Associative array with members views and time.
   */
  public function getViews() {
    $result = db_select('islandora_usage_stats_object_views', 'object_views')
      ->fields('object_views')
      ->condition('pid_id', $this->pid_id)
      ->execute();
    $record = $result->fetchAssoc();
    if (!$record) {
      return array(
        'views' => 0,
        'time' => '',
      );
    }
    return $record;
  }

  /**
   * Retrieve and update the number and last time an object was viewed.
   *
   * Retrieves the number of times an item has been viewed from the database
   * and the last time it was viewed, BUT ALSO UPDATES THE LAST VIEWED TIME
   * AFTER THE RESULTS ARE RETURNED
   */
  public function getViewCount() {
    $result = db_query("SELECT views, time from {islandora_usage_stats_object_views} WHERE pid_id = :pid_id", array(
        ':pid_id' => $this->pid_id,
    ));
    $record = $result->fetchAssoc();
    if (!$record) {
      return array(
        'views' => 0,
        'time' => '',
      );
    }

    // Update the last viewed time, don't store it.
    $this->setLastViewedTime();

    return $record;
  }

  /**
   * Increases the datastream download count by one in the database.
   *
   * Count is not increased if user's IP is on exclusion list.
   */
  public function addDownload($dsid) {
    $result = '';

    $logging = variable_get('islandora_usage_stats_use_access_log', 1);
    // Check if user's IP is excluded.
    if (!$this->IpExcluded()) {
      $result = $this->increaseObjectDSDownloadCounter($dsid);

      if ($logging) {
        $this->logObjectDownload($dsid);
      }
    }

    return $result;
  }

  /**
   * Increases the top datastream download count by one in the database.
   *
   * Count is not increased if user's IP is on exclusion list.
   *
   * @param string $dsid
   *   The datastream id.
   *
   * @return string
   *   Results of the query.
   */
  public function addTopDownload($dsid) {
    $result = '';
    $logging = variable_get('islandora_usage_stats_use_access_log', 1);

    // Check if user's ip is excluded.
    if (!$this->IpExcluded()) {
      $last_viewed_time = $this->getLastViewTimeForVisitor();
      // User has never viewed item before.
      if (!$last_viewed_time) {
        $this->dBIncreaseViewCounter();
        if ($logging) {
          $this->logObjectDownload($dsid);
        }
      }
      else {
        // Check if user has requested this item in the past X seconds.
        $cooldown_time = variable_get('islandora_usage_stats_cooldown_seconds', 300);
        if (($last_viewed_time + $cooldown_time) < REQUEST_TIME) {
          $result = $this->increaseTopObjectDSDownloadCounter($dsid);
          if ($logging) {
            $this->logObjectDownload($dsid);
            $this->updateLastViewTimeForVisitor();
          }
        }
      }
    }

    return $result;
  }

  /**
   * The number of times the object's datastream has been downloaded.
   */
  public function getDownloads($dsid) {
    $result = db_query("SELECT downloads, time from {islandora_usage_stats_object_ds_downloads} WHERE pid_id = :pid_id AND dsid = :dsid", array(
        ':pid_id' => $this->pid_id,
        ':dsid' => $dsid,
    ));
    $record = $result->fetchAssoc();

    if ($record) {
      return $record;
    }
    else {
      return array(
        'downloads' => 0,
        'time' => NULL,
      );
    }
  }

  /**
   * Retrieves the number of times any object datastream has been downloaded.
   *
   * @param string $dsid
   *   The datastream id.
   *
   * @return array
   *   Records affected by top downloads.
   */
  private function getTopDownloads($dsid) {
    module_load_include('inc', 'islandora_usage_stats', 'includes/db');

    $object = islandora_object_load($this->pid);
    $record = islandora_usage_stats_find_existing_downloads($this->pid_id, $dsid, $object[$dsid]->label);

    if ($record['result']) {
      return $record;
    }
    else {
      return array(
        'downloads' => 0,
        'time' => NULL,
        'ds_id' => $record['ds_id'],
        'result' => NULL,
      );
    }
  }

  /**
   * Increases the top download count for the given object's datastream ID.
   *
   * @param string $dsid
   *   The datastream id.
   *
   * @return array
   *   The number of total downloads
   */
  private function increaseTopObjectDSDownloadCounter($dsid) {
    module_load_include('inc', 'islandora_usage_stats', 'includes/db');
    $current_info = $this->getTopDownloads($dsid);
    if ($current_info['result']) {
      islandora_usage_stats_increment_top_object_ds_download_counter(
        REQUEST_TIME,
        $this->pid_id,
        $current_info['result']['ds_id']
      );
      return $this->getTopDownloads($dsid);
    }
    else {
      islandora_usage_stats_insert_top_download(REQUEST_TIME, $this->pid_id, $current_info['ds_id']);
      return array(
        'downloads' => 1,
        'time' => NULL,
        'ds_id' => $current_info['ds_id'],
      );
    }
    return NULL;
  }

  /**
   * Increases the download count for the given object's datastream ID.
   *
   * @param string $dsid
   *   The datastream id.
   *
   * @return int
   *   The number of total downloads
   */
  public function increaseObjectDSDownloadCounter($dsid) {
    $current_info = $this->getDownloads($dsid);
    if ($current_info['downloads']) {
      // Increase view count by one.
      $result = db_update('islandora_usage_stats_object_ds_downloads')
      ->expression('downloads', 'downloads + :downloads',
        array(
          ':downloads' => 1,
        )
      )
      ->expression('time', REQUEST_TIME)
      ->condition('pid_id', $this->pid_id)
      ->condition('dsid', $dsid)
      ->execute();
      return $this->getDownloads($dsid);
    }
    else {
      $result = db_insert('islandora_usage_stats_object_ds_downloads')
      ->fields(
        array(
          'pid_id' => $this->pid_id,
          'dsid' => $dsid,
          'downloads' => 1,
          'time' => REQUEST_TIME,
        )
      )
      ->execute();
      return array(
        'downloads' => 1,
        'time' => NULL,
      );
    }
  }

  /**
   * Log a view of a Fedora object.
   *
   * @global user
   *
   * @todo look into batching the DB updates.
   */
  public function logObjectView() {
    module_load_include('inc', 'islandora_basic_collection', 'includes/utilities');
    global $user;
    $object = islandora_object_load($this->pid);

    $object_access_id = db_insert('islandora_usage_stats_object_access_log')->fields(array(
        'pid_id' => $this->pid_id,
        'time' => REQUEST_TIME,
        'ip' => ip_address(),
        'user' => ($user->uid) ? $user->name : variable_get('anonymous', t('Anonymous')),
    ))->execute();

    // Log content models.
    $models = $object->models;
    $basic_model_key = array_search('fedora-system:FedoraObject-3.0', $models);
    if ($basic_model_key !== FALSE) {
      unset($models[$basic_model_key]);
    }
    foreach ($models as $model) {
      $model_id = islandora_usage_stats_get_pid_id($model);
      db_insert('islandora_usage_stats_content_model_access_log')->fields(array(
        'object_access_id' => $object_access_id,
        'content_model' => $model_id,
      ))->execute();
    }
    // Log parents.
    $collections = islandora_basic_collection_get_parent_pids($object);
    foreach ($collections as $collection) {
      $collection_id = islandora_usage_stats_get_pid_id($collection);
      db_insert('islandora_usage_stats_collection_access_log')->fields(array(
        'object_access_id' => $object_access_id,
        'collection' => $collection_id,
      ))->execute();
    }
  }

  /**
   * Track the top used search terms.
   *
   * @param string $term
   *   The search text to track.
   */
  public static function trackSearchTerm($term) {
    module_load_include('inc', 'islandora_usage_stats', 'includes/db');
    $term = strtolower($term);
    $result = islandora_usage_stats_get_search_term($term);
    if ($result) {
      // Term is found, increment by one.
      islandora_usage_stats_increment_search_term($result['id']);
    }
    else {
      islandora_usage_stats_set_search_term($term);
    }
  }

  /**
   * Log the download of a Fedora object datastream.
   */
  public function logObjectDownload($dsid) {
    global $user;
    $ip = ip_address();

    $result = db_insert('islandora_usage_stats_object_ds_access_log')->fields(array(
        'pid_id' => $this->pid_id,
        'time' => REQUEST_TIME,
        'ip' => $ip,
        'dsid' => $dsid,
        'user' => ($user->uid) ? $user->name : variable_get('anonymous', t('Anonymous')),
    ))->execute();

    return $result;
  }

  /**
   * The last time an IP accessed this item.
   *
   * Queries the database access logs.
   *
   * @return mixed
   *   NULL or a string of an integer since the unix epoch.
   */
  public function getLastViewTimeForIp() {
    $ip = ip_address();
    $result = db_query("SELECT max(time) from {islandora_usage_stats_object_access_log} WHERE pid_id = :pid_id AND ip = :ip", array(
        ':pid_id' => $this->pid_id,
        ':ip' => $ip,
    ));
    return $result->fetchField(0);
  }

  /**
   * The last time an IP downloaded the given DSID.
   *
   * Queries the datastream access log.
   *
   * @param string $dsid
   *   The DSID to check on.
   *
   * @return string
   *   String of an integer since the unix epoch.
   */
  public function getLastDownloadTimeForIp($dsid) {
    $ip = ip_address();
    $result = db_query("SELECT max(time) as time from {islandora_usage_stats_object_ds_access_log} WHERE pid_id = :pid_id AND dsid = :dsid AND ip = :ip", array(
      ':pid_id' => $this->pid_id,
      ':dsid' => $dsid,
      ':ip' => $ip,
    ));
    $record = $result->fetch();
    return $record->time;
  }

  /**
   * Uses session variables to check if user requested this item before.
   *
   * @return mixed
   *   NULL or a string of an integer since the unix epoch.
   */
  public function getLastViewTimeForVisitor() {
    $time = @$_SESSION['islandora_usage_stats'][ip_address()][$this->pid];
    // User has NOT requested item in this session.
    if (!$time) {
      return NULL;
    }
    return $time;
  }

  /**
   * Updates the session to indicate the user has accessed the object.
   */
  public function updateLastViewTimeForVisitor() {
    $_SESSION['islandora_usage_stats'][ip_address()][$this->pid] = REQUEST_TIME;
  }

  /**
   * Returns the object's last viewed time.
   *
   * @return string|null
   *   The last viewed time, or NULL if none existed.
   */
  public function getLastViewedTime() {
    $result = db_query("SELECT time from {islandora_usage_stats_object_views} WHERE pid_id = :pid_id", array(
      ':pid_id' => $this->pid_id,
    ));

    $record = $result->fetch();

    if (!$record) {
      return NULL;
    }
    else {
      return $record->time;
    }
  }

  /**
   * Updates the time that the user requested the item if appropriate.
   *
   * Performs a check to see if the user's IP is on the exclusion list, and
   * a check to determine the last time they requested to view the object.
   * If the last time the user requested the object falls within the cooldown
   * period, nothing is updated. Otherwise, this function calls
   * dBSetLastViewedTime and updates the time that the user requested item.
   *
   * @return array
   *   Empty or the result of dBSetLastViewedTime.
   */
  public function setLastViewedTime() {
    $result = array();

    $logging = variable_get('islandora_usage_stats_use_access_log', 1);
    // Check if user's IP is excluded.
    if (!$this->IpExcluded()) {

      $last_viewed = $this->getLastViewTimeForVisitor();

      // User has never viewed item before.
      if (!$last_viewed) {
        $result = $this->dBSetLastViewedTime();
        $this->updateLastViewTimeForVisitor();
      }
      else {
        // Default 5 min.
        $cooldown_time = variable_get('islandora_usage_stats_cooldown_seconds', 300);

        if ($last_viewed + ($cooldown_time) < REQUEST_TIME) {
          $result = $this->dBSetLastViewedTime();
          $this->updateLastViewTimeForVisitor();
        }
      }
    }

    return $result;
  }

  /**
   * Sets the last accessed time of an object.
   *
   * If it exists, otherwise creates a new entry for it view a view count of 1
   * and access time of 0.
   *
   * @return array
   *   Last viewed time keyed on 'timelastviewed'.
   */
  public function dBSetLastViewedTime() {
    $result = array();

    $current_views = $this->getViews();

    if ($current_views['views']) {
      // Set last viewed time to now.
      $update_result = db_update('islandora_usage_stats_object_views')->expression('time', REQUEST_TIME)
        ->condition('pid_id', $this->pid_id)
        ->execute();

      $result['timelastviewed'] = REQUEST_TIME;
    }
    else {
      // Insert a new row into the DB for this object.
      $insert_result = db_insert('islandora_usage_stats_object_views')->fields(array(
        'pid_id' => $this->pid_id,
        'views' => 1,
        'time' => 0,
      ))->execute();

      $result['timelastviewed'] = 0;
    }

    return $result;
  }

  /**
   * Increase ONLY the view count by 1 for the given IslandoraUsageStats object.
   *
   * If the object does not have an entry in the database already, it creates a
   * new one.
   *
   * @return int
   *   The view count.
   */
  public function dBIncreaseViewCounter() {
    $current_views = $this->getViews();

    if ($current_views['views']) {
      // Increase view count by one.
      $update_result = db_update('islandora_usage_stats_object_views')->expression('views', 'views + :num', array(
        ':num' => 1,
        ))
        ->condition('pid_id', $this->pid_id)
        ->execute();

      $result = $current_views['views'] + 1;
    }
    else {
      $insert_result = db_insert('islandora_usage_stats_object_views')->fields(array(
        'pid_id' => $this->pid_id,
        'views' => 1,
        'time' => 0,
      ))->execute();

      $result = 1;
    }

    return $result;
  }

  /**
   * Increases an item's view count by one and logs the access.
   *
   * If a user's IP address is not excluded and a user has not viewed this
   * particular item in the last X seconds (cooldown time), then this function
   * increases an item's view count by 1 and logs the access, if logging is
   * enabled.
   *
   * @return Ambigous
   *   Number or string? of the current view counter.
   */
  public function plusOneView() {
    $views = $this->getViews();
    $result = $views['views'];

    $logging = variable_get('islandora_usage_stats_use_access_log', 1);
    // Check if user's IP is excluded.
    if (!$this->IpExcluded()) {
      $last_viewed = $this->getLastViewTimeForVisitor();

      // User has never viewed item before.
      if (!$last_viewed) {
        $result = $this->dBIncreaseViewCounter();
        if ($logging) {
          $this->logObjectView();
          $this->updateLastViewTimeForVisitor();
        }
      }
      else {
        // Check if user has requested this item in the past X seconds.
        $cooldown_time = variable_get('islandora_usage_stats_cooldown_seconds', 300);

        if (($last_viewed + $cooldown_time) < REQUEST_TIME) {
          $result = $this->dBIncreaseViewCounter();
          if ($logging) {
            $this->logObjectView();
            $this->updateLastViewTimeForVisitor();
          }
        }
      }
    }
    return $result;
  }
}
