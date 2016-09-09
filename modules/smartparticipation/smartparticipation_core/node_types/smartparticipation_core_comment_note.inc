<?php

/**
 * @file 
 * Define the comment note node type.
 * 
 * @ingroup smartparticipation_core
 */
 
/**
 * Create the comment note node type.
 */
function smartparticipation_core_create_comment_note_node_type() {
  
  $t = get_t();
  
  $comment_note = array(
    'type' => 'sp_comment_note',
    'name' => $t('Comment note'),
    // Drupal will handle this node as if we had designed it in the UI.
    'base' => 'node_content', 
    'has_title' => FALSE, 
    'description' => $t('The content type for comment notes.'),
    'custom' => TRUE,
    'help' => '',
  );  
  
  // Set defaults not explicitly declared above.
  $node_type = node_type_set_defaults($comment_note);

    // Add body field and set label.
  node_add_body_field($node_type);
  
  // Modify properties of the body field.
  $instance = field_read_instance('node', 'body', $comment_note['type']);
  // Don't allow addition of a summary.
  $instance['label'] = $t('Note');
  $instance['settings']['display_summary'] = false;
  $instance['widget']['weight'] = 30;
  field_update_instance($instance);

  node_type_save($node_type);


  // Create the fields we are adding to the comment note node type.
  foreach (smartparticipation_core_comment_note_installed_fields() as $field) {
    $field['entity_types'] = array('node');    
    field_create_field($field);
  }

  // Create all the instances for our fields.
  foreach (smartparticipation_core_comment_note_installed_instances() as $instance) {
    $instance['bundle'] = $comment_note['type'];
    $instance['entity_type'] = 'node';
    field_create_instance($instance);
  }
  
  // Comment notes do not receive comments
  variable_set('comment_sp_comment_note', COMMENT_NODE_HIDDEN);

  variable_set('node_options_sp_comment_note', array('status'));
  
}

/**
 * Delete the comment note node type.
 * 
 */
function smartparticipation_core_delete_comment_note_node_type() {
  smartparticipation_core_delete_node_type('sp_comment_note');
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
function smartparticipation_core_comment_note_installed_fields() {
  
  return array(

    'field_sp_comment_note_comment' => array(
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'field_name' => 'field_sp_comment_note_comment',
      'foreign keys' => array(
        'node' => array(
          'table' => 'node',
          'columns' => array(
            'target_id' => 'nid',
          ),
        ),
      ),
      'indexes' => array(
        'target_id' => array(
          0 => 'target_id',
        ),
      ),           
      'locked' => '0',
      'module' => 'entityreference',
      'settings' => array(
        'handler' => 'base',
        'handler_settings' => array(
          'behaviors' => array(
            'views-select-list' => array(
              'status' => 0,
            ),
          ),
          'sort' => array(
            'type' => 'none',
          ),
          'target_bundles' => array(
            'comment_node_sp_subtopic' => 'comment_node_sp_subtopic',
          ),
        ),
        'target_type' => 'comment',
      ),
      'translatable' => '0',
      'type' => 'entityreference',
    ),
      
    'field_sp_comment_emails' => array(
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'field_name' => 'field_sp_comment_emails',
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
function smartparticipation_core_comment_note_installed_instances() {
  $t = get_t();
  
  return array(

    'field_sp_comment_note_comment' => array(
      'bundle' => 'sp_comment_note',
      'default_value' => NULL,
      'default_value_function' => 'entityreference_prepopulate_field_default_value',
      'deleted' => '0',   
      'description' => '',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'entityreference',
          'settings' => array(
            'link' => FALSE,  
          ),
          'type' => 'entityreference_label',
          'weight' => 0,
        ),
        'teaser' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0,
        ),
      ),
      'entity_type' => 'node',
      'field_name' => 'field_sp_comment_note_comment',
      'label' => $t('Comment reference'),      

      'required' => 1,
      'settings' => array(
        'text_processing' => '0',
        'behaviors' => array(
          'prepopulate' => array(
            'action' => 'disable',
            'action_on_edit' => 1,
            'fallback' => 'none',
            'og_context' => 0,
            'skip_perm' => '0',
            'status' => 1,
          ),
        ),
        'user_register_form' => FALSE,
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'entityreference',
        'settings' => array(
          'match_operator' => 'CONTAINS', 
          'path' => '', 
          'size' => '60',
        ),
        'type' => 'entityreference_autocomplete',
        'weight' => '0',
      ),    
    ),
    
    'field_sp_comment_emails' => array(
      'bundle' => 'sp_comment_note',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => '',   
      'display' => array(
        'default' => array(
          'label' => 'above',
          'type' => 'text_default',
          'settings' => array(),
          'module' => 'text',
          'weight' => 1,
        ),
        'teaser' => array(
          'type' => 'hidden',
          'label' => 'above',
          'settings' => array(),
          'weight' => 0,
        ),
      ),
      'entity_type' => 'node', 
      'field_name' => 'field_sp_comment_emails',
      'label' => $t('Email addresses'),
      'required' => 0,
      'settings' => array(
        'text_processing' => '0', 
        'user_register_form' => FALSE,
      ),
      'widget' => array(
        'weight' => '1',
        'type' => 'emailwidget',
        'module' => 'html5_tools',
        'active' => 0,
        'settings' => array(),
      ),
    ),

  );
}