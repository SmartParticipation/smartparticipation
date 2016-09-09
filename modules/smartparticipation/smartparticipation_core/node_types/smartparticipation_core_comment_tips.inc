<?php

/**
 * @file
 * Define the announcement node type.
 *
 * @ingroup smartparticipation_core
 */

/**
 * Create the announcement node type.
 */
function smartparticipation_core_create_comment_tips_node_type() {

  $t = get_t();

  $comment_tips = array(
    'type' => 'sp_comment_tips',
    'name' => $t('Comment tips'),
    // Drupal will handle this node as if we had designed it in the UI.
    'base' => 'node_content',
    'has_title' => TRUE,
    'description' => $t('Instructional tips on how write an effective comment.  They will be displayed on a proposal topic.'),
    'custom' => TRUE,
    'help' => '',
  );

  // Set defaults not explicitly declared above.
  $node_type = node_type_set_defaults($comment_tips);

  // Add body field and set label.
  node_add_body_field($node_type);

  // Modify properties of the body field.
  $instance = field_read_instance('node', 'body', 'sp_comment_tips');
  $instance['widget']['weight'] = 35;
  field_update_instance($instance);

  node_type_save($node_type);

  // Create the fields we are adding to the announcement node type.
  foreach (smartparticipation_core_comment_tips_installed_fields() as $field) {
    $field['entity_types'] = array('node');
    field_create_field($field);
  }

  // Create all the instances for our fields.
  foreach (smartparticipation_core_comment_tips_installed_instances() as $instance) {
    $instance['bundle'] = $comment_tips['type'];
    $instance['entity_type'] = 'node';
    field_create_instance($instance);
  }

  // Disable commenting for this node type
  variable_set('comment_sp_comment_tips', COMMENT_NODE_HIDDEN);

  // Pathauto alias pattern
  variable_set('pathauto_node_sp_comment_tips_pattern', 'comment_tips/[node:title]');

  // Create a revision by default when a new version is saved
  variable_set('node_options_sp_comment_tips', array('revision', 'status'));
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
function smartparticipation_core_comment_tips_installed_fields()
{
  return array(

    'field_sp_comment_tips_phase' => array(
      'active' => '1',
      'cardinality' => '1',
      'columns' => array(
        'tid' => array(
          'type' => 'int',
          'unsigned' => TRUE,
          'not null' => FALSE,
        ),
      ),
      'deleted' => '0',
      'field_name' => 'field_sp_comment_tips_phase',
      'foreign keys' => array(
        'tid' => array(
          'table' => 'taxonomy_term_data',
          'columns' => array(
            'tid' => 'tid',
          ),
        ),
      ),
      'indexes' => array(
        'tid' => array(
          0 => 'tid',
        ),
      ),
      'locked' => '0',
      'module' => 'taxonomy',
      'settings' => array(
        'allowed_values' => array(
          0 => array(
            'vocabulary' => 'sp_proposal_phase',
            'parent' => '0',
          ),
        ),
      ),
      'translatable' => '0',
      'type' => 'taxonomy_term_reference',
    ),

    'field_sp_comment_tips_proposal' => array(
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'field_name' => 'field_sp_comment_tips_proposal',
      'foreign keys' => array(
        'nid' => array(
          'columns' => array(
            'nid' => 'nid',
          ),
          'table' => 'node',
        ),
      ),
      'indexes' => array(
        'nid' => array(
          0 => 'nid',
        ),
      ),
      'locked' => '0',
      'module' => 'node_reference',
      'settings' => array(
        'referenceable_types' => array(
          'sp_proposal' => 'sp_proposal',
        ),
        'view' => array(
          'args' => array(),
          'display_name' => '',
          'view_name' => '',
        ),
      ),
      'translatable' => '0',
      'type' => 'node_reference',
    ),

    'field_sp_comment_tips_sentence' => array(
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_sp_comment_tips_sentence',
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
      'settings' => array(),
      'translatable' => '0',
      'type' => 'text_long',
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
function smartparticipation_core_comment_tips_installed_instances()
{
  $t = get_t();

  $instances['field_sp_comment_tips_phase'] = array(
    'default_value' => NULL,
    'deleted' => '0',
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'hidden',
        'type' => 'taxonomy_term_reference_plain',
        'settings' => array(),
        'module' => 'taxonomy',
        'weight' => 2,
      ),
      'teaser' => array(
        'type' => 'hidden',
        'label' => 'above',
        'settings' => array(),
        'weight' => 0,
      ),
    ),
    'field_name' => 'field_sp_comment_tips_phase',
    'label' => 'Phase',
    'required' => 1,
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'weight' => '10',
      'type' => 'options_select',
      'module' => 'options',
      'active' => 1,
      'settings' => array(),
    ),
  );

  $instances['field_sp_comment_tips_proposal'] = array(
    'default_value' => NULL,
    'deleted' => '0',
    'description' =>$t('The proposal these comment tips belong to.'),
    'display' => array(
      'default' => array(
        'label' => 'hidden',
        'module' => 'node_reference',
        'settings' => array(
          'link' => TRUE,
        ),
        'type' => 'node_reference_default',
        'weight' => 1,
      ),
      'teaser' => array(
        'label' => 'hidden',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'field_name' => 'field_sp_comment_tips_proposal',
    'label' => $t('Proposal'),
    'required' => 1,
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'options',
      'settings' => array(),
      'type' => 'options_select',
      'weight' => '5',
    ),
  );

  $instances['field_sp_comment_tips_sentence'] = array(
    'bundle' => 'sp_comment_tips',
    'default_value' => NULL,
    'deleted' => '0',
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'text',
        'settings' => array(),
        'type' => 'text_default',
        'weight' => 3,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'entity_type' => 'node',
    'field_name' => 'field_sp_comment_tips_sentence',
    'label' => $t('Lead sentence'),
    'required' => 0,
    'settings' => array(
      'text_processing' => '0',
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'text',
      'settings' => array(
        'rows' => '2',
      ),
      'type' => 'text_textarea',
      'weight' => '4',
    ),
  );

  return $instances;
}

/**
 * Delete the announcement node type.
 *
 */
function smartparticipation_core_delete_comment_tips_node_type() {
  smartparticipation_core_delete_node_type('sp_comment_tips');
}