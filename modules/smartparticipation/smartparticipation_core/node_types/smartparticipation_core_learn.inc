<?php

/**
 * @file 
 * Define the learn node type.
 * 
 * @ingroup smartparticipation_core
 */
 
/**
 * Create the learn node type.
 */
function smartparticipation_core_create_learn_node_type() {
  
  $t = get_t();
  
  $learn = array(
    'type' => 'sp_learn',
    'name' => $t('Learn page'),
    // Drupal will handle this node as if we had designed it in the UI.
    'base' => 'node_content', 
    'has_title' => TRUE,  
    'description' => $t('Content type for site learn pages.'),
    'custom' => TRUE,
    'help' => '',
  );  
  
  // Set defaults not explicitly declared above.
  $node_type = node_type_set_defaults($learn);
  
  // Add body field and set label.
  node_add_body_field($node_type);
  
  // Modify properties of the body field.
  $instance = field_read_instance('node', 'body', 'sp_learn');
  $instance['widget']['weight'] = 35;
  field_update_instance($instance);
  
  node_type_save($node_type);

  // Create the fields we are adding to the learn node type.
  foreach (smartparticipation_core_learn_installed_fields() as $field) {
    $field['entity_types'] = array('node');
    field_create_field($field);
  }

  // Create all the instances for our fields.
  foreach (smartparticipation_core_learn_installed_instances() as $instance) {
    $instance['bundle'] = $learn['type'];
    $instance['entity_type'] = 'node';
    field_create_instance($instance);
  }
  
  // Disable commenting for this node type  
  variable_set('comment_sp_learn', COMMENT_NODE_HIDDEN);

  // Pathauto alias pattern  
  variable_set('pathauto_node_sp_learn_pattern', 'learn/[node:field_sp_menu_title]');

  // Create a revision by default when a new version is saved
  variable_set('node_options_sp_learn', array('revision', 'status'));
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

function smartparticipation_core_learn_installed_fields() {

  return array(

    'field_sp_learn_teaser' => array(
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_sp_learn_teaser',
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
    ),

    'field_sp_display_in_learn_panel' => array(
          'translatable' => '0',
          'entity_types' => array(),
          'settings' => array(
              'allowed_values' => array(
                  'No',
                  'Yes',
              ),
              'allowed_values_function' => '',
          ),
          'storage' => array(
              'type' => 'field_sql_storage',
              'settings' => array(),
              'module' => 'field_sql_storage',
              'active' => '1',
              'details' => array(
                  'sql' => array(
                      'FIELD_LOAD_CURRENT' => array(
                          'field_data_field_sp_display_in_learn_panel' => array(
                              'value' => 'field_sp_display_in_learn_panel_value',
                          ),
                      ),
                      'FIELD_LOAD_REVISION' => array(
                          'field_revision_field_sp_display_in_learn_panel' => array(
                              'value' => 'field_sp_display_in_learn_panel_value',
                          ),
                      ),
                  ),
              ),
          ),
          'foreign keys' => array(),
          'indexes' => array(
              'value' => array(
                  'value',
              ),
          ),
          'field_name' => 'field_sp_display_in_learn_panel',
          'type' => 'list_boolean',
          'module' => 'list',
          'active' => '1',
          'locked' => '0',
          'cardinality' => '1',
          'deleted' => '0',
          'columns' => array(
              'value' => array(
                  'type' => 'int',
                  'not null' => FALSE,
              ),
          ),
          'bundles' => array(
              'node' => array(
                'sp_learn',
              ),
          ),
      ),


    'field_sp_learn_video' => array(
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_sp_learn_video',
      'foreign keys' => array(
        'fid' => array(
          'columns' => array(
            'fid' => 'fid',
          ),
          'table' => 'file_managed',
        ),
      ),
      'indexes' => array(
        'fid' => array(
          0 => 'fid',
        ),
      ),
      'locked' => '0',
      'module' => 'file',
      'settings' => array(
        'display_default' => 0,
        'display_field' => 0,
        'uri_scheme' => 'public',
      ),
      'translatable' => '0',
      'type' => 'file',
    ),
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
function smartparticipation_core_learn_installed_instances() {
  $t = get_t();
  $site = variable_get('smartparticipation_site_machine_name');
  
  return array(
    'field_sp_learn_teaser' => array(
      'default_value' => NULL,
      'deleted' => '0', 
      'description' => 'This teaser text will be displayed in the learn carousel.',
      'display' => array(
        'default' => array(
          'type' => 'hidden',
        ),
        'teaser' => array(
          'type' => 'hidden',
        ),
      ), 
      'field_name' => 'field_sp_learn_teaser',
      'label' => $t('Teaser'),
      'required' => 1,
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
        'weight' => '5',
      ),   
    ),

    'field_sp_display_in_learn_panel' => array(
          'label' => 'Display in learn panel',
          'widget' => array(
              'weight' => '3',
              'type' => 'options_buttons',
              'module' => 'options',
              'active' => 1,
              'settings' => array(),
          ),
          'settings' => array(
              'user_register_form' => FALSE,
          ),
          'display' => array(
              'default' => array(
                  'label' => 'above',
                  'type' => 'hidden',
                  'weight' => '9',
                  'settings' => array(),
              ),
              'teaser' => array(
                  'type' => 'hidden',
                  'label' => 'above',
                  'settings' => array(),
                  'weight' => 0,
              ),
          ),
          'required' => 1,
          'description' => 'Should this be displayed in learn panel accordion on front page?',
          'default_value' => array(
              array(
                  'value' => '1',
              ),
          ),
          'field_name' => 'field_sp_display_in_learn_panel',
          'entity_type' => 'node',
      'bundle' => 'sp_learn',
          'deleted' => '0',
      ),


   'field_sp_learn_image' => array(
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
        'file_directory' => "smartparticipation_core/$site/learn/images",
        'file_extensions' => 'png gif jpg jpeg',
        'max_filesize' => '300K',
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
    
    'field_sp_learn_video' => array(
      'deleted' => '0',
      'description' => '',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'file',
          'settings' => array(),
          'type' => 'file_default',
          'weight' => 7,
        ),
        'teaser' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0,
        ),
      ),
      'entity_type' => 'node',
      'field_name' => 'field_sp_learn_video',
      'label' => $t('Video'),
      'required' => 0,
      'settings' => array(
        'description_field' => 0,
        'file_directory' => "smartparticipation_core/$site/learn/videos",
        'file_extensions' => 'mp4',
        'max_filesize' => '100M',
        'user_register_form' => FALSE,
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'media',
        'settings' => array(
          'allowed_schemes' => array(
            'public' => 'public',
          ),
          'allowed_types' => array(
            'audio' => 0,
            'default' => 0,
            'image' => 'image',
            'video' => 0,
          ),
          'progress_indicator' => 'throbber',
        ),
        'type' => 'media_generic',
        'weight' => '25',
      ),
    ),
    
    'field_sp_menu_title' => array(
      'default_value' => NULL,
      'deleted' => '0', 
      'description' => $t('Alternate short title used in accordion tabs and the page\'s default URL.'),
      'display' => array(
        'default' => array(
          'type' => 'hidden',
        ),
        'teaser' => array(
          'type' => 'hidden',
        ),
      ), 
      'field_name' => 'field_sp_menu_title',
      'label' => $t('Tab title'),
      'required' => 1,
      'settings' => array(
        'text_processing' => '0',
        'user_register_form' => FALSE,
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'text',
        'settings' => array(
          'size' => '75',
        ),
        'type' => 'text_textfield',
        'weight' => '0',
      ),   
    ),
  );
}

