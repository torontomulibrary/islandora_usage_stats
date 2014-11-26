<?php
/**
 * @file
 * Database interaction helpers.
 */

/**
 * Gets the database id related to a PID.
 *
 * Sets one if it doesn't exist.
 */
function islandora_usage_stats_get_pid_id($pid) {
  $id = islandora_usage_stats_pid_id($pid);
  // If no matches, create one.
  if (!$id) {
    return islandora_usage_stats_pid_to_db($pid);
  }
  return $id;
}

/**
 * Gets the database id related to a PID.
 */
function islandora_usage_stats_pid_id($pid) {
  $result = db_select('islandora_usage_stats_objects')
    ->fields('islandora_usage_stats_objects', array('id'))
    ->condition('pid', $pid)
    ->execute();
  $record = $result->fetchAssoc();
  return isset($record['id']) ? $record['id'] : FALSE;
}

/**
 * Gets the database id related to a PID.
 */
function islandora_usage_stats_pid_to_db($pid) {
  $object = islandora_object_load($pid);
  return db_insert('islandora_usage_stats_objects')
    ->fields(array('pid' => $pid, 'label' => $object->label))
    ->execute();
}

/**
 * Get a number of the most recently viewed objects.
 *
 * @param int $count
 *   The number of objects to get.
 * @param array $exclude
 *   A list of PIDs to exclude.
 *
 * @return array
 *   A list of results containing an object with attributes of pid and views.
 */
function islandora_usage_stats_get_most_viewed($count, $exclude = NULL) {
  $query = db_select('islandora_usage_stats_object_access_log', 'log');
  $query->join('islandora_usage_stats_objects', 'objects', 'log.pid_id = objects.id');
  $query->addExpression('COUNT(log.pid_id)', 'views');
  $results = $query->fields('objects', array('pid'))
    ->groupBy('log.pid_id')
    ->orderBy('views', 'DESC')
    ->range(0, $count)
    ->execute();
  $records = $results->fetchAllAssoc('pid');
  return $records;
}

/**
 * Get a number of the most viewed objects.
 *
 * @param int $count
 *   The number of objects to get.
 * @param array $exclude
 *   A list of PIDs to exclude.
 *
 * @return array
 *   A list of PIDs.
 */
function islandora_usage_stats_get_recently_viewed($count, $exclude = NULL) {
  $query = db_select('islandora_usage_stats_object_access_log', 'log');
  $query->join('islandora_usage_stats_objects', 'objects', 'log.pid_id = objects.id');
  $results = $query->fields('objects', array('pid'))
    ->orderBy('log.time', 'DESC')
    ->range(0, $count)
    ->execute();
  $records = $results->fetchAllAssoc('pid');
  return array_keys($records);
}

/**
 * Update an object's label if necessary.
 *
 * @param string $pid
 *   The PID of the object.
 * @param string $label
 *   The label to set.
 */
function islandora_usage_stats_update_object_label($pid, $label) {
  db_update('islandora_usage_stats_objects')
    ->fields(array('label' => $label))
    ->condition('pid', $pid, '=')
    ->condition('label', $label, '<>')
    ->execute();
}

/**
 * Select top downloads, ordered by most downloaded.
 *
 * @param int $count
 *   The number of max results to return.
 *
 * @return array
 *   A list of PIDS.
 */
function islandora_usage_stats_get_top_downloads($count) {
  $query = db_select('islandora_usage_stats_top_downloads', 'dl');
  $query->join('islandora_usage_stats_objects', 'objects', 'dl.pid_id = objects.id');
  $query->join('islandora_usage_stats_datastreams', 'ds', 'dl.pid_id = ds.pid_id');
  $results = $query->fields('objects', array('pid'))
    ->fields('dl', array('ds_id'))
    ->orderBy('dl.downloads', 'DESC')
    ->range(0, $count)
    ->execute();
  $records = $results->fetchAllAssoc('pid');
  return array_keys($records);
}

/**
 * Increment the ds download counter for top downloads.
 *
 * @param int $time
 *   Using REQUEST_TIME, constant available time as int.
 * @param int $pid_id
 *   The pid_id, or id key in islandora_usage_stats_objects.
 * @param int $dsid
 *   The ds_id, or id key in islandora_usage_stats_datastreams.
 *
 * @return array
 *   The query result.
 */
function islandora_usage_stats_increment_top_object_ds_download_counter($time, $pid_id, $dsid) {
  $result = db_update('islandora_usage_stats_top_downloads')
    ->expression('downloads', 'downloads + :downloads',
      array(
        ':downloads' => 1,
      )
    )
    ->expression('time', $time)
    ->condition('pid_id', $pid_id)
    ->condition('ds_id', $dsid)
    ->execute();
  return $result;
}

/**
 * Insert record to top downloads table.
 *
 * @param int $time
 *   Using REQUEST_TIME, constant available time as int.
 * @param int $pid_id
 *   The pid_id, or id key in islandora_usage_stats_objects.
 * @param int $dsid
 *   The ds_id, or id key in islandora_usage_stats_datastreams.
 *
 * @return array
 *   The query result.
 */
