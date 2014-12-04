<?php
/**
 * @file
 * Default views.
 */

/**
 * Implements hook_views_default_views().
 */
function islandora_usage_stats_views_default_views() {
  $view = new view();
  $view->name = 'islandora_usage_stats_report';
  $view->description = 'A tool for browsing Islandora usage data.';
  $view->tag = 'default';
  $view->base_table = 'islandora_usage_stats_object_access_log';
  $view->human_name = 'Islandora Usage Stats Report';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Islandora Usage Stats Report';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['group_by'] = TRUE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'view islandora usage stats reports';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['exposed_form']['options']['reset_button'] = TRUE;
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '10';
  $handler->display->display_options['style_plugin'] = 'responsive_grid';
  $handler->display->display_options['style_options']['columns'] = '2';
  $handler->display->display_options['style_options']['alignment'] = 'vertical';
  $handler->display->display_options['row_plugin'] = 'fields';
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
  /* Field: COUNT(DISTINCT Islandora Usage Stats: Log ID) */
  $handler->display->display_options['fields']['id']['id'] = 'id';
  $handler->display->display_options['fields']['id']['table'] = 'islandora_usage_stats_object_access_log';
  $handler->display->display_options['fields']['id']['field'] = 'id';
  $handler->display->display_options['fields']['id']['group_type'] = 'count_distinct';
  $handler->display->display_options['fields']['id']['label'] = 'Total';
  /* Filter criterion: Islandora Usage Stats: Label */
  $handler->display->display_options['filters']['label']['id'] = 'label';
  $handler->display->display_options['filters']['label']['table'] = 'islandora_usage_stats_objects';
  $handler->display->display_options['filters']['label']['field'] = 'label';
  $handler->display->display_options['filters']['label']['relationship'] = 'collection';
  $handler->display->display_options['filters']['label']['operator'] = 'contains';
  $handler->display->display_options['filters']['label']['exposed'] = TRUE;
  $handler->display->display_options['filters']['label']['expose']['operator_id'] = 'label_op';
  $handler->display->display_options['filters']['label']['expose']['label'] = 'Label';
  $handler->display->display_options['filters']['label']['expose']['use_operator'] = TRUE;
  $handler->display->display_options['filters']['label']['expose']['operator'] = 'label_op';
  $handler->display->display_options['filters']['label']['expose']['identifier'] = 'label';
  $handler->display->display_options['filters']['label']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
  );
  /* Filter criterion: Islandora Usage Stats: Time */
  $handler->display->display_options['filters']['time']['id'] = 'time';
  $handler->display->display_options['filters']['time']['table'] = 'islandora_usage_stats_object_access_log';
  $handler->display->display_options['filters']['time']['field'] = 'time';
  $handler->display->display_options['filters']['time']['operator'] = 'between';
  $handler->display->display_options['filters']['time']['exposed'] = TRUE;
  $handler->display->display_options['filters']['time']['expose']['operator_id'] = 'time_op';
  $handler->display->display_options['filters']['time']['expose']['label'] = 'Time';
  $handler->display->display_options['filters']['time']['expose']['use_operator'] = TRUE;
  $handler->display->display_options['filters']['time']['expose']['operator'] = 'time_op';
  $handler->display->display_options['filters']['time']['expose']['identifier'] = 'time';
  $handler->display->display_options['filters']['time']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
  );
  $handler->display->display_options['filters']['time']['form_type'] = 'date_popup';
  $handler->display->display_options['filters']['time']['date_fields'] = array(
    'islandora_usage_stats_object_access_log.time' => 'islandora_usage_stats_object_access_log.time',
  );

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page');
  $handler->display->display_options['defaults']['style_plugin'] = FALSE;
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'label' => 'label',
    'label_1' => 'label_1',
  );
  $handler->display->display_options['style_options']['default'] = 'label_1';
  $handler->display->display_options['style_options']['info'] = array(
    'label' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'label_1' => array(
      'sortable' => 1,
      'default_sort_order' => 'desc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
  );
  $handler->display->display_options['defaults']['style_options'] = FALSE;
  $handler->display->display_options['defaults']['row_plugin'] = FALSE;
  $handler->display->display_options['defaults']['row_options'] = FALSE;
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: Islandora Usage Stats: Label */
  $handler->display->display_options['fields']['label']['id'] = 'label';
  $handler->display->display_options['fields']['label']['table'] = 'islandora_usage_stats_objects';
  $handler->display->display_options['fields']['label']['field'] = 'label';
  $handler->display->display_options['fields']['label']['relationship'] = 'pid_id';
  $handler->display->display_options['fields']['label']['label'] = 'Object';
  /* Field: COUNT(Islandora Usage Stats: Label) */
  $handler->display->display_options['fields']['label_1']['id'] = 'label_1';
  $handler->display->display_options['fields']['label_1']['table'] = 'islandora_usage_stats_objects';
  $handler->display->display_options['fields']['label_1']['field'] = 'label';
  $handler->display->display_options['fields']['label_1']['relationship'] = 'pid_id';
  $handler->display->display_options['fields']['label_1']['group_type'] = 'count';
  $handler->display->display_options['fields']['label_1']['label'] = 'Page Views';
  /* Field: Islandora Usage Stats: Label */
  $handler->display->display_options['fields']['label_2']['id'] = 'label_2';
  $handler->display->display_options['fields']['label_2']['table'] = 'islandora_usage_stats_objects';
  $handler->display->display_options['fields']['label_2']['field'] = 'label';
  $handler->display->display_options['fields']['label_2']['relationship'] = 'collection';
  $handler->display->display_options['fields']['label_2']['exclude'] = TRUE;
  $handler->display->display_options['defaults']['filter_groups'] = FALSE;
  $handler->display->display_options['defaults']['filters'] = FALSE;
  /* Filter criterion: Islandora Usage Stats: Label */
  $handler->display->display_options['filters']['label']['id'] = 'label';
  $handler->display->display_options['filters']['label']['table'] = 'islandora_usage_stats_objects';
  $handler->display->display_options['filters']['label']['field'] = 'label';
  $handler->display->display_options['filters']['label']['relationship'] = 'collection';
  $handler->display->display_options['filters']['label']['operator'] = 'contains';
  $handler->display->display_options['filters']['label']['exposed'] = TRUE;
  $handler->display->display_options['filters']['label']['expose']['operator_id'] = 'label_op';
  $handler->display->display_options['filters']['label']['expose']['label'] = 'Collection Title';
  $handler->display->display_options['filters']['label']['expose']['use_operator'] = TRUE;
  $handler->display->display_options['filters']['label']['expose']['operator'] = 'label_op';
  $handler->display->display_options['filters']['label']['expose']['identifier'] = 'label';
  $handler->display->display_options['filters']['label']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
  );
  $handler->display->display_options['filters']['label']['expose']['autocomplete_filter'] = 1;
  $handler->display->display_options['filters']['label']['expose']['autocomplete_items'] = '10';
  $handler->display->display_options['filters']['label']['expose']['autocomplete_min_chars'] = '0';
  $handler->display->display_options['filters']['label']['expose']['autocomplete_field'] = 'label_2';
  $handler->display->display_options['filters']['label']['expose']['autocomplete_raw_suggestion'] = 1;
  $handler->display->display_options['filters']['label']['expose']['autocomplete_raw_dropdown'] = 1;
  $handler->display->display_options['filters']['label']['expose']['autocomplete_dependent'] = 0;
  /* Filter criterion: Islandora Usage Stats: Time */
  $handler->display->display_options['filters']['time']['id'] = 'time';
  $handler->display->display_options['filters']['time']['table'] = 'islandora_usage_stats_object_access_log';
  $handler->display->display_options['filters']['time']['field'] = 'time';
  $handler->display->display_options['filters']['time']['operator'] = 'between';
  $handler->display->display_options['filters']['time']['exposed'] = TRUE;
  $handler->display->display_options['filters']['time']['expose']['operator_id'] = 'time_op';
  $handler->display->display_options['filters']['time']['expose']['label'] = 'Time Period';
  $handler->display->display_options['filters']['time']['expose']['use_operator'] = TRUE;
  $handler->display->display_options['filters']['time']['expose']['operator'] = 'time_op';
  $handler->display->display_options['filters']['time']['expose']['identifier'] = 'time';
  $handler->display->display_options['filters']['time']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
  );
  $handler->display->display_options['filters']['time']['form_type'] = 'date_popup';
  $handler->display->display_options['filters']['time']['date_fields'] = array(
    'islandora_usage_stats_object_access_log.time' => 'islandora_usage_stats_object_access_log.time',
  );
  $handler->display->display_options['path'] = 'admin/reports/islandora_usage_stats_report';
  $handler->display->display_options['menu']['type'] = 'normal';
  $handler->display->display_options['menu']['title'] = 'Islandora Usage Stats';
  $handler->display->display_options['menu']['description'] = 'Browse the logs of object views.';
  $handler->display->display_options['menu']['weight'] = '0';
  $handler->display->display_options['menu']['name'] = 'management';
  $handler->display->display_options['menu']['context'] = 0;
  $handler->display->display_options['menu']['context_only_inline'] = 0;

  /* Display: Attachment */
  $handler = $view->new_display('attachment', 'Attachment', 'attachment_1');
  $handler->display->display_options['pager']['type'] = 'some';
  $handler->display->display_options['displays'] = array(
    'page' => 'page',
    'default' => 0,
  );
  $handler->display->display_options['attachment_position'] = 'after';
  $handler->display->display_options['inherit_exposed_filters'] = TRUE;

  /* Display: Data export */
  $handler = $view->new_display('views_data_export', 'Data export', 'views_data_export_1');
  $handler->display->display_options['pager']['type'] = 'some';
  $handler->display->display_options['pager']['options']['items_per_page'] = '0';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['style_plugin'] = 'views_data_export_csv';
  $handler->display->display_options['style_options']['provide_file'] = 1;
  $handler->display->display_options['style_options']['parent_sort'] = 0;
  $handler->display->display_options['style_options']['quote'] = 1;
  $handler->display->display_options['style_options']['trim'] = 0;
  $handler->display->display_options['style_options']['replace_newlines'] = 0;
  $handler->display->display_options['style_options']['header'] = 1;
  $handler->display->display_options['style_options']['keep_html'] = 0;
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: Islandora Usage Stats: Label */
  $handler->display->display_options['fields']['label']['id'] = 'label';
  $handler->display->display_options['fields']['label']['table'] = 'islandora_usage_stats_objects';
  $handler->display->display_options['fields']['label']['field'] = 'label';
  $handler->display->display_options['fields']['label']['relationship'] = 'pid_id';
  $handler->display->display_options['fields']['label']['label'] = 'Object';
  $handler->display->display_options['fields']['label']['element_label_colon'] = FALSE;
  /* Field: COUNT(Islandora Usage Stats: PID ID) */
  $handler->display->display_options['fields']['pid_id']['id'] = 'pid_id';
  $handler->display->display_options['fields']['pid_id']['table'] = 'islandora_usage_stats_object_access_log';
  $handler->display->display_options['fields']['pid_id']['field'] = 'pid_id';
  $handler->display->display_options['fields']['pid_id']['group_type'] = 'count';
  $handler->display->display_options['fields']['pid_id']['label'] = 'Page Views';
  $handler->display->display_options['fields']['pid_id']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['pid_id']['separator'] = '';
  $handler->display->display_options['defaults']['sorts'] = FALSE;
  /* Sort criterion: COUNT(Islandora Usage Stats: PID ID) */
  $handler->display->display_options['sorts']['pid_id']['id'] = 'pid_id';
  $handler->display->display_options['sorts']['pid_id']['table'] = 'islandora_usage_stats_object_access_log';
  $handler->display->display_options['sorts']['pid_id']['field'] = 'pid_id';
  $handler->display->display_options['sorts']['pid_id']['group_type'] = 'count';
  $handler->display->display_options['sorts']['pid_id']['order'] = 'DESC';
  $handler->display->display_options['path'] = 'admin/reports/islandora_usage_stats_report/export';
  $handler->display->display_options['displays'] = array(
    'page' => 'page',
    'default' => 0,
  );
  $translatables['islandora_usage_stats_report'] = array(
    t('Master'),
    t('Islandora Usage Stats Report'),
    t('more'),
    t('Apply'),
    t('Reset'),
    t('Sort by'),
    t('Asc'),
    t('Desc'),
    t('Items per page'),
    t('- All -'),
    t('Offset'),
    t('« first'),
    t('‹ previous'),
    t('next ›'),
    t('last »'),
    t('Collection'),
    t('Object'),
    t('Collection Object'),
    t('Total'),
    t('.'),
    t(','),
    t('Label'),
    t('Time'),
    t('Page'),
    t('Page Views'),
    t('Collection Title'),
    t('Time Period'),
    t('Attachment'),
    t('Data export'),
  );

  return array('islandora_usage_stats_report' => $view);
}
