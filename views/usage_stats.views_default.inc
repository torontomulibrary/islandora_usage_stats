<?php
$view = new view();
$view->name = 'test';
$view->description = 'Usage Stats for Collections';
$view->tag = 'default';
$view->base_table = 'islandora_usage_stats_object_access_log';
$view->human_name = 'Islandora Usage Stats for Collections';
$view->core = 7;
$view->api_version = '3.0';
$view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

/* Display: Master */
$handler = $view->new_display('default', 'Master', 'default');
$handler->display->display_options['title'] = 'Usage Stats';
$handler->display->display_options['use_more_always'] = FALSE;
$handler->display->display_options['group_by'] = TRUE;
$handler->display->display_options['access']['type'] = 'perm';
$handler->display->display_options['cache']['type'] = 'none';
$handler->display->display_options['query']['type'] = 'views_query';
$handler->display->display_options['exposed_form']['type'] = 'basic';
$handler->display->display_options['pager']['type'] = 'some';
$handler->display->display_options['pager']['options']['items_per_page'] = '5';
$handler->display->display_options['style_plugin'] = 'table';
$handler->display->display_options['style_options']['columns'] = array(
  'id' => 'id',
  'pid_id' => 'pid_id',
  'id_1' => 'id_1',
  'id_2' => 'id_2',
  'pid' => 'pid',
);
$handler->display->display_options['style_options']['default'] = '-1';
$handler->display->display_options['style_options']['info'] = array(
  'id' => array(
    'sortable' => 0,
    'default_sort_order' => 'asc',
    'align' => '',
    'separator' => '',
    'empty_column' => 0,
  ),
  'pid_id' => array(
    'sortable' => 0,
    'default_sort_order' => 'asc',
    'align' => '',
    'separator' => '',
    'empty_column' => 0,
  ),
  'id_1' => array(
    'sortable' => 0,
    'default_sort_order' => 'asc',
    'align' => '',
    'separator' => '',
    'empty_column' => 0,
  ),
  'id_2' => array(
    'sortable' => 0,
    'default_sort_order' => 'asc',
    'align' => '',
    'separator' => '',
    'empty_column' => 0,
  ),
  'pid' => array(
    'sortable' => 0,
    'default_sort_order' => 'asc',
    'align' => '',
    'separator' => '',
    'empty_column' => 0,
  ),
);
/* Relationship: Islandora Usage Stats: Collection */
$handler->display->display_options['relationships']['id']['id'] = 'id';
$handler->display->display_options['relationships']['id']['table'] = 'islandora_usage_stats_object_access_log';
$handler->display->display_options['relationships']['id']['field'] = 'id';
/* Relationship: Islandora Usage Stats: Object */
$handler->display->display_options['relationships']['pid_id']['id'] = 'pid_id';
$handler->display->display_options['relationships']['pid_id']['table'] = 'islandora_usage_stats_object_access_log';
$handler->display->display_options['relationships']['pid_id']['field'] = 'pid_id';
/* Relationship: Islandora Usage Stats: Collection Object */
$handler->display->display_options['relationships']['collection']['id'] = 'collection';
$handler->display->display_options['relationships']['collection']['table'] = 'islandora_usage_stats_collection_access_log';
$handler->display->display_options['relationships']['collection']['field'] = 'collection';
$handler->display->display_options['relationships']['collection']['relationship'] = 'id';
/* Relationship: Islandora Usage Stats: Object Collection Access */
$handler->display->display_options['relationships']['object_access_id']['id'] = 'object_access_id';
$handler->display->display_options['relationships']['object_access_id']['table'] = 'islandora_usage_stats_collection_access_log';
$handler->display->display_options['relationships']['object_access_id']['field'] = 'object_access_id';
$handler->display->display_options['relationships']['object_access_id']['relationship'] = 'id';
/* Field: Islandora Usage Stats: Label */
$handler->display->display_options['fields']['label']['id'] = 'label';
$handler->display->display_options['fields']['label']['table'] = 'islandora_usage_stats_objects';
$handler->display->display_options['fields']['label']['field'] = 'label';
$handler->display->display_options['fields']['label']['relationship'] = 'pid_id';
/* Field: Islandora Usage Stats: PID */
$handler->display->display_options['fields']['pid']['id'] = 'pid';
$handler->display->display_options['fields']['pid']['table'] = 'islandora_usage_stats_objects';
$handler->display->display_options['fields']['pid']['field'] = 'pid';
$handler->display->display_options['fields']['pid']['relationship'] = 'pid_id';
$handler->display->display_options['fields']['pid']['element_label_colon'] = FALSE;
/* Field: COUNT(DISTINCT Islandora Usage Stats: Log ID) */
$handler->display->display_options['fields']['id']['id'] = 'id';
$handler->display->display_options['fields']['id']['table'] = 'islandora_usage_stats_object_access_log';
$handler->display->display_options['fields']['id']['field'] = 'id';
$handler->display->display_options['fields']['id']['group_type'] = 'count_distinct';
/* Contextual filter: Islandora Usage Stats: PID */
$handler->display->display_options['arguments']['pid']['id'] = 'pid';
$handler->display->display_options['arguments']['pid']['table'] = 'islandora_usage_stats_objects';
$handler->display->display_options['arguments']['pid']['field'] = 'pid';
$handler->display->display_options['arguments']['pid']['relationship'] = 'collection';
$handler->display->display_options['arguments']['pid']['default_action'] = 'default';
$handler->display->display_options['arguments']['pid']['default_argument_type'] = 'raw';
$handler->display->display_options['arguments']['pid']['default_argument_options']['index'] = '2';
$handler->display->display_options['arguments']['pid']['summary']['number_of_records'] = '0';
$handler->display->display_options['arguments']['pid']['summary']['format'] = 'default_summary';
$handler->display->display_options['arguments']['pid']['summary_options']['items_per_page'] = '25';
$handler->display->display_options['arguments']['pid']['limit'] = '0';

/* Display: Block */
$handler = $view->new_display('block', 'Block', 'usage_stats');
$handler->display->display_options['block_description'] = 'Usage Stats for Collections';
$translatables['test'] = array(
  t('Master'),
  t('Usage Stats'),
  t('more'),
  t('Apply'),
  t('Reset'),
  t('Sort by'),
  t('Asc'),
  t('Desc'),
  t('Collection'),
  t('Object'),
  t('Collection Object'),
  t('Object Collection Access'),
  t('Label'),
  t('PID'),
  t('Log ID'),
  t('.'),
  t(','),
  t('All'),
  t('Block'),
  t('Usage Stats for Collections'),
);
