<?php

/**
 * @file 
 * Define the proposal node type.
 * 
 * @ingroup smartparticipation_core
 */
 
/**
 * Create the proposal node type.
 */
function smartparticipation_core_create_proposal_node_type() {
  
  $t = get_t();
  
  $proposal = array(
    'type' => 'sp_proposal',
    'name' => $t('Proposal'),
    // Drupal will handle this node as if we had designed it in the UI.
    'base' => 'node_content', 
    'has_title' => TRUE, 
    'title_label' => $t('Title'),   
    'description' => $t('A document, such as a rule or plan, that is the subject of discussion.'),
    'custom' => TRUE, 
    'help' => '',
  );  
  
  // Set defaults not explicitly declared above.
  $node_type = node_type_set_defaults($proposal);
  
    // Add body field and set label.
  node_add_body_field($node_type);
  
  // Modify properties of the body field.
  $instance = field_read_instance('node', 'body', 'sp_proposal');
  $instance['label'] = $t('Summary');
  $instance['widget']['weight'] = 20;
  field_update_instance($instance);

  node_type_save($node_type);

  // Create the fields we are adding to the proposal node type.
  foreach (smartparticipation_core_proposal_installed_fields() as $field) { 
    field_create_field($field);
  }

  // Create all the instances for our fields.
  foreach (smartparticipation_core_proposal_installed_instances() as $instance) {
    // NB These are not the correct values for the field_collection field
    // instances.
    //$instance['bundle'] = $proposal['type'];
    //$instance['entity_type'] = 'node';
    field_create_instance($instance);
  }
  
  smartparticipation_core_configure_proposal_phase_field_collection();
  
  // Proposals do not receive comments directly.
  variable_set('comment_sp_proposal', COMMENT_NODE_HIDDEN);
  
  // Create revisions by default when editing. Save unpublished by default.
  variable_set('node_options_sp_proposal', array('revision'));
 
  // Pathauto alias pattern
  $proposal_type = 
    variable_get('smartparticipation_site_proposal_type_plural', 'proposals');
  variable_set('pathauto_node_sp_proposal_pattern',
    $proposal_type . '/[node:title]');  
}

/**
 * Module function to add records to variable table to support the 
 * field_sp_proposal_phase field collection on smartparticipation_core_proposal.
 * 
 * TODO Not sure if this is really needed for the field collection to function. 
 * When adding the field collection via the UI, these variables get set, so we
 * model that here.
 * 
 */
function smartparticipation_core_configure_proposal_phase_field_collection() {

  $value = array(
    'view_modes' => array(),
    'extra_fields' => array(
      'form' => array(),
      'display' => array(),
    ),
  );
  variable_set('field_bundle_settings_field_collection_item__field_sp_proposal_phase', $value);
  
  $value = array(
    'view_modes' => array(),
    'extra_fields' => array(
      'form' => array(
        'title' => array(
          'weight' => '-5',
        ),
        'path' => array(
          'weight' => '30',
        ),
      ),
      'display' => array(),
    ),  
  );
  variable_set('field_bundle_settings_node__sp_proposal', $value);
} 

/**
 * Delete the proposal node type.
 * 
 */
function smartparticipation_core_delete_proposal_node_type() {
  smartparticipation_core_delete_node_type('sp_proposal');
}


/**
 *
 * This is factored into a function so it can be used in both
 * smartparticipation_core_install() and smartparticipation_core_uninstall().
 *
 * @return
 *  An associative array specifying the fields we wish to add to our
 *  new node type.
 *
 */
