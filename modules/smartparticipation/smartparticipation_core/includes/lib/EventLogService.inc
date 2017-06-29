<?php

namespace SmartParticipation;


use PDO;
use SmartParticipation\model\EventLogRecord;

class EventLogService
{
  const EVENT_LOG_TABLE = 'sp_event_log';

  /**
   * @param array $event_names
   * @return EventLogRecord[]
   */
  public function findAll(array $event_names = null)
  {
    $query = db_select(self::EVENT_LOG_TABLE, 'el')
      ->fields('el', array(
        'eid',
        'name',
        'detail',
        'detail_long',
        'entity_type',
        'entity_id',
        'path',
        'uid',
        'hostname',
        'timestamp',
        'instance_start',
      ))
        ->fields('u', array('name'))
      ->orderBy('timestamp','DESC');


    if (!empty($event_names)) {
      $query->condition('el.name', $event_names, 'IN');
    }

    $query->join('users', 'u', 'u.uid = el.uid');
    $query->addField('u', 'name', 'username');

    $result = $query->execute()->fetchAllAssoc('eid', PDO::FETCH_BOTH);

    $event_log_records = array();
    foreach ($result as $eid => $event_log_record) {
      $event_log_records[$eid] = new EventLogRecord(
        $event_log_record['eid'],
        $event_log_record['name'],
        $event_log_record['detail'],
        $event_log_record['detail_long'],
        $event_log_record['entity_type'],
        $event_log_record['entity_id'],
        $event_log_record['path'],
        $event_log_record['uid'],
        $event_log_record['username'],
        $event_log_record['hostname'],
        $event_log_record['timestamp'],
        $event_log_record['instance_start']
      );
    }

    return $event_log_records;
  }
}