function islandora_usage_stats_insert_top_download($time, $pid_id, $dsid) {
  $result = db_insert('islandora_usage_stats_top_downloads')
    ->fields(
      array(
        'pid_id' => $pid_id,
        'ds_id' => $dsid,
        'downloads' => 1,
        'time' => $time,
      )
    )
    ->execute();
  return $result;
}

/**
 * Finds top downloads for given information.
 *
 * In addition to finding top downloads, this
 * function will also create an entry in
 * islandora_usage_stats_datastreams if it
 * does not exist.
 */
function islandora_usage_stats_find_existing_downloads($pid_id, $dsid, $label) {
  $result = db_select('islandora_usage_stats_datastreams')
    ->fields('islandora_usage_stats_datastreams', array('id'))
    ->condition('pid_id', $pid_id)
    ->condition('dsid', $dsid)
    ->execute();
  $record = $result->fetchAssoc();
  $ds_id = isset($record['id']) ? $record['id'] : FALSE;
  if (!$ds_id) {
    // This record does not exist yet, we must add it.
    $ds_id = db_insert('islandora_usage_stats_datastreams')
      ->fields(array('pid_id' => $pid_id, 'dsid' => $dsid, 'ds_label' => $label))
      ->execute();
    $ds_id = $ds_id['id'];
  }

  $result = db_query("SELECT downloads, time, ds_id from {islandora_usage_stats_top_downloads} WHERE pid_id = :pid_id AND ds_id = :dsid", array(
    ':pid_id' => $pid_id,
    ':dsid' => $ds_id,
  ));
  $data = $result->fetchAssoc();
  $return = array(
    'result' => $data,
    'ds_id' => $ds_id,
  );
  return $return;
}

/**
 * Get similar search terms for a given term.
 *
 * @param string $term
 *   The Current search term.
 *
 * @return array
 *   The search term as an associated array.
 */
function islandora_usage_stats_get_search_term($term) {
  $result = db_select('islandora_usage_stats_top_searches', 'ts')
    ->fields('ts')
    ->condition('term', db_like($term) . '%', 'LIKE')
    ->execute()
    ->fetchAssoc();
  return $result;
}

/**
 * Increase search term usage by row id.
 *
 * @param int $id
 *   The row id in db to increment.
 *
 * @return array
 *   Increment results array.
 */
function islandora_usage_stats_increment_search_term($id) {
  $result = db_update('islandora_usage_stats_top_searches')
    ->expression('search_total', 'search_total + :search_total', array(
      ':search_total' => 1,
      )
    )
    ->condition('id', $id)
    ->execute();
  return $result;
}

/**
 * Set a new search term to track.
 *
 * @param string $term
 *   The search term to track.
 *
 * @return array
 *   Results of the insertion.
 */
function islandora_usage_stats_set_search_term($term) {
  $result = db_insert('islandora_usage_stats_top_searches')->fields(
    array(
      'term' => $term,
      'search_total' => 1,
    )
  )->execute();
  return $result;
}

/**
 * Get top searches used on this site.
 *
 * @param int $count
 *   Max number of results to return
 *
 * @return array
 *   Results array of top search terms.
 */
function islandora_usage_stats_get_top_searched($count) {
  $query = db_select('islandora_usage_stats_top_searches', 'ts');
  $results = $query->orderBy('search_total', 'DESC')
    ->fields('ts', array('term', 'search_total'))
    ->range(0, $count)
    ->execute();
  $records = $results->fetchAll();
  return $records;
}

/**
 * Clear information from tables.
 *
 * @param string $table
 *   The base table to remove information from info from other tables may be
 *   removed as well.
 * @param string $pid
 *   The PID to remove information about.
 */
function islandora_usage_stats_clear_tracking($table, $pid = NULL) {
  $results = array();
  if ($pid == NULL) {
    $results[] = db_truncate($table)->execute();
    if ($table == 'islandora_usage_stats_object_access_log') {
      db_truncate('islandora_usage_stats_collection_access_log')->execute();
      db_truncate('islandora_usage_stats_content_model_access_log')->execute();
    }
  }
  else {
    if ($table == 'islandora_usage_stats_objects') {
      $results[] = db_delete($table)
        ->condition('pid', $pid)
        ->execute();
    }
    else {
      $pid_id = islandora_usage_stats_pid_id($pid);
      if ($table == 'islandora_usage_stats_object_access_log') {
        $in_query = db_select('islandora_usage_stats_object_access_log')
          ->fields('islandora_usage_stats_object_access_log', array('id'))
          ->condition('pid_id', $pid_id);
        $results[] = db_delete('islandora_usage_stats_collection_access_log')
          ->condition('object_access_id', $in_query, 'IN')
          ->execute();
        $results[] = db_delete('islandora_usage_stats_content_model_access_log')
          ->condition('object_access_id', $in_query, 'IN')
          ->execute();
      }
      if ($table == 'islandora_usage_stats_collection_access_log') {
        $pid_id_field = 'collection';
      }
      elseif ($table == 'islandora_usage_stats_content_model_access_log') {
        $pid_id_field = 'content_model';
      }
      else {
        $pid_id_field = 'pid_id';
      }
      $results[] = db_delete($table)
        ->condition($pid_id_field, $pid_id)
        ->execute();
    }
  }
  return $results;
}