function smartparticipation_core_proposal_installed_fields() {
  
  $t = get_t();
  
  $fields = array(     
        
    // PROPOSAL DOCUMENTS
    // This is a corresponding node reference field, and will not be edited 
    // from the proposal form.
    'field_sp_proposal_document' => array(
      'active' => '1',
      'cardinality' => '-1',
      'deleted' => '0',
      'entity_types' => array('node'),     
      'field_name' => 'field_sp_proposal_document',
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
          'sp_document' => 'sp_document',
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
    'field_sp_document_label' => array(
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'field_name' => 'field_sp_document_label',
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

    // ANNOUNCEMENTS
    // This is a corresponding node reference field, and will not be edited 
    // from the proposal form.
    'field_sp_proposal_announcement' => array(
      'active' => '1',
      'cardinality' => '-1',
      'deleted' => '0',
      'entity_types' => array('node'),     
      'field_name' => 'field_sp_proposal_announcement',
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
          'sp_announcement' => 'sp_announcement',
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

    // CLOSED PROPOSAL IMAGE
    'field_sp_proposal_image_closed' => array(
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array('node'),
      'field_name' => 'field_sp_proposal_image_closed',
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
      'module' => 'image',
      'settings' => array(
        'default_image' => 0,
        'uri_scheme' => 'public',
      ),
      'translatable' => '0',
      'type' => 'image',
    ),
 
    // STATUS (open vs. closed)
    'field_sp_proposal_status' => array(
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array('node'),
      'field_name' => 'field_sp_proposal_status',
      'foreign keys' => array(),
      'indexes' => array(
        'value' => array(
          0 => 'value',
        ),
      ),
      'locked' => '0',
      'module' => 'list',
      'settings' => array(
        'allowed_values' => array(
          0 => $t('Closed'),
          1 => $t('Open'),
        ),
        'allowed_values_function' => '',
      ),
      'translatable' => '0',
      'type' => 'list_boolean',
    ),  

    // NEWS
    // This is a corresponding node reference field, and will not be edited 
    // from the proposal form.
    'field_sp_proposal_news' => array(
      'active' => '1',
      'cardinality' => '-1',
      'deleted' => '0',
      'entity_types' => array('node'),     
      'field_name' => 'field_sp_proposal_news',
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
          'sp_news' => 'sp_news',
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
              
    // PROPOSAL TOPICS
    // This is a corresponding node reference field, and will not be edited 
    // from the proposal.    
    'field_sp_proposal_topic' => array(
      'active' => '1',
      'cardinality' => '-1',
      'deleted' => '0',
      'entity_types' => array('node'),
      'field_name' => 'field_sp_proposal_topic',
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
          'sp_topic' => 'sp_topic',
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

    // SURVEY
    'field_sp_interest_survey' => array(
      'active' => '1',
      'cardinality' => '-1',
      'deleted' => '0',
      'entity_types' => array('node'),     
      'field_name' => 'field_sp_interest_survey',
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
          'sp_interest_survey' => 'sp_interest_survey',
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
    
    'field_sp_closed_proposal_summary' =>
      smartparticipation_core_closed_proposal_summary_field(),
  );
 
  $fields += smartparticipation_core_proposal_phase_installed_fields();
  
  return $fields;
}

/**
 * Defines the closed proposal summary field.
 */
function smartparticipation_core_closed_proposal_summary_field() {
  
  return array(
    'active' => 1,
    'cardinality' => 1,
    'deleted' => 0,
    'entity_types' => array('node'),
    'field_name' => 'field_sp_proposal_summary_closed',
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
 * Returns a structured array defining the fields for the proposal phase
 * field collection.
 *
 */
function smartparticipation_core_proposal_phase_installed_fields() {

  return array(

    // PROPOSAL PHASE (a proposal phase with start and end dates)
    'field_sp_proposal_phase' => array(
      'active' => '1',
      'cardinality' => '-1',
      'deleted' => '0',
      'entity_types' => array('node'),
      'field_name' => 'field_sp_proposal_phase',
      'foreign keys' => array(),
      'indexes' => array(),
      'locked' => '0',
      'module' => 'field_collection',
      'settings' => array(
        'path' => '',
      ),
      'translatable' => '0',
      'type' => 'field_collection',
    ),
    
    // PROPOSAL PHASE TERM (reference to a proposal phase vocabulary term)
    'field_sp_proposal_phase_term' => array(
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array('field_collection_item'),
      'field_name' => 'field_sp_proposal_phase_term',
      'foreign keys' => array(
        'tid' => array(
          'columns' => array(
            'tid' => 'tid',
          ),
          'table' => 'taxonomy_term_data',
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

    // PROPOSAL PHASE START AND END DATES    
    'field_sp_proposal_phase_dates' => array(
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array('field_collection_item'),
      'field_name' => 'field_sp_proposal_phase_dates',
      'foreign keys' => array(),
      'indexes' => array(),
      'locked' => '0',
      'module' => 'date',
      'settings' => array(
        'cache_count' => '4',
        'cache_enabled' => 0,
        'granularity' => array(
          'day' => 'day',
          'hour' => 'hour',
          'minute' => 'minute',
          'month' => 'month',
          'second' => 0,
          'year' => 'year',
        ),
        // TODO Make sure the timezone values are correct
        'timezone_db' => '',
        'todate' => 'optional',
        'tz_handling' => 'none',
      ),
      'translatable' => '0',
      'type' => 'datetime',
    ),

      // Comment Bubble Phase Open Text
      'field_sp_phase_open_text' => array(
          'translatable' => '0',
          'entity_types' => array(),
          'settings' => array(
              'max_length' => '30',
          ),
          'storage' => array(
              'type' => 'field_sql_storage',
              'settings' => array(),
              'module' => 'field_sql_storage',
              'active' => '1',
              'details' => array(
                  'sql' => array(
                      'FIELD_LOAD_CURRENT' => array(
                          'field_data_field_sp_phase_open_text' => array(
                              'value' => 'field_sp_phase_open_text_value',
                              'format' => 'field_sp_phase_open_text_format',
                          ),
                      ),
                      'FIELD_LOAD_REVISION' => array(
                          'field_revision_field_sp_phase_open_text' => array(
                              'value' => 'field_sp_phase_open_text_value',
                              'format' => 'field_sp_phase_open_text_format',
                          ),
                      ),
                  ),
              ),
          ),
          'foreign keys' => array(
              'format' => array(
                  'table' => 'filter_format',
                  'columns' => array(
                      'format' => 'format',
                  ),
              ),
          ),
          'indexes' => array(
              'format' => array(
                  'format',
              ),
          ),
          'field_name' => 'field_sp_phase_open_text',
          'type' => 'text',
          'module' => 'text',
          'active' => '1',
          'locked' => '0',
          'cardinality' => '1',
          'deleted' => '0',
          'columns' => array(
              'value' => array(
                  'type' => 'varchar',
                  'length' => '30',
                  'not null' => FALSE,
              ),
              'format' => array(
                  'type' => 'varchar',
                  'length' => 255,
                  'not null' => FALSE,
              ),
          ),
          'bundles' => array(
              'field_collection_item' => array(
                  'field_sp_proposal_phase',
              ),
          ),
      ),

      // Comment Bubble Phase Closed Text
      'field_sp_phase_closed_text' => array(
          'translatable' => '0',
          'entity_types' => array(),
          'settings' => array(
              'max_length' => '30',
          ),
          'storage' => array(
              'type' => 'field_sql_storage',
              'settings' => array(),
              'module' => 'field_sql_storage',
              'active' => '1',
              'details' => array(
                  'sql' => array(
                      'FIELD_LOAD_CURRENT' => array(
                          'field_data_field_sp_phase_closed_text' => array(
                              'value' => 'field_sp_phase_closed_text_value',
                              'format' => 'field_sp_phase_closed_text_format',
                          ),
                      ),
                      'FIELD_LOAD_REVISION' => array(
                          'field_revision_field_sp_phase_closed_text' => array(
                              'value' => 'field_sp_phase_closed_text_value',
                              'format' => 'field_sp_phase_closed_text_format',
                          ),
                      ),
                  ),
              ),
          ),
          'foreign keys' => array(
              'format' => array(
                  'table' => 'filter_format',
                  'columns' => array(
                      'format' => 'format',
                  ),
              ),
          ),
          'indexes' => array(
              'format' => array(
                  'format',
              ),
          ),
          'field_name' => 'field_sp_phase_closed_text',
          'type' => 'text',
          'module' => 'text',
          'active' => '1',
          'locked' => '0',
          'cardinality' => '1',
          'deleted' => '0',
          'columns' => array(
              'value' => array(
                  'type' => 'varchar',
                  'length' => '30',
                  'not null' => FALSE,
              ),
              'format' => array(
                  'type' => 'varchar',
                  'length' => 255,
                  'not null' => FALSE,
              ),
          ),
          'bundles' => array(
              'field_collection_item' => array(
                  'field_sp_proposal_phase',
              ),
          ),
      ),

      // Phase Display Text
      'field_sp_phase_display_text' => array(
          'translatable' => '0',
          'entity_types' => array(),
          'settings' => array(
              'max_length' => '100',
          ),
          'storage' => array(
              'type' => 'field_sql_storage',
              'settings' => array(),
              'module' => 'field_sql_storage',
              'active' => '1',
              'details' => array(
                  'sql' => array(
                      'FIELD_LOAD_CURRENT' => array(
                          'field_data_field_sp_phase_display_text' => array(
                              'value' => 'field_sp_phase_display_text_value',
                              'format' => 'field_sp_phase_display_text_format',
                          ),
                      ),
                      'FIELD_LOAD_REVISION' => array(
                          'field_revision_field_sp_phase_display_text' => array(
                              'value' => 'field_sp_phase_display_text_value',
                              'format' => 'field_sp_phase_display_text_format',
                          ),
                      ),
                  ),
              ),
          ),
          'foreign keys' => array(
              'format' => array(
                  'table' => 'filter_format',
                  'columns' => array(
                      'format' => 'format',
                  ),
              ),
          ),
          'indexes' => array(
              'format' => array(
                  'format',
              ),
          ),
          'field_name' => 'field_sp_phase_display_text',
          'type' => 'text',
          'module' => 'text',
          'active' => '1',
          'locked' => '0',
          'cardinality' => '1',
          'deleted' => '0',
          'columns' => array(
              'value' => array(
                  'type' => 'varchar',
                  'length' => '100',
                  'not null' => FALSE,
              ),
              'format' => array(
                  'type' => 'varchar',
                  'length' => 255,
                  'not null' => FALSE,
              ),
          ),
          'bundles' => array(
              'field_collection_item' => array(
                  'field_sp_proposal_phase',
              ),
          ),
      ),

      // Phase Subtopic Heading
      'field_sp_phase_subtopic_heading' => array(
          'translatable' => '0',
          'entity_types' => array(),
          'settings' => array(
              'max_length' => '255',
          ),
          'storage' => array(
              'type' => 'field_sql_storage',
              'settings' => array(),
              'module' => 'field_sql_storage',
              'active' => '1',
              'details' => array(
                  'sql' => array(
                      'FIELD_LOAD_CURRENT' => array(
                          'field_data_field_sp_phase_subtopic_heading' => array(
                              'value' => 'field_sp_phase_subtopic_heading_value',
                              'format' => 'field_sp_phase_subtopic_heading_format',
                          ),
                      ),
                      'FIELD_LOAD_REVISION' => array(
                          'field_revision_field_sp_phase_subtopic_heading' => array(
                              'value' => 'field_sp_phase_subtopic_heading_value',
                              'format' => 'field_sp_phase_subtopic_heading_format',
                          ),
                      ),
                  ),
              ),
          ),
          'foreign keys' => array(
              'format' => array(
                  'table' => 'filter_format',
                  'columns' => array(
                      'format' => 'format',
                  ),
              ),
          ),
          'indexes' => array(
              'format' => array(
                  'format',
              ),
          ),
          'field_name' => 'field_sp_phase_subtopic_heading',
          'type' => 'text',
          'module' => 'text',
          'active' => '1',
          'locked' => '0',
          'cardinality' => '1',
          'deleted' => '0',
          'columns' => array(
              'value' => array(
                  'type' => 'varchar',
                  'length' => '255',
                  'not null' => FALSE,
              ),
              'format' => array(
                  'type' => 'varchar',
                  'length' => 255,
                  'not null' => FALSE,
              ),
          ),
          'bundles' => array(
              'field_collection_item' => array(
                  'field_sp_proposal_phase',
              ),
          ),
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
function smartparticipation_core_proposal_installed_instances() {
  $t = get_t();
  $site = variable_get('smartparticipation_site_machine_name');
  
  $instances = array(
    
    'field_sp_document_label' => array(
      'bundle' => 'sp_proposal',
      'deleted' => '0',
      'description' => $t('Label used for proposal documents links, menus, and other displays. Defaults to the site value.'),  
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
      'field_name' => 'field_sp_document_label',
      'label' => 'Agency documents display name',
      'required' => 1,
      'settings' => array(
        'text_processing' => '0', 
        'user_register_form' => FALSE,
      ),
      'widget' => array(
        'weight' => '1',
        'type' => 'text_textfield',
        'active' => 0,
        'settings' => array(
          'size' => '40',
          'max_length' => '100',
        ),
      ),
    ),

    /* Not using this for now: changing main menu link text is complicated, and
     * if the menu title is used in the URL it doesn't extend to child nodes 
     * (topics and documents). Tried installing subpathauto for this purpose
     * but it doesn't work - perhaps because these are not "known" subpaths.
     * Will require additional effort, so postponing.
    'field_sp_menu_title' => array(
      'bundle' => 'smartparticipation_core_proposal',
      'default_value' => NULL,
      'deleted' => '0', 
      'description' => $t('Alternate short title used in menus and the proposal\'s default URL.'),
      'display' => array(
        'default' => array(
          'type' => 'hidden',
        ),
        'teaser' => array(
          'type' => 'hidden',
        ),
      ), 
      'entity_type' => 'node', 
      'field_name' => 'field_sp_menu_title',
      'label' => $t('Menu title'),
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
    */
    
    // AGENCY DOCUMENTS
    // This is a corresponding node reference field instance, and will not be 
    // edited  from the proposal.
    'field_sp_proposal_document' => array(
      'bundle' => 'sp_proposal',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => 'Label used to reference and display proposal documents. Use lowercase.',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'node_reference',
          'settings' => array(),
          'type' => 'node_reference_default',
          'weight' => 4,
        ),
        'teaser' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0,
        ),
      ),
      'entity_type' => 'node',
      'field_name' => 'field_sp_proposal_document',
      'label' => $t('Agency document'),
      'required' => 0,
      'settings' => array(
        'user_register_form' => FALSE,
      ),
      'widget' => array(
        'active' => 0,
        'module' => 'options',
        'settings' => array(),
        'type' => 'options_select',
        'weight' => '0',
      ),
    ),

    // ANNOUNCEMENTS
    // This is a corresponding node reference field instance, and will not be 
    // edited  from the proposal.
    'field_sp_proposal_announcement' => array(
      'bundle' => 'sp_proposal',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => '',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'node_reference',
          'settings' => array(),
          'type' => 'node_reference_default',
          'weight' => 4,
        ),
        'teaser' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0,
        ),
      ),
      'entity_type' => 'node',
      'field_name' => 'field_sp_proposal_announcement',
      'label' => $t('Announcements'),
      'required' => 0,
      'settings' => array(
        'user_register_form' => FALSE,
      ),
      'widget' => array(
        'active' => 0,
        'module' => 'options',
        'settings' => array(),
        'type' => 'options_select',
        'weight' => '0',
      ),
    ),
    
    // CLOSED PROPOSAL IMAGE
    'field_sp_proposal_image_closed' => array(
      'bundle' => 'sp_proposal',
      'deleted' => '0',
      'description' => $t('This image displays in the list of closed proposals.'),
      'display' => array(
        'default' => array(
          'label' => 'hidden',
          'module' => 'image',
          'settings' => array(
            'image_link' => '',
            'image_style' => '',
          ),
          'type' => 'image',
          'weight' => 7,
        ),
        'teaser' => array(
          'label' => 'hidden',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0,
        ),
      ),
      'entity_type' => 'node',
      'field_name' => 'field_sp_proposal_image_closed',
      'label' => $t('Closed image'),
      'required' => 0,
      'settings' => array(
        'alt_field' => 1,
        'default_image' => 0,
        'file_directory' => "smartparticipation_core/$site/proposals/images",
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
        'weight' => '23',
      ),
    ),
    
    // MAIN IMAGE
    'field_sp_proposal_image_main' => array(
      'bundle' => 'sp_proposal',
      'deleted' => '0',
      'description' => $t('This image displays on the home page and proposal page, and on the proposal\'s topic and document pages.'),
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
      'label' => $t('Main image'),
      'required' => 0,
      'settings' => array(
        'alt_field' => 1,
        'default_image' => 0,
        'file_directory' => "smartparticipation_core/$site/proposals/images",
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

    // NEWS
    // This is a corresponding node reference field instance, and will not be 
    // edited  from the proposal.
    'field_sp_proposal_news' => array(
      'bundle' => 'sp_proposal',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => '',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'node_reference',
          'settings' => array(),
          'type' => 'node_reference_default',
          'weight' => 4,
        ),
        'teaser' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0,
        ),
      ),
      'entity_type' => 'node',
      'field_name' => 'field_sp_proposal_news',
      'label' => $t('News'),
      'required' => 0,
      'settings' => array(
        'user_register_form' => FALSE,
      ),
      'widget' => array(
        'active' => 0,
        'module' => 'options',
        'settings' => array(),
        'type' => 'options_select',
        'weight' => '0',
      ),
    ),
    
    // STATUS (open / closed)
    'field_sp_proposal_status' => array(
      'bundle' => 'sp_proposal',
      'default_value' => array(
        0 => array(
          'value' => '1',
        ),
      ),
      'deleted' => '0',
      'description' => $t('Determines whether a proposal displays as open or closed. Topics cannot be added to closed proposals.'),
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'list',
          'settings' => array(),
          'type' => 'list_default',
          'weight' => 10,
        ),
        'teaser' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0,
        ),
      ),
      'entity_type' => 'node',
      'field_name' => 'field_sp_proposal_status',
      'label' => $t('Status'),
      'required' => 1,
      'settings' => array(
        'user_register_form' => FALSE,
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'options',
        'settings' => array(),
        'type' => 'options_buttons',
        'weight' => '5',
      ),
    ),
        
    // PROPOSAL TOPICS
    // This is a corresponding node reference field instance, and will not be 
    // edited from the proposal.    
   'field_sp_proposal_topic' => array(
     'bundle' => 'sp_proposal',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => '',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'node_reference',
          'settings' => array(),
          'type' => 'node_reference_default',
          'weight' => 4,
        ),
        'teaser' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0,
        ),
      ),
      'entity_type' => 'node',
      'field_name' => 'field_sp_proposal_topic',
      'label' => $t('Topic'),
      'required' => 0,
      'settings' => array(
        'user_register_form' => FALSE,
      ),
      'widget' => array(
        'active' => 0,
        'module' => 'options',
        'settings' => array(),
        'type' => 'options_select',
        'weight' => '0',
      ),
    ),
        

    // SURVEY
    'field_sp_interest_survey' => array(
      'bundle' => 'sp_proposal',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => '',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'node_reference',
          'settings' => array(),
          'type' => 'node_reference_default',
          'weight' => 5,
        ),
        'teaser' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0,
        ),
      ),
      'entity_type' => 'node',
      'field_name' => 'field_sp_interest_survey',
      'label' => 'Interest survey',
      'required' => 0,
      'settings' => array(
        'user_register_form' => FALSE,
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'options',
        'settings' => array(),
        'type' => 'options_select',
        'weight' => '0',
      ),
    ),   
    
    'field_sp_proposal_summary_closed' => smartparticipation_core_closed_proposal_summary_instance(),
    
    'field_sp_url' => smartparticipation_core_proposal_url_instance(),
  );
  
  $instances +=  smartparticipation_core_proposal_phase_installed_instances();
  
  return $instances;
}

