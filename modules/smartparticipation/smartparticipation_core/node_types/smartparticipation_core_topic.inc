<?php

/**
 * @file 
 * Define the topic node type.
 * 
 * @ingroup smartparticipation_core
 */
 
/**
 * Create the topic node type.
 */
function smartparticipation_core_create_topic_node_type() {
  
  $t = get_t();
  
  $topic = array(
    'type' => 'sp_topic',
    'name' => $t('Proposal topic'),
    // Drupal will handle this node as if we had designed it in the UI.
    'base' => 'node_content', 
    'has_title' => TRUE, 
    'title_label' => $t('Title'),   
    'description' => $t('A section of the proposal text that displays on a page. Topic content is further divided into subtopics.'),
    'custom' => TRUE,
    'help' => '',
  );  
  
  // Set defaults not explicitly declared above.
  $node_type = node_type_set_defaults($topic);

  node_type_save($node_type);


  // Create the fields we are adding to the topic node type.
  foreach (smartparticipation_core_topic_installed_fields() as $field) {
    $field['entity_types'] = array('node');    
    field_create_field($field);
  }

  // Create all the instances for our fields.
  foreach (smartparticipation_core_topic_installed_instances() as $instance) {
    $instance['bundle'] = $topic['type'];
    $instance['entity_type'] = 'node';
    field_create_instance($instance);
  }
  
  // Topics do not receive comments directly, but only via their subtopics.
  variable_set('comment_sp_topic', COMMENT_NODE_HIDDEN);

  // Pathauto alias pattern
  $proposal_type = 
    variable_get('smartparticipation_site_proposal_type_plural', 'proposals');
  variable_set('pathauto_node_sp_topic_pattern',
    $proposal_type . '/[node:field_sp_topic_proposal]/[node:field_sp_topic_phase]/[node:field_sp_menu_title]');

  // Create a revision by default when a new version is saved
  variable_set('node_options_sp_topic', array('revision', 'status'));
}

/**
 * Delete the topic node type.
 * 
 */
