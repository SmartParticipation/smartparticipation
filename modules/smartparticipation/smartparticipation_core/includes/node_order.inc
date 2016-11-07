<?php

/**
 * Page callback for subtopic and topic node order forms. 
 * 
 * @param $form - the form
 * @param $node - the node this form belongs to: the topic node in the case of
 * the subtopic order form, and the proposal node in the case of the topic 
 * order form.
 */
function smartparticipation_core_order_form($form, $node) {
  
  $nodereference_type = smartparticipation_core_get_nodereferences_type($node['build_info']['args'][0]->type);

  switch ($nodereference_type) {
    
    /*
    TODO Not sure why, but documents are handled differently. We go straight
    to smartparticipation_core_document_order_form(). Should we either handle documents here,
    or separate topics and subtopics as well?
    case 'document':
      $form = smartparticipation_core_document_order_form($form, $node);
      break;
    */
    
    case 'subtopic':
      $form = smartparticipation_core_subtopic_order_form($form, $node);
      break;
      
    case 'topic':
      $form = smartparticipation_core_topic_order_form($form, $node);
      break;
  }

  return $form;

}

////////////////////////////// DOCUMENTS //////////////////////////////

/**
 * Form for ordering proposal documents.
 * 
 * @param $form - the form object
 * @param $form_proposal - the proposal data from the form
 */
function smartparticipation_core_document_order_form($form, $form_proposal) {

  $proposal = $form_proposal['build_info']['args'][0];
  $proposal_nid = $proposal->nid; 
  
  $message = l('Add new document','node/add/sp-document/' . $proposal_nid);
  $form['message'] = array(
    '#type' => 'markup',
    '#markup' => $message,
  );

  // Show the order form only if there are documents for this proposal    
  if (isset($proposal->field_sp_proposal_document[LANGUAGE_NONE])) {
    
    // Create a parent element and use our custom theme
    $form['characters'] = array(
      '#tree' => TRUE,
      '#theme' => 'smartparticipation_core_node_order',
    );

    $documents = smartparticipation_core_proposal_documents($proposal_nid, 0, true, 0);

    foreach ($documents as $index=>$document) {
      $dnode = node_load($document->nid);

      $form['characters'][$index]['nid'] = array(
        '#type' => 'hidden',
        '#default_value' => $dnode->nid,
      );
      
      $form['characters'][$index]['title'] = array(
        '#type' => 'textfield',
        '#default_value' => $dnode->title,
      );
      
      $form['characters'][$index]['weight'] = array(
        '#type' => 'textfield',
        '#default_value' => $document->weight,
        '#size' => 3,
        '#attributes' => array('class' => array('rank-weight')), // needed for table dragging
      );
      
      $form['characters'][$index]['operations'] = array(
        '#type' => 'markup',
        '#markup' => l('edit document', 'node/'. $dnode->nid .'/edit'),
      );
    }

    $form['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Submit'),
      '#submit' => array('smartparticipation_core_document_order_form_submit'),
    );

  }

  return $form;
}

/**
 * Submit handler for the document node order form
 */
function smartparticipation_core_document_order_form_submit($form, &$form_state) {

  $edit = $form_state['values']['characters'];

  $proposal = $form_state['build_info']['args'][0];
  $proposal_nid = $proposal->nid; 

  if ( is_array($proposal->field_sp_proposal_document) && count($proposal->field_sp_proposal_document) > 0 ) {
    $documents = field_get_items('node', $proposal, 'field_sp_proposal_document');

    $table = 'sp_document_node_order';
    
    // Delete any existing weights 
    db_delete($table)
      ->condition('pnid', $proposal_nid)
      ->execute();
    
    $query = db_insert($table)->fields(array('pnid', 'dnid', 'weight'));
    
    foreach (array_keys($documents) as $document_nid) {

      // The weight can be emptied by using the input elements interface rather 
      // than the drag-and-drop interface. This produces a null value.
      if ($edit[$document_nid]['weight'] != null) {
        $record = array(
          'pnid' => $proposal_nid,
          'dnid' => $edit[$document_nid]['nid'],
          // TODO Later, fix this to always start numbering from 0. 
          'weight' => $edit[$document_nid]['weight'],
        );
        $query->values($record); 
      }
    }
    
    $query->execute();  
    
  }
  
}


////////////////////////////// SUBTOPICS //////////////////////////////

/** 
 * Form for ordering topic subtopics.
 * 
 * @param $form - the form object
 * @param $form_topic - the topic data from teh form
 */