/**
 * Specify an instance of field_sp_url to store closed proposal archive URL,
 * if the closed proposal resides on an external site.
 */
function smartparticipation_core_proposal_url_instance() {
  
  $t = get_t();
  
  return array(
    'bundle' => 'sp_proposal',
    'default_value' => NULL,
    'deleted' => '0',   
    'description' => $t('Add an archive URL for a closed proposal if and only if the closed proposal resides on an external site.'),
    'display' => array(
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
    'entity_type' => 'node',
    'field_name' => 'field_sp_url',
    'label' => $t('Archive URL'),      
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
      'weight' => 27,
    ),    
  );
}

function smartparticipation_core_closed_proposal_summary_instance() {

  $t = get_t();
  
  return array(
    'bundle' => 'sp_proposal',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => $t('The summary to display in the closed proposal block.'),
    'display' => array(
      'default' => array(
        'type' => 'hidden',
        ),
        'teaser' => array(
          'type' => 'hidden',
        ),
    ),
    'entity_type' => 'node',
    'field_name' => 'field_sp_proposal_summary_closed',
    'label' => $t('Closed proposal summary'),
    'required' => 0,
    'settings' => array(
      'text_processing' => 1,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'text',
      'settings' => array(
        'rows' => '5',
      ),
      'type' => 'text_textarea',
      'weight' => 25,
    ),
  );

}

/**
 * Returns a structured array defining the instances of the proposal phase 
 * field collection.
 *
 *
 * @return
 *  An associative array specifying the instances.
 *
 */
