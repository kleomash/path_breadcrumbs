<?php

/**
 * @File
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
    '#description' => t('Title of the link that points to the front page.')
  );

  /*$form['path_breadcrumbs_rdfa'] = array(
    '#type' => 'fieldset',
    '#title' => t('Microformats')
  );

  $form['path_breadcrumbs_rdfa']['path_breadcrumbs_rdfa_enabled'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable RDFa for breadcrumbs'),
    '#default_value' => variable_get('path_breadcrumbs_rdfa_enabled', 0),
  );*/

  return system_settings_form($form);
}
