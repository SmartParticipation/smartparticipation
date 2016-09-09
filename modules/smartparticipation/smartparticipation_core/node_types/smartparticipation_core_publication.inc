<?php

/**
 * @file 
 * Define the publication node type.
 * 
 * @ingroup smartparticipation_core
 */
 
/**
 * Create the publication node type.
 */
function smartparticipation_core_create_publication_node_type() {
  
  $t = get_t();
  
  $publication = array(
    'type' => 'sp_publication',
    'name' => $t('Publication'),
    // Drupal will handle this node as if we had designed it in the UI.
    'base' => 'node_content', 
    'has_title' => TRUE,  
    'description' => $t('Content type for project publications.'),
    'custom' => TRUE,
    'help' => '',
  );  
  
  // Set defaults not explicitly declared above.
  $node_type = node_type_set_defaults($publication);
  
  // Add body field and set label.
  node_add_body_field($node_type, $t('Abstract'));
  
  // Modify properties of the body field.
  $instance = field_read_instance('node', 'body', 'sp_publication');
  $instance['widget']['weight'] = 35;
  field_update_instance($instance);
  
  node_type_save($node_type);

  // Create the fields we are adding to the learn node type.
  foreach (smartparticipation_core_publication_installed_fields() as $field) {
    $field['entity_types'] = array('node');
    field_create_field($field);
  }
  
  // Create field instances.
  foreach (smartparticipation_core_publication_installed_instances() as $instance) {
    $instance['bundle'] = $publication['type'];
    $instance['entity_type'] = 'node';
    field_create_instance($instance);
  }

  // Disable commenting for this node type  
  variable_set('comment_sp_publication', COMMENT_NODE_HIDDEN);

  // Create a revision by default when a new version is saved
  variable_set('node_options_sp_publication', array('revision', 'status'));
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
function smartparticipation_core_publication_installed_fields() {
    $t = get_t();

  return array( 
  
    // This is just a simple text field.
    'field_sp_publication_author' => array(
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'field_name' => 'field_sp_publication_author',
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

    'field_sp_publication_year' => array(
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'field_name' => 'field_sp_publication_year',
      'foreign keys' => array(),
      'indexes' => array(),
      'locked' => '0',
      'module' => 'number',
      'settings' => array(),
      'translatable' => '0',
      'type' => 'number_integer',
    ),

    'field_sp_publication_date' => array(
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_sp_publication_date',
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
            
    'field_sp_publication_data' => array(
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'field_name' => 'field_sp_publication_data',
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

      // Featured
      'field_sp_publication_featured' => array(
          'active' => '1',
          'cardinality' => '1',
          'deleted' => '0',
          'entity_types' => array('node'),
          'field_name' => 'field_sp_publication_featured',
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
                  0 => $t('No'),
                  1 => $t('Yes'),
              ),
              'allowed_values_function' => '',
          ),
          'translatable' => '0',
          'type' => 'list_boolean',
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
function smartparticipation_core_publication_installed_instances() {
  $t = get_t();
  
  return array(
    
    'field_sp_publication_author' => array(
      'default_value' => NULL,
      'deleted' => '0', 
      'description' => $t('The author(s) of the publication, entered exactly as they are to be displayed.'),
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
      'field_name' => 'field_sp_publication_author',
      'label' => $t('Author(s)'),
      'required' => 1,
      'settings' => array(
        'text_processing' => '0',
        'user_register_form' => FALSE,
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'text',
        'settings' => array(
          'size' => '250',
        ),
        'type' => 'text_textfield',
        'weight' => '5',
      ),   
    ),

    'field_sp_publication_data' => array(
      'default_value' => NULL,
      'deleted' => '0', 
      'description' => $t('For an article, specify the journal, book, etc. where this article was published. For a book, specify publisher. Add full publication data, including issue and page numbers, location of publisher, etc., exactly as it should be displayed.'),
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
      'field_name' => 'field_sp_publication_data',
      'label' => $t('Publication information'),
      'required' => 1,
      'settings' => array(
        'text_processing' => '0',
        'user_register_form' => FALSE,
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'text',
        'settings' => array(
          'size' => '250',
        ),
        'type' => 'text_textfield',
        'weight' => '25',
      ),   
    ),

    'field_sp_publication_date' => array(
      'default_value' => NULL,
      'deleted' => '0',   
      'description' => $t('Description of publication date, if year is not available. For example: "forthcoming" or "in press". If not blank, will display instead of publication year.'),
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
      'field_name' => 'field_sp_publication_date',
      'label' => $t('Descriptive publication date'),      
      'required' => 0,
      'settings' => array(
        'text_processing' => '0',
        'user_register_form' => FALSE,
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'text',
        'settings' => array(
          'size' => '40',
        ),
        'type' => 'text_textfield',
        'weight' => '20',
      ),    
    ),
        
    'field_sp_publication_url' => array(
      'default_value' => NULL,
      'deleted' => '0',
      'description' => $t('Optional link to full article or other source.'),
      'display' => array(
        'default' => array(
          'label' => 'hidden',
          'module' => 'text',
          'settings' => array(),
          'type' => 'text_default',
        ),
        'teaser' => array(
          'label' => 'hidden',
          'module' => 'text',
          'settings' => array(),
          'type' => 'text_default',
        ),
      ),
      'field_name' => 'field_sp_url',
      'label' => $t('Link'),
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
        'weight' => '30',
      ),
    ),
    
    'field_sp_publication_year' => array(
      'default_value' => NULL,
      'deleted' => '0',
      'description' => $t('If publication date should read "forthcoming", "in press", etc., add the estimated future year for correct sorting, then add a descriptive publication date for display.'),
      'display' => array(
        'default' => array(
          'label' => 'hidden',
          'module' => 'number',
          'settings' => array(
            'decimal_separator' => '.',
            'prefix_suffix' => TRUE,
            'scale' => 0,
            'thousand_separator' => '',
          ),
          'type' => 'number_integer',
          'weight' => '9',
        ),
        'teaser' => array(
          'label' => 'hidden',
          'module' => 'number',
          'settings' => array(
            'decimal_separator' => '.',
            'prefix_suffix' => TRUE,
            'scale' => 0,
            'thousand_separator' => '',
          ),
          'type' => 'number_integer',
          'weight' => '9',
        ),
      ),
      'field_name' => 'field_sp_publication_year',
      'label' => $t('Year of publication'),
      'required' => 1,
      'settings' => array(
        'max' => '2100',
        'min' => '1980',
        'prefix' => '',
        'suffix' => '',
        'user_register_form' => FALSE,
      ),
      'widget' => array(
        'active' => 0,
        'module' => 'number',
        'settings' => array(),
        'type' => 'number',
        'weight' => '15',
      ),
    ),

      // Featured
      'field_sp_publication_featured' => array(
          'default_value' => array(
              0 => array(
                  'value' => 0,
              ),
          ),
          'deleted' => '0',
          'description' => $t('Should this publication be featured?.'),
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
          'field_name' => 'field_sp_publication_featured',
          'label' => $t('Featured'),
          'required' => 1,
          'settings' => array(
              'user_register_form' => FALSE,
          ),
          'widget' => array(
              'active' => 1,
              'module' => 'options',
              'settings' => array(),
              'type' => 'options_buttons',
              'weight' => '35',
          ),
      ),


  );
}


/**
 * Delete the publication node type.
 * 
 */
function smartparticipation_core_delete_publication_node_type() {
  smartparticipation_core_delete_node_type('sp_publication');
}

function smartparticipation_core_publication_node_view(&$node, $view_mode, $langcode) {

  $content = $node->content;
  
  if (isset($content['field_sp_url'])) {
    
    $full_article_url = $content['field_sp_url'][0]['#markup'];
    
    if (! node_is_page($node)) {
      $content['title'] = l(
          $node->title,
          $full_article_url,
          array(
            'attributes' => array(
              'target' => '_blank',
            ),
            'external' => true,
          )
      );
    }
    
    $content['full_article_link'] = l(
        t('View full article'),
        $full_article_url,
        array(
          'attributes' => array(
            'target' => '_blank',
            'title' => t("View full text of @title", array('@title' => $node->title)),
          ),
          'external' => true,
        )
    );
  } else {
    
    $content['title'] = $node->title;
    
  }
  
  /* 
   * field_sp_publication_date is a text field that will override the year in
   * the display if present; e.g., "forthcoming" or "in press.") The year is 
   * still required for sorting. 
   */
  $content['publication_date'] = 
    isset($content['field_sp_publication_date']) ?
    $content['field_sp_publication_date'] :
    $content['field_sp_publication_year'];
  $content['publication_date'] = $content['publication_date'][0]['#markup'];
  

  $node->content = $content;
}

