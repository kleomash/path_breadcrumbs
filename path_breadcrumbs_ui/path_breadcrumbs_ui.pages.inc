<?php

/**
 * @file
 *
 */

/**
 * Page callback for arguments configuration in path.
 */
function path_breadcrumbs_ui_configure_arguments_page($path_name, $keyword) {

  // Include ctools modal plugin.
  ctools_include('modal');

  // Load argument settings form.
  $form = drupal_get_form('path_breadcrumbs_ui_argument_context_form', $path_name, $keyword);
  $commands = ctools_modal_form_render(array('title' => t('Choose argument')), $form);
  return array('#type' => 'ajax', '#commands' => $commands);
}

/**
 * Page callback for settings form for arguments.
 *
 * @param $path_name
 * @param $keyword
 * @return array
 */
function path_breadcrumbs_ui_change_arguments_page($path_name, $keyword) {
  // Include ctools modal plugin.
  ctools_include('modal');

  $breadcrumb = path_breadcrumbs_object_cache_get($path_name);

  // Set form values for second step of context settings form.
  $form_state = array(
    'storage' => array(
      'step' => 2,
      'path_name' => $path_name,
      'keyword' => $keyword,
      'argument' => $breadcrumb->arguments[$keyword]['argument'],
    ),
  );

  $form = drupal_build_form('path_breadcrumbs_ui_argument_context_form', $form_state);
  $commands = ctools_modal_form_render(array('title' => t('Choose argument')), $form);
  return array('#type' => 'ajax', '#commands' => $commands);
}

/**
 * Form for editing path breadcrumbs.
 */
function path_breadcrumbs_ui_edit_page($path_breadcrumb) {

  $page['content']['#prefix'] = '<div id="path-breadcrumbs-ui-wrapper" class="clearfix">';
  $page['content']['#suffix'] = '</div>';

  // Save form in cache.
  path_breadcrumbs_object_cache_set($path_breadcrumb->machine_name, $path_breadcrumb);

  $links[] = array(
    '#type' => 'link',
    '#title' => t('Basic'),
    '#href' => 'admin/structure/path-breadcrumbs/edit/' . $path_breadcrumb->path_id . '/ajax/1',
    '#attributes' => array('class' => array('use-ajax')),
  );

  $links[] = array(
    '#type' => 'link',
    '#title' => t('Arguments'),
    '#href' => 'admin/structure/path-breadcrumbs/edit/' . $path_breadcrumb->path_id . '/ajax/2',
    '#attributes' => array('class' => array('use-ajax')),
  );

  $links[] = array(
    '#type' => 'link',
    '#title' => t('Breadcrumbs'),
    '#href' => 'admin/structure/path-breadcrumbs/edit/' . $path_breadcrumb->path_id . '/ajax/3',
    '#attributes' => array('class' => array('use-ajax')),
  );

  $rendered_links = array();
  foreach ($links as $link) {
    $rendered_links[] = render($link);
  }

  $page['content']['menu'] = array(
    '#markup' => '<div id="path-breadcrumbs-ui-menu">' . theme('item_list', array('items' => $rendered_links)) . '</div>',
  );

  $form = drupal_get_form('path_breadcrumbs_ui_edit_form', $path_breadcrumb, 1);
  $page['content']['edit_form'] = array(
    '#markup' => render($form),
  );

  drupal_add_library('system', 'drupal.ajax');
  drupal_add_css(drupal_get_path('module', 'path_breadcrumbs_ui') . '/css/path_breadcrumbs_ui.css');

  return $page;
}

/**
 * Ajax callback for path_breadcrumbs EDIT links.
 */
function path_breadcrumbs_ui_edit_form_ajax_links($breadcrumb, $step) {

  $form = drupal_get_form('path_breadcrumbs_ui_edit_form', $breadcrumb, $step);

  $commands = array();
  $commands[] = ajax_command_replace('#path-breadcrumbs-ui-edit-form-wrapper', render($form));
  return array('#type' => 'ajax', '#commands' => $commands);
}

/**
 * Disables path breadcrumb.
 */
function path_breadcrumbs_ui_disable_breadcrumbs_page($breadcrumb) {

  // Update breadcrumb in database.
  db_update('path_breadcrumbs')
    ->fields(array(
      'disabled' => 1,
    ))
    ->condition('path_id', $breadcrumb->path_id)
    ->execute();

  drupal_set_message(t('Breadcrumb %name was disabled', array('%name' => $breadcrumb->name)));

  drupal_goto('admin/structure/path-breadcrumbs');
}

/**
 * Enables path breadcrumb.
 */
function path_breadcrumbs_ui_enable_breadcrumbs_page($breadcrumb) {

  // Update breadcrumb in database.
  db_update('path_breadcrumbs')
    ->fields(array(
      'disabled' => 0,
    ))
    ->condition('path_id', $breadcrumb->path_id)
    ->execute();

  drupal_set_message(t('Breadcrumb %name was enabled', array('%name' => $breadcrumb->name)));

  drupal_goto('admin/structure/path-breadcrumbs');
}