function smartparticipation_core_proposal_phase_installed_instances() {
  $t = get_t();

  return array(
    
    // PROPOSAL PHASE
    'field_sp_proposal_phase' => array(
      'bundle' => 'sp_proposal',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => $t('Add the phases this proposal will have. Start and end dates can be added when known.'),
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'field_collection',
          'settings' => array(
            'view_mode' => 'full',
          ),
          'type' => 'field_collection_fields',
          'weight' => 4,
        ),
        'teaser' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0,
        ),
      ),
      'entity_type' => 'node',
      'field_name' => 'field_sp_proposal_phase',
      'label' => $t('Proposal phases'),
      'required' => 1,
      'settings' => array(
        'user_register_form' => FALSE,
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'field_collection',
        'settings' => array(),
        'type' => 'field_collection_embed',
        'weight' => '10',
      ),
    ),

    // PROPOSAL PHASE TERM
    'field_sp_proposal_phase_term' => array(
      // NB NOT 'smartparticipation_core_proposal'
      'bundle' => 'field_sp_proposal_phase',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => '',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'taxonomy',
          'settings' => array(),
          'type' => 'taxonomy_term_reference_link',
          'weight' => 0,
        ),
      ),
      'entity_type' => 'field_collection_item',
      'field_name' => 'field_sp_proposal_phase_term',
      'label' => $t('Phase'),
      'required' => 1,
      'settings' => array(
        'user_register_form' => FALSE,
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'options',
        'settings' => array(),
        'type' => 'options_select',
        'weight' => '0',
      ),
    ),

    // PROPOSAL PHASE START AND END DATES
    'field_sp_proposal_phase_dates' => array(
      // NB NOT 'smartparticipation_core_proposal'
      'bundle' => 'field_sp_proposal_phase',
      'deleted' => '0',
      'description' => '',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'date',
          'settings' => array(
            'format_type' => 'long',
            'fromto' => 'both',
            'multiple_from' => '',
            'multiple_number' => '',
            'multiple_to' => '',
          ),
          'type' => 'date_default',
          'weight' => 1,
        ),
      ),
      'entity_type' => 'field_collection_item',
      'field_name' => 'field_sp_proposal_phase_dates',
      'label' => $t('Start and end dates'),
      'required' => 0,
      'settings' => array(
        'default_value' => 'blank', 
        'default_value2' => 'blank', 
        'default_value_code' => '',
        'default_value_code2' => '',
        'user_register_form' => FALSE,
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'date',
        'settings' => array(
          'increment' => '15',
          'input_format' => 'n/j/Y - g:ia', // 1/3/2010 6:06am
          'input_format_custom' => '',
          'label_position' => 'above',
          'text_parts' => array(),
          'year_range' => '-1:+3',
        ),
        'type' => 'date_popup',
        'weight' => '2',
      ),
    ),

      // Comment Bubble Phase Open Text
      'field_sp_phase_open_text' => array(
          'label' => 'Phase open text',
          'widget' => array(
              'weight' => '3',
              'type' => 'text_textfield',
              'module' => 'text',
              'active' => 1,
              'settings' => array(
                  'size' => '60',
              ),
          ),
          'settings' => array(
              'text_processing' => '0',
              'user_register_form' => FALSE,
          ),
          'display' => array(
              'default' => array(
                  'label' => 'above',
                  'type' => 'text_default',
                  'settings' => array(),
                  'module' => 'text',
                  'weight' => 2,
              ),
          ),
          'required' => 1,
          'description' => 'Bubble Text for this phase when open',
          'default_value' => array(
              array(
                  'value' => 'Comment On',
              ),
          ),
          'field_name' => 'field_sp_phase_open_text',
          'entity_type' => 'field_collection_item',
          'bundle' => 'field_sp_proposal_phase',
          'deleted' => '0',
      ),

      // Comment Bubble Phase Closed Text
      'field_sp_phase_closed_text' => array(
          'label' => 'Phase closed text',
          'widget' => array(
              'weight' => '4',
              'type' => 'text_textfield',
              'module' => 'text',
              'active' => 1,
              'settings' => array(
                  'size' => '60',
              ),
          ),
          'settings' => array(
              'text_processing' => '0',
              'user_register_form' => FALSE,
          ),
          'display' => array(
              'default' => array(
                  'label' => 'above',
                  'type' => 'text_default',
                  'settings' => array(),
                  'module' => 'text',
                  'weight' => 3,
              ),
          ),
          'required' => 1,
          'description' => 'Bubble text for this phase when closed',
          'default_value' => array(
              array(
                  'value' => 'Read',
              ),
          ),
          'field_name' => 'field_sp_phase_closed_text',
          'entity_type' => 'field_collection_item',
          'bundle' => 'field_sp_proposal_phase',
          'deleted' => '0',
      ),

      // Phase Display Text
      'field_sp_phase_display_text' => array(
          'label' => 'Phase display text',
          'widget' => array(
              'weight' => '1',
              'type' => 'text_textfield',
              'module' => 'text',
              'active' => 1,
              'settings' => array(
                  'size' => '60',
              ),
          ),
          'settings' => array(
              'text_processing' => '0',
              'user_register_form' => FALSE,
          ),
          'display' => array(
              'default' => array(
                  'label' => 'above',
                  'type' => 'text_default',
                  'weight' => '1',
                  'settings' => array(),
                  'module' => 'text',
              ),
          ),
          'required' => 0,
          'description' => 'Display text for phase.  Leave blank to use phase name.',
          'default_value' => NULL,
          'field_name' => 'field_sp_phase_display_text',
          'entity_type' => 'field_collection_item',
          'bundle' => 'field_sp_proposal_phase',
          'deleted' => '0',
      ),

      // Phase Subtopic Heading
      'field_sp_phase_subtopic_heading' => array(
          'label' => 'Phase subtopic heading',
          'widget' => array(
              'weight' => '5',
              'type' => 'text_textfield',
              'module' => 'text',
              'active' => 1,
              'settings' => array(
                  'size' => '60',
              ),
          ),
          'settings' => array(
              'text_processing' => '0',
              'user_register_form' => FALSE,
          ),
          'display' => array(
              'default' => array(
                  'label' => 'above',
                  'type' => 'text_default',
                  'settings' => array(),
                  'module' => 'text',
                  'weight' => 5,
              ),
          ),
          'required' => 0,
          'description' => 'Subtopic heading text.  Leave blank to use value already defined for proposal phase term.',
          'default_value' => NULL,
          'field_name' => 'field_sp_phase_subtopic_heading',
          'entity_type' => 'field_collection_item',
          'bundle' => 'field_sp_proposal_phase',
          'deleted' => '0',
      ),

  );
}
 
/** 
 * Implements hook_form_FORM_ID_alter().
 */
function smartparticipation_core_form_sp_proposal_node_form_alter(&$form, &$form_state, $form_id) {

  // When creating a new proposal, document label is filled in with the default
  // site document label.
  if (! isset($form_state['node']->nid)) {
    $form['field_sp_document_label'][LANGUAGE_NONE][0]['value']['#default_value'] =
        variable_get('smartparticipation_default_document_label', 'agency documents');
  }

  // The corresponding node reference field values are not editable from the
  // proposal, only from the corresponding nodes.
  $form['field_sp_proposal_document']['#access'] = false;
  $form['field_sp_proposal_announcement']['#access'] = false;
  $form['field_sp_proposal_news']['#access'] = false;
  $form['field_sp_proposal_topic']['#access'] = false;

  $form['field_sp_proposal_summary_closed'][LANGUAGE_NONE]['0']['#format'] = 'full_html';

  // Put closed proposal fields into a fieldset

    $form['closed_proposal'] = array(
        '#type' => 'fieldset',
        '#title' => t('Closed Proposal'),
        '#weight' => 23,
        '#collapsible' => TRUE,
        '#collapsed' => TRUE,
    );

    $form['closed_proposal']['field_sp_proposal_image_closed'] = $form['field_sp_proposal_image_closed'];
    unset($form['field_sp_proposal_image_closed']);
    $form['closed_proposal']['field_sp_proposal_summary_closed'] = $form['field_sp_proposal_summary_closed'];
    unset($form['field_sp_proposal_summary_closed']);
    $form['closed_proposal']['field_sp_url'] = $form['field_sp_url'];
    unset($form['field_sp_url']);

    // Do not allow manual setting of proposal paths, because they don't carry
    // over to the child paths (documents and topics). Even subpathauto doesn't
    // achieve this. Needs further investigation.
    // $form['path']['#access'] = true;

    // Not sure about this
    // $form['field_sp_proposal_survey']['#access'] = FALSE;

    // $form['#validate'][] = '_smartparticipation_core_proposal_node_form_validate';

    $proposal_nid = arg(1);

    if ($proposal_nid == 'add' || $proposal_nid == 'ajax') return;

    $proposal = node_load($proposal_nid);

    $phases = _smartparticipation_core_get_proposal_phase_data($proposal);

    $num_phases = sizeof($phases);

    foreach ($phases as $phase) {
        $phase_id = $phase['tid'];
        $topic_ids = smartparticipation_core_topic_same_phase_proposal($phase_id, $proposal_nid);
        if(!empty($topic_ids)) {
            for ($i = 0; $i < $num_phases; $i++) {
                if ($form['field_sp_proposal_phase'][LANGUAGE_NONE][$i]['field_sp_proposal_phase_term'][LANGUAGE_NONE]['#default_value'][0] == $phase_id) {
                    $form['field_sp_proposal_phase'][LANGUAGE_NONE][$i]['remove_button']['#disabled'] = true;
                    $form['field_sp_proposal_phase'][LANGUAGE_NONE][$i]['remove_button']['#attributes']['title']
                      = t('Phase contains topics - removal disallowed.');
                }
            }
        }
    }

}

/** 
 * Custom validator for proposal node form.
 * 
 * Doesn't work. Not sure why - works fine with announcements. At the end of 
 * this method the end date is null, but some later code resets it.
function _smartparticipation_core_proposal_node_form_validate($form, &$form_state) {

  $phases = $form_state['values']['field_sp_proposal_phase'][LANGUAGE_NONE];

  foreach ($phases as $index => $phase) {
    if (is_array($phase)) {
      $dates = $phase['field_sp_proposal_phase_dates'][LANGUAGE_NONE][0];
      $start_date = $dates['value'];
      $end_date = $dates['value2'];
      if ($end_date != NULL && $end_date == $start_date) {
        $form_state['values']['field_sp_proposal_phase'][LANGUAGE_NONE][$index]['field_sp_proposal_phase_dates'][LANGUAGE_NONE][0]['value2'] = NULL;
      }
    }
  }
}
*/


/**
 * Return the list of topic nodes for each proposal
 */
function _smartparticipation_core_get_proposal_topics($nid) {
  $query = 'SELECT * FROM {field_data_field_sp_topic_proposal} WHERE field_sp_topic_proposal_nid = :nid';
  $topics = db_query($query, array(':nid' => $nid))
    ->fetchAllAssoc('entity_id', PDO::FETCH_ASSOC);
  
  $topic_list = array();
  foreach ($topics as $topic) {
    $topic_list[] = $topic['entity_id'];
  }
  
  return $topic_list;
}


/**
 * Return the list of topic nodes for each proposal in a certain phase
 * 
 * @param $nid - proposal nid
 * @param $pid - the proposal phase field collection id
 */
function _smartparticipation_core_get_proposal_topics_phase($nid, $pid) {

  $query = '
    SELECT ph.*, ph.entity_id AS node_id FROM {field_data_field_sp_topic_phase} ph
    INNER JOIN {field_data_field_sp_topic_proposal} p ON ph.entity_id = p.entity_id 
    INNER JOIN {field_data_field_sp_proposal_phase_term} n ON ph.field_sp_topic_phase_tid = n.field_sp_proposal_phase_term_tid 
    INNER JOIN {node} no ON p.entity_id = no.nid 
    LEFT OUTER JOIN {sp_topic_node_order} o ON o.ttid = :pid AND o.tnid = ph.entity_id
    WHERE p.field_sp_topic_proposal_nid = :nid 
    AND n.entity_id = :pid 
    AND no.status = 1
    ORDER BY o.weight';

  $topics = db_query($query, array(':nid' => $nid, ':pid' => $pid))
    ->fetchAllAssoc('entity_id', PDO::FETCH_ASSOC);
  
  $topic_list = array();
  foreach ($topics as $topic) {
    $nid_topic = $topic['node_id'];
    $topic = node_load($nid_topic);
    
    if(smartparticipation_core_topic_access($topic)) {
      $topic_list[$nid_topic] = smartparticipation_core_topic_data($nid_topic);
    }
  }
  
  return $topic_list;
}

