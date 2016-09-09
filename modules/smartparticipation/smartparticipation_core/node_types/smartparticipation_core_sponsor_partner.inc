<?php

/**
 * @file 
 * Define the sponsor_partner node type.
 * 
 * @ingroup smartparticipation_core
 */
 
/**
 * Create the sponsor_partner node type.
 */
function smartparticipation_core_create_sponsor_partner_node_type() {
  
  $t = get_t();
  
  $sponsor_partner = array(
    'type' => 'sp_sponsor_partner',
    'name' => $t('Sponsor/Partner'),
    // Drupal will handle this node as if we had designed it in the UI.
    'base' => 'node_content', 
    'has_title' => TRUE,  
    'description' => $t('Content type for describing project sponsors and partners.'),
    'custom' => TRUE,
    'help' => '',
  );  
  
  // Set defaults not explicitly declared above.
  $node_type = node_type_set_defaults($sponsor_partner);
  
  // Add body field and set label.
  node_add_body_field($node_type, $t('Description'));
  
  // Modify properties of the body field.
  $instance = field_read_instance('node', 'body', 'sp_sponsor_partner');
  $instance['widget']['weight'] = 35;
  field_update_instance($instance);
  
  node_type_save($node_type);

  // Create field instances.
  foreach (smartparticipation_core_sponsor_partner_installed_instances() as $instance) {
    $instance['bundle'] = $sponsor_partner['type'];
    $instance['entity_type'] = 'node';
    field_create_instance($instance);
  }

  // Disable commenting for this node type  
  variable_set('comment_sp_sponsor_partner', COMMENT_NODE_HIDDEN);
  
  // Create a revision by default when a new version is saved
  variable_set('node_options_sp_sponsor_partner', array('revision', 'status'));

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
function smartparticipation_core_sponsor_partner_installed_instances() {
  $t = get_t();
  $site = variable_get('smartparticipation_site_machine_name');
  
  return array(
    
    'field_sp_sponsor_partner_image' => array(
      'deleted' => '0',
      'description' => 'Optional graphic, logo, or image for the sponsor/partner.',
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
        'file_directory' => "smartparticipation_core/$site/sponsor_partner",
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

    'field_sp_sponsor_partner_website' => array(
      'default_value' => NULL,
      'deleted' => '0',   
      'description' => $t('Optional URL of sponsor/partner website.'),
      'display' => array(
        // TODO Should this be type 'hidden'? We want the data included in the
        // array sent to the template
        'default' => array(
          'label' => 'above',
          'module' => 'text',
          'settings' => array(),
          'type' => 'text_default',
          'weight' => 8,
        ),
        'teaser' => array(
          'type' => 'hidden',
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
        'weight' => '25',
      ),    
    ),
        
  );
}

/**
 * Delete the sponsor_partner node type.
 * 
 */
function smartparticipation_core_delete_sponsor_partner_node_type() {
  smartparticipation_core_delete_node_type('sp_sponsor_partner');
}

function smartparticipation_core_sponsor_partner_node_view(&$node, $view_mode, $langcode) {

  $content = $node->content;
  
  $url = $content['field_sp_url'][0]['#markup'];
  $content['url'] = $url;
  
  if (! node_is_page($node)) {
    $content['title_link'] = l(
        $node->title,
        $url,
        array(
          'attributes' => array(
            'target' => '_blank',
          ),
          'external' => true,
        )
    );
  }
  
  // Write new content back to the node
  $node->content = $content;

}

