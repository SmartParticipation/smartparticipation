<?php

/**
 * Smartparticipation_core customizations to built-in page node type.
 */
 
/**
 * Custom alterations to page node type.
 */
function smartparticipation_core_alter_page_node_type() {
 
  foreach (smartparticipation_core_page_installed_instances() as $instance) {
    field_create_instance($instance);
  }

  // Pathauto alias pattern
  variable_set('pathauto_node_page_pattern', '[node:title]');
  
  // Create a revision by default when a new version is saved
  variable_set('node_options_page', array('revision', 'status'));  
  
}

/**
 * Remove custom alterations to page node type.
 */
function smartparticipation_core_restore_page_node_type() {
  
  foreach (smartparticipation_core_page_installed_instances() as $instance_name => $instance) {
    // false = don't delete the field!
    field_delete_instance($instance, false);
  }     
  
  field_purge_batch(1000);
}

/**
 * Define custom field instances for page node type.
 */
function smartparticipation_core_page_installed_instances() {

  $t = get_t();
  $site = variable_get('smartparticipation_site_machine_name');
    
  return array(
  
    'rr_page_image_instance' => array(    
      'bundle' => 'page',
      'deleted' => '0',
      'description' => '',
      'display' => array(
        'default' => array(
          'label' => 'hidden',
          'module' => 'image',
          'settings' => array(
            'image_link' => '',
            'image_style' => '',
          ),
          'type' => 'image',
          'weight' => 5,
        ),
        'teaser' => array(
          'label' => 'hidden',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0,
        ),
      ),
      'entity_type' => 'node',
      'field_name' => 'field_image',
      'label' => $t('Image'),
      'required' => 0,
      'settings' => array(
        'alt_field' => 1,
        'default_image' => 0,
        'file_directory' => "smartparticipation_core/$site/page",
        'file_extensions' => 'png gif jpg jpeg',
        'max_filesize' => '500K',
        'max_resolution' => '',
        'min_resolution' => '',
        'title_field' => 1,
        'user_register_form' => FALSE,
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'image',
        'settings' => array(
          'preview_image_style' => 'thumbnail',
          'progress_indicator' => 'throbber',
        ),
        'type' => 'image_image',
        'weight' => '15',
      ),
    ),
    
  );
}

/**
 * Module function to create the set of default page nodes. These nodes can
 * be edited through the editing interface. Because we create links to these
 * pages in the footer block and elsewhere, the pages are created here to 
 * ensure correct functioning of the links.
 */
function smartparticipation_core_create_default_page_nodes() {
  
  $path_to_403 = 'access-denied';
  $path_to_404 = 'page-not-found';
  
  $nodes = array(
    'Access denied' => $path_to_403,
    //'Contact Us' => 'contact',
    'Page not found' => $path_to_404,
    'Terms of Use & Privacy' => 'terms',
  );
  
  $nids = array();
  
  foreach ($nodes as $title => $alias) {
    $node = new stdClass();
    $node->type = 'page';
    node_object_prepare($node);
    $node->title = $title;
    $node->language = LANGUAGE_NONE;
    $node->uid = 1;
    $node->status = 1;
    $node->promote = 0;
    $node->sticky = 0;
    // Specify the alias explicitly rather than relying on pathauto. Must set to
    // FALSE, not just omit.
    $node->path['pathauto'] = FALSE;
    $node->path['alias'] = $alias;

    if ($alias === $path_to_403 || $alias === $path_to_404) {
      switch ($alias) {
        case $path_to_403:
          $file_name = '403-error.jpg';
          break;
        case $path_to_404:
          $file_name = '404-error.jpg';
          break;
      }
      $file_path = drupal_get_path('profile', 'smartparticipation') . '/themes/smartparticipation_base/images/error-pages/' . $file_name;
      // Preparing file object
      $file = (object)array(
        "uid" => 1,
        "uri" => $file_path,
        "filemime" => file_get_mimetype($file_path),
        "status" => 1
      );
      $file = file_copy($file, 'public://', FILE_EXISTS_REPLACE);
      // Set our field to use the newly copied file
      $node->field_image[LANGUAGE_NONE][] = (array)$file;
    }

    node_save($node);
    $nids[] = $node->nid;
  }
  
  // Store the node ids in the database so they can be retrieved for deletion
  // during module uninstall.
  if (! empty($nids)) {
    variable_set('smartparticipation_default_page_node_ids', join(',', $nids) );
  }

  /* We are using custom nodes for the 403 and 404 pages to work around the 
   * problem of non-display of the footer menu. See 
   * http://drupal.org/node/233807. The other option is patching Drupal core, 
   * which is undesirable.
   */
  variable_set('site_403', $path_to_403);  
  variable_set('site_404', $path_to_404);

}

function smartparticipation_core_delete_default_page_nodes() {
  
  $nids = explode(',', variable_get('smartparticipation_default_page_node_ids'));
  node_delete_multiple($nids); 
  
  variable_del('site_403');
  variable_del('site_404');
}

/** 
 * Implements hook_form_FORM_ID_alter().
 */
function smartparticipation_core_form_page_node_form_alter(&$form, &$form_state, $form_id) {
  
  /* 403 and 404 error page nodes: we don't display the body in the template,
   * so remove it from the editing form to avoid confusion.
   */
  if ($form['path']['alias']['#default_value'] == 'page-not-found' ||
      $form['path']['alias']['#default_value'] == 'access-denied') {
        $form['body']['#access'] = FALSE;
      }
}  

function smartparticipation_core_page_node_view($node, $view_mode, $langcode) {
  
  smartparticipation_core_apply_glossary_to_node($node);
  
}