/**
 * Return an array of topic nids for the specified proposal nid and phase tid
 * 
 * @param $proposal_nid - proposal node nid
 * @param $term_tid - phase taxonomy term tid
 * 
 * @return an array of topic node nids
 */
function smartparticipation_core_proposal_phase_topic_nids($proposal_nid, $term_tid) {
  
  $sql = 'SELECT nid 
          FROM {node} n
          INNER JOIN {field_data_field_sp_topic_proposal} tp ON n.nid = tp.entity_id
          INNER JOIN {field_data_field_sp_topic_phase} tph ON n.nid = tph.entity_id    
          WHERE tp.field_sp_topic_proposal_nid = :proposal_nid
          AND tph.field_sp_topic_phase_tid = :term_tid';

  $topic_nids = db_query($sql, array(
      ':proposal_nid' => $proposal_nid, 
      ':term_tid' => $term_tid
  ))
    ->fetchCol();
  
  return $topic_nids;
}

function smartparticipation_core_proposal_node_update($node) {
  smartparticipation_core_update_proposal_glossary($node);
  smartparticipation_core_menu_update_parent($node);
}

/**
 * Helper function to populate the first dropdown. This would normally be
 * pulling data from the database.
 *
 * @return array of options
 */
function _smartparticipation_core_get_proposal_options($dist = 'sp_proposal') {
  $query = db_select('node', 'n')
    ->fields('n', array('title', 'nid'));
  $query->join('field_data_field_sp_proposal_status', 's', 's.entity_id = n.nid');
  $query->where("n.type = :dist", array(':dist' => $dist));
  $query->where("s.field_sp_proposal_status_value = 1");
  $result = $query->execute();

  //$j = 0;
  $options = array();
  foreach($result as $key => $value) {
	$options[''] = t('- Select a value -');
    $options[$value->nid] = t($value->title);
    //$j++;
  }

  /*
  return $j > 0 ? $options : NULL; 
  */
  return $options;
}

/**
 * Remove the related topics when a proposal is deleted
 */
function smartparticipation_core_delete_related_topics($nid) {
  $result = db_select('field_data_field_sp_proposal_topic', 't')
    ->fields('t', array('entity_id', 'field_sp_proposal_topic_nid'))
    ->condition('t.entity_id', $nid)
    ->execute();

  foreach ( $result as $key => $node_related ) {
    node_delete($node_related->field_sp_proposal_topic_nid);
  }
}

/**
 * Remove the related documents when a proposal is deleted
 */
function smartparticipation_core_delete_related_documents($nid) {
  $result = db_select('field_data_field_sp_proposal_document', 'd')
    ->fields('d', array('entity_id', 'field_sp_proposal_document_nid'))
    ->condition('d.entity_id', $nid)
    ->execute();

  foreach ( $result as $key => $node_related ) {
    node_delete($node_related->field_sp_proposal_document_nid);
  }
}

/**
 * Return proposal data for display of proposal overview on home and proposal 
 * pages. Includes data for the tabbed topic carousel.
 * 
 * @param $nid - proposal nid
 * 
 * @return an array of structured data for display
 */
function smartparticipation_core_proposal_overview_data($nid) {
  
  $proposal = node_load($nid);
  $title = $proposal->title;
  $link = l($title, 'node/' . $nid);
  $description = smartparticipation_core_proposal_description_text($proposal);
  $status_label =  smartparticipation_core_proposal_status_label($proposal);
  
  $comment_count = smartparticipation_core_proposal_comment_count($nid);  
  $comment_count_title = t($comment_count . ' comment' .
    ($comment_count == 1 ? '' : 's') . ' on @proposal', array(
      '@proposal' => $title,
    )
  ); 

  $dates = smartparticipation_core_proposal_dates($proposal);
  
  //$documents_link = smartparticipation_core_get_proposal_documents_link($proposal, $proposal->type, false);
  
  $image = smartparticipation_core_proposal_image_tag($proposal);
  
  $tabbed_topic_carousel_data = smartparticipation_core_tabbed_topic_carousel_data($nid);

  return array(
    '#theme' => 'proposal_overview',
    'proposal' => array(
      'comment_count' => $comment_count,
      'comment_count_title' => $comment_count_title,
      'dates' => $dates,
      'description' => $description,
      // 'documents_link' => $documents_link,
      'image' => $image,    
      'link' => $link,
      'nid' => $nid,
      'status_label' => $status_label,
      'tabbed_topic_carousel_data' => $tabbed_topic_carousel_data,
      'title' => $title,
        'data_truncate_limit' => variable_get('smartparticipation_data_truncate_limit'),
    ),
  ); 
}

/**
 * Return proposal image markup.
 * 
 * @param $proposal - the proposal node
 * @return Markup for image display
 */
function smartparticipation_core_proposal_image_tag($proposal) {

  $proposal_image = '';
  $image = smartparticipation_core_field_get_item('node', $proposal, 'field_image');
  if ($image) {    
    $proposal_image = theme(
      'image', 
      array(
        'path' => file_create_url($image['uri']),
        'alt' => $image['alt'],
        'title' => $image['title'],
        'width' => $image['width'],
        'height' => $image['height'],
        'attributes' => array(
          'class' => 'topic-header-image',
        ),
      )
    );
  }
      
  return $proposal_image;  
}

/**
 * 
 * Return closed proposal image markup
 * @param $proposal - the proposal node
 * @return Markup for closed proposal image display
 */
function smartparticipation_core_closed_proposal_image_tag($proposal) {

  $proposal_image = '';
  $image = smartparticipation_core_field_get_item('node', $proposal, 'field_sp_proposal_image_closed');
  if ($image) {
    $proposal_image = theme(
        'image',
        array(
          'path' => file_create_url($image['uri']),
        )
    );
  }

  return $proposal_image;
}

function smartparticipation_core_proposal_description_text($proposal) {
  
  $description = smartparticipation_core_field_get_item_value('node', $proposal, 'body', 'value'); 
  $description = smartparticipation_core_apply_glossary_lookup($description, $proposal->nid);
  return $description;
  
}

function smartparticipation_core_proposal_status_label($proposal) {
  
  $proposal_status = smartparticipation_core_field_get_item_value('node', $proposal, 'field_sp_proposal_status', 'value');
  $proposal_type = variable_get('smartparticipation_site_proposal_type');
  return ($proposal_status ? 'open' : 'closed') . ' ' . $proposal_type;
      
}

/**
 * Return proposal phase data for display on tabbed topic carousel on front
 * and proposal pages.
 * 
 * @param $proposal_nid - proposal nid
 * 
 * @return an array of structured data for display
 */
function smartparticipation_core_tabbed_topic_carousel_data($proposal_nid) {
  
  $proposal = node_load($proposal_nid);
  $phases = _smartparticipation_core_proposal_phase_display_data($proposal, false);

  foreach ($phases as $index=>$phase) {
    $phases[$index]['nodes'] = _smartparticipation_core_get_proposal_topics_phase($proposal_nid, $phase['phase_id']);
    $phases[$index]['#theme'] = 'carousel';
  }

  $recommended_comments_service = new \SmartParticipation\RecommendedCommentsService;
  $recommended_comments_enabled = $recommended_comments_service->getStatus();

  return array(
    '#theme' => 'tabbed_topic_carousel',
    'phases' => $phases,  
    'proposal_nid' => $proposal_nid,
    'recommended_comments_enabled' => $recommended_comments_enabled,
  );
}

/**
 * Return proposal data for display on proposal's child pages.
 * 
 * @param $nid - proposal nid
 * @param $include_future_phases - (optional boolean) whether or not to include
 * future phases; defaults to true
 * 
 * @return an array of structured data for display
 */
