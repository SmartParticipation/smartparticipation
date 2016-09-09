<?php
/**
 * Created by PhpStorm.
 * User: Brian
 * Date: 5/15/15
 * Time: 11:11 AM
 */

namespace SmartParticipation;


use PDO;
use SmartParticipation\model\EventLogRecord;

class EventLogService
{
  const EVENT_LOG_TABLE = 'sp_event_log';

  protected $mapper;

  public function __construct()
  {
    $this->mapper = new Mapper('SmartParticipation\model\EventLogRecord');
  }

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
      $event_log_records[$eid] = $this->mapper->create($event_log_record);
    }

    return $event_log_records;
  }

  /*public function mapResult($result) {
    return new EventLogRecord(
      $result['eid'],
      $result['name'],
      $result['detail'],
      $result['detail_long'],
      $result['entity_type'],
      $result['entity_id'],
      $result['path'],
      $result['uid'],
      $result['hostname'],
      $result['timestamp'],
      $result['instance_start']
    );
  }*/
}