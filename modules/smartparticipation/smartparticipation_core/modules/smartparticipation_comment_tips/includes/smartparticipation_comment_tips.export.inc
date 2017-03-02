<?php

use SmartParticipation\model\EventLogRecord;

function _smartparticipation_comment_tips_export_comment_tips_log_csv($proposal)
{
  $proposal_name = smartparticipation_core_slug($proposal->title);
  $current_time = date('Y-m-d-h-i', time()); // year-month-date-hour-minute
  $csv_filename = 'comment-tips-log-' . $proposal_name . '-' . $current_time . '.csv';

  drupal_add_http_header('Content-Type', 'text/csv; utf-8');
  drupal_add_http_header('Content-Disposition', 'attachment; filename=' . $csv_filename);

  $output = fopen("php://output", 'w');

  $headings = array(
    EventLogRecord::getLabel('eid'),
    EventLogRecord::getLabel('name'),
    EventLogRecord::getLabel('detail'),
    t('Comment Text'),
    EventLogRecord::getLabel('entity_type'),
    EventLogRecord::getLabel('entity_id'),
    EventLogRecord::getLabel('path'),
    EventLogRecord::getLabel('uid'),
    EventLogRecord::getLabel('username'),
    EventLogRecord::getLabel('hostname'),
    EventLogRecord::getLabel('timestamp'),
    //t('Event Timestamp'),
    EventLogRecord::getLabel('instance_start'),
    //t('Instance Start Timestamp'),
  );
  fputcsv($output, $headings);

  $min_start_time = _smartparticipation_core_get_proposal_min_start_time ($proposal);
  $max_end_time = _smartparticipation_core_get_proposal_max_end_time($proposal);
  if (empty($max_end_time)) $max_end_time = time();

  $event_log_service = new \SmartParticipation\EventLogService;
  $event_log_records = $event_log_service->findAll(array(
    'comment text beforeunload',
    'close comment tips',
    'open comment tips with button',
    'comment tips revise comment selected',
    'open comment tips prompt',
    'comment tips submit comment selected',
    'comment submitted',
  ));
  /** @var EventLogRecord $event_log_record */
  foreach ($event_log_records as $event_log_record) {
    $event_timestamp = '';
    $event_datetime = '';
    if ($event_log_record->timestamp) {

      if ($event_log_record->timestamp <  $min_start_time || $event_log_record->timestamp > $max_end_time) {
        continue;
      }

      $event_timestamp = $event_log_record->timestamp;
      $event_datetime = date("m/d/y g:i:s a", $event_timestamp);
    }
    $instance_start_timestamp = '';
    $instance_start_datetime = '';
    if ($event_log_record->instance_start) {
      $instance_start_timestamp = $event_log_record->instance_start;
      $instance_start_datetime = date("m/d/y g:i:s a", $instance_start_timestamp);
    }
    $row = array(
      $event_log_record->eid,
      $event_log_record->name,
      $event_log_record->detail,
      $event_log_record->detail_long,
      $event_log_record->entity_type,
      $event_log_record->entity_id,
      $event_log_record->path,
      $event_log_record->uid,
      $event_log_record->username,
      $event_log_record->hostname,
      $event_datetime,
      //$event_timestamp,
      $instance_start_datetime,
      //$instance_start_timestamp,
    );
    fputcsv($output, $row);
  }

  fclose($output);

  exit;
}