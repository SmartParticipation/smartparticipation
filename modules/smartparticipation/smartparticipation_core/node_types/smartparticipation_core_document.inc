<?php

/**
 * @file 
 * Functions pertaining to the document node type.
 * 
 * @ingroup smartparticipation_core
 */
 
/**
 * Create the document node type.
 */
function smartparticipation_core_create_document_node_type() {
  
  $t = get_t();
  
  $document = array(
    'type' => 'sp_document',
    'name' => $t('Proposal document'),
    // Drupal will handle this node as if we had designed it in the UI.
    'base' => 'node_content', 
    'has_title' => TRUE, 
    'title_label' => $t('Title'),   
    'description' => $t('Original agency documents associated with a proposal.'),
    'custom' => TRUE,
    'help' => '',
  );  
  
  // Set defaults not explicitly declared above.
  $node_type = node_type_set_defaults($document);
  
  // Add body field and set label.
  node_add_body_field($node_type);
  
  node_type_save($node_type);

  // Create the fields we are adding to the topic node type.
  foreach (smartparticipation_core_document_installed_fields() as $field) {
    $field['entity_types'] = array('node');
    field_create_field($field);
  }

  // Create all the instances for our fields.
  foreach (smartparticipation_core_document_installed_instances() as $instance) {
    $instance['bundle'] = $document['type'];
    $instance['entity_type'] = 'node';
    field_create_instance($instance);
  }
  
  // Disable commenting for this node type
  variable_set('comment_sp_document', COMMENT_NODE_HIDDEN);

  // Pathauto alias pattern
  $proposal_type = 
    variable_get('smartparticipation_site_proposal_type_plural', 'proposals');
  variable_set('pathauto_node_sp_document_pattern',
    $proposal_type . '/[node:field_sp_document_proposal]/documents/[node:field_sp_menu_title]');

  // Create a revision by default when a new version is saved
  variable_set('node_options_sp_document', array('revision', 'status'));
}

/**
 * Delete the document node type.
 * 
 */
