<?php

/**
 * @file
 * Administration page callbacks for the islandora usage stats module.
 */

/**
 * Form builder.
 * Configure usage stats.
 *
 * @ingroup forms
 * 
 * @see system_settings_form().
 */
function islandora_usage_stats_admin_settings()
{
    $form = array();
    
    $form['usage'] = array(
        '#type' => 'fieldset',
        '#title' => t('Omit IPs'),
        '#description' => t('Add IP addresses that you want to omit from usage tracking')
    );
    
    $form['usage']['islandora_usage_stats_ip_list'] = array(
        '#type' => 'textarea',
        '#description' => t('Add IPs separated by spaces'),
        '#default_value' => variable_get('islandora_usage_stats_ip_list', 'localhost 127.0.0.1')
    );
    
    $form['usage']['islandora_usage_stats_cooldown_seconds'] = array(
        '#type' => 'textfield',
        '#title' => 'Cooldown time (in seconds)',
        '#description' => t('The number of seconds to wait before counting a repeated view request from the same IP'),
        '#default_value' => variable_get('islandora_usage_stats_cooldown_seconds', '300')
    );
    
    $form['usage']['islandora_usage_stats_use_access_log'] = array(
        '#type' => 'checkbox',
        '#title' => 'Enable view and download log *REQUIRED FOR RECENT ACTIVITY BLOCK*',
        '#description' => t('Keep a log of all object view/download activity'),
        '#default_value' => variable_get('islandora_usage_stats_use_access_log', '1'),
    );
    
    $form['usage']['islandora_usage_stats_exclude_bots'] = array(
    	  '#type' => 'checkbox',
        '#title' => 'Exclude common bots to avoid artifical view count inflation',
        '#description' => t('Exclude bots who play nicely from view counts'),
        '#default_value' => variable_get('islandora_usage_stats_exclude_bots', '1'),
    );
    
    $form['clear'] = array(
        '#type' => 'fieldset',
        '#title' => t('Clear values')
    );
    
    $form['clear']['usage']['access_log'] = array(
    '#type' => 'fieldset',
    '#title' => t('Access log'),
    );
    
    $form['clear']['usage']['access_log']['clear_object_access_log_submit'] = array(
        '#type' => 'submit',
        '#name' => 'clearObjectAccessLogSubmit',
        '#default_value' => t('Clear views access log'),
    );
    
    $form['clear']['usage']['access_log']['clear_ds_access_log_submit'] = array(
        '#type' => 'submit',
        '#name' => 'clearDSAccessLogSubmit',
        '#default_value' => t('Clear datastream download access log'),
    );
    
    $form['clear']['usage']['views'] = array(
        '#type' => 'fieldset',
        '#title' => t('Object Views')
    );
    
    $form['clear']['usage']['views']['clear_view_by_pid'] = array(
        '#type' => 'textfield',
        '#title' => t('PID'),
        '#description' => t('This should be in the format <em>islandora:1</em>'),
    );
    
    $form['clear']['usage']['views']['clear_view_by_pid_submit'] = array(
        '#type' => 'submit',
        '#name' => 'clearViewByPIDSubmit',
        '#default_value' => t('Clear object view count')
    );
    
    $form['clear']['usage']['views']['clear_view_all'] = array(
        '#type' => 'submit',
        '#name' => 'clearViewAllSubmit',
        '#default_value' => t('Clear all object view counts')
    );
    
    $form['clear']['usage']['ds'] = array(
        '#type' => 'fieldset',
        '#title' => t('Datastream Downloads')
    );
    
    $form['clear']['usage']['ds']['clear_ds_by_pid'] = array(
        '#type' => 'textfield',
        '#title' => t('PID'),
        '#description' => t('This should be in the format <em>islandora:1</em>'),
    );
    
    $form['clear']['usage']['ds']['clear_ds_by_pid_submit'] = array(
        '#type' => 'submit',
        '#name' => 'clearDSByPIDSubmit',
        '#default_value' => t('Clear datastream download count')
    );
    
    $form['clear']['usage']['ds']['clear_ds_all'] = array(
        '#type' => 'submit',
        '#name' => 'clearDSAllSubmit',
        '#default_value' => t('Clear all datastream download counts')
    );
    
    return system_settings_form($form);
}

/**
 * We use the validate function as a pseudo-submit function as the call to 
 * system_settings_form prevents us from using our own submit handler.
 * 
 * @param unknown $form
 * @param unknown $form_state
 */
function islandora_usage_stats_admin_settings_validate($form, &$form_state)
{
    $result = '';
    
    $button = $form_state['triggering_element']['#id'];
    switch ($button) {
    	  case 'edit-clear-object-access-log-submit':
    	      $result = islandora_usage_stats_clear_tracking('islandora_usage_stats_object_access_log');
    	      if ($result) {
    	          drupal_set_message(t("Object view access logs have been cleared"));
    	      }
    	      break;
    	  case 'edit-clear-ds-access-log-submit':
    	      $result = islandora_usage_stats_clear_tracking('islandora_usage_stats_object_ds_access_log');
    	      if ($result) {
    	          drupal_set_message(t("Datastream download access logs have been cleared"));
    	      }
    	      break;
        case 'edit-clear-view-by-pid-submit':
            $pid = $form_state['values']['clear_view_by_pid'];
            $result = islandora_usage_stats_clear_tracking('islandora_usage_stats_object_views', $pid);
            if ($result) {
                drupal_set_message(t("Page tracking for PID '@pid' deleted!", array(
                    '@pid' => $pid
                )));
            }
            break;
        case 'edit-clear-view-all':
            $result = islandora_usage_stats_clear_tracking('islandora_usage_stats_object_views');
            if ($result) {
                drupal_set_message(t('All page tracking deleted!'));
            }
            break;
        case 'edit-clear-ds-by-pid-submit':
            $pid = $form_state['values']['clear_ds_by_pid'];
            $result = islandora_usage_stats_clear_tracking('islandora_usage_stats_object_ds_downloads', $pid);
            if ($result) {
                drupal_set_message(t("Datastream tracking for PID '@pid' deleted!", array(
                    '@pid' => $pid
                )));
            }
            break;
        case 'edit-clear-ds-all':
            $result = islandora_usage_stats_clear_tracking('islandora_usage_stats_object_ds_downloads');
            if ($result) {
                drupal_set_message(t('All datastream tracking deleted!'));
            }
            break;
    }
}

function islandora_usage_stats_clear_tracking($table, $pid = NULL)
{
    $result = '';
    if ($pid == NULL) {
        // $result = db_query("TRUNCATE TABLE %s", $table);
        $result = db_truncate($table)->execute();
    } else {
        // $result = db_query("DELETE FROM {%s} WHERE pid = '%s'", $table, $pid);
        $result = db_delete($table)->condition('pid', $pid)->execute();
    }
    return $result;
}
