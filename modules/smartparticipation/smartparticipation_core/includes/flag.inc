<?php
/** 
 * Smartparticipation_core module custom flags.
 */

/**
 * Create the custom flags
 */
function smartparticipation_core_create_flags() {
  $flag_recommendation = flag_flag::factory_by_content_type('node');

  // Exported flag: "Recommend"
  $flag_recommendation->name = 'comment_recommendation';
  $flag_recommendation->title = 'Recommend';
  $flag_recommendation->content_type = 'comment';
  $flag_recommendation->global = 1;

  // Properties required by the UI.
  $flag_recommendation->roles = array(
    'flag' => array(
      0 => '3',
    ),
    'unflag' => array(
      0 => '3',
    ),
  ); // An array of role IDs. 2 is for "authenticated users".

  $flag_recommendation->types = array('comment_node_sp_subtopic'); // An array of node types.
  $flag_recommendation->flag_short = 'Recommend';
  $flag_recommendation->unflag_short = 'Unrecommend';

  // Optional properties, defaults are defined for these (and more).
  // Use a print_r() or dsm() to see all the available flag properties.
  $flag_recommendation->global = 0;
  $flag_recommendation->flag_long = '';
  $flag_recommendation->flag_message = '';
  $flag_recommendation->show_on_form = FALSE;
  $flag_recommendation->show_on_node = FALSE;
  $flag_recommendation->show_on_teaser = FALSE;
  $flag_recommendation->show_on_comment = FALSE;
  $flag_recommendation->link_type = 'normal'; // For JS link. Other options: 'normal' and 'confirm'.
  $flag_recommendation->access_author = 'comment_others';
  $flag_recommendation->locked = array('name');
  
  // Save the flag.
  $flag_recommendation->save();


  // Exported flag: "Comment endorsement".
  $flag_endorsement = flag_flag::factory_by_content_type('node');
  
  $flag_endorsement->name = 'comment_endorsement';
  $flag_endorsement->title = 'Comment endorsement';
  $flag_endorsement->content_type = 'comment';
  $flag_endorsement->global = 0;

  // Properties required by the UI.
  $flag_endorsement->roles = array(
    'flag' => array(
      0 => '2',
      1 => '3',
    ),
    'unflag' => array(
      0 => '2',
      1 => '3',
    ),
  ); // An array of role IDs. 2 is for "authenticated users".

  $flag_endorsement->types = array('comment_node_sp_subtopic'); // An array of node types.
  $flag_endorsement->flag_short = 'Endorse';
  $flag_endorsement->unflag_short = 'Unendorse';

  // Optional properties, defaults are defined for these (and more).
  // Use a print_r() or dsm() to see all the available flag properties.
  $flag_endorsement->global = 0;
  $flag_endorsement->flag_long = '';
  $flag_endorsement->flag_message = '';
  $flag_endorsement->show_on_form = FALSE;
  $flag_endorsement->show_on_node = FALSE;
  $flag_endorsement->show_on_teaser = FALSE;
  $flag_endorsement->show_on_comment = FALSE;
  $flag_endorsement->link_type = 'normal'; // For JS link. Other options: 'normal' and 'confirm'.
  $flag_endorsement->access_author = 'comment_others';
  $flag_endorsement->locked = array('name');

  // Save the flag.
  $flag_endorsement->save();
}

/**
 * Implements hook_flag_javascript_info_alter().
 */
function smartparticipation_core_flag_javascript_info_alter(&$flag) {
  if ($flag['flagName'] == 'comment_endorsement') {
    if ($flag['flagStatus'] == 'flagged') {
      $flag['endorsements'] = _smartparticipation_core_comment_endorsement_count($flag['contentId']);
    }
  }
}

/**
 * Implements hook_flag().
 * 
 * @param $action - flag or unflag
 * @param $flag - the flag that is being set
 * @param $content_id - the cid of the comment being flagged
 * @param $account - the user setting the flag
 * 
 */
function smartparticipation_core_flag($action, $flag, $content_id, $account) {

  $comment = comment_load($content_id, $reset = FALSE);
  $uid = $comment->uid;

  switch ($flag->name) {
    
    case 'comment_endorsement':

      if ($action == 'flag') {     
        $action_text = 'endorsed';
        smartparticipation_core_comment_flagged_email_send($comment, $flag);      
      } else {
        $action_text = 'unendorsed';
      }
    
      /* We need this to track unendorse actions. The flag module simply deletes
       * the flag from the flag_content table.
       */
      $fcid = db_insert('sp_flag_content_history')
        ->fields(array(
          'fid' => $flag->fid,
          'content_type' => 'comment',
          'content_id' => $content_id,
          'uid' => $account->uid,
          'action' => $action_text, 
          'timestamp' => REQUEST_TIME,
        ))
        ->execute();
      
      return $fcid;

    /* TODO The recommend action never gets here. The whole flag module has been
     * sidestepped. Rewrite.
    case 'comment_recommendation':

      if ($action == 'flag') {
        smartparticipation_core_comment_flagged_email_send($comment, $flag);     
      }
      
      break;
    */
  }
  
}

/**
 * Send user an email notification when his/her comment has been endorsed or 
 * recommended.
 * 
 * @param $comment - the comment
 * @param $flag - the flag
 */
function smartparticipation_core_comment_flagged_email_send($comment, $flag) {

  switch ($flag->name) {
    
    case 'comment_endorsement':
      $variable_prefix = 'smartparticipation_mail_comment_endorsement';
      break;
      
    case 'comment_recommendation':
      $variable_prefix = 'smartparticipation_mail_comment_recommendation';
      break;
    
    default:
      return;
  }

  $comment_author = user_load($comment->uid);
  
  // Don't send email is user has opted out of comment response notifications.
  if (!smartparticipation_core_user_accepts_comment_emails($comment_author)) {
    return;
  }
  
  $message_body = variable_get($variable_prefix . '_body');

  if ($message_body) {
    
    $to = $comment_author->mail;
    
    $message_subject = variable_get($variable_prefix . '_subject');
    
    _smartparticipation_core_drupal_mail('default_from', $to, $message_subject, $message_body, 
        array('comment' => $comment));
  }
  
}

/**
 * Remove the custom flags
 */
function smartparticipation_core_delete_flags() {
  $string = 'comment_%';
  $query = 'SELECT f.fid, f.name FROM {flags} f WHERE f.name LIKE :string';
  $result = db_query($query,array(':string' => $string));
  //$count = $result->rowCount();
  
  while ($record = $result->fetchObject()) {
    // delete from 'flag_types' table
    $flag_types_deleted = db_delete('flag_types')
      ->condition('fid', $record->fid)
      ->execute();

    // delete from 'flag_counts' table
    $flag_types_deleted = db_delete('flag_counts')
      ->condition('fid', $record->fid)
      ->execute();

    // delete from 'flag_content' table
    $flag_types_deleted = db_delete('flag_content')
      ->condition('fid', $record->fid)
      ->execute();

    // delete from 'flags' table
    $flag_types_deleted = db_delete('flags')
      ->condition('fid', $record->fid)
      ->execute();
  }
}