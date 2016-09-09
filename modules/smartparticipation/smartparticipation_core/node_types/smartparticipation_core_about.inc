<?php

/**
 * @file 
 * Define the about node type.
 * 
 * @ingroup smartparticipation_core
 */
 
/**
 * Create the about node type.
 */
function smartparticipation_core_create_about_node_type() {
  
  $t = get_t();
  
  $about = array(
    'type' => 'sp_about',
    'name' => $t('About page'),
    // Drupal will handle this node as if we had designed it in the UI.
    'base' => 'node_content', 
    'has_title' => TRUE,  
    'description' => $t('Content type for site about pages.'),
    'custom' => TRUE,
    'help' => '',
  );  
  
  // Set defaults not explicitly declared above.
  $node_type = node_type_set_defaults($about);
  
  // Add body field and set label.
  node_add_body_field($node_type);
  
  // Modify properties of the body field.
  $instance = field_read_instance('node', 'body', 'sp_about');
  $instance['widget']['weight'] = 35;
  field_update_instance($instance);
  
  node_type_save($node_type);

  // Create the fields we are adding to the node type.
  foreach (smartparticipation_core_about_installed_fields() as $field) {
    $field['entity_types'] = array('node');
    field_create_field($field);
  }
  
  // Create field instances.
  foreach (smartparticipation_core_about_installed_instances() as $instance) {
    $instance['bundle'] = $about['type'];
    $instance['entity_type'] = 'node';
    field_create_instance($instance);
  }

  // Disable commenting for this node type  
  variable_set('comment_sp_about', COMMENT_NODE_HIDDEN);
  
  // Pathauto alias pattern
  variable_set('pathauto_node_sp_about_pattern', 'about/[node:title]');

  // Create a revision by default when a new version is saved
  variable_set('node_options_sp_about', array('revision', 'status'));

}

/**
 * Returns a structured array defining the fields created by this node type.
 *
 * This is factored into a function so it can be used in both
 * smartparticipation_core_install() and smartparticipation_core_uninstall().
 *
 * @return
 *  An associative array specifying the fields we wish to add to our
 *  new node type.
 *
 */
function smartparticipation_core_about_installed_fields() {
  
  return array(
    'field_sp_about_intro' => smartparticipation_core_about_intro_field(),
    'field_sp_about_node_list_heading' => smartparticipation_core_about_node_list_heading_field(),
  );
  
}

/**
 * Return an array specifying the field_sp_about_intro field.
 */
function smartparticipation_core_about_intro_field() {
  
  return array(
    'active' => 1,
    'cardinality' => 1,
    'deleted' => 0,
    'entity_types' => array('node'),
    'field_name' => 'field_sp_about_intro',
    'foreign keys' => array(
      'format' => array(
        'columns' => array(
            'format' => 'format',
        ),
        'table' => 'filter_format',
      ),
    ),
    'indexes' => array(
      'format' => array(
          0 => 'format',
      ),
    ),
    'locked' => 0,
    'module' => 'text',
    'settings' => array(),
    'translatable' => 0,
    'type' => 'text_long',
  );
}

/**
 * Returns an array specifying the field_sp_about_node_list_heading field.
 */
function smartparticipation_core_about_node_list_heading_field() {
  
  return array(
    'active' => '1',
    'cardinality' => '1',
    'deleted' => '0',
    'entity_types' => array('node'),
    'field_name' => 'field_sp_about_node_list_heading',
    'foreign keys' => array(
      'format' => array(
        'columns' => array(
          'format' => 'format',
        ),
        'table' => 'filter_format',
      ),
    ),
    'indexes' => array(
      'format' => array(
        0 => 'format',
      ),
    ),
    'locked' => '0',
    'module' => 'text',
    'settings' => array(
      'max_length' => '255',
    ),
    'translatable' => '0',
    'type' => 'text',      
  );
}

/**
 * Returns a structured array defining the instances for this node type.
 *
 * The instance lets Drupal know which widget to use to allow the user to enter
 * data and how to react in different view modes.  
 *
 * This is factored into a function so it can be used in both
 * smartparticipation_core_install() and smartparticipation_core_uninstall().
 *
 * @return
 *  An associative array specifying the instances we wish to add to our new
 *  node type.
 *
 */
function smartparticipation_core_about_installed_instances() {
  $t = get_t();
  $site = variable_get('smartparticipation_site_machine_name');
  
  $instances = array();

  $instances['field_sp_about_image'] = array(
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
      'field_name' => 'field_image',
      'label' => $t('Main image'),
      'required' => 0,
      'settings' => array(
        'alt_field' => 1,
        'default_image' => 0,
        'file_directory' => "smartparticipation_core/$site/about",
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
        'weight' => 0,
      ),
    );
  
    $instances['field_sp_about_intro'] = smartparticipation_core_about_intro_instance();
    
    $instances['field_sp_about_node_list_heading'] = smartparticipation_core_about_node_list_heading_instance();

    $instances['field_sp_secondary_image'] = smartparticipation_core_about_secondary_image_instance();
    
    return $instances;

}

