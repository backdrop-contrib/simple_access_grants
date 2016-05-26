<?php
/**
 * @file
 * Provide themes for simple access grants administration.
 */

/**
 * Theme a list of simple access profiles.
 */
function theme_simple_access_grants_profile_list($variables) {
  $form =& $variables['form'];
  $rows = array();
  foreach (element_children($form['profiles']) as $id) {
    $row = array(
      'data' => array(
        backdrop_render($form['profiles'][$id]['name']),
        backdrop_render($form['profiles'][$id]['number']),
        backdrop_render($form['profiles'][$id]['operations']),
      ),
    );
    $rows[] = $row;
  }
  $output = backdrop_render_children($form);
  $header = array(t('Name'), t('Number'), t('Operations'));
  $output .= theme('table', array(
    'header' => $header,
    'rows' => $rows,
    'attributes' => array('id' => 'sa-profile')
  ));
  return $output;
}


/**
 * Theme simple access form.
 */
function theme_simple_access_grants_form($variables) {
  $form =& $variables['form'];
  $options = array();
  $output = '';
  $options['header'] = array(t('Role'));
  $options['header'][] = t('View');
  $options['header'][] = t('Update');
  $options['header'][] = t('Delete');

  foreach (element_children($form) as $gid) {
    $row = array(
      array('data' => backdrop_render($form[$gid]['name'])),
    );
    $row[] = array(
      'data' => backdrop_render($form[$gid]['view']),
    );
    $row[] = array(
      'data' => backdrop_render($form[$gid]['update']),
    );
    $row[] = array(
      'data' => backdrop_render($form[$gid]['delete']),
    );
    $options['rows'][] = $row;
  }
  if (!empty($options['rows'])) {
    $output .= theme('table', $options);
    return $output;
  }
}
