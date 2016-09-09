<?php

/**
 * Schema related to smartparticipation_core flags.
 */

/**
 * Return the schema for the smartparticipation_core_flag_content_history table.
 */
function smartparticipation_core_flag_schema() {
  
  $t = get_t();
  
  return array(

    'sp_flag_content_history' => array(
      'description' => $t('Stores comment endorsements.'),
      'fields' => array(
        'fid' => array(
          'description' => $t('Flag id.'),
          'type' => 'int',
          'not null' => TRUE,
        ),
        'content_type' => array(
          'description' => $t('Type of flagged content.'),
          'type' => 'varchar',
          'length' => 50, 
          'not null' => TRUE,
        ),
        'content_id' => array(
          'description' => $t('Flagged content id.'),
          'type' => 'int',
          'not null' => TRUE,
        ),
        'uid' => array(
          'description' => $t('User id of endorser.'),
          'type' => 'int',
          'not null' => TRUE,
        ),
        'action' => array(
          'description' => $t('Flag action: endorse or unendorse.'),
          'type' => 'varchar',
          'length' => 100, 
          'not null' => TRUE,
        ),
        'timestamp' => array(
          'description' => $t('Timestamp of endorsement.'),
          'type' => 'int',
          'not null' => TRUE,
        ),
      ),
    ),
    
  );
}