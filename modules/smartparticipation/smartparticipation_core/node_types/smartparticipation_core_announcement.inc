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
function smartparticipation_core_create_announcement_node_type() {
  
  $t = get_t();
  
  $announcement = array(
    'type' => 'sp_announcement',
    'name' => $t('Announcement'),
    // Drupal will handle this node as if we had designed it in the UI.
    'base' => 'node_content', 
    'has_title' => TRUE,  
    'description' => $t('Content for the announcements page. Includes both site-wide and proposal-specific announcements.'),
    'custom' => TRUE,
    'help' => '',
  );  
  
  // Set defaults not explicitly declared above.
  $node_type = node_type_set_defaults($announcement);
  
  // Add body field and set label.
  node_add_body_field($node_type);
  
  // Modify properties of the body field.
  $instance = field_read_instance('node', 'body', 'sp_announcement');
  $instance['widget']['weight'] = 35;
  field_update_instance($instance);
  
  node_type_save($node_type);

  // Create the fields we are adding to the announcement node type.
  foreach (smartparticipation_core_announcement_installed_fields() as $field) {
    $field['entity_types'] = array('node');
    field_create_field($field);
  }
  
  // Create all the instances for our fields.
  foreach (smartparticipation_core_announcement_installed_instances() as $instance) {
    $instance['bundle'] = $announcement['type'];
    $instance['entity_type'] = 'node';
    field_create_instance($instance);
  }

  // Disable commenting for this node type  
  variable_set('comment_sp_announcement', COMMENT_NODE_HIDDEN);
  
  // Pathauto alias pattern  
  variable_set('pathauto_node_sp_announcement_pattern', 'announcements/[node:title]');

  // Create a revision by default when a new version is saved
  variable_set('node_options_sp_announcement', array(
    'revision',
    'status'
  ));
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
function smartparticipation_core_announcement_installed_fields() {
  
  return array(

    // Start date with optional end date
    'field_sp_announcement_dates' => array(
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'field_name' => 'field_sp_announcement_dates',
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
    
    'field_sp_announcement_proposal' => array(
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'field_name' => 'field_sp_announcement_proposal',
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
function smartparticipation_core_announcement_installed_instances() {
  $t = get_t();
   
  return array(

    'field_sp_announcement_date' => array(
      'deleted' => '0',
      'description' => 'The date(s) the announcement is active. The announcement is activated on the start date, and remains active until the end date, or indefinitely if no end date is specified.',
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
          'weight' => 5,
        ),
        'teaser' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0,
        ),
      ),
      'field_name' => 'field_sp_announcement_dates',
      'label' => $t('Start and end dates'),
      // Make the field optional so an announcement can be prepared before the
      // active date is known.
      'required' => 0,
      'settings' => array(
        'default_value' => 'now',
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
        'weight' => '15',
      ),
    ),
       
    'field_sp_announcement_proposal' => array(
      'default_value' => NULL,
      'deleted' => '0',
      'description' =>$t('Optional proposal this announcement refers to. If none is specified, it is a site-wide announcement. Announcements associated with a proposal will have the proposal glossary applied.'),
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
      'field_name' => 'field_sp_announcement_proposal',
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
        'weight' => '25',
      ),
    ),
  );
}

/**
 * Delete the announcement node type.
 * 
 */
function smartparticipation_core_delete_announcement_node_type() {
  smartparticipation_core_delete_node_type('sp_announcement');
}

/**
 * Implements hook_form_FORM_ID_alter().
 */
function smartparticipation_core_form_sp_announcement_node_form_alter(&$form, &$form_state, $form_id) {
  $form['#validate'][] = '_smartparticipation_core_announcement_node_form_validate';
}

/** 
 * Custom validator for announcement node form.
 */
function _smartparticipation_core_announcement_node_form_validate($form, &$form_state) {

  $dates = $form_state['values']['field_sp_announcement_dates'][LANGUAGE_NONE][0];
  $start_date = $dates['value'];
  $end_date = $dates['value2'];
  if ($end_date != NULL && $end_date == $start_date) {
    $form_state['values']['field_sp_announcement_dates'][LANGUAGE_NONE][0]['value2'] = NULL;
  }

}

/**
 * Page callback for Announcements page
 */
function smartparticipation_core_announcements_page() {

  // Fetch the data
  $sql = 'SELECT n.nid 
          FROM {node} n 
          LEFT OUTER JOIN {field_data_field_sp_announcement_dates} d
          ON n.nid = d.entity_id
          WHERE n.type = :type
          AND n.status = 1
          AND d.field_sp_announcement_dates_value <= NOW()
          AND (d.field_sp_announcement_dates_value2 IS NULL 
          OR d.field_sp_announcement_dates_value2 >= NOW())
          ORDER BY d.field_sp_announcement_dates_value DESC';

  $nids = db_query($sql, array(':type' => 'sp_announcement'))
    ->fetchAllAssoc('nid', PDO::FETCH_ASSOC);


  // Assemble the node data into a structured array
  $announcements = array();
  foreach ($nids as $nid => $announcement) {    
    $node = node_load($nid);
    $start_date = smartparticipation_core_field_get_item_value('node', $node, 'field_sp_announcement_dates', 'value');
    $body = smartparticipation_core_field_get_item_value('node', $node, 'body', 'safe_value');
    
    // Apply glossary: proposal glossary for a proposal-specific announcement,
    // otherwise site glossary.
    $proposal_nid = smartparticipation_core_field_get_item_value('node', $node, 'field_sp_announcement_proposal', 'nid');
    //if ($proposal_nid) {
      $body = smartparticipation_core_apply_glossary_lookup($body, $proposal_nid);
    //}
    
    $announcements[] = array(
      '#theme' => 'announcement',
      'title' => $node->title,
      'body' => $body,
      'start_date' => strftime('%B %e, %Y', strtotime($start_date)),
    );   
  }
  
  
  // Return a render array
  return array(
    '#items' => $announcements, 
    '#theme' => 'announcements',
    'image_path' => drupal_get_path('theme', 'smartparticipation_base') . '/images',
  );  

}

/**
 * Return number of new announcements.
 * 
 * For now, a "new" announcements is one whose start display date is within the
 * last month. A future improvement will involve counting the announcements 
 * made since the user last visited the site (or the Announcements page), to be
 * tracked with a cookie.
 */
function _smartparticipation_core_new_announcement_count() {
  
  /*
   * EntityFieldQuery doesn't allow OR field conditions. We need either value2
   * > now OR empty.
  $query = new EntityFieldQuery;
  
  $one_month_ago = new DateTime();
  $one_month_ago->sub(new DateInterval('P1M'));
  $now = new DateTime();

  $result = $query
    ->count()
    ->entityCondition('entity_type', 'node')
    ->propertyCondition('type', 'smartparticipation_core_announcement')
    ->propertyCondition('status', 1)
    ->fieldCondition('field_sp_announcement_dates', 'value', $one_month_ago->format('Y-m-d'), '>=')
    ->fieldCondition('field_sp_announcement_dates', 'value2', $now->format('Y-m-d'), '<')
    ->execute();
  
  return($result);
  */
  
  $query = 'SELECT nid 
            FROM {node} n
            LEFT OUTER JOIN field_data_field_sp_announcement_dates d ON d.entity_id = n.nid
            WHERE n.type = :type
            AND n.status = 1
            AND d.field_sp_announcement_dates_value >= DATE_SUB(CURDATE(), INTERVAL 1 MONTH)
            AND (d.field_sp_announcement_dates_value2 IS NULL OR d.field_sp_announcement_dates_value2 > CURDATE())';
  
  $result = db_query($query, array(
    ':type' => 'sp_announcement',
  ));
  
  return $result->rowCount();
}