<?php

/**
 * @file
 * Administrative callbacks for PATH BREADCRUMBS UI module.
 */

/**
 * Page callback for module settings page.
 */
function path_breadcrumbs_ui_breadcrumbs_list($form, $form_state) {

  // Load path breadcrumbs.
  $result = db_select('path_breadcrumbs', 'p')
    ->fields('p', array('path_id', 'name', 'path', 'machine_name', 'weight', 'disabled'))
    ->orderBy('p.disabled')
    ->orderBy('p.weight')
    ->execute();

  $form['#tree'] = TRUE;

  foreach ($result as $path) {
    $form[$path->path_id]['title']['#markup'] = $path->name;
    $form[$path->path_id]['name']['#markup'] = $path->machine_name;
    $form[$path->path_id]['path']['#markup'] = $path->path;

    // Create operations for current breadcrumb.
    $operations = array();

    if ($path->disabled == TRUE) {
      $operations[] = array(
        'title' => t('Enable'),
        'href' => 'admin/structure/path-breadcrumbs/enable/' . $path->path_id,
      );
    }

    $operations[] = array(
      'title' => t('Edit'),
      'href' => 'admin/structure/path-breadcrumbs/edit/' . $path->path_id,
    );
    $operations[] = array(
      'title' => t('Clone'),
      'href' => 'admin/structure/path-breadcrumbs/clone/' . $path->path_id,
    );
    $operations[] = array(
      'title' => t('Export'),
      'href' => 'admin/structure/path-breadcrumbs/export/' . $path->path_id,
    );
    $operations[] = array(
      'title' => t('Delete'),
      'href' => 'admin/structure/path-breadcrumbs/delete/' . $path->path_id,
    );

    if ($path->disabled == FALSE) {
      $operations[] = array(
        'title' => t('Disable'),
        'href' => 'admin/structure/path-breadcrumbs/disable/' . $path->path_id,
      );
    }

    $form[$path->path_id]['actions']['#markup'] = theme('links__ctools_dropbutton',
      array('links' => $operations, 'attributes' => array('class' => array('links', 'inline'))));

    $form[$path->path_id]['weight'] = array(
      '#type' => 'weight',
      '#default_value' => $path->weight,
      '#delta' => 30,
      '#attributes' => array('class' => array('path-breadcrumbs-ui-table-weight')),
    );

    $form[$path->path_id]['disabled'] = array(
      '#type' => 'value',
      '#value' => $path->disabled,
    );
  }

  $form['actions'] = array(
    '#type' => 'action',
  );

  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );

  $form['#attached']['css'][] = drupal_get_path('module', 'path_breadcrumbs_ui') . '/css/path_breadcrumbs_ui.css';

  return $form;
}

/**
 * Submit callback for path_breadcrumbs_ui_breadcrumbs_list form.
 */
function path_breadcrumbs_ui_breadcrumbs_list_submit($form, &$form_state) {
  foreach ($form_state['values'] as $path_id => $value) {
    if (isset($value['weight'])) {
      db_update('path_breadcrumbs')
        ->fields(array(
        'weight' => $value['weight'],
      ))
        ->condition('path_id', $path_id)
        ->execute();
    }
  }

  drupal_set_message(t('Path breadcrumbs was updated.'));
}

/**
 * Path breadcrumbs settings form.
 */
function path_breadcrumbs_ui_settings($form, $form_state) {

  $form['path_breadcrumbs_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Breadcrumbs settings'),
  );

  $form['path_breadcrumbs_settings']['path_breadcrumbs_delimited'] = array(
    '#type' => 'textfield',
    '#title' => t('Delimiter'),
    '#default_value' => variable_get('path_breadcrumbs_delimited', '»'),
    '#size' => 3,
    '#description' => t('Symbol that separates breadcrumbs.'),
  );

  $form['path_breadcrumbs_settings']['path_breadcrumbs_rdfa_enabled'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable RDFa for breadcrumbs'),
    '#default_value' => variable_get('path_breadcrumbs_rdfa_enabled', 0),
    '#description' => t('This checkbox provide microformats support for breadcrumbs. It is important to build a structure of a site in the SERP.')
  );

  $form['path_breadcrumbs_settings']['path_breadcrumbs_hide_single_breadcrumb'] = array(
    '#type' => 'checkbox',
    '#title' => t('Hide breadcrumbs navigation for single breadcrumb'),
    '#default_value' => variable_get('path_breadcrumbs_hide_single_breadcrumb', 0),
    '#description' => t('If breacrumbs navigation contains only one breadcrumb than breadcrumb navigation will be hidden.'),
  );

  $internal_render = variable_get('path_breadcrumbs_internal_render', 1);
  if (!$internal_render) {
    $form['path_breadcrumbs_settings']['path_breadcrumbs_delimited']['#disabled'] = TRUE;
    $form['path_breadcrumbs_settings']['path_breadcrumbs_rdfa_enabled']['#disabled'] = TRUE;
    $form['path_breadcrumbs_settings']['path_breadcrumbs_hide_single_breadcrumb']['#disabled'] = TRUE;
    $notice = '<div style="color:red">' . t('You are not able to use this feature until internal render is disabled.') . '</div>';
    $form['path_breadcrumbs_settings']['path_breadcrumbs_delimited']['#description'] .= $notice;
    $form['path_breadcrumbs_settings']['path_breadcrumbs_rdfa_enabled']['#description'] .= $notice;
    $form['path_breadcrumbs_settings']['path_breadcrumbs_hide_single_breadcrumb']['#description'] .= $notice;
  }

  $form['path_breadcrumbs_settings']['path_breadcrumbs_internal_render'] = array(
    '#type' => 'checkbox',
    '#title' => t('Use module breadcrumbs render function'),
    '#default_value' => variable_get('path_breadcrumbs_internal_render', 1),
    '#description' => t("If this value checked module will replace system or theme breadcrumb render function by module's one.<br/>Note that if this checkbox is disabled module will not be able to implement settings above.")
  );

  $form['path_breadcrumbs_home_link'] = array(
    '#type' => 'fieldset',
    '#title' => t('Home link settings'),
  );

  $form['path_breadcrumbs_home_link']['path_breadcrumbs_home_link_enabled'] = array(
    '#type' => 'checkbox',
    '#title' => t('Home link should be prepend to breadcrumbs navigation by default'),
    '#default_value' => variable_get('path_breadcrumbs_home_link_enabled', 1),
    '#description' => t('This value does not changes current breadcrumbs navigation. It is just set default value for new path breadcrumbs.'),
  );

  $form['path_breadcrumbs_home_link']['path_breadcrumbs_home_link_title'] = array(
    '#type' => 'textfield',
    '#title' => t('Home link title'),
    '#default_value' => variable_get('path_breadcrumbs_home_link_title', 'Home'),
    '#description' => t('Title of the link that points to the front page.'),
    '#size' => 30,
  );

  $form['#submit'][] = 'path_breadcrumbs_ui_settings_submit';

  return system_settings_form($form);
}

