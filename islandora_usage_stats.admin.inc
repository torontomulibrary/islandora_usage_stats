<?php

/**
 * @file
 * Administration page callbacks for the islandora usage stats module.
 */


/**
 * Form builder. Configure usage stats.
 *
 * @ingroup forms
 * @see system_settings_form().
 */
function islandora_usage_stats_admin_settings() {
	$form = array();

	$form['tracking'] = array(
			'#type' => 'fieldset',
			'#title' => t('Omit IPs'),
			'#description' => t('Add IP addresses that you want to omit from the tracking'),
	);

	$form['tracking']['islandora_usage_stats_ip_list'] = array(
			'#type' => 'textarea',
			'#description' => t('Add IPs separated by spaces'),
			'#default_value' => variable_get('islandora_usage_stats_ip_list', 'localhost 127.0.0.1'),
	);

	$form['tracking']['islandora_usage_stats_cooldown_seconds'] = array(
		  '#type' => 'textfield',
	    '#title' => 'Cooldown time (in seconds)',
	    '#description' => t('The number of seconds to wait before counting a repeated view request from the same IP'),
	    '#default_value' => variable_get('islandora_usage_stats_cooldown_seconds', '3000'),
	);
	
	$form['clear'] = array(
			'#type' => 'fieldset',
			'#title' => t('Clear values'),
	);

	$form['clear']['options'] = array(
			'#value' => drupal_get_form('islandora_usage_stats_admin_form'),
	);

	return system_settings_form($form);
}


function islandora_usage_stats_admin_form($form_state) {

  $form['tracking']['page'] = array(
    '#type' => 'fieldset',
    '#title' => t('Pages'),
  );

  $form['tracking']['page']['clear_page_by_pid'] = array(
    '#type' => 'textfield',
    '#title' => t('PID'),
  );

  $form['tracking']['page']['clear_page_by_pid_submit'] = array(
    '#type' => 'submit',
    '#value' => t('Clear page count'),
  );

  $form['tracking']['page']['clear_page_all'] = array(
    '#type' => 'submit',
    '#value' => t('Clear all page counts'),
  );

  $form['tracking']['ds'] = array(
    '#type' => 'fieldset',
    '#title' => t('Datastreams'),
  );

  $form['tracking']['ds']['clear_ds_by_pid'] = array(
    '#type' => 'textfield',
    '#title' => t('PID'),
  );

  $form['tracking']['ds']['clear_ds_by_pid_submit'] = array(
    '#type' => 'submit',
    '#value' => t('Clear datastream count'),
  );

  $form['tracking']['ds']['clear_ds_all'] = array(
    '#type' => 'submit',
    '#value' => t('Clear all datastream counts'),
  );

  return $form;
}

function islandora_usage_stats_admin_form_submit($form, &$form_state) {
  $button = $form_state['clicked_button']['#id'];
  $result = '';
  switch ($button) {
    case 'edit-clear-page-by-pid-submit':
      $pid = $form_state['values']['clear_page_by_pid'];
      $result = islandora_usage_stats_clear_tracking('islandora_usage_stats_page_tracking', $pid);
      if ($result) {
        drupal_set_message(t("Page tracking for PID '@pid' deleted!", array('@pid' => $pid)));
      }
      break;
    case 'edit-clear-page-all':
      $result = islandora_usage_stats_clear_tracking('islandora_usage_stats_page_tracking');
      if ($result) {
        drupal_set_message(t('All page tracking deleted!'));
      }
      break;
    case 'edit-clear-ds-by-pid-submit':
      $pid = $form_state['values']['clear_ds_by_pid'];
      $result = islandora_usage_stats_clear_tracking('islandora_usage_stats_ds_tracking', $pid);
      if ($result) {
        drupal_set_message(t("Datastream tracking for PID '@pid' deleted!", array('@pid' => $pid)));
      }
      break;
    case 'edit-clear-ds-all':
      $result = islandora_usage_stats_clear_tracking('islandora_usage_stats_ds_tracking');
      if ($result) {
        drupal_set_message(t('All datastream tracking deleted!'));
      }
      break;
  }
//  drupal_set_message('Button: ' . implode(',', $button));
//  islandora_scholar_clear_tracking('islandora_scholar_page_tracking');
}

function islandora_usage_stats_clear_tracking($table, $pid = NULL) {
  $result = '';
  if ($pid == NULL) {
    //$result = db_query("TRUNCATE TABLE %s", $table);
		$result = db_truncate($table)->execute();
  }
  else {
    //$result = db_query("DELETE FROM {%s} WHERE pid = '%s'", $table, $pid);
    $result = db_delete($table)
    						->condition('pid', $pid)
    						->execute();
  }
  return $result;
}


