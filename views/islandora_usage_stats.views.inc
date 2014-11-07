<?php
/**
 * @file
 * Views integration.
 */

/**
 * Implements hook_views_data().
 */
function islandora_usage_stats_views_data() {
  $data = array();
  $data['islandora_usage_stats_object_access_log'] = array(
    'table' => array(
      'group' => t('Islandora Usage Stats'),
      'base' => array(
        'field' => 'id',
        'title' => t('Islandora Usage Stats Object Access'),
        'help' => t('The log of objects being visited.'),
      ),
    ),
    'id' => array(
      'title' => t('Log ID'),
      'help' => t('ID of the log entry.'),
      'field' => array(
        'handler' => 'views_handler_field_numeric',
        'click sortable' => TRUE,
      ),
      'sort' => array(
        'handler' => 'views_handler_sort',
      ),
      'filter' => array(
        'handler' => 'views_handler_filter_numeric',
      ),
    ),
    'pid' => array(
      'title' => t('PID'),
      'help' => t('PID of the object being logged.'),
      'field' => array(
        'handler' => 'views_handler_field',
        'click sortable' => TRUE,
      ),
      'sort' => array(
        'handler' => 'views_handler_sort',
      ),
      'filter' => array(
        'handler' => 'views_handler_filter_string',
      ),
      'argument' => array(
        'handler' => 'views_handler_argument_string',
      ),
    ),
    'ip' => array(
      'title' => t('IP'),
      'help' => t('IP of user that accessed the object.'),
      'field' => array(
        'handler' => 'views_handler_field',
        'click sortable' => TRUE,
      ),
      'sort' => array(
        'handler' => 'views_handler_sort',
      ),
      'filter' => array(
        'handler' => 'views_handler_filter_string',
      ),
      'argument' => array(
        'handler' => 'views_handler_argument_string',
      ),
    ),
    'user' => array(
      'title' => t('User'),
      'help' => t('Name of the Drupal user that accessed the object.'),
      'field' => array(
        'handler' => 'views_handler_field',
        'click sortable' => TRUE,
      ),
      'sort' => array(
        'handler' => 'views_handler_sort',
      ),
      'filter' => array(
        'handler' => 'views_handler_filter_string',
      ),
      'argument' => array(
        'handler' => 'views_handler_argument_string',
      ),
    ),
    'time' => array(
      'title' => t('Time'),
      'help' => t('Time the view is being logged.'),
      'field' => array(
        'handler' => 'views_handler_field_date',
        'click sortable' => TRUE,
      ),
      'sort' => array(
        'handler' => 'views_handler_sort_date',
      ),
      'filter' => array(
        'handler' => 'datepicker_views_filter_handler',
        'is date' => TRUE,
      ),
    ),
  );
  $data['islandora_usage_stats_collection_access_log'] = array(
    'table' => array(
      'group' => t('Islandora Usage Stats'),
      'base' => array(
        'field' => 'id',
        'title' => t('Islandora Usage Stats Collection Access'),
        'help' => t('The log of the parents of objects that are visited.'),
      ),
      'join' => array(
        'islandora_usage_stats_object_access_log' => array(
          'left_field' => 'id',
          'field' => 'object_access_id',
        ),
      ),
    ),
    'collection' => array(
      'title' => t('Collection'),
      'help' => t('The parent of the object being logged.'),
      'field' => array(
        'handler' => 'views_handler_field',
        'click sortable' => TRUE,
      ),
      'sort' => array(
        'handler' => 'views_handler_sort',
      ),
      'filter' => array(
        'handler' => 'views_handler_filter_string',
      ),
      'argument' => array(
        'handler' => 'views_handler_argument_string',
      ),
    ),
    'id' => array(
      'title' => t('ID'),
      'help' => t('ID for a logged parent.'),
      'field' => array(
        'handler' => 'views_handler_field_numeric',
        'click sortable' => TRUE,
      ),
      'sort' => array(
        'handler' => 'views_handler_sort',
      ),
      'filter' => array(
        'handler' => 'views_handler_filter_numeric',
      ),
    ),
    'object_access_id' => array(
      'title' => t('Log ID'),
      'help' => t('ID of the log entry.'),
      'field' => array(
        'handler' => 'views_handler_field_numeric',
        'click sortable' => TRUE,
      ),
      'sort' => array(
        'handler' => 'views_handler_sort',
      ),
      'filter' => array(
        'handler' => 'views_handler_filter_numeric',
      ),
    ),
  );
  $data['islandora_usage_stats_content_model_access_log'] = array(
    'table' => array(
      'group' => t('Islandora Usage Stats'),
      'base' => array(
        'field' => 'id',
        'title' => t('Islandora Usage Stats Content Model Access'),
        'help' => t('The log of content models of objects that are visited.'),
      ),
      'join' => array(
        'islandora_usage_stats_object_access_log' => array(
          'left_field' => 'id',
          'field' => 'object_access_id',
        ),
      ),
    ),
    'content_model' => array(
      'title' => t('Content Model'),
      'help' => t('The content model of the object being logged.'),
      'field' => array(
        'handler' => 'views_handler_field',
        'click sortable' => TRUE,
      ),
      'sort' => array(
        'handler' => 'views_handler_sort',
      ),
      'filter' => array(
        'handler' => 'views_handler_filter_string',
      ),
      'argument' => array(
        'handler' => 'views_handler_argument_string',
      ),
    ),
    'id' => array(
      'title' => t('ID'),
      'help' => t('ID for a logged content model.'),
      'field' => array(
        'handler' => 'views_handler_field_numeric',
        'click sortable' => TRUE,
      ),
      'sort' => array(
        'handler' => 'views_handler_sort',
      ),
      'filter' => array(
        'handler' => 'views_handler_filter_numeric',
      ),
    ),
    'object_access_id' => array(
      'title' => t('Log ID'),
      'help' => t('ID of the log entry.'),
      'field' => array(
        'handler' => 'views_handler_field_numeric',
        'click sortable' => TRUE,
      ),
      'sort' => array(
        'handler' => 'views_handler_sort',
      ),
      'filter' => array(
        'handler' => 'views_handler_filter_numeric',
      ),
    ),
  );
  return $data;
}
