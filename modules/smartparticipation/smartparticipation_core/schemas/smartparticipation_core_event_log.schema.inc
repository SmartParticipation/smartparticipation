<?php 

/**
 * Implements hook_schema().
 */
function smartparticipation_core_event_log_schema() {
  
  $t = get_t();
  
  return array(
      
    'sp_event_log' => array(
        
      'description' => $t('Logs events for statistical analysis of user behavior on the site.'),
        
      'fields' => array(
          
        // EVENT IDENTIFIERS
        'eid' => array(
          'type' => 'serial',
          'not null' => true,
          'description' => $t('Primary Key: Unique accesslog ID.'),
        ),
        'name' => array(
          'type' => 'varchar',
          'length' => 255,
          'not null' => false,
          'description' => $t('Name of event.'),
        ),
        'detail' => array(
          'type' => 'varchar',
          'length' => 255,
          'not null' => false,
          'description' => $t('Optional additional details, where an entity is irrelevant or insufficient to fully identify the event.'),
        ),
        'detail_long' => array(
          'type' => 'text',
          'size' => 'big',
          'not null' => false,
          'description' => $t('Optional additional details, where full text data is necessary.'),
        ),
                    
        // RELATED ENTITY
        'entity_type' => array(
          'type' => 'varchar',
          'length' => 32,
          'not null' => false,
          'description' => $t('Type of entity, if any, that is related to the event.'),            
        ),        
        'entity_id' => array(
          'type' => 'int',
          'unsigned' => true,
          'not null' => false,
          'description' => $t('Id of entity, if any, that is related to the event.'),            
        ),      
          
        // PAGE EVENT OCCURRED ON
        'path' => array(
          'type' => 'varchar',
          'length' => 255,
          'not null' => false,
          'description' => $t('Internal path to page event occurred on (relative to Drupal root).'),
        ),   

        // USER INFORMATION: uid, hostname, session id
        'uid' => array(
          'type' => 'int',
          'unsigned' => true,
          'not null' => true,
          'default' => 0,
          'description' => $t('User {users}.uid that initiated the event.'),
        ),        
        'hostname' => array(
          'type' => 'varchar',
          'length' => 128,
          'not null' => false,
          'description' => $t('Hostname of user that initiated the event.'),
        ),
        /*
        'sid' => array(
          'type' => 'varchar',
          'length' => 128,
          'not null' => true,
          'default' => '',
          'description' => $t('Browser session ID of user that visited page.'),
        ),
        */
          
        /*
        // ADDITIONAL DATA ABOUT EVENT: use only as a last resort
        'data' => array(
          'type' => 'blob',
          'not null' => false,
          'size' => 'big',
          'serialize' => true,
          'description' => $t('A serialized array of key-value pairs that are related to the event. Use only as a last resort: use entity_id, entity_type, and/or entity_description instead whenever possible.'),
        ),
        */

        // SESSION ID
        'sid' => array(
          'type' => 'varchar',
          'length' => 128,
          'not null' => false,
          'description' => 'Session ID of user that initiated the event.',
        ),

        // TIMESTAMP OF INSTANCE START
        'instance_start' => array(
          'type' => 'int',
          'unsigned' => true,
          'not null' => false,
          'default' => 0,
          'description' => $t('Timestamp of when the event occurred.'),
        ),
          
        // TIMESTAMP OF EVENT
        'timestamp' => array(
          'type' => 'int',
          'unsigned' => true,
          'not null' => true,
          'default' => 0,
          'description' => $t('Timestamp of when the event occurred.'),
        ),

      ),
        
      'indexes' => array(
        'instance_start' => array('instance_start'),
        'timestamp' => array('timestamp'),
        'uid' => array('uid'),
      ),
        
      'primary key' => array('eid'),
        
      'foreign keys' => array(
        'visitor' => array(
          'table' => 'users',
          'columns' => array('uid' => 'uid'),
        ),
      ),
          
    ),
  );
  
}