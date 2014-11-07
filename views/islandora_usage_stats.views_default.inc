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
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'view islandora usage stats reports';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '15';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['pager']['options']['id'] = '0';
  $handler->display->display_options['pager']['options']['quantity'] = '9';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'pid' => 'pid',
    'collection' => 'collection',
    'content_model' => 'content_model',
    'ip' => 'ip',
    'user' => 'user',
    'time' => 'time',
  );
  $handler->display->display_options['style_options']['default'] = 'time';
  $handler->display->display_options['style_options']['info'] = array(
    'pid' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 1,
    ),
    'collection' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 1,
    ),
    'content_model' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 1,
    ),
    'ip' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 1,
    ),
    'user' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 1,
    ),
    'time' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 1,
    ),
  );
  /* Field: Islandora Usage Stats: PID */
  $handler->display->display_options['fields']['pid']['id'] = 'pid';
  $handler->display->display_options['fields']['pid']['table'] = 'islandora_usage_stats_object_access_log';
  $handler->display->display_options['fields']['pid']['field'] = 'pid';
  /* Field: Islandora Usage Stats: Collection */
  $handler->display->display_options['fields']['collection']['id'] = 'collection';
  $handler->display->display_options['fields']['collection']['table'] = 'islandora_usage_stats_collection_access_log';
  $handler->display->display_options['fields']['collection']['field'] = 'collection';
  /* Field: Islandora Usage Stats: Content Model */
  $handler->display->display_options['fields']['content_model']['id'] = 'content_model';
  $handler->display->display_options['fields']['content_model']['table'] = 'islandora_usage_stats_content_model_access_log';
  $handler->display->display_options['fields']['content_model']['field'] = 'content_model';
  /* Field: Islandora Usage Stats: IP */
  $handler->display->display_options['fields']['ip']['id'] = 'ip';
  $handler->display->display_options['fields']['ip']['table'] = 'islandora_usage_stats_object_access_log';
  $handler->display->display_options['fields']['ip']['field'] = 'ip';
  /* Field: Islandora Usage Stats: User */
  $handler->display->display_options['fields']['user']['id'] = 'user';
  $handler->display->display_options['fields']['user']['table'] = 'islandora_usage_stats_object_access_log';
  $handler->display->display_options['fields']['user']['field'] = 'user';
  /* Field: Islandora Usage Stats: Time */
  $handler->display->display_options['fields']['time']['id'] = 'time';
  $handler->display->display_options['fields']['time']['table'] = 'islandora_usage_stats_object_access_log';
  $handler->display->display_options['fields']['time']['field'] = 'time';
  /* Filter criterion: Islandora Usage Stats: Collection */
  $handler->display->display_options['filters']['collection']['id'] = 'collection';
  $handler->display->display_options['filters']['collection']['table'] = 'islandora_usage_stats_collection_access_log';
  $handler->display->display_options['filters']['collection']['field'] = 'collection';
  $handler->display->display_options['filters']['collection']['group'] = 1;
  $handler->display->display_options['filters']['collection']['exposed'] = TRUE;
  $handler->display->display_options['filters']['collection']['expose']['operator_id'] = 'collection_op';
  $handler->display->display_options['filters']['collection']['expose']['label'] = 'Collection';
  $handler->display->display_options['filters']['collection']['expose']['use_operator'] = TRUE;
  $handler->display->display_options['filters']['collection']['expose']['operator'] = 'collection_op';
  $handler->display->display_options['filters']['collection']['expose']['identifier'] = 'collection';
  $handler->display->display_options['filters']['collection']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
    3 => 0,
  );
  /* Filter criterion: Islandora Usage Stats: Time */
  $handler->display->display_options['filters']['time']['id'] = 'time';
  $handler->display->display_options['filters']['time']['table'] = 'islandora_usage_stats_object_access_log';
  $handler->display->display_options['filters']['time']['field'] = 'time';
  $handler->display->display_options['filters']['time']['operator'] = '>';
  $handler->display->display_options['filters']['time']['group'] = 1;
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
  $handler->display->display_options['path'] = 'admin/reports/islandora_usage_stats';
  $handler->display->display_options['menu']['type'] = 'normal';
  $handler->display->display_options['menu']['title'] = 'Islandora Usage Stats';
  $handler->display->display_options['menu']['description'] = 'Browse the logs of object views.';
  $handler->display->display_options['menu']['weight'] = '0';
  $handler->display->display_options['menu']['name'] = 'management';
  $handler->display->display_options['menu']['context'] = 0;
  $handler->display->display_options['menu']['context_only_inline'] = 0;
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
    t('PID'),
    t('Collection'),
    t('Content Model'),
    t('IP'),
    t('User'),
    t('Time'),
    t('Page'),
  );
  return array($view);
}