function smartparticipation_core_subtopic_order_form($form, $form_topic) {
  
  $topic = $form_topic['build_info']['args'][0];
  $topic_nid = $topic->nid;

  $message = l('Add new subtopic','node/add/sp-subtopic/' . $topic_nid);
  $form['message'] = array(
    '#type' => 'markup',
    '#markup' => $message,
  );
  
  // Show the order form only if there are subtopics for this topic
  if (isset($topic->field_sp_topic_subtopic[LANGUAGE_NONE])) {
    
    // Create a parent element and use our custom theme
    $form['characters'] = array(
      '#tree' => TRUE,
      '#theme' => 'smartparticipation_core_node_order',
    );

    $subtopics = smartparticipation_core_topic_subtopics($topic_nid, true);
    
    foreach ($subtopics as $index=>$subtopic) {
      $snode = node_load($subtopic->nid);

      $form['characters'][$index]['nid'] = array(
      '#type' => 'hidden',
      '#default_value' => $snode->nid,
      );
      
      $form['characters'][$index]['title'] = array(
        '#type' => 'textfield',
        '#default_value' => $snode->title,
      );
      
      $form['characters'][$index]['weight'] = array(
        '#type' => 'textfield',
        '#default_value' => $subtopic->weight,
        '#size' => 3,
        '#attributes' => array('class' => array('rank-weight')), // needed for table dragging       
      );
      
      $form['characters'][$index]['operations'] = array(
        '#type' => 'markup',
        '#markup' => l('edit subtopic', 'node/' . $snode->nid . '/edit'),
      );
    }
        

    $form['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Submit'),
      '#submit' => array('smartparticipation_core_subtopic_order_form_submit'),
      );
      
  }

  return $form;
}

/**
 * Submit handler for the subtopic node order form
 */
function smartparticipation_core_subtopic_order_form_submit($form, &$form_state) {
  
  $edit = $form_state['values']['characters'];

  $topic = $form_state['build_info']['args'][0];
  $topic_nid = $topic->nid; 

  if ( is_array($topic->field_sp_topic_subtopic) && count($topic->field_sp_topic_subtopic) > 0 ) {
    
    $subtopics = field_get_items('node', $topic, 'field_sp_topic_subtopic');

    $table = 'sp_subtopic_node_order';

    // Delete any existing weights 
    db_delete($table)
      ->condition('tnid', $topic_nid)
      ->execute();

    $query = db_insert($table)->fields(array('tnid', 'snid', 'weight'));
             
    foreach (array_keys($subtopics) as $subtopic_nid) {
      
      // The weight can be emptied by using the input elements interface rather 
      // than the drag-and-drop interface. This produces a null value.
      if ($edit[$subtopic_nid]['weight'] != null) {
        $record = array(
          'tnid' => $topic_nid,
          'snid' => $edit[$subtopic_nid]['nid'],
          // TODO Later, fix this to always start numbering from 0. 
          'weight' => $edit[$subtopic_nid]['weight'],
        );
        $query->values($record); 
      }
    }
    
    $query->execute();  
    
  }
  
}

////////////////////////////// TOPICS //////////////////////////////


function smartparticipation_core_topic_order_form($form, $node) {

  $nid = $node['build_info']['args'][0]->nid;
  $proposal = node_load($nid);

  $message = l('Add new topic','node/add/sp-topic/'.$nid);
  $form['message'] = array(
    '#type' => 'markup',
    '#markup' => $message,
    #'#tree' => true,
  );

  // Show the order form only if there are topics for this proposal    
  if (isset($proposal->field_sp_proposal_topic[LANGUAGE_NONE])) {
    // Create a parent element and use our custom theme
    $form['characters'] = array(
      '#tree' => TRUE,
      '#theme' => 'smartparticipation_core_node_order',
      '#show_phase_column' => TRUE,
    );

    // Get the topics for the current proposal
    // 1. Get the topic ids from the field
    // 2. get the order from the table based on topic node ids. Order by phase
    // and the order of that phase within the proposal. This suggests that we
    // want to use phase_id rather than term_tid.



    $phases = _smartparticipation_core_proposal_phase_display_data($proposal, false);
    $phases_keyed = array();
    $phase_collections_ids = array();
    foreach ($phases as $index=>$phase) {
      $phase_tid = $phase['tid'];
      $phase_id = $phase['phase_id'];
      $phases_keyed[$phase_tid] = $phase;
      $phase_field_collections_ids[] = $phase_id;
    }
    $phases = $phases_keyed;

    $in_phases = implode(',',$phase_field_collections_ids);
    $query = 'SELECT o.ttid, o.tnid, o.weight FROM {sp_topic_node_order} o WHERE o.ttid IN ('.$in_phases.') ORDER BY o.weight ASC';
    $result = db_query($query,array(':pnid' => $nid))->fetchAllAssoc('tnid');
    $topics_weights = $result;

    $topics = field_get_items('node', $proposal, 'field_sp_proposal_topic', $langcode = 'LANGUAGE_NONE');
    foreach ($topics as $topic) {
      $topic_nid = $topic['nid'];
      $phase_tid = smartparticipation_core_get_topic_phase_tid($topic_nid);
      $topic_node = node_load($topic_nid);
      $phases[$phase_tid]['topics'][$topic_nid] = $topic_node;
    }

    function add_form_row(&$form, $phase, $topic_nid, $topic_title, $topic_weight) {
      $phase_id = $phase['phase_id'];
      $phase_title = $phase['name'];
      $form['characters'][$topic_nid]['tnid'] = array(
        '#type' => 'hidden',
        '#default_value' => $topic_nid,
      );
      $form['characters'][$topic_nid]['title'] = array(
        '#type' => 'textfield',
        '#default_value' => $topic_title,
      );
      $form['characters'][$topic_nid]['weight'] = array(
        '#type' => 'textfield',
        '#default_value' => $topic_weight,
        '#size' => 3,
        '#attributes' => array('class' => array('rank-weight')), // needed for table dragging
      );
      $form['characters'][$topic_nid]['phase_id'] = array(
        '#type' => 'hidden',
        '#default_value' => $phase_id,
      );
      $form['characters'][$topic_nid]['phase'] = array(
        '#type' => 'markup',
        '#markup' => $phase_title,
      );
      $form['characters'][$topic_nid]['operations'] = array(
        '#type' => 'markup',
        '#markup' => l('edit topic', 'node/'.$topic_nid.'/edit'),
      );
    }

    foreach ($phases as $key => $phase) {
      if (count($phase['topics']) > 0) {
        $topics = $phase['topics'];
        $ordered_topics = array();
        $no_weight = 0;
        foreach ($topics as $topic) {
          $topic_nid = $topic->nid;
          if (array_key_exists($topic_nid, $topics_weights)) {
            $ordered_topics[$topics_weights[$topic_nid]->weight][] = $topic;
          } else {
            $ordered_topics[$no_weight][] = $topic;
          }
        }
        ksort($ordered_topics);
        foreach ($ordered_topics as $weight => $topics) {
          foreach ($topics as $topic) {
            add_form_row($form, $phase, $topic->nid, $topic->title, $weight);
          }
        }
      }
    }

    $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Submit'),
      '#submit' => array('smartparticipation_core_topic_order_form_submit'),
    );

  }
  
  return $form;
}

