<?php


namespace SmartParticipation;


use PDO;
use SmartParticipation\model\AccessLogRecord;

class AccessLogService {

  const ACCESS_LOG_TABLE = 'accesslog';

  /**
   * @return AccessLogRecord[]
   */
  public function findAll($start_time = null, $end_time = null)
  {
    $cols = array(
      'aid',
      'sid',
      'title',
      'path',
      'url',
      'hostname',
      'uid',
      'timer',
      'timestamp',
    );
    // Better Statistics module can add the user_agent field
    if (db_field_exists('accesslog', 'user_agent')) {
      array_push($cols, 'user_agent');
    }
    $query = db_select(self::ACCESS_LOG_TABLE, 'al')
      ->fields('al', $cols)
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

      $access_log_records[$aid] = new AccessLogRecord(
        $access_log_record['aid'],
        $access_log_record['sid'],
        $access_log_record['title'],
        $access_log_record['path'],
        $access_log_record['url'],
        $access_log_record['hostname'],
        $access_log_record['uid'],
        !empty($access_log_record['user_agent']) ? $access_log_record['user_agent'] : null,
        $access_log_record['timer'],
        $access_log_record['timestamp']
      );

    }

    return $access_log_records;
  }
}