function smartparticipation_core_proposal_data($nid, $include_future_phases = true) {
  
  global $user;
  
  $proposal = node_load($nid);

  $proposal_title = $proposal->title;
  
  $proposal_link = l($proposal_title, 'node/' . $nid);
      
  $proposal_image = smartparticipation_core_proposal_image_tag($proposal); 

  $proposal_topics = field_get_items('node', $proposal, 'field_sp_proposal_topic');
  
  $proposal_documents_link = smartparticipation_core_get_proposal_documents_link($proposal, $proposal->type, false);
  
  $proposal_phases = _smartparticipation_core_proposal_phase_display_data($proposal, $include_future_phases);

  $proposal_description = smartparticipation_core_proposal_description_text($proposal);

  /*
  $proposal_closed_image = '';
  $image = smartparticipation_core_field_get_item('node', $proposal, 'field_sp_proposal_image_closed');
  if ($image) {    
    $proposal_closed_image = theme(
      'image', 
      array(
        'path' => file_create_url($image['uri']),
        'alt' => $image['alt'],
        'title' => $image['title'],
        'width' => $image['width'],
        'height' => $image['height'],
        'attributes' => array(
          'class' => 'topic-header-image',
        ),
      )
    );
  } */
  
  $proposal_publication_status = $proposal->status;
    
  $proposal_status_label =  smartparticipation_core_proposal_status_label($proposal);
 
  //$proposal_comment_count = smartparticipation_core_proposal_comment_count($nid);
  $proposal_comment_count = 0;
  foreach ($proposal_phases as $phase) {
    if (isset($phase['comment_count'])) {
      $proposal_comment_count += $phase['comment_count'];
    }
  }
  
  return array(
    'comment_count' => $proposal_comment_count,
    'description' => $proposal_description,

    'image' => $proposal_image,
      
    // TODO only needed for closed proposal. That should go to a different 
    // function, because the data is much more limited.
    //'image_closed' => $proposal_closed_image,
      
    'link' => $proposal_link,
    'nid_proposal' => $nid,
    'publication_status' => $proposal_publication_status,
    'phases' => $proposal_phases,
    'proposal_documents_link' => $proposal_documents_link,
    'status_label' => $proposal_status_label,
    'title' => $proposal_title,
    'topics' => $proposal_topics,
    'nid' => $nid
  );
}

/**
 * Return an indexed array of proposal phase data to send to the templates.
 * 
 * @param $proposal - the proposal node object
 * @param $show_future_phases (boolean) - optional; whether or not to include
 * future phases; defaults to true
 * 
 * @return an indexed array of proposal phase data for display
 */
function _smartparticipation_core_proposal_phase_display_data($proposal, $show_future_phases = true) {
  
  // Start by getting the basic data from the field collection
  $proposal_phases = _smartparticipation_core_get_proposal_phase_data($proposal);

  // Add additional phase data needed by the templates
  $phases_to_display = array();

  $active_phase = -1;
    
  foreach ($proposal_phases as $index=>$phase) {
    
    // Get the taxonomy term    
    $phase_term = taxonomy_term_load($phase['tid']);

    // Phase status: past, current, future
    $phase_status = _smartparticipation_core_get_proposal_phase_status_from_position($index, $phase['dates']);
 
    if (!$show_future_phases && $phase_status == 'future') {
      continue;
    }

    if (isset($phase['phase_display_text'])) {
        $phase['name'] = $phase['phase_display_text'];
    }
    else {
        $phase['name'] = $phase_term->name;
    }
    
    $phase['phase_status'] = $phase_status;
    $phase['phase_class'] = $phase_status . '-phase';
    $phase['active_class'] = '';

    // Messages
    $phase['no_topics_message'] = 
        t('There are no @phase_name topics to display.', array(
          '@phase_name' => $phase['name']
        ));
              
    $phase['phase_unavailable_message'] = '';
         
    if ($phase_status == 'current') {
      $active_phase = $index;
    } else if ($phase_status == 'future') {
      $phase['phase_unavailable_message'] = 
          t('The @phase_name is not yet available.', array(
            '@phase_name' => $phase['name'],
          ));
    } 
    
    // Get phase comment count
    $phase['phase_allows_comments'] = $phase_allows_comments = 
        smartparticipation_core_field_get_item_value('taxonomy_term', $phase_term, 'field_sp_phase_allows_comments', 'value');
    
    $phase['show_comment_count'] = $phase_allows_comments && $phase_status != 'future';

    $recommended_comments_service = new \SmartParticipation\RecommendedCommentsService;
    $recommended_comments_enabled = $recommended_comments_service->getStatus();
    $phase['recommended_comments_enabled'] = $recommended_comments_enabled;
    
    if ($phase['show_comment_count']) {
      $phase_comment_count = smartparticipation_core_proposal_phase_comment_count($proposal->nid, $phase_term->tid);
      $phase['comment_count'] = $phase_comment_count;
      $phase['comment_count_title'] = t($phase_comment_count . ' comment' . 
          ($phase_comment_count == 1 ? '' : 's') . ' on @phase', array(
            '@phase' => $phase['name'],
          ));                
     }
     
    $commentable = $phase_allows_comments && $phase_status == 'current';

    $phase['carousel_prompt'] = smartparticipation_core_carousel_prompt($commentable, 'topic', $phase);

    $phases_to_display[$index] = $phase;

  }
  
  // Assign active phase if not already assigned (i.e., there is no current phase):
  if ($active_phase == -1) {

    //$last = count($phases_to_display) - 1;
    
    // If the last phase is past, make it active; else make the first phase active.
    //$active_phase = $phases_to_display[$last]['phase_status'] == 'past' ? $last : 0;

    // Make the first phase active
    $active_phase = 0;
  }
  $phases_to_display[$active_phase]['active_class'] = 'active';

  return $phases_to_display;
}

/**
 * Return the comment count for the specified proposal phase. The caller has
 * already determined that the phase allows comments.
 * 
 * @param $proposal_nid - the proposal nid
 * @param $phase_tid - the tid of the phase taxonomy term
 * 
 * @return the number of comments for all topics in the specified phase. 
 */
function smartparticipation_core_proposal_phase_comment_count($proposal_nid, $phase_tid) {
  
  $comment_count = 0;

  $topic_nids = smartparticipation_core_proposal_phase_topic_nids($proposal_nid, $phase_tid);
  foreach ($topic_nids as $topic_nid) {
    $comment_count += smartparticipation_core_topic_comment_count($topic_nid);
  }

  return $comment_count;
  
}



/**
 * @return array of proposals
 */
function _smartparticipation_core_get_proposal_list($status) {
  
  // TODO Are there problems with this query? Is that why we're having problems
  // with the open/closed proposal menu items? When I tried using the same
  // query in _smartparticipation_core_get_proposal_count(), it generated errors.
  
  $query = db_select('node', 'n')
    ->fields('n', array('title', 'nid'));
  $query->join('field_data_field_sp_proposal_status', 's', 's.entity_id = n.nid');
  $query->where("n.type = :type AND n.status = :nstatus", array(
    ':type' => 'sp_proposal',
    ':nstatus' => 1
  ));
  $query->where("s.field_sp_proposal_status_value = :status", array(':status' => $status));
    $result = $query->execute();

  $proposals = array();
  foreach($result as $key => $value) {
    $proposals[$value->nid] = $value->title;
  }
  
  return $proposals;
}

function _smartparticipation_core_proposal_count($status) {
  
  $query = "SELECT n.nid FROM {node} n
            INNER JOIN {field_data_field_sp_proposal_status} s ON n.nid = s.entity_id
            WHERE n.type = :type
            AND n.status = :nstatus
            AND s.field_sp_proposal_status_value = :status";

  $result = db_query($query, array(
    ':nstatus' => 1,
    ':type' => 'sp_proposal',
    ':status' => $status,
  ))->fetchAll();
  
  return count($result);
}


/**
 * @return array of all proposals
 */
function _smartparticipation_core_get_proposal_list_all() {
  $query = db_select('node', 'n')
    ->fields('n', array('title', 'nid'));
  $query->where("n.type = :type", array(':type' => 'sp_proposal'));
  $result = $query->execute();

  $proposals = array();
  foreach($result as $key => $value) {
    $proposals[$value->nid] = $value->title;
  }
  
  return $proposals;
 
}


/**
 * Return the proposal phase field collection objects for the specified 
 * proposal.
 * 
 * @param $proposal - proposal node object
 * 
 * @return an array of field collection objects
 */
function smartparticipation_core_get_proposal_phase_objects($proposal) {
  
  $phases = field_get_items('node', $proposal, 'field_sp_proposal_phase');
  $phase_ids = field_collection_field_item_to_ids($phases);
  $phases = field_collection_item_load_multiple($phase_ids); 
  
  return $phases;
  
}

/**
 * Return an array of phase data for the specified proposal. This function
 * retrieves the proposal phase field collection objects and re-assembles it 
 * into an indexed array, but does not include any additional data. Included 
 * are the field collection item id, the term tid, and the phase dates. If the
 * term name is desired, it should be retrieved afterwards, since it is not 
 * included in the data returned by the field collection module.
 * 
 * @param $proposal - proposal node object
 * 
 * @return an indexed array of phase data
 */
function _smartparticipation_core_get_proposal_phase_data($proposal) {

  $phases = smartparticipation_core_get_proposal_phase_objects($proposal);
  
  $proposal_phases = array();
  $index = 0;
  foreach ($phases as $phase) {
    
    $proposal_phases[$index] = array();
    
    // This is the field collection item id
    $proposal_phases[$index]['phase_id'] = $phase->item_id;
    
    // This is the taxonomy term id
    $tid = $phase->field_sp_proposal_phase_term[LANGUAGE_NONE][0]['tid'];
    $proposal_phases[$index]['tid'] = $tid;

    // Start and end dates
    $proposal_phases[$index]['dates'] = $dates = array();
    $dates['start'] = NULL; // or ''?
    $dates['end'] = NULL; // or ''?
    
    if (! empty($phase->field_sp_proposal_phase_dates))  {
      $phase_dates = $phase->field_sp_proposal_phase_dates[LANGUAGE_NONE][0];

      $dates['start'] = $phase_dates['value'];
      $dates['end'] = $phase_dates['value2'];
    
      // The field collection (or date?) module sets an empty end date equal to 
      // the start  date, and we are unable to modify that when the data is 
      // entered. Normalize it here, so we can later test for a null  
      // (unspecified) end date rather than an end date equal to the start date.
      if ($dates['end'] == $dates['start']) {
        $dates['end'] = NULL; // or ''?
      }   
    }
 
    $proposal_phases[$index]['dates'] = $dates;

    if (! empty($phase->field_sp_phase_open_text)) {
        $proposal_phases[$index]['phase_open_text'] = $phase->field_sp_phase_open_text[LANGUAGE_NONE][0]['value'];
    }

    if (! empty($phase->field_sp_phase_closed_text)) {
        $proposal_phases[$index]['phase_closed_text'] = $phase->field_sp_phase_closed_text[LANGUAGE_NONE][0]['value'];
    }

    if (! empty($phase->field_sp_phase_display_text)) {
        $proposal_phases[$index]['phase_display_text'] = $phase->field_sp_phase_display_text[LANGUAGE_NONE][0]['value'];
    }

    if (! empty($phase->field_sp_phase_subtopic_heading)) {
        $proposal_phases[$index]['phase_subtopic_heading'] = $phase->field_sp_phase_subtopic_heading[LANGUAGE_NONE][0]['value'];
    }

      $index++;
  }

  return $proposal_phases;
}

