<?php

/**
 * Define custom smartparticipation_core node functionality.
 */
 
/**
 * Module function to alter built-in Drupal node types.
 */
function smartparticipation_core_alter_node_types() {
  smartparticipation_core_alter_page_node_type();
  smartparticipation_core_disable_node_type('article');
} 

/**
 * Module function to create default nodes during module install. Applies only
 * to non-custom node types, since default nodes of custom types are created
 * when the type is created.
 */
function smartparticipation_core_create_default_non_smartparticipation_core_nodes() {
  smartparticipation_core_create_default_page_nodes();
}

/**
 * Module function to create custom smartparticipation_core node types.
 * 
 */
function smartparticipation_core_create_node_types() {
  foreach (smartparticipation_core_custom_node_types() as $node_type) {
    call_user_func('smartparticipation_core_create_' . $node_type . '_node_type');
  }
}

/**
 * Module function to get a list of defined node types, used on install to 
 * create the node types and on uninstall to delete the node types. 
 */
function smartparticipation_core_custom_node_types() {
  $node_types = array();
  $module_name = 'smartparticipation_core';
  $module_prefix = $module_name . '_';
  $files = file_scan_directory(drupal_get_path('module', $module_name) . '/node_types', '/\.inc/');
  foreach (array_keys($files) as $filename) {
    $node_type = basename($filename, '.inc');
    // NB === instead of == (since returns false if string not found, which
    // evalutes to 0).
    if (strpos($node_type, $module_prefix) === 0) {
      $node_types[] = substr($node_type, strlen($module_prefix));
    } 
  }  
  return $node_types;
}

/**
 * Implements hook_form_node_form_alter().
 * 
 * NB Many custom node types also implement hook_form_FORM_ID_alter().
 */
function smartparticipation_core_form_node_form_alter(&$form, &$form_state, $form_id) {

  // TODO Access should be controlled from permissions rather than
  // by brute-force here.
  $form['comment_settings']['#access'] = FALSE;
  $form['menu']['#access'] = FALSE;  
  
  // Restored to true for proposals, topics, and documents.
  $form['path']['#access'] = FALSE;
  
  hide($form['options']['promote']);
  hide($form['options']['sticky']);  
  
  $form['title']['#size'] = 150;

}
 
function smartparticipation_core_get_nodereferences_type($type) {
  switch($type) {
    case 'sp_proposal':
      $nodereference_type = 'topic';
      break;
    case 'sp_topic':
      $nodereference_type = 'subtopic';
      break;
    default:
      $nodereference_type = '';
      break;
  }

  return $nodereference_type;
}


/**
 * Implementation of hook_node_delete().
 */
function smartparticipation_core_node_delete($node) {

  switch ($node->type) {

    case 'sp_about':
      // smartparticipation_core_update_menu_link_text($node);
      smartparticipation_core_update_about_menu($node, 'delete');
      break;

    case 'sp_document':
      // Remove any references in the node order table
      $node_deleted = db_delete('sp_document_node_order')
        ->condition('dnid', $node->nid)
        ->execute();      
      break;

    case 'sp_proposal':
         
      /* Handled when we delete the topics and documents.
      // Remove any referenced overrides.
      $node_deleted = db_delete('smartparticipation_core_topic_node_order')
        ->condition('ttid', $node->nid)
        ->execute();
  
      // Remove any referenced overrides.
      $node_deleted = db_delete('smartparticipation_core_document_node_order')
        ->condition('pnid', $node->nid)
        ->execute();
      */
        
      // Remove the related topics & subtopics
      smartparticipation_core_delete_related_topics($node->nid);
  
      // Remove the related documents
      smartparticipation_core_delete_related_documents($node->nid);
      
      // Remove the proposal glossary
      smartparticipation_core_delete_proposal_glossary($node);
      
      smartparticipation_core_disable_unused_menus();
      break;

    case 'sp_topic':
    
      /* Handled when we delete the subtopics.
      // Remove any referenced overrides.
      $node_deleted = db_delete('smartparticipation_core_subtopic_node_order')
        ->condition('tnid', $node->nid)
        ->execute();
      */
      
      // Remove any references in the node order table
      $node_deleted = db_delete('sp_topic_node_order')
        ->condition('tnid', $node->nid)
        ->execute();
  
      // Remove the related subtopics
      smartparticipation_core_delete_related_subtopics($node->nid);
      break;


    case 'sp_subtopic':
  
      // Remove any references in the node order table
      $node_deleted = db_delete('sp_subtopic_node_order')
        ->condition('snid', $node->nid)
        ->execute();
      break;
      }

}

