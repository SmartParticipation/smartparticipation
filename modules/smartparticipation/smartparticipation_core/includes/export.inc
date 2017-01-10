<?php
use SmartParticipation\model\AccessLogRecord;
use SmartParticipation\model\User;
use SmartParticipation\model\EventLogRecord;
use SmartParticipation\model\ABTest;
use SmartParticipation\model\ABTestUserVariant;

function smartparticipation_core_export_form($form, $node) {
  $nid = $node['build_info']['args'][0]->nid;
  $proposal = node_load($nid);

  $message = '<h2>Export CSV</h2>';

  $message .= '<ol>';
  
  $user = user_load(2);

  $export_menu_items = array(
    'Access log' => 'node/'.$nid.'/export-access-log',
    'Comment data' => 'node/'.$nid.'/export-comments',
    'Comment tips log' => 'node/'.$nid.'/export-comment-tips-log',
    'Endorsement data' => 'node/'.$nid.'/export-endorsements',
    'Event log' => 'node/'.$nid.'/export-event-log',
  );

  if (db_table_exists('botlog')) {
    $export_menu_items['Bot log'] = 'node/' . $nid . '/export-bot-log';
  }

  drupal_alter('smartparticipation_export_menu_items', $export_menu_items, $nid);

  ksort($export_menu_items);

  foreach ($export_menu_items as $item_title => $item_path) {
    $message .= '<li>'.l($item_title, $item_path).'</li>';
  }

  /*$message .= '<li>'.l('A/B test descriptions', 'node/'.$nid.'/export-ab-test-descriptions').'</li>';
  $message .= '<li>'.l('A/B test user assignments', 'node/'.$nid.'/export-ab-test-assignments').'</li>';
  $message .= '<li>'.l('Access log', 'node/'.$nid.'/export-access-log').'</li>';
  if (db_table_exists('botlog'))
    $message .= '<li>'.l('Bot log', 'node/'.$nid.'/export-bot-log').'</li>';
  $message .= '<li>'.l('Comment data', 'node/'.$nid.'/export-comments').'</li>';
  $message .= '<li>'.l('Comment tips log', 'node/'.$nid.'/export-comment-tips-log').'</li>';
  $message .= '<li>'.l('Endorsement data', 'node/'.$nid.'/export-endorsements').'</li>';
  $message .= '<li>'.l('Event log', 'node/'.$nid.'/export-event-log').'</li>';*/

  $message .= '</ol>';
  
  $form['message'] = array(
    '#type' => 'markup',
    '#markup' => $message,
  );

  return $form;
}


/**
 * This function will prepare a string to be used in the URLs
 */
function smartparticipation_core_slug($str){
  $str = strtolower(trim($str));
  $str = preg_replace('/[^a-z0-9-]/', '-', $str);
  $str = preg_replace('/-+/', "-", $str);

  return $str;
}

function _smartparticipation_core_export_access_log_csv($proposal)
{
  ini_set('memory_limit', '4096M');
  ini_set('max_execution_time', 300); //300 seconds = 5 minutes

  $proposal_name = smartparticipation_core_slug($proposal->title);
  $current_time = date('Y-m-d-h-i', time()); // year-month-date-hour-minute
  $csv_filename = 'access-log-' . $proposal_name . '-' . $current_time . '.csv';

  drupal_add_http_header('Content-Type', 'text/csv; utf-8');
  drupal_add_http_header('Content-Disposition', 'attachment; filename=' . $csv_filename);

  $output = fopen("php://output", 'w');

  $headings = array(
    AccessLogRecord::getLabel('aid'),
    AccessLogRecord::getLabel('sid'),
    AccessLogRecord::getLabel('title'),
    AccessLogRecord::getLabel('path'),
    AccessLogRecord::getLabel('url'),
    AccessLogRecord::getLabel('hostname'),
    AccessLogRecord::getLabel('uid'),
    t('Username'),
    AccessLogRecord::getLabel('user_agent'),
    AccessLogRecord::getLabel('timer'),
    AccessLogRecord::getLabel('timestamp'),
  );
  fputcsv($output, $headings);

  $min_start_time = _smartparticipation_core_get_proposal_min_start_time ($proposal);
  $max_end_time = _smartparticipation_core_get_proposal_max_end_time($proposal);
  if (empty($max_end_time)) $max_end_time = time();

  $access_log_service = new \SmartParticipation\AccessLogService;
  $access_log_records = $access_log_service->findAll(/*$start_time, $end_time*/);
  $user_service = new \SmartParticipation\UserService;
  /** @var EventLogRecord $event_log_record */
  foreach ($access_log_records as $access_log_record) {

    if ($access_log_record->timestamp <  $min_start_time || $access_log_record->timestamp > $max_end_time) {
      continue;
    }

    $username = '';
    if ($access_log_record->uid) {
      $user = $user_service->find($access_log_record->uid);
      if (!empty($user->name)) {
        $username = $user->name;
      }
    }
    $row = array(
      $access_log_record->aid,
      $access_log_record->sid,
      $access_log_record->title,
      $access_log_record->path,
      $access_log_record->url,
      $access_log_record->hostname,
      $access_log_record->uid,
      $username,
      $access_log_record->user_agent,
      $access_log_record->timer,
      date("m/d/y g:i:s a", $access_log_record->timestamp),
    );
    fputcsv($output, $row);
  }

  fclose($output);

  exit;
}

