<?php
/**
 * @file
 * Provide themes for simple access grants administration.
 */


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