/**
 * Implements hook_node_view().
 */
function smartparticipation_core_node_view($node, $view_mode, $langcode) {
  
  $type = $node->type;
  
  switch ($type) {

    case 'sp_about':
      smartparticipation_core_about_node_view($node, $view_mode, $langcode);
      break;

    case 'sp_document':
      smartparticipation_core_document_node_view($node, $view_mode, $langcode);
      break;

    case 'sp_learn':
      smartparticipation_core_learn_node_view($node, $view_mode, $langcode);
      break;

    case 'sp_news':
      smartparticipation_core_news_node_view($node, $view_mode, $langcode);
      break;

    case 'sp_proposal':
      smartparticipation_core_proposal_node_view($node, $view_mode, $langcode);
      break;

    case 'sp_publication':
      smartparticipation_core_publication_node_view($node, $view_mode, $langcode);
      break;

    case 'sp_research':
      smartparticipation_core_research_node_view($node, $view_mode, $langcode);
      break;

    case 'sp_sponsor_partner':
      smartparticipation_core_sponsor_partner_node_view($node, $view_mode, $langcode);
      break;

    case 'sp_subtopic':
      smartparticipation_core_subtopic_node_view($node, $view_mode, $langcode);
      break;

    case 'sp_team_member':
      smartparticipation_core_team_member_node_view($node, $view_mode, $langcode);
      break;

    case 'sp_topic':
      smartparticipation_core_topic_node_view($node, $view_mode, $langcode);
      break;
  
    default:
      break;
  }

  
  /* TODO We still need to apply the glossary to other nodes in other functions,
   * such as the proposal summary on the home page and proposal page; the 
   * subtopic body on the topic page; faq body on the FAQ page; announcements 
   * on the Announcement page; etc. because we are not correctly making use of
   * node views for those displays. Rewrite to use node views for all node 
   * displays so that we can apply the glossary lookup just once, here. 
   */
  if ($type == 'page' ||
    $type == 'sp_about' ||
    $type == 'sp_learn'
      /* It's not clear whether the glossary should apply to this content. Check
       * with research team.
      $type == 'smartparticipation_core_news' ||
      $type == 'smartparticipation_core_publication' ||
      $type == 'smartparticipation_core_research' ||
      $type == 'smartparticipation_core_sponsor_partner' ||
      $type == 'smartparticipation_core_team_member'    
      */  
     ) {
    smartparticipation_core_apply_glossary_to_node_body($node);   
  }
     
}

/**
 * Apply glossary lookup to the node body.
 * 
 * @param $node - the node object
 * 
 * @return none; $node is passed by reference
 */
function smartparticipation_core_apply_glossary_to_node_body(&$node) {
  
  $type = $node->type;
  
  switch ($type) {
    
    /*
    case 'smartparticipation_core_news':
      $field_name = 'field_sp_news_proposal';
      break;
    */
    
    default:
      $field_name = '';
      break;
  }
  
  $proposal_nid = null;
  if ($field_name) {
    $proposal_nid = smartparticipation_core_field_get_item_value('node', $node, $field_name, 'nid');
  } 

  if (isset($node->content['body']) && ! empty($node->content['body'][0])) {
    $body = $node->content['body'][0]['#markup'];
    $body = smartparticipation_core_apply_glossary_lookup($body, $proposal_nid);
    $node->content['body'][0]['#markup'] = $body;  
  }
}

/**
 * Implements hook_node_update().
 */
