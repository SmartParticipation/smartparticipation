<?php

/**
 * Schema for RegRoom node ordering tables.
 */
 


/**
 * Return table schemas for ordering subtopics, topics, and documents relative
 * to a "parent" entity.
 */
function smartparticipation_core_order_schema() {
  
  $t = get_t();
  
  return array(

    'sp_document_node_order' => array(
      'description' => $t('Store weights of a proposal\'s document nodes.'),
      'fields' => array(
        'dnid' => array(
          'description' => $t('Document node nid.'),
          'type' => 'int',
          'not null' => TRUE,
        ),
        'pnid' => array(
          'description' => $t('Proposal node nid.'),
          'type' => 'int',
          'not null' => TRUE,
        ),
        'weight' => array(
          'description' => $t('Document node weight.'),
          'type' => 'int',
          'not null' => TRUE,
        ),
      ),
      'primary key' => array('dnid'),
      'unique keys' => array(
        'dnid_pnid' => array('dnid', 'pnid'),
        'dnid_weight' => array('dnid', 'weight'),
      ),
    ),

    'sp_subtopic_node_order' => array(
      'description' => $t('Store weights of a topic\'s subtopic nodes.'),
      'fields' => array(
        'snid' => array(
          'description' => $t('Subtopic node nid.'),
          'type' => 'int',
          'not null' => TRUE,
        ),
        'tnid' => array(
          'description' => $t('Topic node nid.'),
          'type' => 'int',
          'not null' => TRUE,
        ),
        'weight' => array(
          'description' => $t('Subtopic node weight.'),
          'type' => 'int',
          'not null' => TRUE,
        ),
      ),
      'primary key' => array('snid'),
      'unique keys' => array(
        'snid_tnid' => array('snid', 'tnid'),
        'snid_weight' => array('snid', 'weight'),
      ),
    ),

    'sp_topic_node_order' => array(
      'description' => $t('Store weights of topic nodes within a proposal phase.'),
      'fields' => array(
        'tnid' => array(
          'description' => $t('Topic node nid.'),
          'type' => 'int',
          'not null' => TRUE,
        ),
        
        // TODO Should this be the term tid, or the id of the proposal phase 
        // assigned in field_sp_proposal_phase? What makes querying easier?
        // Change to either ttid or phid.
        'ttid' => array(
          'description' => $t('Phase vocabulary term tid.'),
          'type' => 'int',
          'not null' => TRUE,
        ),  
                
        'weight' => array(
          'description' => $t('Topic node weight.'),
          'type' => 'int',
          'not null' => TRUE,
        ),
      ),
      'primary key' => array('tnid'),
      'unique keys' => array(
        'tnid_ttid' => array('tnid', 'ttid'),
        'tnid_weight' => array('tnid', 'weight'),
      ),  
    ),
     
  );

}
