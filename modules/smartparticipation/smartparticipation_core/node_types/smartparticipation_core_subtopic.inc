<?php

/**
 * @file 
 * Define the subtopic node type.
 * 
 * @ingroup smartparticipation_core
 */
 
/**
 * Create the subtopic node type.
 */
function smartparticipation_core_create_subtopic_node_type() {
  
  $t = get_t();
  
  $subtopic = array(
    'type' => 'sp_subtopic',
    'name' => $t('Proposal subtopic'),
    // Drupal will handle this node as if we had designed it in the UI.
    'base' => 'node_content', 
    'has_title' => TRUE, 
    'title_label' => $t('Title'),   
    'description' => $t('Proposal subtopics are the units of commenting. Subtopics are organized into topics and display on the topic page.'),
    'custom' => TRUE,
    'help' => '',
  );  
  
  // Set defaults not explicitly declared above.
  $node_type = node_type_set_defaults($subtopic);
  
  // Add body field and set label.
  node_add_body_field($node_type);
  
  node_type_save($node_type);

  // Create the fields we are adding to the subtopic node type.
  foreach (smartparticipation_core_subtopic_installed_fields() as $field) {
    $field['entity_types'] = array('node');   
    field_create_field($field);
  }

  // Create all the instances for our fields.
  foreach (smartparticipation_core_subtopic_installed_instances() as $instance) {
    $instance['bundle'] = $subtopic['type'];
    $instance['entity_type'] = 'node';
    field_create_instance($instance);
  }

  // Create a revision by default when a new version is saved
  variable_set('node_options_sp_subtopic', array('revision', 'status'));
  
  // No comment subject titles
  variable_set('comment_subject_field_sp_subtopic', 0);
  
}

/**
 * Delete the subtopic node type.
 * 
 */