function smartparticipation_core_node_update($node) {
  
  switch ($node->type) {

    case 'sp_about':
      // smartparticipation_core_update_menu_link_text($node);
      smartparticipation_core_update_about_menu($node, 'update');
      break;


    case 'sp_learn':
      //smartparticipation_core_update_menu_link_text($node);
      smartparticipation_core_set_menu_param_from_nid('Learn', $node->nid, 'link_title', $node->title);
      break;


    case 'sp_proposal':
      smartparticipation_core_proposal_node_update($node);
      smartparticipation_core_disable_unused_menus();
      break;
      
    default:
      break;
      
  }
}



/**
 * Remove custom features of built-in Drupal node types.
 */
function smartparticipation_core_restore_node_types() {
  smartparticipation_core_restore_page_node_type();
  smartparticipation_core_enable_node_type('article');
  field_purge_batch(1000);
   
} 

/**
 * Delete default nodes that were created during module install. Applies only to
 * non-custom node types, since nodes of custom smartparticipation_core node types are deleted
 * during the deletion of the node type itself.
 */
function smartparticipation_core_delete_default_non_smartparticipation_core_nodes() {
  smartparticipation_core_delete_default_page_nodes();
}


/**
 * Delete custom smartparticipation_core node types.
 */
function smartparticipation_core_delete_custom_node_types() {
  foreach (smartparticipation_core_custom_node_types() as $node_type) {
    call_user_func('smartparticipation_core_delete_' . $node_type . '_node_type');
  }
}

function smartparticipation_core_delete_node_type($type) {
  
  // Gather all the content
  $nids = db_select('node', 'n')
    ->fields('n', array('nid'))
    ->condition('n.type', $type)
    ->execute()
    ->fetchAllAssoc('nid', PDO::FETCH_ASSOC);
  
  node_delete_multiple(array_keys($nids));
  
  /* These are not needed. The fields and instances are deleted automatically
   * with the node type when after field_purge_batch() is called.
  // Delete all the fields defined for this node type
  foreach (array_keys(smartparticipation_core_X_installed_fields()) as $field) {
    field_delete_field($field);
  }
  // Delete field instances attached to this node type
  foreach (smartparticipation_core_X_installed_fields() as $key => $instance) {
    field_delete_instance($instance);
  }
  */
    
  // Delete the node type
  node_type_delete($type);
  
  field_purge_batch(1000);
}


/**
 * Implements hook_node_insert().
 */
function smartparticipation_core_node_insert($node) {

    switch ($node->type) {

      case 'sp_about':
        // TODO Change to smartparticipation_core_about_autocreatemenu($node);
        // See sample function in about.inc.
        smartparticipation_core_autocreatemenu($node);
        break;

      case 'sp_document':
        smartparticipation_core_document_node_insert($node);
        break;

      case 'sp_learn':
        // TODO Change to smartparticipation_core_learn_autocreatemenu($node);
        // See sample function in learn.inc.
        smartparticipation_core_autocreatemenu($node);
        break;

      case 'sp_proposal':
        // TODO Change to smartparticipation_core_proposal_autocreatemenu($node);
        // See sample function in proposal.inc.
        smartparticipation_core_add_proposal_glossary($node);
        smartparticipation_core_autocreatemenu($node);
        break;

      default:
        break;
    }

}

/**
 * Implements hook_form_FORM_ID_alter().
 */
function smartparticipation_core_form_node_delete_confirm_alter(&$form, &$form_state, $form_id) {
  
  $node = node_load($form_state['build_info']['args'][0]->nid);

  // Change the confirmation message   
  switch ($node->type) {
    case 'sp_proposal':
      $form['description']['#markup'] = 'This action will delete all topics, documents, and subtopics of this proposal. It cannot be undone.';
      break;

    case 'sp_topic':
      $form['description']['#markup'] = 'This action will delete all subtopics of this topic. It cannot be undone.';
      break;
  }

}

/** 
 * Implements hook_node_access().
 */