function smartparticipation_core_delete_document_node_type() {
  smartparticipation_core_delete_node_type('sp_document');
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
function smartparticipation_core_document_installed_fields() {

  return array(
    'field_sp_document_proposal' => array(
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'field_name' => 'field_sp_document_proposal',
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
function smartparticipation_core_document_installed_instances() {
  $t = get_t();
  
  return array( 
    'field_sp_document_proposal' => array(
      'default_value' => NULL,
      'deleted' => '0',
      'description' =>$t('The proposal this document belongs to.'),
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
      'field_name' => 'field_sp_document_proposal',
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
    ),

    'field_sp_menu_title' => array(
      'default_value' => NULL,
      'deleted' => '0', 
      'description' => $t('Alternate short title used in menus, the document carousel, and the document\'s default URL.'),
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
        'weight' => '-3',
      ),   
    ),
    
  );
}
 

/** 
 * Implements hook_form_FORM_ID_alter().
 */
function smartparticipation_core_form_sp_document_node_form_alter(&$form, &$form_state, $form_id) {
  
  $options_first = _smartparticipation_core_get_proposal_options();

  if (count($options_first) == 0) {
	  $message = 'There are no open proposals in the system. You must '.l('add at least one open proposal','node/add/sp-proposal').' before you can create a proposal document.';
    drupal_set_message($message, $type = 'error', $repeat = FALSE);
    hide($form);
  } else {
    $form['field_sp_document_proposal'][LANGUAGE_NONE]['#options'] = $options_first;
  }
  
  if($form['nid']['#value']) {
    $document = node_load($form['nid']['#value']);
    $selected_proposal = smartparticipation_core_field_get_item_value('node', $document, 'field_sp_document_proposal', 'nid', $langcode = 'LANGUAGE_NONE');
  } else {
    $selected_proposal = isset($_REQUEST['field_sp_document_proposal'][LANGUAGE_NONE]) ? $_REQUEST['field_sp_document_proposal'][LANGUAGE_NONE] : key($options_first);
  }
  
  if($selected_proposal == '') {
    $selected_proposal = arg(3);
  }
  
  $form['field_sp_document_proposal'][LANGUAGE_NONE]['#default_value'] = $selected_proposal;
  
  
  // Override default setting of false for nodes in 
  // smartparticipation_core_form_node_form_alter().
  $form['path']['#access'] = true;
}

/**
 * Module function to modify the content sent to the topic node template.
 * 
 * See documentation of hook_node_view:
 * The module may add elements to $node->content prior to rendering. This hook 
 * will be called after hook_view(). The structure of $node->content is a 
 * renderable array as expected by drupal_render().
 */
function smartparticipation_core_document_node_view($node, $view_mode, $langcode) {
  
  $proposal_nid = smartparticipation_core_field_get_item_value('node', $node, 'field_sp_document_proposal', 'nid', $langcode);
  $proposal_details = smartparticipation_core_proposal_data($proposal_nid);

  $node->content['proposal'] = $proposal_details;
  
  // Apply proposal glossary to document content
  $body_markup = $node->content['body'][0]['#markup'];
  $body_markup = smartparticipation_core_apply_glossary_lookup($body_markup, $proposal_nid);
  $node->content['body'][0]['#markup'] = $body_markup;
  
}

/**
 * Return an array of documents of the specified proposal, ordered by weight. 
 * 
 * @param $nid_proposal - the proposal nid
 * @param $nid_document (optional, integer) - exclude this document from the 
 * result set
 * @param $select_weight (optional, boolean) - if true, include weight in the 
 * result set
 * @param $limit (optional, integer) - if non-zero, include a LIMIT clause equal 
 * to the value of $limit
 *
 * @return If $select_weight is true, return an indexed array of objects, else 
 * return an indexed array of document nids.
 */
function smartparticipation_core_proposal_documents($nid_proposal, $nid_document = 0, $select_weight = false, $limit = 0, $dir = 'ASC') {
  
  $query = 'SELECT n.nid';
  
  if ($select_weight) {
    $query .= ', o.weight'; 
  }
  
  $query .= ' FROM {node} n            
            INNER JOIN {field_data_field_sp_proposal_document} pd ON n.nid = pd.field_sp_proposal_document_nid
            LEFT OUTER JOIN {sp_document_node_order} o ON n.nid = o.dnid AND pd.entity_id = o.pnid
            WHERE n.status = 1
            AND pd.entity_id = :nid_proposal
            AND n.nid != :nid_document
            ORDER BY o.weight ' . $dir . ', n.created ASC';
  
  if ($limit) {
    $query .= ' LIMIT ' . $limit;
  }
  
  $result = db_query($query, array(
    ':nid_proposal' => $nid_proposal,
    ':nid_document' => $nid_document,  
  ));
  
  if ($select_weight) {
    // Returned an indexed array of objects containing nid and weight values
    $documents = $result->fetchAll();
  } else {
    // Return an indexed array of nids
    $documents = array_keys($result->fetchAllAssoc('nid', PDO::FETCH_ASSOC));        
  }

  return $documents;
    
}

/**
 * Return the nid of the first document for the specified proposal
 * 
 * @param $proposal nid - the proposal nid
 * 
 * @return nid of the first proposal document
 */
function smartparticipation_core_get_first_proposal_document($proposal_nid) {
   
  $document_nids = smartparticipation_core_proposal_documents($proposal_nid, 0, false, 1);
  return $document_nids[0];
}

/**
 * Return a link to the proposal documents. This link is displayed on the 
 * proposal page, and in the header used on the topic and document page.
 * 
 * @param $proposal - the proposal object
 * @param $display_node_type - the type of node being displayed
 * 
 * @return a link to the proposal documents. Currently this is a link to the
 * proposal's FIRST document, as defined by node weights.
 */
function smartparticipation_core_get_proposal_documents_link($proposal, $display_node_type, $show_link_if_no_documents) {

  $document_label = smartparticipation_core_field_get_item_value('node', $proposal, 'field_sp_document_label', 'value');

  $link_text = ucwords($document_label);
  
  $link_title = t('Browse @documents', array('@documents' => $document_label));

  $link_class = array('documents');
  if ($display_node_type == 'sp_document') {
    $link_class[] = 'active';
  }
  
  $proposal_documents = field_get_items('node', $proposal, 'field_sp_proposal_document', $langcode = 'LANGUAGE_NONE');

  if ($proposal_documents) {
    $document_nid = smartparticipation_core_get_first_proposal_document($proposal->nid);   
    $link_path = 'node/' . $document_nid;
  } else {
    $link_path = '';
  }
  
  if ($link_path == '' && !$show_link_if_no_documents) {
    $documents_link = '';
  } else {
    $documents_link = l(
      $link_text,
      $link_path,
      array(
        'attributes' => array(
          'class' => $link_class,
          'title' => $link_title,     
        ),
      )
    );
  }

  return $documents_link;
  
}

/**
 * Return an array with details about the proposal to be displayed in the 
 * document node page header.
 *
 * @param $proposal_data - an array of proposal data
 * @param $nid_document - the nid of the document node being displayed
 *
 * @return An array of data used to display the header on the page
 */
function smartparticipation_core_get_document_header_data($proposal_data, $nid_document) {

  $proposal = node_load($proposal_data['nid_proposal']);
  
  $document = node_load($nid_document);

  $proposal_documents_link = smartparticipation_core_get_proposal_documents_link($proposal, 'sp_document', TRUE);
  
  // These items are displayed on the topic page but not the document page.
  unset($proposal_data['description']);
  unset($proposal_data['status_label']);
  
  $document_links = smartparticipation_core_proposal_document_links($proposal->nid);
  // The "select other documents" dropdown should not display if this is the only document
  if (count($document_links < 2)) {
    $document_links = array();
  }

  return array(
    'node_nid' => $nid_document,
    'node_title' => $document->title,
    'node_type' => 'document',
    'document_links' => $document_links,
    'proposal' => $proposal_data,
    'proposal_documents_link' => $proposal_documents_link,
    'proposal_phases' => $proposal_data['phases'],
  );

}

/**
 * Return an array of links to the documents for a proposal. The id of a 
 * document to exclude can optionally be specified.
 * 
 * @param $nid_proposal - the nid of the proposal
 * @param $nid_document (optional) - if this is specified, exclude this document
 * from the result set to create an array of "other documents".
 * 
 * @return An array of document node links
 */
function smartparticipation_core_proposal_document_links($nid_proposal, $nid_document = 0) {
  
  // Get the ids of documents for this proposal
  $document_nids = smartparticipation_core_proposal_documents($nid_proposal);

  // Return an array of links to these documents
  return smartparticipation_core_node_menu_title_links($document_nids);

}


/**
 * Return a structured array for a carousel display of the documents in the same 
 * proposal as the current document.
 * 
 * @param $node - the document node
 * 
 * @return structured array of document data
 */
function smartparticipation_core_document_carousel_data($document) {

  $proposal_nid = smartparticipation_core_field_get_item_value('node', $document, 'field_sp_document_proposal', 'nid');
  $proposal = node_load($proposal_nid);
  
  $document_label = smartparticipation_core_field_get_item_value('node', $proposal, 'field_sp_document_label', 'value');
 
  $document_nids = smartparticipation_core_proposal_documents($proposal_nid);
  
  // Return a render array
  // If only one document (the current one), don't display a carousel.
  if (count($document_nids) > 1) {
  
    $document_links = smartparticipation_core_proposal_document_links($proposal_nid);
    // DO THIS INSTEAD! We already have the nids
    // $document_links = smartparticipation_core_node_menu_title_links($document_nids);

    $nodes = array();    
    foreach ($document_links as $link) {
      // We add additional structure to the array in order to share the carousel
      // template used for topic nodes, which display additional data for each
      // node.
      $nodes[] = array(
        'link' => $link,
      );
    }

    $recommended_comments_service = new \SmartParticipation\RecommendedCommentsService;
    $recommended_comments_enabled = $recommended_comments_service->getStatus();
    
    $document_data = array(
      '#theme' => 'node_carousel',
      'carousel_prompt' => smartparticipation_core_carousel_prompt(false, 'document'),
      'node_type' => 'document',        
      'nodes' => $nodes,
      'title' => t('All @document_label', array('@document_label' => $document_label)),
      'recommended_comments_enabled' => $recommended_comments_enabled,
    );
      
  } else {
    $document_data = array();
  }

  return $document_data;

}

/**
 * Custom node insert function for node type sp_document.
 * 
 * @param $node - the document node that has just been created
 */
function smartparticipation_core_document_node_insert($node) {

  // If the proposal documents are weighted, insert this document at the end of
  // the queue.
  $proposal_nid = smartparticipation_core_field_get_item_value('node', $node, 'field_sp_document_proposal', 'nid');
  $last_document_weight = smartparticipation_core_last_proposal_document_weight($proposal_nid);

  if ($last_document_weight != null) {
    
    $weight = $last_document_weight + 1;

    db_insert('sp_document_node_order')
      ->fields(array(
        'dnid' => $node->nid,
        'pnid' => $proposal_nid,
        'weight' => $weight,
      ))
      ->execute();
  }
}

/**
 * Return the weight of the last weighted document of the specified proposal, or
 * -1 if the documents are not weighted.
 * 
 * @param $proposal_nid - nid of the proposal node
 * 
 * @return integer - weight of last proposal document, or -1 if the documents
 * are not weighted.
 */
function smartparticipation_core_last_proposal_document_weight($proposal_nid) {

  // If no documents, we get an empty array back
  $last_document = smartparticipation_core_proposal_documents($proposal_nid, 0, true, 1, 'DESC');
  
  $last = array_shift($last_document);

  // Note that the weights could be negative, so can't use a negative value to
  // signal no weights.
  // Note also that isset($last->weight) doesn't error if $last is null.
  return isset($last->weight) ? $last->weight : null;

}