function _smartparticipation_core_export_bot_log_csv($proposal)
{
  $proposal_name = smartparticipation_core_slug($proposal->title);
  $current_time = date('Y-m-d-h-i', time()); // year-month-date-hour-minute
  $csv_filename = 'bot-log-' . $proposal_name . '-' . $current_time . '.csv';

  drupal_add_http_header('Content-Type', 'text/csv; utf-8');
  drupal_add_http_header('Content-Disposition', 'attachment; filename=' . $csv_filename);

  $output = fopen("php://output", 'w');

  $headings = array(
    t('Event ID'),
    t('IP Address'),
    t('User Agent'),
    t('User Session'),
    t('Event Datetime')
  );
  fputcsv($output, $headings);

  $min_start_time = _smartparticipation_core_get_proposal_min_start_time ($proposal);
  $max_end_time = _smartparticipation_core_get_proposal_max_end_time($proposal);
  if (empty($max_end_time)) $max_end_time = time();

  $query = "
    SELECT 
      eid,
      hostname,
      useragent,
      sid,
      timestamp
    FROM
      {botlog}
  ";

  $result = db_query($query)->fetchAll();

  foreach ($result as $bot_log_record) {

    $row = array(
      $bot_log_record->eid,
      $bot_log_record->hostname,
      $bot_log_record->useragent,
      $bot_log_record->sid,
      getNiceDateFormat($bot_log_record->timestamp)
    );

    fputcsv($output, $row);
  }

  fclose($output);

  exit;
}