/**
 * Delete the learn node type.
 * 
 */
function smartparticipation_core_delete_learn_node_type() {
  smartparticipation_core_delete_node_type('sp_learn');
}

function _smartparticipation_core_preprocess_learn_node(&$variables) {
  $variables['force_node_title'] = TRUE;
}
/**
 * Assemble data for the learn node. This node type uses the default node
 * template. 
 */
function smartparticipation_core_learn_node_view($node, $view_mode, $langcode) {
 
  // Get the learn panel data. Assemble it like the learn panel block on the
  // home page.
  // TODO Can probably be done with module_invoke() to get the defined block.
  $learn_nodes[] = smartparticipation_core_learn_panel_node_data($node);

  $node->content['panel'] = array(
    '#items' => $learn_nodes, 
    '#theme' => 'learn_panel',
    '#weight' => 0,
    'single_node_view' => TRUE,
  );
  
  $node->content['body']['#weight'] = 10;
  
  hide($node->content['field_image']);
  hide($node->content['field_sp_learn_teaser']);
  hide($node->content['field_sp_learn_video']);

}

/**
 * Assemble the data needed to display the learn panel, both for the learn 
 * pages themselves and the home page panel that displays data for all 
 * learn pages.
 */
function smartparticipation_core_learn_panel_node_data($node) {
  
  $image_uri = smartparticipation_core_field_get_item_value('node', $node, 'field_image', 'uri');
  $image = $image_uri ? file_create_url($image_uri) : '';
  
  $teaser = smartparticipation_core_field_get_item_value('node', $node, 'field_sp_learn_teaser', 'value');

  $display_in_learn_panel = smartparticipation_core_field_get_item_value('node', $node, 'field_sp_display_in_learn_panel', 'value');

  $video_uri = smartparticipation_core_field_get_item_value('node', $node, 'field_sp_learn_video', 'uri');
  $video = $video_uri ? file_create_url($video_uri) : '';
  
  $tab_title = smartparticipation_core_field_get_item_value('node', $node, 'field_sp_menu_title', 'value');
  
  $url = url('node/' . $node->nid);
  
  return array(
    'image' => $image,
    'nid' => $node->nid,
    'tab_title' => $tab_title,
    'teaser' => $teaser,
    'display_in_learn_panel' => $display_in_learn_panel,
    'title' => $node->title,
    'url' => $url,
    'video' => $video,
  );  
}


// TODO this (roughly) should replace smartparticipation_core_autocreatemenu() and the
// switch statement at the top of smartparticipation_core_automenu_automenu_create(). See 
// notes there. The code needs to be tested and debugged, however.
function smartparticipation_core_learn_autocreatemenu($node) {
  $params = smartparticipation_core_automenu_automenu_create($node, 'Learn');
  $last_weight = _smartparticipation_core_get_last_menu_item_weight($node->type);
  smartparticipation_core_automenu_add_menu_link($params, 8);
}
