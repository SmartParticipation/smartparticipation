<?php

/**
 * @file 
 * Define the faq node type.
 * 
 * @ingroup smartparticipation_core
 */
 
/**
 * Create the faq node type.
 */
function smartparticipation_core_create_faq_node_type() {
  
  $t = get_t();
  
  $faq = array(
    'type' => 'sp_faq',
    'name' => $t('FAQ'),
    // Drupal will handle this node as if we had designed it in the UI.
    'base' => 'node_content', 
    'has_title' => TRUE, 
    'title_label' => $t('Question'),   
    'description' => $t('Frequently asked questions with answers.'),
    'custom' => TRUE,
    'help' => '',
  );  
  
  // Set defaults not explicitly declared above.
  $node_type = node_type_set_defaults($faq);
  
  // Add body field and set label.
  node_add_body_field($node_type, $t('Answer'));
  
  node_type_save($node_type);

  // Create the fields we are adding to the faq node type.
  foreach (smartparticipation_core_faq_installed_fields() as $field) {
    $field['entity_types'] = array('node');
    field_create_field($field);
  }

  // Create all the instances for our fields.
  foreach (smartparticipation_core_faq_installed_instances() as $instance) {
    $instance['bundle'] = $faq['type'];
    $instance['entity_type'] = 'node';
    field_create_instance($instance);
  }

  // Disable commenting for this node type  
  variable_set('comment_sp_faq', COMMENT_NODE_HIDDEN);

  // Create a revision by default when a new version is saved
  variable_set('node_options_sp_faq', array('revision', 'status'));
  
  smartparticipation_core_set_faq_weight_settings();

}

/**
 * Configure the weight module to sort smartparticipation_core_faq nodes.
 */
function smartparticipation_core_set_faq_weight_settings() {

  // Configure the weight module to sort smartparticipation_core_faq nodes.
  $settings = array(
    'sp_faq' => array(
      'enabled' => 1,
      'range' => 50,
      'menu_weight' => 0,
      'default' => 0,
      'sync_translations' => 0,      
    ),
  );
  
  // This does a db_merge, so this function can be called from install, or 
  // update, or both, and we'll still end up with only one record in the table.
  weight_settings_save($settings);
    
}

/**
 * Delete the faq node type.
 * 
 */
function smartparticipation_core_delete_faq_node_type() {

  smartparticipation_core_delete_node_type('sp_faq');
  smartparticipation_core_unset_faq_weight_settings();
}

/**
 * Delete the smartparticipation_core_faq weight settings during module uninstall.
 */
function smartparticipation_core_unset_faq_weight_settings() {
  
  db_delete('weight_settings')
    ->condition('type', 'sp_faq')
    ->execute(); 
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
function smartparticipation_core_faq_installed_fields() {

  return array(
    'field_sp_faq_category' => array(
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'field_name' => 'field_sp_faq_category',
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
      'locked' => '1',
      'module' => 'taxonomy',
      'settings' => array(
        'allowed_values' => array(
          0 => array(
            'vocabulary' => 'sp_faq_category',
            'parent' => '0',
          ),
        ),
      ),
      'translatable' => '0',
      'type' => 'taxonomy_term_reference',
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
function smartparticipation_core_faq_installed_instances() {
  $t = get_t();
  
  return array(
    'field_sp_faq_category' => array(
      'default_value' => NULL,
      'deleted' => '0',
      'description' => $t('Select a category from the list.'),
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'taxonomy',
          'settings' => array(),
          'type' => 'taxonomy_term_reference_link',
          'weight' => 1,
        ),
        'teaser' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0,
        ),
      ),
      'field_name' => 'field_sp_faq_category',
      'label' => $t('FAQ category'),
      'required' => 1,
      'settings' => array(
        'user_register_form' => FALSE,
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'options',
        'settings' => array(),
        'type' => 'options_select',
        'weight' => '-10',
      ),
    ),
  );
}
 
/**
 * Page callback for FAQ page
 */
function smartparticipation_core_faq_page() {

  // Fetch the data
  $sql = 'SELECT t.name AS faq_category, n.nid
          FROM {node} n
          INNER JOIN {field_data_body} b ON b.entity_id = n.nid
          INNER JOIN {field_data_field_sp_faq_category} c ON n.nid = c.entity_id
          INNER JOIN {taxonomy_term_data} t ON c.field_sp_faq_category_tid = t.tid
          LEFT OUTER JOIN {weight_weights} w ON w.entity_id = n.nid
          WHERE n.type = :type
          AND n.status = 1
          ORDER BY t.weight, w.weight, n.title';

  $nids = db_query($sql, array(':type' => 'sp_faq'))
    ->fetchAllAssoc('nid', PDO::FETCH_ASSOC);
    
  
  // Assemble the node data into a structured array
  $faq_categories = array();
  
  if (count($nids)) {
    
    $glossary_data = smartparticipation_core_get_site_glossary_data();
    
    foreach ($nids as $nid => $faq) {    
      $faq_category = $faq['faq_category'];
      if (end($faq_categories) && $faq_category != key(end($faq_categories))) {
        $faq_categories[$faq_category] = array();
      }
      /* 
       * We need to get the node data from node_load() rather than just pulling
       * it in the query, since we need info like the format for the body. This is
       * a problem elsewhere, too: e.g., in the subtopic display. Even better
       * would be to invoke the node display via a template for the partial view.
       */
      $node = node_load($nid);
      $answer = smartparticipation_core_field_get_item_value('node', $node, 'body', 'safe_value');
      $answer = smartparticipation_core_insert_glossary_definitions($answer, $glossary_data); 
      
      $faq_categories[$faq_category][] = array(
        'question' => $node->title,
        'answer' => $answer,
      );
      
    }
  }
  
  // Return a render array
  return array(
    '#items' => $faq_categories, 
    '#theme' => 'faqs',
  );  

}
 