/**
 * Return a structured array of phase data for the specified proposal and phase
 * term tid. Return an empty array if the phase is not defined for the proposal.
 * 
 * @param $proposal - proposal node object
 * @param $phase_tid - taxonomy term tid for the phase
 * 
 * @return a structured array of phase data including tid, and start and end 
 * dates. If the phase is not defined for the proposal, returns an empty array.
 */
function _smartparticipation_core_get_proposal_phase($proposal, $phase_tid) {
  
  $proposal_phases = _smartparticipation_core_get_proposal_phase_data($proposal);

  $phase = array();
  
  foreach ($proposal_phases as $proposal_phase) {
    
    if ($proposal_phase['tid'] == $phase_tid) {
      $phase = $proposal_phase;
      break;
    }
  }
  
  return $phase;
  
}

/**
 * Return the proposal phase start and end dates. If the phase is not defined 
 * for the proposal, return an empty array. This is in contrast to the case
 * where a date is unspecified; in that case, the array key is still defined,
 * but it points to a null value. 
 * 
 * @param $proposal - proposal node object
 * @param $phase_tid - taxonomy term tid for the phase
 * 
 * @return an array containing the proposal phase start and end dates. If the
 * phase is not defined for the proposal, returns an empty array.
 */
function _smartparticipation_core_get_proposal_phase_dates($proposal, $phase_tid) {

  $phase = _smartparticipation_core_get_proposal_phase($proposal, $phase_tid);
  
  $dates = array();
  
  if (isset($phase['dates'])) {
    $dates = $phase['dates'];
  }
  
  return $dates;
}

/**
 * Return the minimum start time as a UNIX timestamp for proposal over all phases
 *
 * @param $proposal
 *
 * @return Unix timestamp
 */
function _smartparticipation_core_get_proposal_min_start_time ($proposal) {

    $phases = _smartparticipation_core_get_proposal_phase_data($proposal);
    $start_dates=array();
    foreach($phases as $phase) {
        $start_dates[] = strtotime($phase['dates']['start']);
    }

    return (min($start_dates));
}

/**
 * Return the maximum end time as a UNIX timestamp for proposal over all phases
 *
 * @param $proposal
 *
 * @return Unix timestamp
 */
function _smartparticipation_core_get_proposal_max_end_time ($proposal) {

    $phases = _smartparticipation_core_get_proposal_phase_data($proposal);
    $end_dates=array();
    foreach($phases as $phase) {
        $end_dates[] = strtotime($phase['dates']['end']);
    }

    return (max($end_dates));
}

/**
 * Return the proposal phase status (past, current, future).
 * 
 * TODO Combine with _smartparticipation_core_get_proposal_phase_status_from_position().
 * 
 * @param $proposal - the proposal node object
 * @param $phase - the phase taxonomy term object
 * 
 * @return string - 'past', 'current', or 'future'
 */
function _smartparticipation_core_proposal_phase_status($proposal, $phase) {
  
  $dates = _smartparticipation_core_get_proposal_phase_dates($proposal, $phase->tid);
  
  $date_start = $dates['start'];
  $date_end = $dates['end'];
  
  $now = date('Y-m-d H:i:s');
  
  if (!$date_start || $date_start > $now) {
    return 'future';
  }
  
  if ($date_end && $date_end < $now) {
    return 'past';
  }
  
  return 'current';
}


/**
 * Return true iff the proposal phase is closed (past).
 * 
 * @param $proposal - the proposal node object
 * @param $phase - the phase taxonomy term object
 * 
 * @return boolean - true iff the proposal phase is closed
 */
function _smartparticipation_core_proposal_phase_is_closed($proposal, $phase) {

  $status = _smartparticipation_core_proposal_phase_status($proposal, $phase);
  return $status == 'past';
}

// TODO this (roughly) should replace smartparticipation_core_autocreatemenu() and the
// switch statement at the top of smartparticipation_core_automenu_automenu_create(). See 
// notes there. The code needs to be tested and debugged, however.
function smartparticipation_core_proposal_autocreatemenu($node) {
  
  $proposal_status = smartparticipation_core_field_get_item_value('node', $node, 'field_sp_proposal_status', 'value');
  $proposal_type = ucfirst(variable_get('smartparticipation_site_proposal_type_plural'));

  $link_title = $proposal_status == 0 ?
    'Closed ' . $proposal_type : 'Open ' . $proposal_type;

  $params = smartparticipation_core_automenu_automenu_create($node, $link_title);
  $last_weight = _smartparticipation_core_get_last_menu_item_weight($node->type);
  smartparticipation_core_automenu_add_menu_link($params, $last_weight);  
}

/**
 * Get the total number of comments for a specific proposal
 */
function smartparticipation_core_proposal_comment_count($proposal_nid) {
  $query = 'SELECT * FROM {field_data_field_sp_proposal_topic} t
            INNER JOIN {field_data_field_sp_topic_subtopic} s ON t.field_sp_proposal_topic_nid = s.entity_id
            INNER JOIN {comment} c ON s.field_sp_topic_subtopic_nid = c.nid
            WHERE t.entity_id = :proposal_nid and c.status = 1';

  $result = db_query($query, array(':proposal_nid' => $proposal_nid));

  return $result->rowCount();
}


/**
 * Return the status of the phase based on position and start and end dates.
 * 
 * TODO Combine with _smartparticipation_core_proposal_phase_status().
 * 
 * @param $phase_position - numeric position of the phase in the proposal
 * @param $phase_dates - associative array of phase start and end dates
 * 
 * @return string - status of the phase (past, current, future)
 */
function _smartparticipation_core_get_proposal_phase_status_from_position($phase_position, $phase_dates) {
  
  $now = date('Y-m-d H:i:s');
  $status = '';
  
  $date_start = $phase_dates['start'];
  $date_end = $phase_dates['end'];

  // If the first phase has no start date, or a future start date, it will be
  // considered current. This helps in building out the proposal before the
  // opening date is known, so editors can see the topic carousel.
  // TODO This is a hack. Instead: include topics in the topic carousel based on
  // node access. So if an editor has access to the topic, it will show in the
  // carousel. If an ordinary user can't access the topic, it won't show in the
  // carousel. See RR-1825. Then combine this with _smartparticipation_core_proposal_phase_status().
  if (!$date_start || $date_start > $now) {
    if ($phase_position == 0) {
      $status = 'current';
    } else {
      $status = 'future';
    }
  } else if ($date_end && $date_end < $now) {
    $status = 'past';
  } else {
    $status = 'current';
  }

  return $status;
}

/**
 * Return true iff the proposal phase has an end date defined.
 * 
 * This is a workaround for the field collection module setting an empty end 
 * date equal to the start date. See notes on commented-out function 
 * _smartparticipation_core_proposal_node_form_validate(). We would prefer to store an empty 
 * end date, as we do for announcements and other node types that use a simple
 * date field rather than a date field as part of a field collection.
 * 
 * @param $date_start - the phase start date
 * @param $date_end - the phase end date
 */
function _smartparticipation_core_proposal_phase_has_end_date($date_start, $date_end) {
  return $date_end != $date_start;
}


/**
 * Assemble data for proposal node view.
 * @param $node - the proposal node
 * @param $view_mode - the view mode
 * @param $langcode
 */
function smartparticipation_core_proposal_node_view($node, $view_mode, $langcode) {

  if ($view_mode == 'full') {
    
    $request_path = request_path();
    $path_parts = explode('/', $request_path);
    
    // Add blocks to the page, but not on editing tabs
    if ($path_parts[0] != 'node') {

      $node->content['sp_learn_panel'] =
          module_invoke('smartparticipation_core', 'block_view', 'learn_panel');
      
      // Open vs. closed proposal
      $proposal_status = smartparticipation_core_field_get_item_value('node', $node, 'field_sp_proposal_status', 'value');

      if ($proposal_status) {
      
        $recent_comments = module_invoke('smartparticipation_core', 'block_view', 'recent_comments');
        $recommended_comments = module_invoke('smartparticipation_core', 'block_view', 'recommended_comments');
     
        $items = array(
          'smartparticipation_core_recent_comments' => $recent_comments,
          'smartparticipation_core_recommended_comments' => $recommended_comments,  
        );
        
      } else {        
         // Later there may be other features to display if the proposal is
         // closed. 
         $items = array();   
      }

      $node->content['features'] = smartparticipation_core_page_features($items);
    }
    
    $node->content['proposal_overview_data'] = array(
      'proposal' => smartparticipation_core_proposal_overview_data($node->nid),
    );

  }
}