function smartparticipation_core_node_access($node, $op, $account) {

  if (! is_object($node)) {
    return NODE_ACCESS_IGNORE;
  }

  // just in case the $node parameter is not really a node object or
  // the $node->type is one other than our own
  $type = is_string($node) ? $node : $node->type;
  
  if ($op == 'view') {
    switch ($type)  {

      case 'sp_announcement':
        if (!user_access('view sp_announcement', $account)) {
          return NODE_ACCESS_DENY;
        }
        break;

      case 'sp_document':
        $proposal_nid = smartparticipation_core_field_get_item_value('node', $node, 'field_sp_document_proposal', 'nid', $langcode = 'LANGUAGE_NONE');
        // TODO We don't need to get all this data just to answer the access question. Refactor.
        $proposal_details = smartparticipation_core_proposal_data($proposal_nid);
        if ($proposal_details['publication_status'] == 0 && smartparticipation_core_is_anonymous_or_basic_authenticated_user($account)) {
          return NODE_ACCESS_DENY;
        }
        break;

      case 'sp_faq':
        if (!user_access('view sp_faq', $account)) {
          return NODE_ACCESS_DENY;
        }
        break;

      case 'sp_interest_survey':
        if (!user_access('view sp_interest_survey', $account)) {
          return NODE_ACCESS_DENY;
        }
        break;

      case 'sp_news':
        if (!user_access('view sp_news', $account)) {
          return NODE_ACCESS_DENY;
        }
        break;

      case 'sp_publication':
        if (!user_access('view sp_publication', $account)) {
          return NODE_ACCESS_DENY;
        }
        break;

      case 'sp_research':
        if (!user_access('view sp_research', $account)) {
          return NODE_ACCESS_DENY;
        }
        break;

      case 'sp_sponsor_partner':
        if (!user_access('view sp_sponsor_partner', $account)) {
          return NODE_ACCESS_DENY;
        }
        break;

      case 'sp_subtopic':
        if (!user_access('view sp_subtopic', $account)) {
          return NODE_ACCESS_DENY;
        }
        break;

      case 'sp_team_member':
        if (!user_access('view sp_team_member', $account)) {
          return NODE_ACCESS_DENY;
        }
        break;

      case 'sp_topic':
        $proposal_nid = smartparticipation_core_field_get_item_value('node', $node, 'field_sp_topic_proposal', 'nid', $langcode = 'LANGUAGE_NONE');
        // TODO We don't need to get all this data just to answer the access question. Refactor.
        $proposal_details = smartparticipation_core_proposal_data($proposal_nid);
        if(smartparticipation_core_is_anonymous_or_basic_authenticated_user($account)) {
          if ($proposal_details['publication_status'] == 0) {
            return NODE_ACCESS_DENY;
          }
          
          if(!smartparticipation_core_topic_access($node)) {
            return NODE_ACCESS_DENY;
          }
        }
        break;

    }
  }
  
}

/**
 * Disable a node type.
 * 
 * @param $type - the node type to disable
 */
function smartparticipation_core_disable_node_type($type) {
  
  $node_type = node_type_load($type);
  if ($node_type) {
    $node_type->disabled = TRUE;
    node_type_save($node_type);
  }
  
}

/**
 * Enable a node type.
 * 
 * @param $type - the node type to disable
 *
 */
function smartparticipation_core_enable_node_type($type) {
  
  /* Doesn't work due to an apparent Drupal bug: node_type_load() doesn't load
   * the node type. I've traced this back to _node_type_build() not including
   * the disabled node type in the array of node types it returns, despite the
   * function documentation claiming that disabled types are included.
  $node_type = node_type_load($type);
  if ($node_type) {
    $node_type->disabled = FALSE;
    node_type_save($node_type);
  }
  */
  
  db_update('node_type')
    ->fields(array(
      'disabled' => 0,
    ))
    ->condition('type', $type)
    ->execute();

}

/**
 * Return an array of menu_title links.
 * 
 * @param $nids - an array of node nids
 * @param $title - optional text to use as the link title attribute
 * 
 * @return an array of links to the nodes
 */
