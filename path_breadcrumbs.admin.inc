<?php

/**
 * @File
 * Administrative callbacks for PATH BREADCRUMBS module.
 */

/**
 * Page callback for module settings page.
 */
function path_breadcrumbs_admin_page() {

  // Load path breadcrumbs.
  $result = db_select('path_breadcrumbs', 'p')
    ->fields('p', array('path_id', 'name', 'path'))
    ->execute();

  $rows = array();
  foreach ($result as $path) {
    $row    = array();
    $row[]  = $path->name;
    $row[]  = $path->path;
    $row[]  = theme('item_list', array('items' => array(
        l(t('Edit'), 'admin/structure/path-breadcrumbs/edit/' . $path->path_id),
        l(t('Delete'), 'admin/structure/path-breadcrumbs/delete/' . $path->path_id),
      ),
    ));
    $rows[] = $row;
  }

  if ($rows) {
    // Build table if data exists.
    $headers = array(t('Name'), t('Path'), t('Actions'));
    $output = array(
      '#theme' => 'table',
      '#header' => $headers,
      '#rows' => $rows,
    );
  }
  else {
    // Return message if no data exists.
    $output = t('There are no any path created yet.');
  }

  return $output;
}