function smartparticipation_core_delete_subtopic_node_type() {
  smartparticipation_core_delete_node_type('sp_subtopic');
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
function smartparticipation_core_subtopic_installed_fields() {

  $fields = array();
  
 
  $fields['field_sp_subtopic_topic'] = array(
    'active' => '1',
    'cardinality' => '1',
    'deleted' => '0',
    'field_name' => 'field_sp_subtopic_topic',
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
  );
  
  $fields['field_sp_subtopic_doc'] = smartparticipation_core_subtopic_doc_field();
  
  return $fields;
}

/**
 * Define the field used to store documents associated with a subtopic.
 */
function smartparticipation_core_subtopic_doc_field() {
  
  return array(
    'active' => '1',
    'cardinality' => '1',
    'deleted' => '0',
    'entity_types' => array('node'),
    'field_name' => 'field_sp_subtopic_doc',
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
function smartparticipation_core_subtopic_installed_instances() {
  $t = get_t();

  return array(
    'field_sp_subtopic_topic' => array(
      'default_value' => NULL,
      'deleted' => '0',
      'description' => '',
      'display' => array(
        'default' => array(
          'label' => 'hidden',
          'module' => 'node_reference',
          'settings' => array(
            'link' => FALSE,
          ),
          'type' => 'hidden',
          'weight' => 1,
        ),
        'teaser' => array(
          'label' => 'hidden',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0,
        ),
      ),
      'field_name' => 'field_sp_subtopic_topic',
      'label' => $t('Topic'),
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
    
    'field_sp_subtopic_doc' => smartparticipation_core_subtopic_doc_instance(),
    
    'field_sp_phase_closed_message' =>
      smartparticipation_core_subtopic_phase_closed_message_instance(),
  );

}

/**
 * Define the instance of field field_sp_subtopic_doc attached to the
 * subtopic node type.
 */
function smartparticipation_core_subtopic_doc_instance() {

  $t = get_t();
  $site = variable_get('smartparticipation_site_machine_name');
  
  return array(
    'bundle' => 'sp_subtopic',
    'default_value' => NULL,
    'deleted' => '0',
    'description' => $t('A link to this document will be provided in the subtopic display.'),
    'display' => array(
      'default' => array(
        'label' => 'hidden',
        'module' => 'file',
        // Formatter settings
        'settings' => array(
          'link_text' => $t('View in PDF format'),
        ),
        // Formatter
        'type' => 'smartparticipation_core_associated_doc',
        // 'type' => 'file_default',
        //'type' => 'file_url_plain',
        'weight' => 2,
      ),
      'teaser' => array(
        'label' => 'hidden',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'entity_type' => 'node',
    'field_name' => 'field_sp_subtopic_doc',
    'label' => $t('Associated document'),
    'required' => 0,
    'settings' => array(
      'description_field' => 0,
      'file_directory' => "smartparticipation_core/$site/subtopics/docs",
      'file_extensions' => 'pdf',
      'max_filesize' => '1M',
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'media',
      'settings' => array(
        'allowed_schemes' => array(
          'private' => 0,
          'public' => 'public',
        ),
        'allowed_types' => array(
          0 => 0,
          'audio' => 0,
          'default' => 0,
          'image' => 'image',
          'video' => 0,
        ),
        'progress_indicator' => 'throbber',
      ),
      'type' => 'media_generic',
      'weight' => 10,
    ),
  );
}

/**
 * Define the phase closed message field instance for the subtopic node type.
 */
function smartparticipation_core_subtopic_phase_closed_message_instance() {

  $t = get_t();

  return array(
    'bundle' => 'sp_subtopic',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => $t('Message to display when the subtopic\'s phase is closed. Overrides message specified on the topic.'),
    'display' => array(
      'default' => array(
        'label' => 'hidden',
        'module' => 'text',
        'settings' => array(),
        'type' => 'text_default',
        'weight' => 7,
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
      'weight' => 5,
    ),
  );
}


/** 
 * Implements hook_form_FORM_ID_alter().
 */
function smartparticipation_core_form_sp_subtopic_node_form_alter(&$form, &$form_state, $form_id) {

  foreach(array_keys($form['field_sp_subtopic_topic'][LANGUAGE_NONE]['#options']) as $val) {
    if ($val != '_none') {

      $topic_data = smartparticipation_core_topic_data($val);
      $proposal_nid = $topic_data['proposal'];
      $proposal = node_load($proposal_nid);
      $proposal_title = $proposal->title;

      $phase = smartparticipation_core_get_topic_phase($val);
      $form['field_sp_subtopic_topic']['und']['#options'][$val] .= ' (' . $proposal_title . ' / ' . $phase->name . ')';
    }
  }

  $open_proposals = _smartparticipation_core_get_proposal_options();

  if (count($open_proposals) == 0) {
    $message = 'There are no open proposals in the system. You must '.l('add at least one open proposal','node/add/sp-proposal').' before you can create a subtopic.';
    drupal_set_message($message, $type = 'error', $repeat = FALSE);
    hide($form);
    return;
  } 
  
  if (count($form['field_sp_subtopic_topic'][LANGUAGE_NONE]['#options']) == 0) {
    $message = 'There are no topics in the system. You must '.l('add at least one topic to an open proposal','node/add/sp-topic').' before you can create a subtopic.';
    drupal_set_message($message, $type = 'error', $repeat = FALSE);
    hide($form);
    return;
  } 
    
  if (isset($form['nid']['#value'])) {
    // edit node form
    $nid = $form['nid']['#value'];
    $predefined_topic_nid = smartparticipation_core_get_subtopic_topic_nid($nid);
  } else {
    // add node form
    $predefined_topic_nid = arg(3);
  }

  $form['field_sp_subtopic_topic'][LANGUAGE_NONE]['#default_value'] = $predefined_topic_nid;
  
  $form['field_sp_phase_closed_message'][LANGUAGE_NONE]['0']['#format'] = 'full_html';
  
}

/**
 * Return the subtopic's phase closed message. This can be defined either on the
 * subtopic node or on its parent topic node, with the latter serving as a
 * default and the former overriding it. If neither is defined, return an
 * empty string. Subtopics and topics in both commenting and non-commenting
 * phases can define this message.
 *
 * Another approach would be to automatically create a link to the corresponding
 * subtopic of the next phase, since this is most often what would be wanted
 * (except in the last phase). The current approach is more cumbersome from an
 * editing standpoint, but allows greater flexibility and doesn't require
 * topics and subtopics to be linked from phase to phase, either with node
 * references or by a convention like identical titles.
 *
 * @param $subtopic - the subtopic node
 * @param $topic_phase_closed_message - (optional) the parent topic's phase
 * closed message. If the calling function knows it, it can be passed in. This
 * avoids multiple lookups where the calling function is looping through the
 * topic's subtopics, for example. Otherwise it will be retrieved here.
 *
 * @return string - the subtopic's phase closed message or an empty string
 */
function smartparticipation_core_subtopic_phase_closed_message($subtopic, $topic_phase_closed_message = null) {

  $message = smartparticipation_core_field_get_item_value('node', $subtopic, 'field_sp_phase_closed_message', 'value');

  if (!$message) {
    if (isset($topic_phase_closed_message)) {
      $message = $topic_phase_closed_message;
    } else {
      $topic = smartparticipation_core_get_subtopic_topic($subtopic->nid);
      $message = smartparticipation_core_field_get_item_value('node', $topic, 'field_sp_phase_closed_message', 'value');
    }
  }

  return $message;
}

/**
 * Return the subtopic's associated topic
 * 
 * @param $subtopic_nid - the subtopic nid
 * 
 * @return the topic nid
 */
function smartparticipation_core_get_subtopic_topic_nid($subtopic_nid) {
  
  $subtopic = node_load($subtopic_nid);
  if (!empty($subtopic)) {
    return smartparticipation_core_field_get_item_value('node', $subtopic, 'field_sp_subtopic_topic', 'nid');
  }
  return false;
}

/**
 * Return the topic node associated with the specified subtopic.
 * 
 * @param $subtopic_nid - the nid of the subtopic
 * 
 * @return the topic node object
 */
function smartparticipation_core_get_subtopic_topic($subtopic_nid) {
  
  $topic_nid = smartparticipation_core_get_subtopic_topic_nid($subtopic_nid);
  $topic = node_load($topic_nid);
  return $topic;
}

/**
 * Return the phase associated with the subtopic (via the topic)
 *
 * @param $subtopic_nid - the subtopic nid
 * 
 * @return the phase taxonomy term
 */

function smartparticipation_core_get_subtopic_phase($subtopic_nid) {
  
  $topic_nid = smartparticipation_core_get_subtopic_topic_nid($subtopic_nid);
  if ($topic_nid) {
    return smartparticipation_core_get_topic_phase($topic_nid);
  }
  return false;
}

/**
 * Return the proposal associated with the specified subtopic.
 * 
 * @param $subtopic_nid - nid of the subtopic
 * 
 * @return the proposal object this subtopic belongs to
 */
function smartparticipation_core_get_subtopic_proposal($subtopic_nid) {
  $topic_nid = smartparticipation_core_get_subtopic_topic_nid($subtopic_nid);
  if (!empty($topic_nid)) {
    $topic = node_load($topic_nid);
    $proposal = smartparticipation_core_get_topic_proposal($topic);
    return $proposal;
  }
  return false;
}


function smartparticipation_core_subtopic_node_view($node, $view_mode, $langcode) {
  
  if ($view_mode == 'full') {
    $topic_nid = smartparticipation_core_field_get_item_value('node', $node, 'field_sp_subtopic_topic', 'nid');
    
    $path = 'node/' . $topic_nid;
    $options = array(
      'fragment' => 'nid-' . $node->nid,  
    );
    
    // After submission of the subtopic node add form, remove the overlay, 
    // else the hashtag gets dropped from the url due to the overlay hashtag /
    // query parameter.
    if (module_exists('overlay') && overlay_get_mode() == 'child') {
      unset($_GET['destination']);
      overlay_close_dialog($path, $options); 
    } else {
      drupal_goto($path, $options);
    }
  }
}



/**
 * This function will return the number of recommended comments for one subtopic
 */
function smartparticipation_core_subtopic_comments_recommended($nid) {
  
  $query = db_select('comment', 'c')
    ->fields('c', array('cid'));
  $query->where("c.nid = :nid", array(':nid' => $nid));

  $query->leftJoin('flag_content', 'fc', 'fc.content_id = c.cid');
  
  $query->leftJoin('flags', 'f', 'f.fid = fc.fid');
  $query->where("f.name = 'comment_recommendation'");

  $result = $query->execute();
  $count = $result->rowCount();

  return $count;
}

function smartparticipation_core_get_proposal_phase_subtopics($proposal_nid, $phase_tid)
{
  $topics = smartparticipation_core_proposal_phase_topic_nids($proposal_nid, $phase_tid);
  $subtopics = array();
  foreach ($topics as $topic_nid) {
    $topic_subtopics = smartparticipation_core_topic_subtopics($topic_nid);
    foreach ($topic_subtopics as $subtopic_nid) {
      array_push($subtopics, $subtopic_nid);
    }

  }
  return $subtopics;
}


/**
 * Called by smartparticipation_core_load() when a subtopic node is loaded.
 *
 * @param $node - the subtopic node that was loaded
 */
/*
function smartparticipation_core_subtopic_load($subtopic) {
  
  // dpm($subtopic);

}
 */