/**
 * Define the intro field instance for the about node type.
 */
function smartparticipation_core_about_intro_instance() {

  $t = get_t();
  
  return array(
    'bundle' => 'sp_about',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => $t('Some About templates allow for intro text. Currently displayed only on Meet the Team page.'),
    'display' => array(
      'default' => array(
        'label' => 'hidden',
        'module' => 'text',
        'settings' => array(),
        'type' => 'text_default',
        'weight' => 7,
      ),
      'teaser' => array(
        'label' => 'hidden',
        'settings' => array(),
        'type' => 'text_default',
        'weight' => 7,
      ),
    ),
    'entity_type' => 'node',
    'field_name' => 'field_sp_about_intro',
    'label' => $t('Intro'),
    'required' => 0,
    'settings' => array(
      'text_processing' => 1,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'text',
      'settings' => array(
          'rows' => '10',
      ),
      'type' => 'text_textarea',
      'weight' => 10,
    ),    
      
  );
}

/**
 * Define the node_list_heading field instance for the about node type.
 */
function smartparticipation_core_about_node_list_heading_instance() {

  $t = get_t();
  
  return array(
    'bundle' => 'sp_about',
    'default_value' => NULL,
    'deleted' => '0',
    'description' => $t('An optional heading to appear above the displayed node list.'),
    'display' => array(
      'default' => array(
        'label' => 'hidden',
        'type' => 'text_plain',
      ),
      'teaser' => array(
        'label' => 'hidden',
        'type' => 'text_plain',
      ),
    ), 
    'entity_type' => 'node',
    'field_name' => 'field_sp_about_node_list_heading',
    'label' => $t('Display Heading'),
    'required' => 0,
    'settings' => array(
      'text_processing' => '0',
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'text',
      'settings' => array(
        'size' => 60,
      ),
      'type' => 'text_textfield',
      'weight' => 15,
    ),     
  );
  
}

/**
 * Define the secondary image field instance for the about node type.
 */
