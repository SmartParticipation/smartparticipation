<?php

/**
 * @file 
 * Define the team_member node type.
 * 
 * @ingroup smartparticipation_core
 */
 
/**
 * Create the team_member node type.
 */
function smartparticipation_core_create_team_member_node_type() {
  
  $t = get_t();
  
  $team_member = array(
    'type' => 'sp_team_member',
    'name' => $t('Team member'),
    // Drupal will handle this node as if we had designed it in the UI.
    'base' => 'node_content', 
    'has_title' => TRUE,  
    'title_label' => $t('Full name'),
    'description' => $t('Content type to display information about team members.'),
    'custom' => TRUE,
    'help' => '',
  );  
  
  // Set defaults not explicitly declared above.
  $node_type = node_type_set_defaults($team_member);
  
  // Add body field and set label.
  node_add_body_field($node_type);
  
  // Modify properties of the body field.
  $instance = field_read_instance('node', 'body', 'sp_team_member');
  $instance['label'] = $t('Description');
  $instance['widget']['weight'] = 35;
  field_update_instance($instance);
  
  node_type_save($node_type);

  // Create the fields we are adding to the team_member node type.
  foreach (smartparticipation_core_team_member_installed_fields() as $field) {
    $field['entity_types'] = array('node');    
    field_create_field($field);
  }
  
  // Create field instances.
  foreach (smartparticipation_core_team_member_installed_instances() as $instance) {
    $instance['bundle'] = $team_member['type'];
    $instance['entity_type'] = 'node';
    field_create_instance($instance);
  }

  // Disable commenting for this node type  
  variable_set('comment_sp_team_member', COMMENT_NODE_HIDDEN);
  
  // Create a revision by default when a new version is saved
  variable_set('node_options_sp_team_member', array('revision', 'status'));

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
function smartparticipation_core_team_member_installed_fields() {
  
  $fields = array();
      
  $fields['field_sp_team_member_title'] = array(
    'active' => '1',
    'cardinality' => '1',
    'deleted' => '0',
    'entity_types' => array(),
    'field_name' => 'field_sp_team_member_title',
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
  
  return $fields;

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
function smartparticipation_core_team_member_installed_instances() {
  
  $t = get_t();
  $site = variable_get('smartparticipation_site_machine_name');
  
  $instances = array();
          
  $instances['field_sp_team_member_image'] = array(
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
    'label' => $t('Image'),
    'required' => 0,
    'settings' => array(
      'alt_field' => 1,
      'default_image' => 0,
      'file_directory' => "smartparticipation_core/$site/team_members/images",
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
  );
  
  $instances['field_sp_team_member_title'] = array(
    'default_value' => NULL,
    'deleted' => '0',   
    'description' => $t('The professional title of the team member.'),
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
    'field_name' => 'field_sp_team_member_title',
    'label' => $t('Title'),      
    'required' => 0,
    'settings' => array(
      'text_processing' => '0',
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'text',
      'settings' => array(
        'size' => '150',
      ),
      'type' => 'text_textfield',
      'weight' => '10',
    ),    
  );
      
  $instances['field_sp_team_member_website'] = array(
    'default_value' => NULL,
    'deleted' => '0',
    'description' => $t('Optional website for the team member.'),
    'display' => array(
      'default' => array(
        'label' => 'hidden',
        'module' => 'text',
        'settings' => array(),
        'type' => 'text_default',
        'weight' => 8,
      ),
      'teaser' => array(
        'label' => 'hidden',
        'module' => 'text',
        'settings' => array(),
        'type' => 'text_default',
        'weight' => 8,
      ),
    ),
    'field_name' => 'field_sp_url',
    'label' => $t('Website'),
    'required' => 0,
    'settings' => array(
      'text_processing' => '0',
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 0,
      'module' => 'html5_tools',
      'settings' => array(),
      'type' => 'urlwidget',
      'weight' => '20',
    ),
  );
        
  return $instances;
}



/**
 * Delete the team_member node type.
 * 
 */
function smartparticipation_core_delete_team_member_node_type() {
  smartparticipation_core_delete_node_type('sp_team_member');
}

/**
 * Called from smartparticipation_core_preprocess_page() to customize smartparticipation_core_team_member node 
 * page.
 */
function smartparticipation_core_preprocess_team_member_page(&$variables) {
  
  $node = $variables['node'];
  
  // Set the page title to the team member's full name
  $full_name = smartparticipation_core_team_member_full_name($node);
  drupal_set_title($full_name);

}

/**
 * Helper function to construct full name from title (last name) and first
 * name field.
 */
function smartparticipation_core_team_member_full_name($node) {

  $title = $node->title; // e.g., "Doe, John"
  $names = explode(', ', $title);
  return implode(' ', array_reverse($names));
  
}


/**
 * Customize sp_team_member node view.
 */
function smartparticipation_core_team_member_node_view(&$node, $view_mode, $langcode) {

  $content = $node->content;

  if (isset($content['field_sp_url'])) {
    $url = $content['field_sp_url'][0]['#markup'];
  }
  
  if (! node_is_page($node)) {
      
    $full_name = smartparticipation_core_team_member_full_name($node);
    
    if (isset($url)) {
      
      $content['full_name'] = l(
          $full_name,
          $url,
          array(
            'attributes' => array(
              'target' => '_blank',
            ),
            'external' => true,
          )
      );
      
    } else {
      
      $content['full_name'] = $full_name;
    }
    
  // Full page view
  } else if (isset($url)) {

    $content['website'] = l(
        t('View website'),
        $url,
        array(
          'attributes' => array(
            'target' => '_blank',
            'title' => t('View website'),
          ),
          'external' => true,
        )          
        
    ); 
  } 
  
  // Write new content back to the node
  $node->content = $content;

}



/**
 * Implements hook_form_FORM_ID_alter().
 */
function smartparticipation_core_form_sp_team_member_node_form_alter(&$form, &$form_state, $form_id) {

  $form['title']['#size'] = 60;
  $form['title']['#description'] = 
      t('IMPORTANT: Enter as "lastname, firstname", including the comma. A middle initial or name can be included. A single name is also possible. Examples: Adams, John; Adams, John Q.; Adams, John Quincy; Socrates.');
  
}