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

  // Load argument settings form.
  $form = drupal_get_form('path_breadcrumbs_argument_context_form', $path_name, $keyword);
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
function path_breadcrumbs_change_arguments_page($path_name, $keyword) {
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

  $form = drupal_build_form('path_breadcrumbs_argument_context_form', $form_state);
  $commands = ctools_modal_form_render(array('title' => t('Choose argument')), $form);
  return array('#type' => 'ajax', '#commands' => $commands);
}

/**
 * Form for editing path breadcrumbs.
 */
function path_breadcrumbs_edit_page($path_breadcrumb) {

  // Save form in cache.
  path_breadcrumbs_object_cache_set($path_breadcrumb->machine_name, $path_breadcrumb);

  $links[] = array(
    '#type' => 'link',
    '#title' => t('Basic'),
    '#href' => 'admin/structure/path-breadcrumbs/edit/' . $path_breadcrumb->path_id . '/ajax/basic',
    '#attributes' => array('class' => array('use-ajax')),
  );

  $links[] = array(
    '#type' => 'link',
    '#title' => t('Arguments'),
    '#href' => 'admin/structure/path-breadcrumbs/edit/' . $path_breadcrumb->path_id . '/ajax/arguments',
    '#attributes' => array('class' => array('use-ajax')),
  );

  $links[] = array(
    '#type' => 'link',
    '#title' => t('Breadcrumbs'),
    '#href' => 'admin/structure/path-breadcrumbs/edit/' . $path_breadcrumb->path_id . '/ajax/breadcrumbs',
    '#attributes' => array('class' => array('use-ajax')),
  );

  $rendered_links = array();
  foreach ($links as $link) {
    $rendered_links[] = render($link);
  }

  $page['menu'] = array(
    '#markup' => theme('item_list', array('items' => $rendered_links)),
  );

  $form = drupal_get_form('path_breadcrumbs_edit_form', $path_breadcrumb, 1);
  $page['edit_form'] = array(
    '#markup' => render($form),
  );

  drupal_add_library('system', 'drupal.ajax');

  return $page;
}

/**
 * Ajax callback for path_breadcrumbs EDIT links.
 */
function path_breadcrumbs_edit_form_ajax_links($breadcrumb, $link_type) {

  $step = 1;
  if ($link_type == 'basic') {
    $step = 1;
  }
  elseif ($link_type == 'arguments') {
    $step = 2;
  }
  elseif ($link_type == 'breadcrumbs') {
    $step = 3;
  }

  $form = drupal_get_form('path_breadcrumbs_edit_form', $breadcrumb, $step);

  $commands = array();
  $commands[] = ajax_command_html('#path-breadcrumbs-edit-form-wrapper', render($form));
  return array('#type' => 'ajax', '#commands' => $commands);
}
