<?php


namespace SmartParticipation;


use PDO;
use SmartParticipation\model\AccessLogRecord;

class AccessLogService {

  const ACCESS_LOG_TABLE = 'accesslog';

  protected $mapper;

  public function __construct()
  {
    $this->mapper = new Mapper('SmartParticipation\model\AccessLogRecord');
  }

  /**
   * @return AccessLogRecord[]
   */
  public function findAll($start_time = null, $end_time = null)
  {
    $query = db_select(self::ACCESS_LOG_TABLE, 'al')
      ->fields('al', array(
        'aid',
        'sid',
        'title',
        'path',
        'url',
        'hostname',
        'uid',
        'user_agent',
        'timer',
        'timestamp',
      ))
      ->orderBy('timestamp','DESC');

    if (!empty($start_time)) {
      $query->condition('timestamp', $start_time, '>=');
    }

    if (!empty($end_time)) {
      $query->condition('timestamp', $end_time, '<=');
    }

    $result = $query->execute()->fetchAllAssoc('aid', PDO::FETCH_BOTH);

    $access_log_records = array();
    foreach ($result as $aid => $access_log_record) {
      $access_log_records[$aid] = $this->mapper->create($access_log_record);
    }

    return $access_log_records;
  }
}