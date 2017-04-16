<?php

/**
 * @file
 * Administrative form for setting Simple Access Grants.
 */

/**
 * Simple Access Grants rebuild.
 */
function simple_access_grants_admin_settings() {
  $form['simple_access_grants'] = array(
    '#type' => 'fieldset',
    '#title' => t('Rebuild permissions'),
    '#description' => t('If changes have been made to node access settings, the Node Access settings for the realm simple_access_grants must be rebuilt.'),
    '#collapsible' => FALSE,
    '#tree' => TRUE,
  );
  $form['actions']['#type'] = 'actions';
  // Add a cancel button
  $form['actions']['cancel'] = array(
    '#type' => 'button',
    '#value' => t('Cancel'),
    '#attributes' => array('onClick' => 'history.go(-1); event.preventDefault();'),
  );

  // Add a submit button
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Next'),
  );

  // Return the form
  return $form;
}

function simple_access_grants_admin_settings_submit() {
  backdrop_goto('admin/reports/status/rebuild');
}
