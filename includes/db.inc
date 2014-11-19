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