/**
 * Return true iff there are topics in the smartparticipation_core_topic_node_order table for a 
 * specific proposal
 * 
 * @param $proposal_nid - the nid of the proposal
 * 
 * @return boolean 
 */
function smartparticipation_core_proposal_topics_ordered($proposal_nid) {

  $query = 'SELECT * FROM sp_topic_node_order WHERE ttid = :proposal_nid';
  $result = db_query($query, array(':proposal_nid' => $proposal_nid));
  $count = $result->rowCount();
  
  return $count > 0;

}

/**
 * Return true iff the specified proposal has an associated interest survey,
 * and that survey has been published.
 * @param unknown $proposal
 */
function smartparticipation_core_proposal_interest_survey($proposal) {
  
  $published_survey_nid = null;
  
  $survey_nid = smartparticipation_core_field_get_item_value('node', $proposal, 'field_sp_interest_survey', 'nid');
  if ($survey_nid) {
    $survey = node_load($survey_nid);
    if ($survey->status) {
      $published_survey_nid = $survey_nid;
    }
  }
  
  return $published_survey_nid;
  
}

/**
 * Return data needed to render data about a closed proposal in the closed 
 * proposal block on the front page.
 * 
 * @param $nid - the proposal nid. The caller has determined that the proposal
 * is closed and published.
 * 
 * @return - an array of data for the closed proposal
 */
function smartparticipation_core_closed_proposal_data($nid) {
  
  $proposal = node_load($nid);
  
  $archive_url = smartparticipation_core_field_get_item_value('node', $proposal, 'field_sp_url', 'value');
  $body = smartparticipation_core_field_get_item_value('node', $proposal, 'body', 'value');

  $path = '';
  $class = '';
  if (! empty($archive_url)) {
    $class = 'archive';
  	$path = $archive_url;
  	$target = '_blank';
  // This test allows for a closed but unarchived proposal. It's a bit of a
  // hack, but not too egregious, using an empty body as a way of testing
  // whether the proposal was ever open on the current site. Otherwise, we'd
  // need an additional field to indicate whether there is a link or not.
  } else if ($body) {
    $path = 'node/' . $nid;
    $target = ''; 	
  }

  if ($path) {

    $attributes = array();

    if (! empty($target)) {
      $attributes['target'] = $target;
    }

    if (! empty($class)) {
      $attributes['class'] = $class;
    }

	$title = l($proposal->title, $path, array(
	  'attributes' => $attributes
	));

  } else {
  	$title = $proposal->title;
  }
  
  $image = smartparticipation_core_closed_proposal_image_tag($proposal);
  
  $dates = smartparticipation_core_proposal_dates($proposal);

  $summary = smartparticipation_core_field_get_item_value('node', $proposal, 'field_sp_proposal_summary_closed', 'value');
  $summary = smartparticipation_core_apply_glossary_lookup($summary, $nid);
  
  return array(
    '#theme' => 'closed_proposal',
    'proposal' => array(
      'dates' => $dates,
      'image' => $image,
      'title' => $title,
      'nid' => $nid,
      'summary' => $summary,
    ),
  ); 
}

/**
 * Compare function for open proposals. Used to sort proposals in the open
 * proposal block. Sorts by proposal open date descending. 
 * 
 * TODO Figure out how to use this in the open proposal menu also.
 * 
 * @param $proposal1 - array of proposal data as returned by 
 * smartparticipation_core_proposal_overview()
 * @param $proposal2 - array of proposal data as returned by 
 * smartparticipation_core_proposal_overview()
 * 
 * @return number - as specified by uasort() comparison function
 */
function smartparticipation_core_sort_open_proposals_by_date_desc($proposal1, $proposal2) {
 
  if (is_null($proposal1['proposal']['dates']['open']) ||
      is_null($proposal2['proposal']['dates']['open'])) {

    $nid1 = $proposal1['proposal']['nid'];
    $nid2 = $proposal2['proposal']['nid']; 

    return $nid1 > $nid2 ? -1 :
           ($nid1 < $nid2 ? 1 : 0);

  }
  
  return smartparticipation_core_sort_proposals_by_open_date_desc($proposal1, $proposal2);
      
}

/**
 * 
 * @param $proposal1 - array of open or closed proposal data
 * @param $proposal2 - array of open or closed proposal data
 * 
 * @return number - as specified by uasort() comparison function
 */
function smartparticipation_core_sort_proposals_by_open_date_desc($proposal1, $proposal2) {
  
  $timestamp1 = $proposal1['proposal']['dates']['open']['timestamp'];
  $timestamp2 = $proposal2['proposal']['dates']['open']['timestamp'];
  
  return $timestamp1 > $timestamp2 ? -1 :
        ($timestamp1 < $timestamp2 ? 1 : 0);  
  
}

/**
 * Compare function for closed proposals. Used to sort proposals in the closed
 * proposal block. Sorts by proposal close date descending. If close dates are
 * the same, sort by open date descending.
 * 
 * TODO Figure out how to use this in the closed proposal menu also.
 * 
 * @param $proposal1 - array of proposal data as returned by 
 * smartparticipation_core_closed_proposal_data()
 * @param $proposal2 - array of proposal data as returned by 
 * smartparticipation_core_closed_proposal_data()
 * 
 * @return number - as specified by uasort() comparison function
 */
function smartparticipation_core_sort_closed_proposals_by_date_desc($proposal1, $proposal2) {

  $timestamp1 = $proposal1['proposal']['dates']['close']['timestamp'];
  $timestamp2 = $proposal2['proposal']['dates']['close']['timestamp'];
  
  if ($timestamp1 > $timestamp2) {
    return -1;
  } 
  
  if ($timestamp1 < $timestamp2) {
    return 1;
  }
  
  return smartparticipation_core_sort_proposals_by_open_date_desc($proposal1, $proposal2);

}

/**
 * Compute proposal open and close dates from phase dates. Proposal open date is
 * start date of first phase; proposal close date is end date of last phase.
 * 
 * If this is needed in multiple display contexts, add to hook_node_load() 
 * implementation so we always get it with the proposal. Currently used only
 * in closed proposal block.
 * 
 * @param $proposal - the proposal node object
 * @return an array containing the proposal open and close dates as Unix
 * timestamps and formatted for display and for the html5 datetime attribute.
 */
function smartparticipation_core_proposal_dates($proposal) {
  
  $dates = array(
    'open' => null,
    'close' => null,    
  );

  $phases = smartparticipation_core_get_proposal_phase_objects($proposal);
  $discussion_phase_tid = smartparticipation_core_get_discussion_phase();

  $first_phase = reset($phases);
  if (! empty($first_phase->field_sp_proposal_phase_dates)) {
    $dates['open'] = $first_phase->field_sp_proposal_phase_dates[LANGUAGE_NONE][0]['value'];
  }

  $close_phase = null;

  if (! empty($discussion_phase_tid)) {
    foreach ($phases as $phase) {
      if (! empty($phase->field_sp_proposal_phase_term)) {
        $phase_tid = $phase->field_sp_proposal_phase_term[LANGUAGE_NONE][0]['tid'];
        if ($phase_tid == $discussion_phase_tid) {
          $close_phase = $phase;
          break;
        }
      }
    }
  }

  if (! $close_phase) {
    $close_phase = end($phases);
  }

  if (! empty($close_phase->field_sp_proposal_phase_dates)) {
    $dates['close'] = $close_phase->field_sp_proposal_phase_dates[LANGUAGE_NONE][0]['value2'];
  }

  foreach ($dates as $key => $date) {
    if (! is_null($date)) {
      $timestamp = strtotime($date);
      $dates[$key] = array(
        'timestamp' => $timestamp,
        'attribute' => date('Y-m-d', $timestamp),
        'display' => date('m.d.Y', $timestamp),        
      );
    }
  }
  
  return $dates;

}

/**
 * @return int the taxonomy term ID for the Discussion phase
 */
function smartparticipation_core_get_discussion_phase()
{
  $query = "SELECT tid FROM {taxonomy_term_data} t
            LEFT JOIN {taxonomy_vocabulary} v ON t.vid = v.vid
            WHERE v.machine_name = 'sp_proposal_phase' AND t.name = 'Discussion'";

  $result = db_query($query)->fetchCol();

  return $result[0];
}

/**
 * @return bool there is an open proposal with an open phase that allows comments
 */
function smartparticipation_core_is_open_discussion()
{
  $open_proposals = _smartparticipation_core_get_proposal_list(1);
  foreach ($open_proposals as $proposal_id => $proposal_name) {
    $proposal = node_load($proposal_id);
    $phases = _smartparticipation_core_get_proposal_phase_data($proposal);
    foreach ($phases as $phase) {
      $phase_term = taxonomy_term_load($phase['tid']);
      $phase_allows_comments = smartparticipation_core_field_get_item_value('taxonomy_term', $phase_term, 'field_sp_phase_allows_comments', 'value');
      $phase_is_open = !_smartparticipation_core_proposal_phase_is_closed($proposal, $phase_term);
      if ($phase_allows_comments && $phase_is_open) {
        return TRUE;
      }
    }
  }
  return false;
}
