<?php
$view = new view();
$view->name = 'usage_stats_';
$view->description = '';
$view->tag = 'default';
$view->base_table = 'islandora_usage_stats_collection_access_log';
$view->human_name = 'Usage Stats for Collections';
$view->core = 7;
$view->api_version = '3.0';
$view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

/* Display: Master */
$handler = $view->new_display('default', 'Master', 'default');
$handler->display->display_options['title'] = 'Usage Stats ';
$handler->display->display_options['use_ajax'] = TRUE;
$handler->display->display_options['use_more_always'] = FALSE;
$handler->display->display_options['group_by'] = TRUE;
$handler->display->display_options['access']['type'] = 'none';
$handler->display->display_options['cache']['type'] = 'none';
$handler->display->display_options['query']['type'] = 'views_query';
$handler->display->display_options['exposed_form']['type'] = 'basic';
$handler->display->display_options['pager']['type'] = 'some';
$handler->display->display_options['pager']['options']['items_per_page'] = '5';
$handler->display->display_options['style_plugin'] = 'default';
$handler->display->display_options['row_plugin'] = 'fields';
/* No results behavior: Global: Result summary */
$handler->display->display_options['empty']['result']['id'] = 'result';
$handler->display->display_options['empty']['result']['table'] = 'views';
$handler->display->display_options['empty']['result']['field'] = 'result';
/* Relationship: Islandora Usage Stats: Collection Object */
$handler->display->display_options['relationships']['collection']['id'] = 'collection';
$handler->display->display_options['relationships']['collection']['table'] = 'islandora_usage_stats_collection_access_log';
$handler->display->display_options['relationships']['collection']['field'] = 'collection';
/* Relationship: Islandora Usage Stats: Object Collection Access */
$handler->display->display_options['relationships']['object_access_id']['id'] = 'object_access_id';
$handler->display->display_options['relationships']['object_access_id']['table'] = 'islandora_usage_stats_collection_access_log';
$handler->display->display_options['relationships']['object_access_id']['field'] = 'object_access_id';
/* Field: Islandora Usage Stats: Label */
$handler->display->display_options['fields']['label']['id'] = 'label';
$handler->display->display_options['fields']['label']['table'] = 'islandora_usage_stats_objects';
$handler->display->display_options['fields']['label']['field'] = 'label';
$handler->display->display_options['fields']['label']['relationship'] = 'collection';
$handler->display->display_options['fields']['label']['label'] = '';
/* Field: Islandora Usage Stats: PID */
$handler->display->display_options['fields']['pid']['id'] = 'pid';
$handler->display->display_options['fields']['pid']['table'] = 'islandora_usage_stats_objects';
$handler->display->display_options['fields']['pid']['field'] = 'pid';
$handler->display->display_options['fields']['pid']['relationship'] = 'collection';
$handler->display->display_options['fields']['pid']['label'] = '';
$handler->display->display_options['fields']['pid']['exclude'] = TRUE;
$handler->display->display_options['fields']['pid']['element_label_colon'] = FALSE;
/* Field: COUNT(Islandora Usage Stats: Log ID) */
$handler->display->display_options['fields']['object_access_id']['id'] = 'object_access_id';
$handler->display->display_options['fields']['object_access_id']['table'] = 'islandora_usage_stats_collection_access_log';
$handler->display->display_options['fields']['object_access_id']['field'] = 'object_access_id';
$handler->display->display_options['fields']['object_access_id']['group_type'] = 'count';
$handler->display->display_options['fields']['object_access_id']['label'] = 'Total Views';
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
$handler = $view->new_display('block', 'Block', 'block');
$translatables['usage_stats_'] = array(
  t('Master'),
  t('Usage Stats '),
  t('more'),
  t('Apply'),
  t('Reset'),
  t('Sort by'),
  t('Asc'),
  t('Desc'),
  t('Displaying @start - @end of @total'),
  t('Collection Object'),
  t('Object Collection Access'),
  t('Total Views'),
  t('.'),
  t(','),
  t('All'),
  t('Block'),
);
