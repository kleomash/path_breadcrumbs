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

  drupal_set_message(t('Breadcrumbs variants weight was updated.'));
}

/**
 * Theming function for path_breadcrumbs_ui_breadcrumbs_list form.
 */
function theme_path_breadcrumbs_ui_breadcrumbs_list($vars) {

  $form = $vars['form'];
  $rows = array();
  foreach (element_children($form) as $path_id) {
    if (isset($form[$path_id]['weight'])) {
      $row = array();
      $row[] = render($form[$path_id]['title']);
      $row[] = render($form[$path_id]['name']);
      $row[] = render($form[$path_id]['path']);
      $row[] = render($form[$path_id]['actions']);
      $row[] = render($form[$path_id]['weight']);

      $class = 'enabled';
      if ($form[$path_id]['disabled']['#value'] == TRUE) {
        $class = 'disabled';
      }

      $rows[] = array(
        'data' => $row,
        'class' => array('draggable', $class),
      );
    }
  }

  if (empty($rows)) {
    $rows = array(array(
      'data' => array(array('data' => t('There are no created path breadcrumbs yet. '), 'colspan' => 5))
    ));
  }

  $header = array(t('Title'), t('Name'), t('Path'), t('Actions'), t('Weight'));
  $output = theme('table', array('header' => $header, 'rows' => $rows, 'attributes' => array('id' => 'path-breadcrumbs-ui-table-list')));
  drupal_add_tabledrag('path-breadcrumbs-ui-table-list', 'order', 'sibling', 'path-breadcrumbs-ui-table-weight');
  return $output . drupal_render_children($form);
}
