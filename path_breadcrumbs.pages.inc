<?php

/**
 * @file
 *
 */

/**
 * Page callback for arguments configuration in path.
 */
function path_breadcrumbs_configure_arguments_page($path_name, $keyword) {

  // Include ctools modal plugin.
  ctools_include('modal');

  // Get data from cache.
  $breadcrumb = path_breadcrumbs_object_cache_get($path_name);

  $form_state = array(
    'ajax'  => TRUE,
    'title' => t('Choose argument context'),
    'default_argument' => isset($breadcrumb->arguments[$keyword]['argument']) ? $breadcrumb->arguments[$keyword]['argument'] : NULL,
  );


  // Load argument settings form.
  $form = drupal_get_form('path_breadcrumbs_argument_context_form', $path_name, $keyword);

  $commands = array();
  $commands = ctools_modal_form_render(array('title' => t('Choose argument')), $form);


  // If form was submited.
  /*if (!empty($form_state['executed'])) {

    // Update cache data.
    $breadcrumb->arguments[$keyword] = array(
      'argument' => $form_state['values']['argument'],
      'position' => $breadcrumb->keywords[$keyword],
    );

    path_breadcrumbs_object_cache_set($path_name, $breadcrumb);

    // Rebuild second step of path breadcrumbs add_form.
    $add_form_state = array(
      'storage' => array(
        'step' => 2,
        'machine_name' => $path_name,
      ));

    $form = drupal_build_form('path_breadcrumbs_add_form', $add_form_state);

    // Include additinal ajax commands.
    $commands[] = ajax_command_replace('#path-breadcrumbs-argument-table', drupal_render($form['table']));
    $commands[] = ctools_modal_command_dismiss();
  }*/

  return array('#type' => 'ajax', '#commands' => $commands);
}

function path_breadcrumbs_change_arguments_page($path_name, $keyword) {
    // Include ctools modal plugin.
  ctools_include('modal');

  // Get data from cache.
  $breadcrumb = path_breadcrumbs_object_cache_get($path_name);

  $form_state = array(
    'ajax'  => TRUE,
    'title' => t('Choose argument context'),
    'default_argument' => isset($breadcrumb->arguments[$keyword]['argument']) ? $breadcrumb->arguments[$keyword]['argument'] : NULL,
  );

  $plugin = ctools_get_argument($breadcrumb->arguments[$keyword]['argument']);
  $form = array();
  if ($function = ctools_plugin_get_function($plugin, 'settings form')) {
    $function($form, $form_state);
  }


  // Load argument settings form.
  $commands = ctools_modal_form_wrapper('path_breadcrumbs_argument_context_form', $form_state);

  // If form was submited.
  if (!empty($form_state['executed'])) {

    // Update cache data.
    $breadcrumb->arguments[$keyword] = array(
      'argument' => $form_state['values']['argument'],
      'position' => $breadcrumb->keywords[$keyword],
    );

    path_breadcrumbs_object_cache_set($path_name, $breadcrumb);

    // Rebuild second step of path breadcrumbs add_form.
    $add_form_state = array(
      'storage' => array(
        'step' => 2,
        'machine_name' => $path_name,
      ));

    $form = drupal_build_form('path_breadcrumbs_add_form', $add_form_state);

    // Include additinal ajax commands.
    $commands[] = ajax_command_replace('#path-breadcrumbs-argument-table', drupal_render($form['table']));
    $commands[] = ctools_modal_command_dismiss();
  }

  return array('#type' => 'ajax', '#commands' => $commands);
}