function smartparticipation_core_node_menu_title_links($nids, $title = '') {
  
  $links = array();
  foreach ($nids as $nid) {
    $node = node_load($nid);
    $links[] = smartparticipation_core_node_menu_title_link($node, $title);
  }
  
  return $links;
}

/**
 * Return a link to a node that has an instance of field_sp_menu_title.
 * This function is safe to call on nodes and node types that don't have this 
 * field; the node title will be substituted for the menu_title.
 * 
 * @param $node - the node
 * @param $title - optional text to use as the link title attribute
 * 
 * @return HTML link element to the node, using menu_title as link text and
 * the node title in the link title attribute value.
 */
function smartparticipation_core_node_menu_title_link($node, $title = '') {

  $nid = $node->nid;
  $node_title = $node->title;
  
  $menu_title = smartparticipation_core_field_get_item_value('node', 
      $node, 'field_sp_menu_title', 'value');
      
  if (!$menu_title) {
    $menu_title = $node_title;
  }
  
  $attributes = array();
  if ($title) {
    $attributes['title'] = t($title);
  }
  
  return l(
      $menu_title, 
      'node/' . $nid,
      array(
        'attributes' => $attributes,   
      )      
  );
}

/**
 * Return true iff the requested page is a full page view of a node.
 */
function smartparticipation_core_is_full_page_node_view($node, $view_mode) {
  
  return ($view_mode == 'full') && node_is_page($node);
  
}

/**
 * Helper function to return the set of published nodes of the specified type.
 * 
 * @param $type - the type of node to retrieve
 * 
 * @return an array mapping node nids to titles
 * 
 * TODO This function combines two previous functions that returned
 * sp_about and sp_learn node lists. Check if there are other
 * type-specific functions that could be replaced by this.
 */
function _smartparticipation_core_get_published_nodes_by_type($type) {
  
  $query = 'SELECT n.nid, n.title
            FROM {node} n
            WHERE n.type = :type
            AND n.status = 1';

  $result = db_query($query,array(
    ':type' => $type,
  ));
  
  $nodes = array();
  while ($value = $result->fetchObject()) {
    $nodes[$value->nid] = $value->title;
  }

  return $nodes;
}

/**
 * Helper function to return the set of ALL nodes of the specified type.
 *
 * @param $type - the type of node to retrieve
 *
 * @return an array mapping node nids to titles
 *
 */
function _smartparticipation_core_get_nodes_by_type($type) {

  $query = 'SELECT n.nid, n.title
            FROM {node} n
            WHERE n.type = :type';


  $result = db_query($query,array(
      ':type' => $type,
  ));

  $nodes = array();
  while ($value = $result->fetchObject()) {
    $nodes[$value->nid] = $value->title;
  }

  return $nodes;
}



/**
 * Helper function to return the number of published nodes of the specified type.
 */
function _smartparticipation_core_get_published_node_count_by_type($type) {

  $query = 'SELECT n.nid
            FROM {node} n 
            WHERE n.type = :type 
            AND n.status = 1';
  
  $result = db_query($query,array(
    ':type' => $type,
  )) 
  ->fetchCol();

  return count($result);
}

function smartparticipation_core_preprocess_node(&$variables) {
  $variables['title_attributes_array']['class'][] = 'node-title';
  $node = $variables['node'];
  switch ($node->type) {
    case 'sp_learn':
      _smartparticipation_core_preprocess_learn_node($variables);
      break;
    case 'sp_interest_survey':
      // Show page messages in the interest survey dialog.
      $variables['messages'] = theme('status_messages');
      break;
  }
}

/**
 * Implements hook_load().
 * 
 * Use this to add additional information to the node object.
 */
/*
function smartparticipation_core_load($nodes) {
  
  foreach ($nodes as $node) {
    
    switch ($node->type) {
      
      case 'smartparticipation_core_subtopic':
        smartparticipation_core_subtopic_load($node);
        break;
    }
  }
}
*/