/**
 * Submit function for path_breadcrumbs_ui_settings form.
 */
function path_breadcrumbs_ui_settings_submit($form, &$form_state) {
  // Flush theme registry if internal render was changed to enable/disable system render in theme_breadcrumb() function.
  $internal_render_new_value = $form_state['values']['path_breadcrumbs_internal_render'];
  $internal_render_old_value = variable_get('path_breadcrumbs_internal_render', 1);
  if ($internal_render_new_value != $internal_render_old_value) {
    drupal_theme_rebuild();
  }
}

/**
 * Form for path breadcrumbs clone.
 */
function path_breadcrumbs_clone_breadcrumb($path_breadcrumb) {
  $path_breadcrumb->machine_name .= '_clone';
  unset($path_breadcrumb->path_id);
  path_breadcrumbs_object_cache_set($path_breadcrumb->machine_name, $path_breadcrumb);
  $form_state = array('storage' => array('machine_name' => $path_breadcrumb->machine_name));
  return drupal_build_form('path_breadcrumbs_ui_add_form', $form_state);
}

/**
 * Form for object export.
 */
function path_breadcrumbs_export_form($form, $form_state, $path_breadcrumb) {

  drupal_set_title(t('Export path breadcrumb "!name"', array('!name' => $path_breadcrumb->name)));

  ctools_include('export');
  $objects = ctools_export_load_object('path_breadcrumbs', 'all', array($path_breadcrumb->path_id));

  if (empty($objects[$path_breadcrumb->path_id])) {
    return t('It is not possible to export path breadcrumb right now. May be you need to flush site cache.');
  }

  $object = $objects[$path_breadcrumb->path_id];
  $code = ctools_export_crud_export('path_breadcrumbs', $object);
  $lines = substr_count($code, "\n");

  $form['export'] = array(
    '#title' => t('Export data'),
    '#type' => 'textarea',
    '#value' => $code,
    '#rows' => $lines,
    '#description' => t('Copy the export text and paste it into import area.'),
  );

  return $form;
}

/**
 * Form for object import.
 */
function path_breadcrumbs_import_form($form, $form_state) {

  $form['import'] = array(
    '#type' => 'textarea',
    '#title' => t('Paste code here to import path breadcrumbs'),
  );

  $form['actions'] = array(
    '#type' => 'action',
  );

  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Import'),
  );

  return $form;
}

/**
 * Submit callback for path breadcrumbs import form.
 */
function path_breadcrumbs_import_form_submit($form, &$form_state) {
  $code = $form_state['values']['import'];
  ctools_include('export');

  $path_breadcrumb = ctools_export_crud_import('path_breadcrumbs', $code);

  $result = db_merge('path_breadcrumbs')
    ->key(array(
      'machine_name' => $path_breadcrumb->machine_name
    ))
    ->fields(array(
      'name' => $path_breadcrumb->name,
      'path' => $path_breadcrumb->path,
      'data' => $path_breadcrumb->data,
      'weight' => $path_breadcrumb->weight,
      'disabled' => $path_breadcrumb->disabled,
    ))
    ->execute();

  if ($result == 1) {
    drupal_set_message(filter_xss(t('Path breadcrumb "!name" was successfully imported.', array('!name' => $path_breadcrumb->name))));
  }
  elseif ($result == 2) {
    drupal_set_message(filter_xss(t('Path breadcrumb "!name" was successfully updated.', array('!name' => $path_breadcrumb->name))));
  }
  else {
    drupal_set_message(t('Could not import path breadcrumb.', 'error'));
  }

  $form_state['redirect'] = 'admin/structure/path-breadcrumbs';
}