/**
 * Submit handler for the topics order form
 */
function smartparticipation_core_topic_order_form_submit($form, &$form_state) {
  $edit = $form_state['values']['characters'];
  $nid = $form_state['build_info']['args'][0]->nid;
  $node = node_load($nid);

  if ( is_array($node->field_sp_proposal_topic) && count($node->field_sp_proposal_topic) > 0 ) {
    $topics = field_get_items('node', $node, 'field_sp_proposal_topic', $langcode = 'LANGUAGE_NONE');

    //foreach ( $node->field_sp_proposal_topic[LANGUAGE_NONE] as $rid => $ref ) {
    foreach ( $topics as $rid => $ref ) {
      $topic_nid = $ref['nid'];
      if ($topic_nid > 0) {
        $deleted =
          db_delete('sp_topic_node_order')
            ->condition('tnid', $edit[$topic_nid]['tnid'])
            ->execute();
        $override = array(
          'ttid' => $edit[$topic_nid]['phase_id'],
          'tnid' => $edit[$topic_nid]['tnid'],
          'weight' => $edit[$topic_nid]['weight'],
        );
        smartparticipation_core_topic_order_save($override);
      }
    }
  }
}


/**
 * Save the topics order
 */
function smartparticipation_core_topic_order_save($override) {

  // Look for existing entries
//  $query = db_select('smartparticipation_core_topic_node_order', 'o')
//    ->fields('o', array('ttid', 'tnid', 'weight'));
//  $query->where("o.ttid = :ttid AND o.tnid = :tnid", array(':ttid' => $override['ttid'], ':tnid' => $override['tnid']));
//
//  $result = $query->execute();
//  $count = $result->rowCount();

  // Insert new or update existing record
//  if ( $count > 0 ) {
//    drupal_write_record('smartparticipation_core_topic_node_order', $override, array('ttid', 'tnid'));
//  } else {
    drupal_write_record('sp_topic_node_order', $override);
//  }
}




////////////////////////////// THEME //////////////////////////////


/**
 * Create the themable table form with dropdown and drag-and-drop node ordering
 * functionality
 */
function theme_smartparticipation_core_node_order($vars) {
  $element = $vars['element'];
  drupal_add_tabledrag('smartparticipation_core_order_form', 'order', 'sibling', 'rank-weight');
  
  $header = array(
    'title' => t('Title'), 
    'weight' => t('Weight'),
    'phase' => t('Phase'),
    'operations' => t('Operations'), 
  );
  if (empty($element['#show_phase_column'])) {
    unset($header['phase']);
  }
  
  $rows = array();

  foreach (element_children($element) as $key) {
    $row = array();
    $row['data'] = array();

    foreach ($header as $fieldname => $title) {
      //tabledrag-hide
      $row['data'][] = drupal_render($element[$key][$fieldname]);
      $row['class'] = array('draggable'); // needed for table dragging
    }
    
    $rows[] = $row;
  }

  return theme('table', array(
  	'header' => $header, 
  	'rows' => $rows,
  	'attributes' => array('id' => 'smartparticipation_core_order_form'),
  	'empty' => '',
    'sticky' => FALSE,
  ));
}