function smartparticipation_core_about_secondary_image_instance() {

  $t = get_t();
  $site = variable_get('smartparticipation_site_machine_name');
  
  return array(
    'bundle' => 'sp_about',
    'deleted' => '0',
    'description' => $t('Some About templates allow for a secondary image. Currently displayed only on Media page.'),
    'display' => array(
      'default' => array(
        'label' => 'hidden',
        'module' => 'image',
        'settings' => array(
          'image_link' => '',
          'image_style' => '',
        ),
        'type' => 'image',
        'weight' => 6,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'entity_type' => 'node',
    'field_name' => 'field_sp_secondary_image',
    'label' => $t('Secondary image'),
    'required' => 0,
    'settings' => array(
      'alt_field' => 1,
      'default_image' => 0,
      'file_directory' => "smartparticipation_core/$site/about",
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
      'weight' => 5,
    ),    
  );
}


/**
 * Delete the about node type.
 */
function smartparticipation_core_delete_about_node_type() {
  smartparticipation_core_delete_node_type('sp_about');
}


// TODO this (roughly) should replace smartparticipation_core_autocreatemenu() and the
// switch statement at the top of smartparticipation_core_automenu_automenu_create. See 
// notes there. The code needs to be tested and debugged, however.
function smartparticipation_core_about_autocreatemenu($node) {
  if($node->sticky == 0) {
    $last_weight = _smartparticipation_core_get_last_about_menu_item_weight();
    $params = smartparticipation_core_automenu_automenu_create($node, 'About', $last_weight);
    smartparticipation_core_automenu_add_menu_link($params, 0);
  }
}

/**
 * About page node views.
 */
function smartparticipation_core_about_node_view($node, $view_mode, $langcode) {
  
  $request_path = request_path(); 
  $path_parts = explode('/', $request_path);
  
  $node_view_function = 'smartparticipation_core_' . str_replace('-', '_', $path_parts[1]) . '_node_view';
  if (function_exists($node_view_function)) {
    // NB call_user_func doesn't pass by reference, so these functions must
    // return a value.
    $node->node_data = call_user_func($node_view_function, $node, $view_mode, $langcode);
  }
  
}

/**
 * Media node view: display all news nodes.
 */
function smartparticipation_core_media_node_view($node, $view_mode, $langcode) {

  // Fetch the data
  $query = 'SELECT nid FROM {node} n
            INNER JOIN {field_data_field_sp_news_date} d ON n.nid = d.entity_id
            WHERE n.type = :type
            AND n.status = :status
            ORDER BY d.field_sp_news_date_value DESC';

  $nids = db_query($query, array(
      ':status' => 1,
    ':type' => 'sp_news',
    ))
    ->fetchCol();


  // Add the news nodes to the media node content array
  $news_items = array();
  foreach ($nids as $nid) {
    $news_items[] = node_view(node_load($nid));
  }

  $node->content['news_items'] = $news_items;

}

/**
 * Research and Publications node view: display all research and publication
 * nodes.
 */
function smartparticipation_core_research_and_publications_node_view($node, $view_mode, $langcode) {
  
  // PUBLICATIONS
  // Fetch the data
  $query = 'SELECT nid FROM {node} n
            INNER JOIN {field_data_field_sp_publication_year} y ON n.nid = y.entity_id
            WHERE n.type = :type
            AND n.status = :status
            ORDER BY y.field_sp_publication_year_value DESC';
  
   $nids = db_query($query, array(
      ':status' => 1,
     ':type' => 'sp_publication',
     ))
     ->fetchCol();
   
  // Add the nodes to the content array
  $publications = array();
  foreach ($nids as $nid) {
    $publications[] = node_view(node_load($nid));
  }
  
  $node->content['publications'] = $publications;  
  
  
  // RESEARCH
  // Fetch the data
  $query = 'SELECT nid FROM {node} n
            WHERE n.type = :type
            AND n.status = :status
            ORDER BY n.title';
  
  $nids = db_query($query, array(
      ':status' => 1,
    ':type' => 'sp_research',
    ))
    ->fetchCol();
  
  // Add the nodes to the content array
  $research = array();
  foreach ($nids as $nid) {
    $research[] = node_view(node_load($nid));
  }
  
  $node->content['research'] = $research;
    
}

/**
 * Sponsors and Partners node view: display all sponsor_partner nodes.
 */
function smartparticipation_core_sponsors_and_partners_node_view($node, $view_mode, $langcode) {

  // Fetch the data
  $query = 'SELECT nid FROM {node}
            WHERE type = :type
            AND status = :status
            ORDER BY title';
  
  $nids = db_query($query, array(
      ':status' => 1,
    ':type' => 'sp_sponsor_partner',
    ))
    ->fetchCol();

  // Add the nodes to the content array
  $sponsors_partners = array();
  foreach ($nids as $nid) {
    $sponsors_partners[] = node_view(node_load($nid));
  }
  
  $node->content['sponsors_partners'] = $sponsors_partners;

}

/**
 * Meet the Team node view: display all smartparticipation_core_team_member nodes.
 */
function smartparticipation_core_meet_team_node_view($node, $view_mode, $langcode) {

  // Fetch the data
  $query = 'SELECT n.nid FROM {node} n
            WHERE n.type = :type
            AND n.status = :status
            ORDER BY n.title ASC';
  
  $nids = db_query($query, array(
    ':type' => 'sp_team_member',
      ':status' => 1,
  ))
  ->fetchCol();
  
  // Add the nodes to the content array
  $team_members = array();
  foreach ($nids as $nid) {
    $team_members[] = node_view(node_load($nid));
  }
  
  $node->content['team_members'] = $team_members;
    
}


/**
 * Implements hook_form_FORM_ID_alter().
 */
function smartparticipation_core_form_sp_about_node_form_alter(&$form, &$form_state, $form_id) {

  // Editing an existing node
  if (isset($form['nid']['#value'])) {
    
    $alias = $form['path']['alias']['#default_value'];
    $path_parts = explode('/', $alias);
    $node_alias = $path_parts[1];
       
  // Adding a new node 
  } else {
    $node_alias = '';
  }

  /* 
   * We can use the $node_alias to figure out which about node is being edited.
   * Note that this value is generated automatically and cannot be manually
   * altered. This gives editors the freedom to change the node title but not
   */
  // Secondary image field is displayed only on the media page at this time.
  if ($node_alias != 'media') {
    $form['field_sp_secondary_image']['#access'] = false;
  }
  
  // Intro field is displayed only on the Meet the Team page at this time.
  if ($node_alias != 'meet-team') {
    $form['field_sp_about_intro']['#access'] = false;
    $form['field_sp_about_node_list_heading']['#access'] = false;
  } else {
    // TODO Set default filter on field_sp_about_intro to 'full_html'. Can this
    // be done on a per-field basis?
  }


}

/**
 * Return the 'About' links block.
 */
function smartparticipation_core_about_links() {

  $links = array();

  $menu_links = menu_load_links('main-menu');

  $active_mlids = array_map(
    function ($link) {
      if (!empty($link['mlid'])) {
        return $link['mlid'];
      }
    }, menu_get_active_trail()
  );

  foreach ($menu_links as $ml) {
    if ($ml['link_title'] == 'About') {
      $plid = $ml['mlid'];
      break;
    }
  }

  foreach ($menu_links as $ml) {
    if ($ml['plid'] == $plid) {
      $options = array();
      if (in_array($ml['mlid'], $active_mlids)) {
        $options = array('attributes' => array('class' => 'active'));
      }
      $links[] = l(t($ml['link_title']), drupal_get_path_alias($ml['link_path']), $options);
    }
  }


  $block['content'] = array(
      'list' => array(
         '#theme' => 'about_links',
         '#items' => array('links'=>$links),
      ),
  );

  return $block;

}