function smartparticipation_core_delete_topic_node_type() {
  smartparticipation_core_delete_node_type('sp_topic');
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
function smartparticipation_core_topic_installed_fields() {
  
  return array(

    'field_sp_topic_description' => array(
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_sp_topic_description',
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
    
    'field_sp_topic_phase' => array(
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
      'field_name' => 'field_sp_topic_phase',
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
      
    'field_sp_topic_proposal' => array(
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'field_name' => 'field_sp_topic_proposal',
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
    
    'field_sp_topic_subtopic' => array(
      'active' => '1',
      'cardinality' => '-1',
      'deleted' => '0',
      'field_name' => 'field_sp_topic_subtopic',
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
          'sp_subtopic' => 'sp_subtopic',
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
function smartparticipation_core_topic_installed_instances() {
  
  $t = get_t();
  $instances = array();
  

  // TODO This could perhaps be removed. We are currently not using it. So far
  // in links we're using the short title as link text and the node title as
  // link title attribute.
  $instances['field_sp_topic_description'] = array(
    'default_value' => NULL,
    'deleted' => '0',   
    'description' => $t('An optional brief description of the topic. This does not display on the topic page, but could be used in link titles, etc.'),
    'display' => array(
      'default' => array(
        'type' => 'hidden',
      ),
      'teaser' => array(
        'type' => 'hidden',
      ),
    ),
    'field_name' => 'field_sp_topic_description',
    'label' => $t('Description'),      
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
  
  $instances['field_sp_topic_phase'] = array(
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
    'field_name' => 'field_sp_topic_phase',
    'label' => 'Phase',
    'required' => 1,
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'weight' => '5',
      'type' => 'options_select',
      'module' => 'options',
      'active' => 1,
      'settings' => array(),
    ),
  );
  
  $instances['field_sp_topic_proposal'] = array(
    'default_value' => NULL,
    'deleted' => '0',
    'description' =>$t('The proposal this topic belongs to.'),
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
    'field_name' => 'field_sp_topic_proposal',
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
      'weight' => '-10',
    ),
  );

  $instances['field_sp_menu_title'] = array(
    'default_value' => NULL,
    'deleted' => '0', 
    'description' => $t('Alternate short title used in menus, the topic carousel, and the topic\'s default URL.'),
    'display' => array(
      'default' => array(
        'type' => 'hidden',
      ),
      'teaser' => array(
        'type' => 'hidden',
      ),
    ), 
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
  );
  
  $instances['field_sp_topic_subtopic'] = array(
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
    'field_name' => 'field_sp_topic_subtopic',
    'label' => $t('Subtopic'),
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
  );
    
  $instances['field_sp_phase_closed_message'] = smartparticipation_core_topic_phase_closed_message_instance();
  
  return $instances;
}

/**
 * Define the phase closed message field instance for the topic node type.
 */
function smartparticipation_core_topic_phase_closed_message_instance() {

  $t = get_t();

  return array(
    'bundle' => 'sp_topic',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => $t('Message to display when the topic\'s phase is closed. Can be overridden by a message specified on the subtopic.'),
    'display' => array(
      'default' => array(
        'type' => 'hidden',
        'label' => 'hidden',
        'module' => 'text',
        'settings' => array(),
        'type' => 'text_default',
      ),
      'teaser' => array(
        'type' => 'hidden',
      ),
    ),
    'entity_type' => 'node',
    'field_name' => 'field_sp_phase_closed_message',
    'label' => $t('Phase closed message'),
    'required' => 0,
    'settings' => array(
      'text_processing' => 1,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'text',
      'settings' => array(
          'rows' => '3',
      ),
      'type' => 'text_textarea',
      'weight' => 15,
    ),
  );
}

/** 
 * Implements hook_form_FORM_ID_alter().
 */
function smartparticipation_core_form_sp_topic_node_form_alter(&$form, &$form_state, $form_id) {
  
  // Get the list of options to populate the first dropdown.
  $options_first = _smartparticipation_core_get_proposal_options();

  // If we have a value for the first dropdown from $form_state['values'] we use
  // this both as the default value for the first dropdown and also as a
  // parameter to pass to the function that retrieves the options for the
  // second dropdown.

  if (count($options_first) == 0) {
    $message = 'There are no open proposals in the system. You must '.l('add at least one open proposal','node/add/smartparticipation_core-proposal').' before you can create a topic.';
      drupal_set_message($message, $type = 'error', $repeat = FALSE);
      hide($form);
      return;
  }

  if($form['nid']['#value']) {
    $selected_proposal = _smartparticipation_core_get_proposal_db($form['nid']['#value']);
    $selected_phase = _smartparticipation_core_get_phase_db($form['nid']['#value']);
  } else {
    $selected_proposal = isset($_REQUEST['field_sp_topic_proposal'][LANGUAGE_NONE]) ? $_REQUEST['field_sp_topic_proposal'][LANGUAGE_NONE] : key($options_first);
    $selected_phase = isset($form_state['values']['field_sp_topic_phase']) ? $form_state['values']['field_sp_topic_phase'] : '';
  }


  $form['field_sp_topic_proposal'][LANGUAGE_NONE]['#ajax'] = array(
    'event' => 'change',
    'wrapper' => 'dropdown-second-replace',
    'callback' => 'smartparticipation_core_topic_form_ajax_callback',
  );
  $form['field_sp_topic_proposal'][LANGUAGE_NONE]['#options'] = $options_first;
  
  /*
  if($selected_proposal != '') {
    $form['field_sp_topic_proposal'][LANGUAGE_NONE]['#default_value'] = $selected_proposal;
  } else {
    $predefined_proposal = arg(3);
    $selected_proposal = $predefined_proposal;
    $form['field_sp_topic_proposal'][LANGUAGE_NONE]['#default_value'] = $predefined_proposal;
  }
  */
  
  if($selected_proposal == '') {
    $selected_proposal = arg(3);
  }
  
  $form['field_sp_topic_proposal'][LANGUAGE_NONE]['#default_value'] = $selected_proposal;
   
  $form['field_sp_topic_phase'][LANGUAGE_NONE]['#prefix'] = '<div id="dropdown-second-replace">';
  $form['field_sp_topic_phase'][LANGUAGE_NONE]['#suffix'] = '</div>';
  $form['field_sp_topic_phase'][LANGUAGE_NONE]['#options'] = _smartparticipation_core_get_phase_options($selected_proposal);
  $form['field_sp_topic_phase'][LANGUAGE_NONE]['#default_value'] = $selected_phase;

  $form['field_sp_phase_closed_message'][LANGUAGE_NONE]['0']['#format'] = 'full_html';
  
  // A subtopic is assigned to a topic via the subtopic form, not the topic 
  // form, so suppress the field here.
  $form['field_sp_topic_subtopic']['#access'] = FALSE;
  
  // Override default setting of false for nodes in 
  // smartparticipation_core_form_node_form_alter().
  $form['path']['#access'] = true;

  $form['#submit'][] = 'smartparticipation_core_topic_form_submit';

}

function smartparticipation_core_topic_form_ajax_callback($form, $form_state) {
  return $form['field_sp_topic_phase'];
}


function _smartparticipation_core_get_proposal_db($nid) {
  $query = db_select('field_data_field_sp_topic_proposal', 'p')
    ->fields('p', array('field_sp_topic_proposal_nid', 'entity_id'));
  $query->where("p.entity_id = :nid", array(':nid' => $nid));
  $query->leftJoin('node', 'n', 'n.nid = p.field_sp_topic_proposal_nid');
  $query->fields('n', array('title'));

  $result = $query->execute();

  foreach($result as $key => $value) {
    return t($value->field_sp_topic_proposal_nid);
  }
}

/**
 * Retrieve phase tid associated with the specified topic from a direct 
 * database query.
 * 
 * TODO Not sure why we need this. Just use smartparticipation_core_get_topic_phase_tid instead.
 * 
 * @param $nid - the topic nid
 * 
 * @return the phase tid
 */
function _smartparticipation_core_get_phase_db($nid) {
  $query = db_select('field_data_field_sp_topic_phase', 'p')
    ->fields('p', array('field_sp_topic_phase_tid', 'entity_id'));
  $query->where("p.entity_id = :nid", array(':nid' => $nid));

  $result = $query->execute();

  foreach($result as $key => $value) {
    return $value->field_sp_topic_phase_tid;
  }
}

/**
 * Return the tid of the taxonomy phase term associated with the specified 
 * topic.
 * 
 * @param $topic_nid - the nid of the topic
 * 
 * @return the phase tid
 */
function smartparticipation_core_get_topic_phase_tid($topic_nid) {
  
  $topic = node_load($topic_nid);
  $phase_tid = smartparticipation_core_field_get_item_value('node', $topic, 'field_sp_topic_phase', 'tid');
  return $phase_tid;
}

/**
 * Return the phase term associated with the specified topic
 * 
 * @param $topic_nid - the topic nid
 * 
 * @return the phase term
 */
function smartparticipation_core_get_topic_phase($topic_nid) {
  
  $phase_tid = smartparticipation_core_get_topic_phase_tid($topic_nid);
  $phase = taxonomy_term_load($phase_tid);
  return $phase;
}

/**
 * Helper function to populate the second dropdown. This would normally be
 * pulling data from the database.
 *
 * @param $key
 *   This will determine which set of options is returned.
 *
 * @return array of options
 */
function _smartparticipation_core_get_phase_options($key) {
  $proposal_list = _smartparticipation_core_get_proposal_options();
  foreach($proposal_list as $id => $proposal_name) {
    $options[$id] = _smartparticipation_core_get_proposal_phase_options($id);
  }

  if (isset($options[$key])) {
    return $options[$key];
  }
  else {
    return array();
  }
}

function _smartparticipation_core_get_proposal_phase_options($nid) {
  if($nid == 0) {
    $options[''] = t('- Select a value -');
  }

  $query = db_select('field_data_field_sp_proposal_phase', 's')
    ->fields('s', array('entity_id', 'field_sp_proposal_phase_value'));
  $query->where("s.entity_id = :nid", array(':nid' => $nid));
  $query->leftJoin('field_data_field_sp_proposal_phase_term', 'p', 'p.entity_id = s.field_sp_proposal_phase_value');
  $query->fields('p', array('field_sp_proposal_phase_term_tid'));
  $query->leftJoin('taxonomy_term_data', 't', 't.tid = p.field_sp_proposal_phase_term_tid');
  $query->fields('t', array('name'));

  $result = $query->execute();
  foreach($result as $key => $value) {
    $options[$value->field_sp_proposal_phase_term_tid] = t($value->name);
  }

  return $options;
}


function smartparticipation_core_topic_form_submit($form, &$form_state) {
  
  if($form_state['values']['nid']) {
    // the node has been edited
    $node = node_load($form_state['values']['nid']);
  } else {
    // the node has been added
    $node = new stdClass();  
  }

  $node->type = 'sp_topic';
  $node->language = LANGUAGE_NONE;

  global $user;
  $node->uid = $user->uid;

  $node->title = $form_state['values']['title'];

  $proposal_nid = $form_state['values']['field_sp_topic_proposal'][LANGUAGE_NONE][0]['nid'];
  $node->field_sp_topic_proposal[LANGUAGE_NONE][0]['nid'] = $proposal_nid;
  
  $node->field_sp_topic_phase[LANGUAGE_NONE][0]['tid'] = $form_state['values']['field_sp_topic_phase'][LANGUAGE_NONE][0]['tid'];

  $node->field_sp_topic_description[LANGUAGE_NONE][0]['value'] = $form_state['values']['field_sp_topic_description'][LANGUAGE_NONE][0]['value'];

}

/**
 * Return an array of subtopics of the specified topic, ordered by weight.
 *
 * @param $nid_topic - the topic nid
 * @param $select_weight (optional, boolean) - if true, include weight in the
 * result set
 *
 * @return If $select_weight is true, return an indexed array of objects, else
   * return an indexed array of document nids.
 */
function smartparticipation_core_topic_subtopics($nid_topic, $select_weight = false) {

  $query = 'SELECT n.nid';

  if ($select_weight) {
    $query .= ', o.weight';
  }

  $query .= ' FROM {node} n
           INNER JOIN {field_data_field_sp_topic_subtopic} ts ON n.nid = ts.field_sp_topic_subtopic_nid
           LEFT OUTER JOIN {sp_subtopic_node_order} o ON n.nid = o.snid AND ts.entity_id = o.tnid
           WHERE n.status = 1
           AND ts.entity_id = :nid_topic
           ORDER BY o.weight ASC, n.created ASC';

  $result = db_query($query, array(
      ':nid_topic' => $nid_topic,
  ));

  if ($select_weight) {
    // Returned an indexed array of objects containing nid and weight values
    $subtopics = $result->fetchAll();
  } else {
    // Return an indexed array of nids
    $subtopics = array_keys($result->fetchAllAssoc('nid', PDO::FETCH_ASSOC));
  }

  return $subtopics;

}
  
/**
 * Module function to modify the content sent to the topic node template.
 * 
 * See documentation of hook_node_view:
 * The module may add elements to $node->content prior to rendering. This hook 
 * will be called after hook_view(). The structure of $node->content is a 
 * renderable array as expected by drupal_render().
 * 
 * TODO Rewrite to make use of subtopic node view rather than compiling a
 * new array of subtopic data.
 */
function smartparticipation_core_topic_node_view($topic, $view_mode, $langcode) {

  $account = $GLOBALS['user'];

  if ($view_mode == 'full') {
    $uid = $account->uid;

    // PROPOSAL DATA
    $proposal_nid = smartparticipation_core_field_get_item_value('node', $topic, 'field_sp_topic_proposal', 'nid', $langcode);
    $proposal_details = smartparticipation_core_proposal_data($proposal_nid);

    
    // OTHER TOPICS IN THE PROPOSAL PHASE
    $topics_list = array();
    $tid = smartparticipation_core_field_get_item_value('node', $topic, 'field_sp_topic_phase', 'tid');
    $topics_list = smartparticipation_core_topic_topics_same_phase($proposal_nid, $tid);

    
    // PHASE DATA
    $topic_phase_tid = smartparticipation_core_field_get_item_value('node', $topic, 'field_sp_topic_phase', 'tid');

    $topic_phase = taxonomy_term_load($topic_phase_tid); // topic's phase


    // This value is based simply on the phase term field value
    $phase_allows_comments = smartparticipation_core_field_get_item_value('taxonomy_term', $topic_phase, 'field_sp_phase_allows_comments', 'value');
 
    $proposal = node_load($proposal_nid);


    $phases = _smartparticipation_core_get_proposal_phase_data($proposal);
    $phase_id = null;
    foreach ($phases as $phase_data) {
      if ($topic_phase_tid == $phase_data['tid']) {
        if (isset($phase_data['phase_subtopic_heading'])) {
          $phase_text_heading = $phase_data['phase_subtopic_heading'];
        }
        else {
          $phase_text_heading = smartparticipation_core_field_get_item_value('taxonomy_term', $topic_phase, 'field_sp_phase_text_heading', 'value');
        }
        break;
      }
    }

    // This value incorporates the phase term field value and a number of other
    // factors: whether the proposal is open or closed, published or 
    // unpublished;  whether the phase is past, present, or future; etc.
    $topic_commentable = smartparticipation_core_topic_commentable($topic, $proposal, $topic_phase, $phase_allows_comments);    

    // INTEREST SURVEY
    // If topic is open for commenting, load the proposal's interest survey if
    // needed for the current user.
    $interest_survey = '';
    if ($topic_commentable) {
      $interest_survey = smartparticipation_core_load_proposal_interest_survey($account, $proposal);
    }


    // COMMENT TIPS
    // Check for related comment tips node
    $comment_tips_service = new \SmartParticipation\CommentTipsService;
    $comment_tips = $comment_tips_service->find($proposal_nid, $topic_phase_tid);
    $user_service = new \SmartParticipation\UserService;
    $user = $user_service->find($uid);
    /*if ($comment_tips instanceof \SmartParticipation\model\CommentTips) {
      $comment_tips_service->applyUserTypeFilter($comment_tips, $account);
    }*/
    $ab_test_service = new \SmartParticipation\ABTestCommentTipsService;
    if ($user && $comment_tips && $ab_test_service->userIsEligible($user)) {
      // Check for active comment tips A/B test
      if ($active_ab_test = $ab_test_service->getActiveABTest()) {
        // Check for an existing user variant for this test
        // Assign the user a variant if they don't have one
        if ($user) {
          $ab_test_user_variant_service = new \SmartParticipation\ABTestUserVariantService;
          $user_variant = $ab_test_user_variant_service->find($active_ab_test, $user);
          // Retrieve info about this test
          $ab_test_info = $ab_test_service->getTestInfo($user_variant, $comment_tips_service, $proposal_nid, $topic_phase_tid);
          // The user is assigned to a variant where comment tips are enabled
          if ($ab_test_service->showCommentTips($ab_test_info)) {
            // Is a second set of comment tips being used for this variant?
            // If so, replace the comment tips set above with the return value
            if ($variant_comment_tips = $ab_test_service->getVariantCommentTips($ab_test_info)) {
              $comment_tips = $variant_comment_tips;
            }
          }
          // Comment tips are disabled for this variant
          else {
            $comment_tips = NULL;
          }
        }
      }
    }

    $use_comment_tips_prompt = FALSE;
    $use_comment_tips_button = FALSE;
    if ($user && $comment_tips) {
      $comment_tips_state = $comment_tips_service->getState($user, $comment_tips);
      $use_comment_tips_prompt = $comment_tips_state->useCommentTipsPrompt;
      $use_comment_tips_button = $comment_tips_state->useCommentTipsButton;
    }

    // SUBTOPIC DATA
    // TO DO Define a subtopic node view and use smartparticipation_core_subtopic_node_view() 
    // to massage the data.
    
    // $topic_comment_count = 0;
    
    $subtopics = array();
          
    if (!empty($topic->field_sp_topic_subtopic)) {
      
      $get_subtopic_phase_closed_messages = false;
      if (!$phase_allows_comments && _smartparticipation_core_proposal_phase_is_closed($proposal, $topic_phase)) {
        $get_subtopic_phase_closed_messages = true;  
      }
    
      $topic_subtopics = smartparticipation_core_topic_subtopics($topic->nid);
    
      $glossary_data = smartparticipation_core_get_proposal_glossary_data($proposal_nid);

      $recommended_comments_service = new \SmartParticipation\RecommendedCommentsService;
      $recommended_comments_enabled = $recommended_comments_service->getStatus();
      

      foreach($topic_subtopics as $index => $nid) {

        $subtopic_data = array();

        $subtopic_data['nid'] = $nid;
        
        $subtopic_node = node_load($nid);

        $subtopic_data['title'] = $subtopic_node->title;
        
        // $topic_comment_count += $subtopic_data['comment_count'] = $subtopic_node->comment_count;
        $subtopic_data['comment_count'] = $subtopic_node->comment_count;
              
        $subtopic_body_value = '';
        if (isset($subtopic_node->body[LANGUAGE_NONE][0]['value'])) {
          $subtopic_body_value = smartparticipation_core_field_get_item_value('node', $subtopic_node, 'body', 'value');
          $subtopic_body_value = smartparticipation_core_insert_glossary_definitions($subtopic_body_value, $glossary_data);
          $subtopic_body_value = nl2br($subtopic_body_value);
        }
        $subtopic_data['body'] = $subtopic_body_value;

        // Use the default formatter specified in the field instance. 
        $subtopic_data['doc'] = field_view_field('node', $subtopic_node, 'field_sp_subtopic_doc', 'default');
             
        if ($phase_allows_comments) {
          $subtopic_node->comment = $topic_commentable;
          $subtopic_node->interest_survey_required = ! empty($interest_survey);
          
          $comment_node_page_additions = comment_node_page_additions($subtopic_node);
          $comment_node_page_additions =
            smartparticipation_core_comment_node_page_additions($comment_node_page_additions, $subtopic_node);
          $subtopic_data['comment_node_page_additions'] = $comment_node_page_additions;
          $subtopic_data['comment_node_page_additions']['use_comment_tips_button'] = $use_comment_tips_button;
        }

        if ($get_subtopic_phase_closed_messages) {
          $topic_phase_closed_message = smartparticipation_core_field_get_item_value('node', $topic, 'field_sp_phase_closed_message', 'value');
          $subtopic_data['phase_closed_message'] = smartparticipation_core_subtopic_phase_closed_message($subtopic_node, $topic_phase_closed_message);
        }

        $subtopic_data['comment_node_page_additions']['recommended_comments_enabled'] = $recommended_comments_enabled;
        
        // Number from 1 rather than 0 for the template
        $subtopics[$index+1] = $subtopic_data;
      }     
    }

    $comment_tips_get_state_base_url = $GLOBALS['base_url'] . base_path() . 'ajax/comment_tips/';

    // Add all the data to the $topic->content array
    $topic->content = '';
    $topic->content = array(
      'commentable' => $topic_commentable,
      'interest_survey' => $interest_survey,
      'phase_allows_comments' => $phase_allows_comments,
      'phase_text_heading' => $phase_text_heading,
      'proposal' => $proposal_details,
      'subtopics' => $subtopics,
      // 'topic_comment_count' => $topic_comment_count,
      'topics_list' => $topics_list,
      'comment_tips' => $comment_tips,
      'use_comment_tips_prompt' => $use_comment_tips_prompt,
      'use_comment_tips_buttons' => $use_comment_tips_button,
      'comment_tips_get_state_base_url' => $comment_tips_get_state_base_url,
    );
  }

}


/**
 * Return commenting status to determine whether to display comment box and
 * comment links. Return value of false means commenting is unavailable; return 
 * value of true means it is available. The value is based on two factors: (1)
 * whether the topic phase allows commenting, and (2) whether the phase is 
 * past, present, or future. Note that a future phase will not display the 
 * comment box, but the div for the comment panel will still be present, so that
 * editors can view the eventual layout of the page while they are building it.
 * 
 * @param $topic - the topic node
 * @param $proposal (optional) - the proposal node 
 * @param $phase (optional) - the topic phase term
 * @param $phase_allows_comments (optional) - whether the phase allows comments.
 * 
 * Optional parameters not supplied will be retrieved here as needed.
 * 
 * @return boolean - true if commenting is allowed, false otherwise
 */
function smartparticipation_core_topic_commentable($topic, $proposal = null, $phase = null, $phase_allows_comments = null) {

  if (is_null($proposal)) {
    $proposal = smartparticipation_core_get_topic_proposal($topic);
  }
  // Unpublished proposal
  if (! $proposal->status) {
    return false;
  }
  
  // Open vs. closed proposal
  $proposal_status = smartparticipation_core_field_get_item_value('node', $proposal, 'field_sp_proposal_status', 'value');
  if (!$proposal_status) {
    return false;
  }

  if (is_null($phase)) {
    $phase = smartparticipation_core_get_topic_phase($topic->nid);
  }
  
  if (is_null($phase_allows_comments)) {
    $phase_allows_comments = smartparticipation_core_field_get_item_value('taxonomy_term', $phase, 'field_sp_phase_allows_comments', 'value');
  }
    
  // Non-commenting phase
  if (!$phase_allows_comments) {
    return false;
  }
  
  $phase_status = _smartparticipation_core_proposal_phase_status($proposal, $phase);
  
  return $phase_status == 'current';

}



/**
 * Return the proposal topics in the same phase with the current topic
 */
function smartparticipation_core_topic_topics_same_phase($proposal_id, $topic_phase_id) {
  
  // TODO Now the query can be combined with smartparticipation_core_topic_same_phase_proposal
  $query = db_select('field_data_field_sp_proposal_topic', 't')
    ->fields('t', array('field_sp_proposal_topic_nid'));
  $query->where("t.entity_id = :proposal_id", array(':proposal_id' => $proposal_id));
  $query->leftJoin('field_data_field_sp_topic_phase', 'p', 'p.entity_id = t.field_sp_proposal_topic_nid');
  $query->fields('p', array('entity_id'));
  $query->where("p.field_sp_topic_phase_tid = :topic_phase_id", array(':topic_phase_id' => $topic_phase_id));

  $result = $query->execute();
  foreach($result as $key => $value) {
    $topics[] = $value->entity_id;
  }

  return $topics;
}


/**
 * Remove the related subtopics when a topic is deleted
 */
function smartparticipation_core_delete_related_subtopics($nid) {
  $result = db_select('field_data_field_sp_topic_subtopic', 'd')
    ->fields('d', array('entity_id', 'field_sp_topic_subtopic_nid'))
    ->condition('d.entity_id', $nid)
    ->execute();

  foreach ( $result as $key => $node_related ) {
    node_delete($node_related->field_sp_topic_subtopic_nid);
  }
}

/**
 * Return topic data needed on display of topic pages.
 * 
 */
function smartparticipation_core_topic_data($nid) {
  
  $topic = node_load($nid);

  $topic_title = $topic->title;

  $topic_link = smartparticipation_core_node_menu_title_link($topic, 'View and comment');
  $topic_phase = smartparticipation_core_field_get_item('node', $topic, 'field_sp_topic_phase');

  $term = array_key_exists('taxonomy_term', $topic_phase) ? 
      $topic_phase['taxonomy_term'] : taxonomy_term_load($topic_phase['tid']);

  $topic_allows_comments = smartparticipation_core_field_get_item_value('taxonomy_term', $term, 'field_sp_phase_allows_comments', 'value');

  $proposal_nid = smartparticipation_core_field_get_item_value('node', $topic, 'field_sp_topic_proposal', 'nid');

  $topic_subtopics = field_get_items('node', $topic, 'field_sp_topic_subtopic');
  
  $topic_comments_number = 0;
  $topic_comments_recommended = 0;

  if ($topic_subtopics && $topic_allows_comments) {
    foreach ($topic_subtopics as $id_subtopic) {
      $nid_subtopic = $id_subtopic['nid'];
      $subtopic = node_load($nid_subtopic);
      $topic_comments_number += $subtopic->comment_count;
      
      if ($subtopic->comment_count) {
        $subtopic_comments_recommended = smartparticipation_core_subtopic_comments_recommended($nid_subtopic);
        $topic_comments_recommended += $subtopic_comments_recommended;
      }
    }
  }

  return array(
    'allows_comments' => $topic_allows_comments,
    'comments_number' => $topic_comments_number,
    'comments_recommended' => $topic_comments_recommended,
    'link' => $topic_link,
    'nid' => $nid,
    'phase' => $topic_phase,
    'proposal' => $proposal_nid,
    'subtopics' => $topic_subtopics,
    'title' => $topic_title,
    // 'url' => $topic_url,
  );
}


/**
 * Return an array with details about the proposal and topic node to be 
 * displayed in the topic page header.
 * 
 * @param $proposal_data - an array of proposal data
 * @param $nid_topic - the nid of the topic node being displayed
 * 
 * @return An array of data used to display the header on the page
 */
function smartparticipation_core_get_topic_header_data($proposal_data, $nid_topic) {
  
  $topic_details = smartparticipation_core_topic_data($nid_topic);
  
  $id_topic_phase = $topic_details['phase']['taxonomy_term']->tid;
  $nid_proposal = $proposal_data['nid_proposal'];
  $proposal = node_load($nid_proposal);
  
  $topic_phase_class = '';
  foreach ($proposal_data['phases'] as $phase) {
    if ($id_topic_phase == $phase['tid']) {
      $topic_phase_class = $phase['phase_class'];
      break;
    }
  }
  
  $proposal_type = variable_get('smartparticipation_site_proposal_type');
  $topic_phase_title = '';
  if ($topic_phase_class == 'past-phase') {
    $topic_phase_title = t('This phase of the ' . $proposal_type . ' is now closed.');
  } else if ($topic_phase_class == 'current-phase') {
    $topic_phase_title = t('This is the current phase of the ' . $proposal_type . '.');
  } // else future-phase - we don't get to the topic page


  // Get topics in order
  $proposal = node_load($nid_proposal);
  $phases = _smartparticipation_core_get_proposal_phase_data($proposal);
  $phase_id = null;
  foreach ($phases as $phase_data) {
    if ($phase_data['tid'] == $topic_details['phase']['taxonomy_term']->tid) {
      $phase_id = $phase_data['phase_id'];
      if (isset($phase_data['phase_display_text'])){
        $topic_phase = $phase_data['phase_display_text'];
      }
      else {
        $topic_phase = $topic_details['phase']['taxonomy_term']->name;
      }
      break;
    }
  }
  $topics = _smartparticipation_core_get_proposal_topics_phase($nid_proposal, $phase_id);

  $same_phase_topics = array();
  foreach ($topics as $topic) {
    $same_phase_topics[] = node_load($topic['nid']);
  }

  // The "select other topics" dropdown should not display if this is the only topic
  if (count($same_phase_topics) < 2) {
    $same_phase_topics = array();
  }

  $proposal_documents_link = smartparticipation_core_get_proposal_documents_link($proposal, 'sp_topic', TRUE);

  foreach($proposal_data['phases'] as $key=>$phase){
    $proposal_data['phases'][$key]['phase_topic_nid'] =
        smartparticipation_core_get_topic_nid_from_topic_title($phase['tid'], $nid_proposal, $topic_details['title']);
    $proposal_data['phases'][$key]['first_topic_nid'] =
        smartparticipation_core_get_first_proposal_phase_topic($phase['tid'], $nid_proposal);
  }

  $data = array(
    'node_nid' => $nid_topic, 
    'node_title' => $topic_details['title'],
    'node_type' => 'topic',
    'proposal' => $proposal_data, 
    'proposal_documents_link' => $proposal_documents_link,
    'proposal_phases' => $proposal_data['phases'], 
    'same_phase_topics' => $same_phase_topics,
    'topic_allows_comments' => $topic_details['allows_comments'],
    'topic_phase_id' => $id_topic_phase, 
    'topic_phase_class' => $topic_phase_class,
    'topic_phase_title' => $topic_phase_title,
    'topic_phase' => $topic_phase,
    'topic_number_comments' => $topic_details['comments_number'], 
  );

  return $data;
}


/**
 * Return the nids of all topics for a proposal and phase. The nid of a topic
 * to exclude can optionally be specified, to create an array of "other topics."
 * 
 * @param $id_topic_phase - the id of the topic phase
 * @param $nid_proposal - the nid of the topic proposal
 * @param $nid_topic - (optional) - if this is specified, exclude this topic
 * from the result set to create an array of "other topics".
 * 
 * @return An array of topic nids
 */
function smartparticipation_core_topic_same_phase_proposal($id_topic_phase, $nid_proposal, $nid_topic = NULL) {
  
  // Build the query  
  $query = 'SELECT n.nid FROM {node} n 
    INNER JOIN {field_data_field_sp_topic_phase} tph ON n.nid = tph.entity_id 
    INNER JOIN {field_data_field_sp_topic_proposal} tp  ON n.nid = tp.entity_id 
    LEFT OUTER JOIN {sp_topic_node_order} o ON o.ttid = :nid_proposal AND o.tnid = n.nid
    WHERE n.type = :type 
    AND n.status = 1
    AND tph.field_sp_topic_phase_tid = :id_topic_phase 
    AND tp.field_sp_topic_proposal_nid = :nid_proposal';
    
  if ($nid_topic) {
    $query .= ' AND n.nid != :nid_topic ';
  }
  
  $query .=  ' ORDER BY o.weight ASC';

  
  // Fetch the topic ids from the database
  $topic_ids = db_query($query, array(
    ':type' => 'sp_topic',
    ':id_topic_phase' => $id_topic_phase, 
    ':nid_proposal' => $nid_proposal, 
    ':nid_topic' => $nid_topic))
    ->fetchAllAssoc('nid', PDO::FETCH_ASSOC);

    
  // Return the ids
  return array_keys($topic_ids);
}


/**
 * Return the first topic in the specified phase of the specified proposal
 */
function smartparticipation_core_get_first_proposal_phase_topic($phase_id, $proposal_nid) {


  $query = 'SELECT n.nid FROM {node} n 
    INNER JOIN {field_data_field_sp_topic_phase} p ON n.nid = p.entity_id 
    INNER JOIN {field_data_field_sp_topic_proposal}  r ON n.nid = r.entity_id 
    LEFT OUTER JOIN {sp_topic_node_order} o ON o.ttid = :nid_proposal AND o.tnid = n.nid
    WHERE type = \'sp_topic\'
    AND p.field_sp_topic_phase_tid = :id_topic_phase 
    AND r.field_sp_topic_proposal_nid = :nid_proposal 
    AND n.status = 1 
    ORDER BY o.weight ASC 
    LIMIT 1';

  $topic_ids = db_query($query, array(
    ':id_topic_phase' => $phase_id, 
    ':nid_proposal' => $proposal_nid))
    ->fetchAllAssoc('nid', PDO::FETCH_ASSOC);

  return array_keys($topic_ids);
}

/**
 * Return nid of topic with given title in the specified phase of the specified proposal
 */
function smartparticipation_core_get_topic_nid_from_topic_title($phase_id, $proposal_nid, $topic_title) {


  $query = 'SELECT n.nid FROM {node} n
    INNER JOIN {field_data_field_sp_topic_phase} p ON n.nid = p.entity_id
    INNER JOIN {field_data_field_sp_topic_proposal}  r ON n.nid = r.entity_id
    LEFT OUTER JOIN {sp_topic_node_order} o ON o.ttid = :nid_proposal AND o.tnid = n.nid
    WHERE type = \'sp_topic\'
    AND n.title = :topic_title
    AND p.field_sp_topic_phase_tid = :id_topic_phase
    AND r.field_sp_topic_proposal_nid = :nid_proposal
    AND n.status = 1
    ORDER BY o.weight ASC
    LIMIT 1';

  $topic_ids = db_query($query, array(
      ':topic_title' => $topic_title,
      ':id_topic_phase' => $phase_id,
      ':nid_proposal' => $proposal_nid))
      ->fetchAllAssoc('nid', PDO::FETCH_ASSOC);

  return array_keys($topic_ids);
}


/**
 * Return true iff a user has access to view a topic
 * 
 * @param $topic - topic node object
 * 
 * @return true iff the current user has access to the topic
 */
function smartparticipation_core_topic_access($topic) {
  
  global $user;
  
  if (smartparticipation_core_is_anonymous_or_basic_authenticated_user($user)) {
    
    $topic_phase_dates = smartparticipation_core_topic_phase_dates($topic);
    $date_start = $topic_phase_dates['start'];
  
    $now = date('Y-m-d H:i:s');
      
    return ($date_start && $date_start <= $now);
  }
  
  return TRUE;
}

/**
 * Return a structured array for a carousel display of the topics in the same 
 * phase as the current topic. 
 * 
 * @param $node - the topic node
 * 
 * @return structured array of topic data
 */
function smartparticipation_core_topic_carousel_data($topic) {

  $phase_tid = smartparticipation_core_field_get_item_value('node', $topic, 'field_sp_topic_phase', 'tid');
  $proposal_nid = smartparticipation_core_field_get_item_value('node', $topic, 'field_sp_topic_proposal', 'nid');

  $topic_nids = smartparticipation_core_topic_same_phase_proposal($phase_tid, $proposal_nid);

  // Return a render array
  // If only one topic (the current one), don't display a carousel.
  if (count($topic_nids) > 1) {

    $phase = taxonomy_term_load($phase_tid);
    $phase_allows_comments = smartparticipation_core_field_get_item_value('taxonomy_term', $phase, 'field_sp_phase_allows_comments', 'value');
    $commentable = smartparticipation_core_topic_commentable($topic, node_load($proposal_nid), $phase, $phase_allows_comments);
    
    /*$nodes = array();
    foreach ($topic_nids as $nid) {
      $nodes[] = smartparticipation_core_topic_data($nid);
    }*/
    
    $node_type = 'topic';

    // Get topics in order
    $proposal = node_load($proposal_nid);
    $phases = _smartparticipation_core_get_proposal_phase_data($proposal);
    $phase_id = null;
    foreach ($phases as $phase_data) {
      if ($phase_data['tid'] == $phase->tid) {
        $phase_id = $phase_data['phase_id'];
        $phase = $phase_data;
        break;
      }
    }
    $topics = _smartparticipation_core_get_proposal_topics_phase($proposal_nid, $phase_id);

    $recommended_comments_service = new \SmartParticipation\RecommendedCommentsService;
    $recommended_comments_enabled = $recommended_comments_service->getStatus();

    $carousel_data = array(
        '#theme' => 'carousel',
        'carousel_prompt' => smartparticipation_core_carousel_prompt($commentable, $node_type, $phase),
        'commentable' => $commentable,
        'node_type' => $node_type,
        'nodes' => $topics,
        'show_comment_count' => true,
        'recommended_comments_enabled' => $recommended_comments_enabled,
    );

    $topic_data = array(
      '#theme' => 'node_carousel',
      'title' => t('All topics'),
      'carousel' => $carousel_data,
    );

  } else {
    $topic_data = array();
  }

  return $topic_data;

}

/**
 * Return the proposal node associated with the specified topic node.
 * 
 * @param $topic - topic node object
 * 
 * @return proposal node associated with the topic
 */
function smartparticipation_core_get_topic_proposal($topic) {
  
  $proposal_nid = smartparticipation_core_field_get_item_value('node', $topic, 'field_sp_topic_proposal', 'nid');
  $proposal = node_load($proposal_nid);
  return $proposal;

}

function smartparticipation_core_topic_phase_dates($topic) {

  $proposal = smartparticipation_core_get_topic_proposal($topic);  
  $phase_tid = smartparticipation_core_field_get_item_value('node', $topic, 'field_sp_topic_phase', 'tid');
  $dates = _smartparticipation_core_get_proposal_phase_dates($proposal, $phase_tid);
  return $dates;

}

/**
 * Return count of the comments in the specified topic (via subtopics).
 * 
 * @param $topic_nid - the topic node nid
 * 
 * @return count of comments in the topic's subtopics
 */
function smartparticipation_core_topic_comment_count($topic_nid) {
  
  $comment_count = 0;
  
  $subtopic_nids = smartparticipation_core_topic_subtopics($topic_nid);
  foreach ($subtopic_nids as $subtopic_nid) {
    $subtopic = node_load($subtopic_nid);
    $comment_count += $subtopic->comment_count;
  }
  
  return $comment_count;
}

/**
 * Return prompt to be displayed on node carousel.
 * 
 * @param boolean $commentable - true iff the phase is commentable (allows
 * comments and is the current phase)
 */
function smartparticipation_core_carousel_prompt($commentable, $node_type, $phase=array()) {

  if ($commentable) {
    if (isset($phase['phase_open_text'])) {
      $text = $phase['phase_open_text'];
    } else {
      $text = 'Comment on';
    }
    $title = 'Select a topic on the right to comment on.';
    // $error = 'Please select a topic that you wish to comment on.';
  } else {
    if (isset($phase['phase_closed_text'])) {
      $text = $phase['phase_closed_text'];
    } else {
      $text = 'Read';
    }
    $node_type = str_replace('smartparticipation_core_', '', $node_type);
    $title = 'Select a ' . $node_type . ' on the right to read.';
    // $error = 'Please select a ' . $node_type . ' that you wish to read.';   
  }

  return array(
      '#theme' => 'carousel_prompt',
      'text' => t($text),
      'title' => t($title),
    // 'error' => t($error),
  );


}

function smartparticipation_core_comment_tips_get_state($subtopic_nid)
{
  $user_service = new \SmartParticipation\UserService;
  $user = $user_service->find();
  if (!empty($user)) {
    $comment_tips_service = new \SmartParticipation\CommentTipsService;
    return $comment_tips_service->getStateRequest($user, $subtopic_nid);
  }
  return false;
}