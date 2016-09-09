<?php

/**
 * @file 
 * Define the news node type.
 * 
 * @ingroup smartparticipation_core
 */
 
/**
 * Create the news node type.
 */
function smartparticipation_core_create_news_node_type() {
  
  $t = get_t();
  
  $news = array(
    'type' => 'sp_news',
    'name' => $t('News item'),
    // Drupal will handle this node as if we had designed it in the UI.
    'base' => 'node_content', 
    'has_title' => TRUE,  
    'description' => $t('News items related to the site as a whole or to a specific proposal.'),
    'custom' => TRUE,
    'help' => '',
  );  
  
  // Set defaults not explicitly declared above.
  $node_type = node_type_set_defaults($news);
  
  // Add body field and set label.
  node_add_body_field($node_type);
  
  // Modify properties of the body field.
  $instance = field_read_instance('node', 'body', 'sp_news');
  $instance['widget']['weight'] = 45;
  field_update_instance($instance);
  
  node_type_save($node_type);

  // Create the fields we are adding to the news node type.
  foreach (smartparticipation_core_news_installed_fields() as $field) {
    $field['entity_types'] = array('node');
    field_create_field($field);
  }
  
  // Create all the instances for our fields.
  foreach (smartparticipation_core_news_installed_instances() as $instance) {
    $instance['bundle'] = $news['type'];
    $instance['entity_type'] = 'node';
    field_create_instance($instance);
  }

  // Disable commenting for this node type  
  variable_set('comment_sp_news', COMMENT_NODE_HIDDEN);

  // Create a revision by default when a new version is saved
  variable_set('node_options_sp_news', array('revision', 'status'));
  
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
function smartparticipation_core_news_installed_fields() {

  return array(

    'field_sp_news_date' => array(
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'field_name' => 'field_sp_news_date',
      'foreign keys' => array(),
      'indexes' => array(),
      'locked' => '0',
      'module' => 'date',
      'settings' => array(
        'cache_count' => '4',
        'cache_enabled' => 0,
        'granularity' => array(
          'day' => 'day',
          'hour' => 0,
          'minute' => 0,
          'month' => 'month',
          'second' => 0,
          'year' => 'year',
        ),
        // TODO Make sure the timezone values are correct
        'timezone_db' => '',
        'todate' => '',
        'tz_handling' => 'none',
      ),
      'translatable' => '0',
      'type' => 'datetime',
    ),
    
    'field_sp_news_proposal' => array(
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'field_name' => 'field_sp_news_proposal',
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
    
    'field_sp_news_source' => array(
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'field_name' => 'field_sp_news_source',
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
function smartparticipation_core_news_installed_instances() {
  $t = get_t();
   
  return array(

    'field_sp_news_date' => array(
      'deleted' => '0',
      'description' => $t('The date the news item was published.'),
      'display' => array(
        'default' => array(
          'label' => 'hidden',
          'module' => 'date',
          'settings' => array(
            'format_type' => 'medium',
            'fromto' => 'both',
            'multiple_from' => '',
            'multiple_number' => '',
            'multiple_to' => '',
          ),
          'type' => 'date_default',
          'weight' => 5,
        ),
        'teaser' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0,
        ),
      ),
      'field_name' => 'field_sp_news_date',
      'label' => $t('Publication date'),
      'required' => 1,
      'settings' => array(
        'default_value' => 'now',
        'default_value2' => 'same',
        'default_value_code' => '',
        'default_value_code2' => '',
        'user_register_form' => FALSE,
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'date',
        'settings' => array(
          'increment' => '15',
          'input_format' => 'm/d/Y',
          'input_format_custom' => '',
          'label_position' => 'above',
          'text_parts' => array(),
          'year_range' => '-3:+3',
        ),
        'type' => 'date_popup',
        'weight' => '15',
      ),
    ),

    'field_sp_url' => array(
      'default_value' => NULL,
      'deleted' => '0',   
      'description' => $t('URL of the full news story.'),
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
        'weight' => '25',
      ),    
    ),
           
    'field_sp_news_proposal' => array(
      'default_value' => NULL,
      'deleted' => '0',
      'description' =>$t('Optional proposal this news item refers to. If none is specified, it is a site-wide news item.'),
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
      'field_name' => 'field_sp_news_proposal',
      'label' => $t('Proposal'),
      'required' => 0,
      'settings' => array(
        'user_register_form' => FALSE,
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'options',
        'settings' => array(),
        'type' => 'options_select',
        'weight' => '35',
      ),
    ),
 
    'field_sp_news_source' => array(
      'default_value' => NULL,
      'deleted' => '0', 
      'description' => $t('The publication, website, or other source of this news item.'),
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
      'field_name' => 'field_sp_news_source',
      'label' => $t('Source'),
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
  );
}

/**
 * Delete the news node type.
 * 
 */
function smartparticipation_core_delete_news_node_type() {
  smartparticipation_core_delete_node_type('sp_news');
}


/**
 * Return data for a single news node for display on the Media page
 * 
 * TODO This should be done via a node view and a call to render() in the
 * template. 
 * TODO Apply glossary (site or proposal). This will be easier if we use a node
 * view.
 * 
 */
function smartparticipation_core_news_data($nid) {

  $news = node_load($nid);

  $url = smartparticipation_core_field_get_item_value('node', $news, 'field_sp_url', 'value');
  $title = '"' . $news->title . '"';
  $title_link = l(
      $title,
      $url,
      array(
        'attributes' => array(
          'target' => '_blank',
          'title' => 'View full article',  
        ),
        'external' => true,                 
  ));
  $full_article_link = l(
      'View full article',
      $url,
      array(
          'attributes' => array(
            'target' => '_blank',
            'title' => 'View full text of ' . $title,
          ),
          'external' => true,
      ));
  
  $date = smartparticipation_core_field_get_item_value('node', $news, 'field_sp_news_date', 'value');
  $date = strftime('%B %e, %Y', strtotime($date));
  
  $source = smartparticipation_core_field_get_item_value('node', $news, 'field_sp_news_source', 'value');
  $body = smartparticipation_core_field_get_item_value('node', $news, 'body', 'value');
  
  return array(
    '#theme' => 'news_item',
    'body' => $body,
    'date' => $date,
    'full_article_link' => $full_article_link,
    'source' => $source,
    'title_link' => $title_link,
  );
}

function smartparticipation_core_news_node_view(&$node, $view_mode, $langcode) {
  
  $content = $node->content;
  
  $full_article_url = $content['field_sp_url'][0]['#markup'];
  
  if (! node_is_page($node)) {
    $content['title'] = l(
        $node->title,
        $full_article_url,
        array(
          'attributes' => array(
            'target' => '_blank',
            'title' => 'View full article',  
          ),
          'external' => true, 
        )                
    );
  }
  
  $content['full_article_link'] = l(
      'View full article',
      $full_article_url,
      array(
        'attributes' => array(
          'target' => '_blank',
          'title' => 'View full text of ' . $node->title,
        ),
        'external' => true,
      )
  );
  
  $content['raw_date'] = smartparticipation_core_field_get_item_value('node', $node, 'field_sp_news_date', 'value');

  // Write new content back to the node
  $node->content = $content;

}