function _smartparticipation_core_export_event_log_csv($proposal)
{
  $proposal_name = smartparticipation_core_slug($proposal->title);
  $current_time = date('Y-m-d-h-i', time()); // year-month-date-hour-minute
  $csv_filename = 'event-log-' . $proposal_name . '-' . $current_time . '.csv';

  drupal_add_http_header('Content-Type', 'text/csv; utf-8');
  drupal_add_http_header('Content-Disposition', 'attachment; filename=' . $csv_filename);

  $output = fopen("php://output", 'w');

  $headings = array(
    EventLogRecord::getLabel('eid'),
    EventLogRecord::getLabel('name'),
    EventLogRecord::getLabel('detail'),
    EventLogRecord::getLabel('detail_long'),
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
  $event_log_records = $event_log_service->findAll();
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

function getNiceDateFormat($timestamp) {
  if (empty($timestamp)) return '';
  return date("m/d/y g:i:s a", $timestamp);
}

function _smartparticipation_core_export_comment_tips_log_csv($proposal)
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

function _smartparticipation_core_export_endorsements_csv($proposal)
{
  $proposal_name = smartparticipation_core_slug($proposal->title);
  $current_time = date('Y-m-d-h-i', time()); // year-month-date-hour-minute
  $csv_filename = 'endorsements-' . $proposal_name . '-' . $current_time . '.csv';

  drupal_add_http_header('Content-Type', 'text/csv; utf-8');
  drupal_add_http_header('Content-Disposition', 'attachment; filename=' . $csv_filename);

  $output = fopen("php://output", 'w');

  $headings = array(
    'Comment ID',
    'Topic title',
    'Subtopic title',
    'Commenter User ID',
    'Commenter Username',
    'Comment Datetime',
    'Number of endorsements',
    'Endorsement ID',
    'Endorser User ID',
    'Endorser Username',
    'Endorsement Datetime',
  );
  fputcsv($output, $headings);

  $min_start_time = _smartparticipation_core_get_proposal_min_start_time ($proposal);
  $max_end_time = _smartparticipation_core_get_proposal_max_end_time($proposal);
  if (empty($max_end_time)) $max_end_time = time();

  $cids = _smartparticipation_core_comment_cids($proposal->nid);

  foreach ($cids as $cid) {

    $comment_details = smartparticipation_core_comment_data($cid);

    $endorsement_service = new \SmartParticipation\EndorsementService;
    $endorsements = $endorsement_service->findByCommentId($cid);

    foreach ($endorsements as $endorsement) {

      if ($endorsement->timestamp <  $min_start_time || $endorsement->timestamp > $max_end_time) {
        continue;
      }

      $row = array(
        $cid,
        $comment_details['comment_topic_title'],
        $comment_details['comment_subtopic_title'],
        $comment_details['uid'],
        $comment_details['username'],
        date("m/d/y g:i:s a", $comment_details['created']),
        $comment_details['no_endorsements'],
        $endorsement->fcid,
        $endorsement->uid,
        $endorsement->user->name,
        date("m/d/y g:i:s a", $endorsement->timestamp)
      );

      fputcsv($output, $row);

    }

  }

  fclose($output);

  exit;
}

/**
 * Function to export the comments
 */
function _smartparticipation_core_export_comments_csv($proposal) {

  $proposal_name = smartparticipation_core_slug($proposal->title);
  $current_time = date('Y-m-d-h-i', time()); // year-month-date-hour-minute
  $csv_filename = 'comments-' . $proposal_name . '-' . $current_time . '.csv';
  
  drupal_add_http_header('Content-Type', 'text/csv; utf-8');
  drupal_add_http_header('Content-Disposition', 'attachment; filename=' . $csv_filename);

  $output = fopen("php://output", 'w');
  
  $headings = array(
    'Comment ID', 
    'Topic title', 
    'Subtopic title', 
    'User ID', 
    'Username', 
    'User email address',
    'IP Address',
    'Datetime', 
    'Comment text', 
    'Comment character count',
    'Comment word count',
    'Parent ID', 
    'Number of endorsements',
    'Endorsers',
    'Number of replies', 
    'Moderation status', 
    'Moderation action', 
    'Recommended', 
    'Original (if redacted)',
  );
  fputcsv($output, $headings);

  $min_start_time = _smartparticipation_core_get_proposal_min_start_time ($proposal);
  $max_end_time = _smartparticipation_core_get_proposal_max_end_time($proposal);
  if (empty($max_end_time)) $max_end_time = time();

  $cids = _smartparticipation_core_comment_cids($proposal->nid, $min_start_time, $max_end_time);

  foreach ($cids as $cid) {

    $comment_details = smartparticipation_core_comment_data($cid);

    // Get a comma-separated list of the endorsement users
    $endorsement_users = '';
    /** @var User $user */
    foreach ($comment_details['endorsement_users'] as $user) {
      if ($endorsement_users) {
        $endorsement_users .= ', ';
      }
      $endorsement_users .= $user->name;
    }
    
    $body = $comment_details['body']['value'];
    
    $row = array(
      $cid,
      $comment_details['comment_topic_title'],
      $comment_details['comment_subtopic_title'],
      $comment_details['uid'],
      $comment_details['username'],
      $comment_details['mail'],
      $comment_details['hostname'],
      date("m/d/y g:i:s a", $comment_details['created']),
      $body,
      strlen($body),
      str_word_count($body),
      $comment_details['pid'],
      $comment_details['no_endorsements'],
      $endorsement_users,
      $comment_details['no_replies'],
      $comment_details['moderation_status'],
      $comment_details['moderation_action'],
      $comment_details['recommended'],
      $comment_details['original_body'],
    );
    
    fputcsv($output, $row);

  } 
  
  fclose($output);

  exit;
}


function _smartparticipation_core_comment_cids($proposal_nid, $start_time=NULL, $end_time=NULL) {

  $start_time_and = isset($start_time) ? ' AND c.created >= ' . $start_time : '';
  $end_time_and = isset($end_time) ? ' AND c.created <= ' . $end_time : '';

  $query = 'SELECT cid 
            FROM {field_data_field_sp_proposal_topic} t 
            INNER JOIN {field_data_field_sp_topic_subtopic} s ON t.field_sp_proposal_topic_nid = s.entity_id 
            INNER JOIN {comment} c ON s.field_sp_topic_subtopic_nid = c.nid 
            WHERE t.entity_id = :proposal_nid' .
            $start_time_and .
            $end_time_and .
            ' ORDER BY c.cid ASC';

  $cids = db_query($query, array(
    ':proposal_nid' => $proposal_nid,
  ))
  ->fetchCol();
  
  return $cids;  
}


/**
 * Exports a CSV.
 *
 * @param array $variables
 *   An associative array of data containing "header" and "rows" keys.
 *   This is ready to be passed to theme_table(). See api.drupal.org/node/22950.
 *   Also contains the key "filename" specifying the filename.
 */
function _smartparticipation_core_export_users_csv() {

  ini_set('memory_limit', '8192M');
  ini_set('max_execution_time', 600); //300 seconds = 5 minutes
  
  $current_time = date('Y-m-d-h-i', time()); // year-month-date-hour-minute  
  $csv_filename = 'users-'. $current_time .'.csv';
  
  drupal_add_http_header('Content-Type', 'text/csv; utf-8');
  drupal_add_http_header('Content-Disposition', 'attachment; filename=' . $csv_filename);

  $output = fopen("php://output", 'w');
  
  $headings = array(
    'User ID', 
    'Username', 
    'Email address',
    'Status',
    'Registration datetime', 
    'Registration path',
    'Social media registration',
    'Last login',
    'Last access', 
    'Subscribes to site emails', 
    'Interest survey submissions',
    'Number of comments by proposal', 
    'Number of endorsements by proposal',
  );

  //$profile = \SmartParticipation\ProfileFactory::build(0);
  $profile_service = new \SmartParticipation\ProfileService;
  $profiles_enabled = $profile_service->userProfilesAreActive();
  if ($profiles_enabled) {
    $profile = $profile_service->find(0);
    $profile_field_info = $profile_service->getProfileFieldInfo($profile);
    if (!empty($profile_field_info)) {
      foreach ($profile_field_info as $field_name => $field_info) {
        $field_heading = trim($field_info['label']);
        if (substr($field_heading, strlen($field_heading) - 1, 1) == ':') {
          $field_heading = substr($field_heading, 0, strlen($field_heading) - 1);
        }
        $headings[] = $field_heading;
      }
    }
  }

  fputcsv($output, $headings);

  $uids = _smartparticipation_core_basic_auth_user_uids();
  $date_format = 'm/d/y g:i:s a'; 
  
  foreach ($uids as $uid) {
  
    $user = user_load($uid);
    
    $created = date($date_format, $user->created);

    // Throw away database default value of '1969-12-31 19:00:00'
    $login = $user->login > mktime(0, 0, 0, 1, 1, 2009) ? date($date_format, $user->login) : '';
    
    // Throw away database default value of '1969-12-31 19:00:00'
    $access = $user->access > mktime(0, 0, 0, 1, 1, 2009) ? date($date_format, $user->access) : '';
    
    $registration_data = _smartparticipation_core_user_export_registration_data($uid); 
    
    $interest_survey_submissions = _smartparticipation_core_user_export_interest_survey_submissions($uid);
    
    $comment_counts = _smartparticipation_core_user_export_comment_count_by_proposal($uid);
    
    $endorsement_counts = _smartparticipation_core_user_export_endorsement_count_by_proposal($uid);

    if (!empty($login)) {

      $row = array(
        $uid,
        $user->name,
        $user->mail,
        $user->status,
        $created,
        $registration_data['path'],
        $registration_data['service'],
        $login,
        $access,
        smartparticipation_core_field_get_item_value('user', $user, 'field_sp_user_site_emails', 'value') == 1 ? 'yes' : 'no',
        $interest_survey_submissions,
        $comment_counts,
        $endorsement_counts,
      );

      if ($profiles_enabled) {
        //$profile = \SmartParticipation\ProfileFactory::build($uid);
        $profile = $profile_service->find($uid);
        foreach ($profile_field_info as $field_name => $field_info) {
          if (!empty($profile->$field_name)) {
            $value = $profile->$field_name;
            if ($field_info['display']['default']['module'] == 'list') {
              $list_options = list_allowed_values(field_info_field($field_name));
              $value = $list_options[$value];
            }
          }
          else {
            $value = '';
          }
          $row[] = $value;
        }
      }

      fputcsv($output, $row);

    }
  
  }
  
  fclose($output);
  
  exit;

}

/**
 * Return uids of all basic authenticated users on the site.
 * 
 * @return an array of uids
 */

function _smartparticipation_core_basic_auth_user_uids() {
  
  $query = 'SELECT uid FROM {users} u
           WHERE uid NOT IN 
           (SELECT uid from {users_roles})
           AND uid > 0
           ORDER BY created ASC';
            
  $uids = db_query($query)
    ->fetchCol();
  
  return $uids;  
}

/**
 * Return the url the specified user registered from, and the name of the
 * social media service used, if any.
 *
 * @param $uid - the user's uid
 *
 * @return an associative array of user registration data
 */
function _smartparticipation_core_user_export_registration_data($uid) {

  $query = 'SELECT path, detail AS \'service\'
            FROM {sp_event_log}
            WHERE name = \'user registration\'
            AND uid = ' . $uid;

  $result = db_query($query)->fetchAssoc();

  return !empty($result) ? $result : array('path' => '', 'service' => '');

}

/**
 * Return the names of all proposals for which the specified user has submitted
 * the interest survey.
 *
 * @param $uid - the user's uid
 *
 * @return a comma-delimited string of proposal titles
 */
function _smartparticipation_core_user_export_interest_survey_submissions($uid) {

  $query = 'SELECT np.title
            FROM {webform_submissions} w
            INNER JOIN {node} ns ON w.nid = ns.nid
            INNER JOIN {field_data_field_sp_interest_survey} s ON ns.nid = s.field_sp_interest_survey_nid
            INNER JOIN {node} np ON s.entity_id = np.nid
            WHERE w.uid = ' . $uid;

  $result = db_query($query)->fetchCol();
  
  return implode(', ', $result);

}

/**
 * Returns the names of all proposals which the specified user has commented on,
 * along with the number of comments made by the user on the proposal.
 *
 * @param $uid = the user's uid
 *
 * @return an array mapping proposal titles to number of comments
 */
function _smartparticipation_core_user_export_comment_count_by_proposal($uid) {

  $query = 'SELECT np.title, count(c.cid) 
            FROM {comment} c
            INNER JOIN {field_data_field_sp_subtopic_topic} st ON c.nid = st.entity_id
            INNER JOIN {field_data_field_sp_topic_proposal} tp ON st.field_sp_subtopic_topic_nid = tp.entity_id
            INNER JOIN {node} np ON tp.field_sp_topic_proposal_nid = np.nid
            WHERE c.uid = ' . $uid . '
            GROUP BY np.title';

  $result = db_query($query)->fetchAllKeyed();
  
  return smartparticipation_core_associative_array_to_string($result);

}

/**
 * Returns the names of all proposals which the specified user has commented on,
 * along with the number of comments made by the user on the proposal.
 *
 * @param $uid = the user's uid
 *
 * @return an array mapping proposal titles to number of comments
 */
function _smartparticipation_core_user_export_endorsement_count_by_proposal($uid) {

  $query = 'SELECT np.title, count(fc.fcid) 
            FROM {flag_content} fc
            INNER JOIN {flags} f ON fc.fid = f.fid
            INNER JOIN {comment} c ON c.cid = fc.content_id
            INNER JOIN {field_data_field_sp_subtopic_topic} st ON c.nid = st.entity_id
            INNER JOIN {field_data_field_sp_topic_proposal} tp ON st.field_sp_subtopic_topic_nid = tp.entity_id
            INNER JOIN {node} np ON tp.field_sp_topic_proposal_nid = np.nid
            WHERE fc.uid = ' . $uid . '
            AND f.name = \'comment_endorsement\'
            GROUP BY np.title';

  $result = db_query($query)->fetchAllKeyed();
  
  return smartparticipation_core_associative_array_to_string($result);

}

/**
 * Convert a one-dimensional associative array to a string for csv output. 
 */
function smartparticipation_core_associative_array_to_string($a_array) {
  
  $n_array = array();
  
  foreach ($a_array as $key => $value) {
    $n_array[] = $key . ': ' . $value;   
  }
  return implode('; ', $n_array);
  
}