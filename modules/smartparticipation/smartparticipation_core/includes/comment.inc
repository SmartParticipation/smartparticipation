<?php

/**
 * Custom smartparticipation_core comment functionality.
 */


/**
 * Module function to add custom fields to comments entity.
 */
function smartparticipation_core_create_custom_comment_fields() {

  // Create the custom comments fields.
  foreach (smartparticipation_core_comment_installed_fields() as $field) {
    $field['entity_types'] = array('comment');
    field_create_field($field);
  }

  // Create all the instances for our fields.
  foreach (smartparticipation_core_comment_installed_instances() as $instance) {
    $instance['entity_type'] = 'comment';
    $instance['bundle'] = 'comment_node_sp_subtopic';
    field_create_instance($instance);
  }  
}


/**
 * Module function to define custom comments fields.
 */
function smartparticipation_core_comment_installed_fields() {
  $t = get_t();
  
  return array(
  
    'field_sp_comment_mod_status' => array(
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'field_name' => 'field_sp_comment_mod_status',
      'foreign keys' => array(),
      'indexes' => array(
        'value' => array(
          0 => 'value',
        ),
      ),
      'locked' => '0',
      'module' => 'list',
      'settings' => array(
        'allowed_values' => array(
          'unread' => $t('unread'),
          'in_progress' => $t('in progress'),
          'done' => $t('done'),
        ),
        'allowed_values_function' => '',
      ),
      'translatable' => '0',
      'type' => 'list_text',
      'translatable' => '0',
    ),

    /*
     * Relevant only to comments with field_sp_comment_mod_status == 'done'.
     * Refers to the final action taken on a comment. Other comments should not
     * have a value for this field.
     */
    'field_sp_comment_mod_action' => array(
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'field_name' => 'field_sp_comment_mod_action',
      'foreign keys' => array(),
      'indexes' => array(
        'value' => array(
          0 => 'value',
        ),
      ),
      'locked' => '0',
      'module' => 'list',
      'settings' => array(
        'allowed_values' => array(
          'reply' => $t('reply'),
          'no_reply' => $t('no reply'),
          'quarantine' => $t('quarantine'),
        ),
        'allowed_values_function' => '',
      ),
      'translatable' => '0',
      'type' => 'list_text',
      'translatable' => '0',
    ),
    
    'field_sp_comment_original' => array(
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'field_name' => 'field_sp_comment_original',
      'foreign keys' => array(
        'format' => array(
          'columns' => array(
            'format' => 'format',
          ),
          'table' => 'filter_format',
        ),
      ),
      'indexes' => array(
        'format' => array(
          0 => 'format',
        ),
      ),
      'locked' => '0',
      'module' => 'text',
      'settings' => array(),
      'translatable' => '0',
      'type' => 'text_long',
    ),

    'field_sp_comment_type' => array(
      'active' => '1',
      'cardinality' => '-1',
      'deleted' => '0',
      'field_name' => 'field_sp_comment_type',
      'foreign keys' => array(),
      'indexes' => array(
          'value' => array(
            0 => 'value',
          ),
      ),
      'locked' => '0',
      'module' => 'list',
      'settings' => array(
        'allowed_values' => array(
          'moderator' => $t('moderator'),
        ),
        'allowed_values_function' => '',
      ),
      'translatable' => '0',
      'type' => 'list_text',
      'translatable' => '0',
    ),
      
  );
}

/**
 * Module function to define custom comments field instances.
 */
function smartparticipation_core_comment_installed_instances() {
  $t = get_t();
  
  return array(
  
    'field_sp_comment_mod_status' => array(
      'default_value' => null,
      'deleted' => '0',
      'description' => '',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'list',
          'settings' => array(),
          'type' => 'list_default',
          'weight' => 1,
        ),
      ),
      'field_name' => 'field_sp_comment_mod_status',
      'label' => $t('Moderation Status'),
      'required' => 0,
      'settings' => array(
        'user_register_form' => 0,
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'options',
        'settings' => array(),
        'type' => 'options_select',
        'weight' => '2',
      ),
    ),
        
    'field_sp_comment_mod_action' => array(
      'default_value' => NULL,
      'deleted' => '0',
      'description' => '',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'list',
          'settings' => array(),
          'type' => 'list_default',
          'weight' => 1,
        ),
      ),
      'field_name' => 'field_sp_comment_mod_action',
      'label' => $t('Moderation Action'),
      'required' => 0,
      'settings' => array(
        'user_register_form' => 0,
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'options',
        'settings' => array(),
        'type' => 'options_select',
        'weight' => '4',
      ),
    ),
       
    'field_sp_comment_original' => array(
      'default_value' => null,
      'deleted' => '0',
      'description' => '',   
      'display' => array(
        'default' => array(
          'label' => 'above',
          'type' => 'text_default',
          'settings' => array(),
          'module' => 'text',
          'weight' => 40,
        ),
        'teaser' => array(
          'type' => 'hidden',
          'label' => 'above',
          'settings' => array(),
          'weight' => 0,
        ),
      ),
      'field_name' => 'field_sp_comment_original',
      'format' => 'filtered_html',
      'label' => $t('Original comment'),
      'required' => 0,
      'settings' => array(
        'text_processing' => '1', 
        'user_register_form' => FALSE,
      ),
      'widget' => array(
        'weight' => '40',
        'type' => 'text_textarea',
        'active' => 0,
        'settings' => array(),
      ),
    ),

    'field_sp_comment_type' => array(
      'default_value' => NULL,
      'deleted' => '0',
      'description' => '',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'list',
          'settings' => array(),
          'type' => 'list_default',
          'weight' => 1,
        ),
      ),
      'field_name' => 'field_sp_comment_type',
      'required' => 0,
      'settings' => array(
        'user_register_form' => 0,
      ),
    ),
      
  );  
}

/**
 * Module function to delete custom comments fields on uninstall.
 * 
 */
function smartparticipation_core_delete_custom_comment_fields() {
  foreach (array_keys(smartparticipation_core_comment_installed_fields()) as $field_name) {
    field_delete_field($field_name);
  }
}

/**
 * Is the given comment being redacted/edited?
 *
 * @param $comment
 * @return bool
 */
function smartparticipation_core_is_comment_edit($comment) {
  return (
    !empty($comment->cid)
    && !empty($comment->hnid)
    && $comment->hnid == 'edit'
  ) ? TRUE : FALSE;
}

/**
 * Implements hook_comment_presave()
 * 
 * @param $comment
 *   The comment being saved
 */
function smartparticipation_core_comment_presave($comment) {

  $user_is_moderator = smartparticipation_core_user_is_moderator();

  // New comment
  if (!$comment->cid) {    
    
    // Mark for later identification as a moderator comment (can't rely on 
    // user role, since that could change). 
    if ($user_is_moderator) {
      $comment->field_sp_comment_type[LANGUAGE_NONE][0]['value'] = 'moderator';
    // Set moderation status of basic user comment
    } else {
      $comment->field_sp_comment_mod_status[LANGUAGE_NONE][0]['value'] = 'unread';
    }

  } else {

    if ($user_is_moderator) {

      // If redacting a user comment, store the original
      // and update the status to 'in_progress'.
      if (smartparticipation_core_is_comment_edit($comment)) {
        if (!smartparticipation_core_is_moderator_comment($comment)) {

          // Store the original comment if it hasn't already been stored.
          if (smartparticipation_core_field_get_item_value('comment', $comment, 'field_sp_comment_original', 'value') == NULL) {
            $comment_before_save = comment_load($comment->cid);
            $comment->field_sp_comment_original[LANGUAGE_NONE][0]['value'] = $comment_before_save->comment_body[LANGUAGE_NONE][0]['value'];
            $comment->field_sp_comment_original[LANGUAGE_NONE][0]['format'] = 'comments';
          }

          smartparticipation_core_promote_comment_to_in_progress($comment);
        }
      }
    }

  }

  // Links should open in new window
  $body = $comment->comment_body[LANGUAGE_NONE][0]['value'];  
  $body = preg_replace('/<a\s+(?!target="_blank")/', '<a target="_blank" ', $body); 
  $comment->comment_body[LANGUAGE_NONE][0]['value'] = $body;

}


/**
 * Implements hook_comment_insert()
 */
function smartparticipation_core_comment_insert($comment) {
  
  
  smartparticipation_core_first_comment_mail_send($comment);

  // the comment is a reply
  if ($comment->pid > 0) {

    // Notify the user of the parent comment of the reply
    smartparticipation_core_comment_reply_mail_send($comment);

    // If a moderator replies, set the mod status of the parent to done.
    if (smartparticipation_core_user_is_moderator()) {

      $parent = comment_load($comment->pid);

      // Revert Quarantine
      $parent->status = 1;

      $parent->field_sp_comment_mod_status[LANGUAGE_NONE][0]['value'] = 'done';
      $parent->field_sp_comment_mod_action[LANGUAGE_NONE][0]['value'] = 'reply';

      comment_save($parent);
    }
  }
}

/**
 * Send user an email after submitting his/her first comment on a proposal.
 *
 * @param $comment - the comment that has just been submitted
 */
function smartparticipation_core_first_comment_mail_send($comment) {
  
  $comment_author = user_load($comment->uid);
  
  // Moderators shouldn't get the first comment email
  if (!smartparticipation_core_is_basic_authenticated_user($comment_author)) {
    return;
  }
  
  // Send email only if user has opted in to comment emails.
  if (!smartparticipation_core_user_accepts_comment_emails($comment_author)) {
    return;
  }
  
  $message_body = variable_get('smartparticipation_mail_first_comment_body');
  
  if (!$message_body) {
    return;
  }

  // TODO We can do away with most of this, now that we're using available
  // Drupal tokens.
  $comment_details = smartparticipation_core_comment_data_for_email($comment->cid);

  $proposal_nid = $comment_details['comment_proposal_nid'];

  $comments_number_user = _smartparticipation_core_user_comments_on_proposal($comment->uid, $proposal_nid);

  if ($comments_number_user != 1) {
    return;
  }
  
  $to = $comment_author->mail;
  
  $message_subject = variable_get('smartparticipation_mail_first_comment_subject');

  _smartparticipation_core_drupal_mail('default_from', $to, $message_subject, $message_body, 
      array('comment' => $comment));  
   
}


/**
 * Email user when a comment he wrote is replied to. Called from smartparticipation_core_
 * comment_insert when the new comment is submitted. It is already known that 
 * that is a reply.
 *
 * @param $reply - the reply
 */
function smartparticipation_core_comment_reply_mail_send($reply) {

  $message_body = variable_get('smartparticipation_mail_comment_reply_body');

  if ($message_body) {

    // Get the email recipient address (author of the reply's parent)
    $parent = comment_load($reply->pid);
    $parent_author = user_load($parent->uid);
    
    // Send email only is user accepts comment response notifications.
    if (smartparticipation_core_user_accepts_comment_emails($parent_author)) {
    
      $to = $parent_author->mail;
  
      $message_subject = variable_get('smartparticipation_mail_comment_reply_subject');
      
      // Manually replace reply author name if it's a moderator reply.
      $message_body = smartparticipation_core_token_replace_comment_author_moderator_name($message_body, $reply);
      
      _smartparticipation_core_drupal_mail('default_from', $to, $message_subject, $message_body, 
          array('comment' => $reply));
      
    } 
  }
}


/**
 * Implements hook_form_comment_form_alter().
 */
function smartparticipation_core_form_comment_form_alter(&$form, &$form_state, $form_id) {

  $is_edit_form = !empty($form['cid']['#value']);
  $is_comment_form = !$is_edit_form && empty($form['pid']['#value']);
  $is_reply_form = !$is_comment_form; // && !$is_edit_form;

  $author_name = '';

  if ($is_edit_form) {
    $author_name = $form['author']['name']['#default_value'];
    $form['author']['#access'] = FALSE;
    $form['field_sp_comment_original']['#access'] = FALSE;
    //unset($form['author']);
    //unset($form['field_sp_comment_original']);
  }

  // Hide the subject field
  // Redundant since we now set comment_subject_field_smartparticipation_core_subtopic to 0,
  // but keep this in case it gets switched on by mistake. That value just 
  // sets #access to false anyway.
  $form['subject']['#access'] = FALSE; 
  
  // Hide custom comment fields
  $form['field_sp_comment_mod_status']['#access'] = FALSE;
  $form['field_sp_comment_mod_action']['#access'] = FALSE;
  $form['field_sp_comment_type']['#access'] = FALSE;
  
  $form['#submit'][] = '_smartparticipation_core_comment_form_submit';
  $form['hnid'] = array(
    '#type' => 'hidden',
    '#default_value' => arg(1),
  );

  $form['author']['_author'] = NULL;
  if (!user_is_logged_in()) {
    $form['author']['name'] = NULL;
  }

  $form['comment_body'][LANGUAGE_NONE][0]['#resizable'] = FALSE;

  // Customize the label and button text for the comment form
  if ($is_comment_form) {
    $label = t('Make a comment.');
    $button_text = t('Comment');
  } else if ($is_edit_form) {
    $label = t('Edit comment by @author.', array('@author' => $author_name));
    $button_text = t('Save');
    $form['#attributes']['class'][] = 'comment-form-edit';
  } else if ($is_reply_form) {
    $label = t('Make a reply.');
    $button_text = t('Reply');
    $form['#attributes']['class'][] = 'comment-form-reply';
  }

  // The placeholder text will be picked up by the CKEditor 'confighelper' plugin.
  $form['comment_body'][LANGUAGE_NONE][0]['#attributes']['placeholder'] = $label;

  $form['comment_body'][LANGUAGE_NONE][0]['#title'] = $label;
  $form['comment_body'][LANGUAGE_NONE][0]['#title'] = '';
  // Use the custom Comments text format for the comment body to enable the WYSIWYG.
  $form['comment_body'][LANGUAGE_NONE][0]['#format'] = 'comments';
  // Custom add link has been replaced by CKEditor add link tool.
  //$form['actions']['#prefix'] = theme('add_link', array('commentFormId' => $form['#id']));
  $form['actions']['submit']['#value'] = $button_text;
  $form['actions']['preview'] = null;
  $form['actions']['cancel'] = array(
    '#type' => 'button',
    '#value' => 'Cancel',
    '#weight' => 20,
    '#attributes' => array(
      'class'=> array(
        'cancel-comment'
      ),
    ),
  );

  // Set autocomplete on all form inputs to 'off'.
  // This prevents an issue where Firefox will maintain disabled input state after page refresh.
  $form['comment_body'][LANGUAGE_NONE][0]['#attributes']['autocomplete'] = 'off';
  $form['actions']['submit']['#attributes']['autocomplete'] = 'off';
  $form['actions']['cancel']['#attributes']['autocomplete'] = 'off';
  $form['form_build_id']['#attributes']['autocomplete'] = 'off';
  $form['form_token']['#attributes']['autocomplete'] = 'off';
  $form['form_id']['#attributes']['autocomplete'] = 'off';
  $form['hnid']['#attributes']['autocomplete'] = 'off';

  $form['comment_body'][LANGUAGE_NONE][0]['#maxlength'] = variable_get('smartparticipation_comment_length_limit',10000);

  $form['#validate'][] = '_smartparticipation_core_comment_form_validate';

}

/**
 * Custom comment form validation handler
 */
function _smartparticipation_core_comment_form_validate($form, &$form_state) {

  $errors = form_get_errors();

  // Customize the validation error message for the comment textarea.
  if ($errors) {
    form_clear_error();

    foreach ($errors as $field => $error_message) {
      //if ($field == 'comment_body][und][0][value') {
      if (preg_match('/field is required./', $error_message)) {
        $error_message = t('The comment field is required.');
      }
      if (preg_match('/comment_body cannot be longer than (\d+)/', strip_tags($error_message), $matches)) {
        $error_message = t('The comment field can not be longer than ' . $matches[1] . ' characters.');
      }
      form_set_error($field, $error_message);
    }
  }

  // Clear errors from message stack.
  drupal_get_messages('error', TRUE);

}


/**
 * Submit handler for smartparticipation_core comment form.
 */
function _smartparticipation_core_comment_form_submit($form, &$form_state) {
  
  $topic_nid = $form_state['values']['hnid'];
  $form_state['redirect'] = "node/" . $topic_nid; 

}

/**
 * Implements hook_preprocess_comment().
 *
 */
function smartparticipation_core_preprocess_comment(&$variables) {

  $variables['is_moderator'] = $is_moderator = smartparticipation_core_moderator_access();

  smartparticipation_core_comment_links($variables, $is_moderator);
  
  // These values are displayed only to moderators, but they've been 
  // incorporated into the comment links and should not display as field values.
  hide($variables['content']['field_sp_comment_mod_action']);
  hide($variables['content']['field_sp_comment_mod_status']);

  // These fields are never displayed
  hide($variables['content']['field_sp_comment_type']);
  hide($variables['content']['field_sp_comment_original']);
    
  $cid = $variables['comment']->cid;

  $uid = $variables['user']->uid;

  $variables['comment_recommended'] = _smartparticipation_core_comment_recommended($cid);

  // Set comment author
  $variables['author'] = smartparticipation_core_comment_author_name($variables['comment']);

  $variables['author_thumbnail'] = theme('user_thumbnail', array('uid' => $variables['comment']->uid, 'username' => strip_tags($variables['author'])));
  
  $variables['comment_depth'] = $variables['comment']->depth;

  $endorsement_service = new \SmartParticipation\EndorsementService;
  if ($endorsement_service->canViewCommentEndorsementCounter($variables['comment'])) {
    $endorsements = $endorsement_service->findByCommentId($cid);
    if (count($endorsements)) {
      $variables['endorsement_count'] = $endorsement_service->getEndorsementCounterMarkup($endorsements);
      if ($endorsement_service->canViewCommentEndorsementUsers()) {
        $variables['endorsements'] = $endorsements;
      }
    }
  }
  
  // Not currently printing permalink in front end comment display.
  // $variables['comment_permalink'] = smartparticipation_core_get_comment_permalink($cid, $variables['node'], 'Permalink');

  if (smartparticipation_core_is_moderator_comment($variables['comment'])) {
    $variables['classes_array'][] = 'moderator-comment';
  }
  
  smartparticipation_core_comment_report_quarantine($variables);

}


/**
 * Determine if the user has endorsed the comment.
 *
 * @param $cid
 * @param $account (optional) - the user, or the global $user if null
 * @return boolean - has the user endorsed the comment?
 */
function _smartparticipation_core_comment_endorsed_by_user($cid, $account = null) {
  
  global $user;
  
  if (is_null($account)) {
    $account = $user;
  }
  
  $uid = $account->uid;

  $query = 'SELECT count(uid) AS has_endorsed
            FROM {flag_content} fc
            LEFT JOIN {flags} AS f ON f.fid = fc.fid
            WHERE f.name = :endorsements
            AND fc.uid = :uid
            AND fc.content_id = :cid';

  $result = db_query($query, array(
    ':endorsements' => 'comment_endorsement', 
    ':uid' => $uid, 
    ':cid' => $cid
  ));

  $record = $result->fetchAssoc();

  return $record['has_endorsed'];
}

/**
 * Return the comment author display: 
 *   - Non-moderators: username with a link to the profile.
 *   - Moderators: "Moderator", with no link. 
 *   
 *   TODO If current user is also a moderator, then display actual username as 
 *   a title.
 *   
 * @param $comment - the comment object
 */
function smartparticipation_core_comment_author_name($comment) {
  
  // Get uid of comment author
  $author_uid = $comment->uid;

  if ($author_uid) {
    
    if (smartparticipation_core_is_moderator_comment($comment)) {     
      $author = t('Moderator');
              
    } else {
      $author = l(
        /* 
         * NB This is NOT the name stored in the comment.name column, which 
         * stores the username when the comment was made and doesn't update if
         * the username changes, and would therefore be incorrect to use in the
         * comment display, but rather the value retrieved from the user.name 
         * field.
         */
        $comment->name,
        smartparticipation_core_path_to_user_profile($author_uid),
        array(
          'attributes' => array(
            'title' => t('View user profile'),
            'class' => 'username',
          ),
        )
      );      
    }
    
  } else {    
    // We don't currently allow comments from anonymous users, so this case
    // won't arise.
    $author = '';    
  }

  return $author;
}

/**
 * Return true if and only if comment author was a moderator. Note that we do
 * not check the current role of the comment author, because that could change.
 * Instead, at the time the comment is made, we store it with a 'moderator'
 * type field value.
 * 
 * @param $comment - the comment 
 */
function smartparticipation_core_is_moderator_comment($comment) {  
  
  $types = smartparticipation_core_field_get_items('comment', $comment, 'field_sp_comment_type');

  foreach ($types as $type) {
    if ($type['value'] == 'moderator') {
      return true;
    }
  }

  return false;
}


/**
 * Return the comment permalink (either the href attribute of the link element
 * or the full link element) for the specified comment cid and subtopic node.
 * 
 * @param $cid - the comment cid
 * @param $node_subtopic - the subtopic node the comment is attached to
 * @param $link_text (optional) - the link text, if a full link element is desired
 * @param $attributes (optional) - an array of attributes passed to l()
 * 
 * @return the comment permalink (href or full link element) 
 *
 */
function smartparticipation_core_get_comment_permalink($cid, $node_subtopic, $link_text = null, $attributes = array()) {
  $nid_topic = smartparticipation_core_field_get_item_value('node', $node_subtopic, 'field_sp_subtopic_topic', 'nid');
  return smartparticipation_core_comment_permalink($cid, $nid_topic, $link_text, $attributes);
}

/**
 * Return the comment permalink (either the href attribute of the link element
 * or the full link element) for the specified comment cid.
 * 
 * @param $cid - the comment cid
 * @param $link_text (optional) - the link text if a full link element is to be returned. 
 *   Defaults to null.
 * @param $attributes (optional) - an array of attributes passed to l().
 * 
 * @return - the comment permalink (href or full link element)
 */
function smartparticipation_core_get_comment_permalink_from_cid($cid, $link_text = NULL, $attributes = array()) {
  
  if (($comment = comment_load($cid)) && ($node_subtopic = node_load($comment->nid))) {
    $nid_topic = smartparticipation_core_field_get_item_value('node', $node_subtopic, 'field_sp_subtopic_topic', 'nid');
    $node = node_load($nid_topic);  
    return smartparticipation_core_comment_permalink($cid, $nid_topic, $link_text, $attributes);
  }
  return false;
}

/**
 * Return the comment permalink (either the href attribute of the link element
 * or the full link element) for a comment. The permalink points to the comment 
 * display on the comment's subtopic's topic page. JavaScript is used to open 
 * the subtopic and scroll to the comment when the comment permalink is 
 * requested.
 * 
 * NB The hash tag cannot be appended to the url, because Drupal will url-encode
 * it. It must be sent as a fragment in the attributes array.
 * 
 * @param $cid - the comment cid
 * @param $topic_nid - the node id of the topic that the comment's subtopic node 
 *   belongs to
 * @param $link_text - the link text if a full link element is to be returned. 
 *   Defaults to null.
 * @param $attributes - an optional array of attributes passed to l().
 * 
 * @return the comment permalink (href value or full link element)
 */
function smartparticipation_core_comment_permalink($cid, $topic_nid, $link_text = null, $attributes = array()) {
  
  $path = 'node/' . $topic_nid;
  
  $options = array(
    'fragment' => 'cid-' . $cid, 
    'absolute' => TRUE,
  );
  
  if ($link_text && !empty($attributes)) {
    if (empty($attributes))
    $options['attributes'] = $attributes;
  }
  
  return $link_text ? l(t($link_text), $path, $options) : url($path, $options);

}

/**
 * Generate the appropriate comment links for moderators and non-moderators.
 */
function smartparticipation_core_comment_links(&$variables, $is_moderator) {

  $nid_subtopic = $variables['node']->nid;
  
  // TODO We've already figured this out for the topic: see smartparticipation_core_topic_node_view().
  // Is there some way to use the value we've already computed?
  //$subtopic_allows_commenting = _smartparticipation_core_subtopic_allows_commenting($nid_subtopic);  
  
  $subtopic_allows_commenting = $variables['node']->comment;
    
  // Get moderator or non-moderator comment links.
  if ($is_moderator) {
    smartparticipation_core_moderator_comment_links($variables, $subtopic_allows_commenting);
  } else {
    smartparticipation_core_non_moderator_comment_links($variables, $subtopic_allows_commenting);
  }

}

/**
 * Generate comment links for non-moderators.
 * 
 */
function smartparticipation_core_non_moderator_comment_links(&$variables, $subtopic_allows_commenting) {
  
  global $user;
  
  // No links on quarantined comments
  if (smartparticipation_core_comment_quarantined($variables['comment'])) {
    
    unset($variables['content']['links']);
  
  } else {
    // Subtopic not open for commenting
    if (!$subtopic_allows_commenting) {
      
      // Display only share link when subtopic is closed for commenting
      $variables['content']['links']['comment']['#links'] = array();
      smartparticipation_core_comment_share_link($variables);
      /*$variables['content']['links']['comment']['#links']['endorsement-counter'] =
        smartparticipation_core_comment_endorsement_counter_link($variables['comment']);*/

      // Subtopic open for commenting
    } else {
    
      // Anonymous user
      if ($user->uid == 0) {
        
        unset($variables['content']['links']['comment']['#links']['comment_forbidden']);
        
      // Authenticated user
      } else {
    
        // Remove delete and edit functions
        unset($variables['content']['links']['comment']['#links']['comment-delete']);
        unset($variables['content']['links']['comment']['#links']['comment-edit']);
      }
    
      smartparticipation_core_comment_share_link($variables);
      smartparticipation_core_comment_endorse_link($variables);
      smartparticipation_core_comment_reply_link($variables, false);
      
    }
  
    unset($variables['content']['links']['flag']);
  
  }

}

function smartparticipation_core_comment_reply_link(&$variables, $is_moderator) {

  $show_reply_link = true;
  
  $max_thread_depth = variable_get('smartparticipation_default_comment_max_thread_depth');
  
  if (!empty($max_thread_depth)) {
  
    // Allow moderators to reply at one level deeper (moderator always has the
    // last word).
    if ($is_moderator) {
      $max_thread_depth++;
    }
  
    // Remove the reply link if the max depth for a thread has been reached
    if ($variables['comment']->depth >= $max_thread_depth - 1) {
      $show_reply_link = false;
    }
  }
  
  if (isset($variables['content']['links']['comment']['#links']['comment-reply'])) {

    $comment_reply = $variables['content']['links']['comment']['#links']['comment-reply'];
  
    // Unset the reply link now, whether it should display or not. If it will
    // display, it needs to be removed from #links to be repositioned at the 
    // end of the list.
    unset($variables['content']['links']['comment']['#links']['comment-reply']);  
  }
  
  if (!$show_reply_link) {
    return;
  }

  // A reply link has not already been generated for anonymous users, and it has
  // been removed for users who must still submit the interest survey. Create a
  // non-functioning one for display.
  if (!isset($comment_reply)) {
    $comment_reply = array();
    $comment_reply['href'] = '#';
    $comment_reply['html'] = TRUE;
  }
  
  $comment_reply['title'] = t('Reply');
  $variables['content']['links']['comment']['#links']['comment-reply'] = $comment_reply;
  

}

function smartparticipation_core_comment_share_link(&$variables) {

  if (smartparticipation_core_is_moderator_comment($variables['comment'])) {
    return;
  }
  
  $pubid = variable_get('smartparticipation_addthis_pubid');
  
  if ($pubid) {
    $cid = $variables['comment']->cid;
  
    $comment_share = array();
    
    $comment_share['title'] = '<span>' . t('Share') . '</span>';
    $comment_share['href'] = 'http://www.addthis.com/bookmark.php';
    $comment_share['html'] = TRUE;
    $comment_share['query'] = array('v' => '300', 'pubid' => $pubid);
    
    $comment_share['attributes'] = array(
      'addthis:url' => smartparticipation_core_get_comment_permalink_from_cid($cid),
      'class' => 'addthis_button_expanded',
      'title' => t('Share this comment'),
        
      // For event logging
      'data-rr-event' => 'click',
      'data-rr-event_name' => 'share comment',
      'data-rr-event_entity-type' => 'comment',
      'data-rr-event_entity-id' => $cid,
    );

    $variables['content']['links']['comment']['#links']['comment-share'] = $comment_share;
  }
}

/**
 * Add the comment endorse link to comment variables.
 */
function smartparticipation_core_comment_endorse_link(&$variables) {
  
  $comment = $variables['comment'];
  
  if (smartparticipation_core_is_moderator_comment($comment)) {
    return;
  }
  
  $cid = $comment->cid;
  
  /*$variables['content']['links']['comment']['#links']['endorsement-counter'] =
      smartparticipation_core_comment_endorsement_counter_link($comment);*/

  // No endorse link on a user's own comment
  if ($comment->uid == $variables['user']->uid) {
    return;
  }  
  
  $flag = flag_get_flag('comment_endorsement');

  if ($flag->is_flagged($cid)) {
    $link_title = $flag->unflag_short;
    $path = 'flag/unflag/comment_endorsement/' . $cid;
  } else {
    $link_title = $flag->flag_short;
    $path = 'flag/flag/comment_endorsement/' . $cid;
  }
  
  $comment_endorse = array();
  $comment_endorse['title'] = $link_title;
  $comment_endorse['href'] = $path;
  $comment_endorse['html'] = TRUE;
  $comment_endorse['query'] = array('destination' => 'node/'.arg(1), 'token' => flag_get_token($cid));
  $comment_endorse['attributes'] = array(
    'title' => t('Endorse a comment that does a good job of making a good point.'),  
  );
  $variables['content']['links']['comment']['#links']['comment-endorse'] = $comment_endorse;

}

/**
 * Create the display of endorsement counts for the specified comment. Note that
 * "link" is a misnomer here, since it is not a link, but it is displayed with
 * the comment links on the topic page comment display.
 * 
 * @param $comment - the comment object
 * @param $show - calling functions can send true to bypass the call to
 * smartparticipation_core_show_comment_endorsements() if they already know the endorsements
 * should be displayed. If null, the function will be called to make the 
 * determination.
 * 
 * @return an array containing endorsement count for display with comment links.
 */
function smartparticipation_core_comment_endorsement_counter_link($comment, $show = null) {
  
  $endorsement_counter = array();

  if ($show || smartparticipation_core_show_comment_endorsements($comment)) {
    
    $endorsement_count = _smartparticipation_core_comment_endorsement_count($comment->cid);
    
    if ($endorsement_count > 0) {
      $endorsement_counter['title'] = smartparticipation_core_comment_endorsement_counter_markup($endorsement_count);
      $endorsement_counter['html'] = TRUE;      
      
    } else {
      $endorsement_counter['attributes'] = array('class' => 'hide');
    }
    
  } else {
    $endorsement_counter['attributes'] = array('class' => 'hide');
  }
  
  return $endorsement_counter;
}

/**
 * Return markup used to display comment endorsement counter on topic and user
 * pages. 
 * 
 * @param $count - endorsement count
 * 
 * @return string with appropriate singular/plural, and span element wrapping
 * the digits, wrapped in t().
 */
function smartparticipation_core_comment_endorsement_counter_markup($count) {
  // TODO What happens to html tags wrapped in t()? Otherwise we have to 
  // parse the string returned by t() and wrap the digits in span tags. 
  return t('<span>' . $count . '</span> endorsement' . ($count > 1 ? 's' : ''));
}

/**
 * Return comment data. // full details
 * 
 */
function smartparticipation_core_comment_data($cid) {
  
  $comment = comment_load($cid);

  $comment_uid = $comment->uid;  
  $comment_author = user_load($comment_uid);

  $comment_hostname = $comment->hostname;

  $comment_author_picture = theme('user_picture', array('account' => $comment_author));
    
  // TODO - should be a link for templates, but not for export
  // Create new function for comment data export.
  // OR: use same function, generate both link and plain username, and use as
  // needed in template/export.
  // $comment_author_link = l();
  $comment_username = $comment_author->name;

  $comment_mail = $comment_author->mail;
  
  $comment_body = smartparticipation_core_field_get_item('comment', $comment, 'comment_body');
  
  $comment_pid = $comment->pid;
  
  // Subtopic details
  $node_subtopic = node_load($comment->nid);
  $comment_subtopic_title = $node_subtopic->title;
  
  // Topic details
  $topic_nid = smartparticipation_core_field_get_item_value('node', $node_subtopic, 'field_sp_subtopic_topic', 'nid');
  $node_topic = node_load($topic_nid);
  $comment_topic_title = $node_topic->title;
  
  // Proposal details
  $proposal_nid = smartparticipation_core_field_get_item_value('node', $node_topic, 'field_sp_topic_proposal', 'nid');
  $node_proposal = node_load($proposal_nid);
  $comment_proposal = $node_proposal->title;
  $comment_proposal_nid = $node_proposal->nid;

  // Original of a redacted comment
  $original = smartparticipation_core_field_get_item('comment', $comment, 'field_sp_comment_original');
  $original_body = smartparticipation_core_field_get_item_value('comment', $comment, 'field_sp_comment_original', 'value');

  // Moderator notes on the comment
  $notes = array();
  
  $query = 'SELECT f.entity_id, f.body_value, n.created FROM {node} AS n 
    LEFT OUTER JOIN {field_data_body} AS f ON n.nid = f.entity_id 
    LEFT OUTER JOIN {field_data_field_sp_comment_note_comment} AS c ON c.entity_id = f.entity_id 
    WHERE  c.field_sp_comment_note_comment_target_id = :cid';
  
  $result = db_query($query, array(':cid' => $cid));
  
  foreach ($result as $r) {
    $r->created = date('M jS, Y g:ia', $r->created);
    $notes[] = $r;
  }

  $created_display_date = format_date($comment->created, 'medium');
  
  // Displayed in moderator interface
  $permalink = smartparticipation_core_get_comment_permalink_from_cid($cid, 'Permalink', array( 
    'target' => '_blank',
    'title' => t('View this comment in the discussion thread'),
  ));
  
  $comment_no_endorsements = _smartparticipation_core_comment_endorsement_count($cid);
  $comment_no_replies = _smartparticipation_core_comment_replies($cid);
  $comment_moderation_status = _smartparticipation_core_comment_moderation_status($cid);
  $comment_moderation_action = _smartparticipation_core_comment_moderation_action($cid);
  $comment_recommended = _smartparticipation_core_comment_recommended($cid) ? 'yes' : 'no';

  $endorsement_service = new \SmartParticipation\EndorsementService;
  $comment_endorsement_users = $endorsement_service->findCommentEndorsers($cid);

  // Retrieve cid and comment body of first moderator reply to this message, if it exists
  $reply_cid = '';
  $reply_body = '';
  if ($comment_moderation_action == 'reply'){
    $query = 'SELECT cid, cb.comment_body_value FROM {comment}  AS c
              LEFT OUTER JOIN {field_data_comment_body} AS cb ON c.cid = cb.entity_id
              LEFT OUTER JOIN {field_data_field_sp_comment_type} ct ON c.cid = ct.entity_id
              WHERE pid = :cid
              AND ct.field_sp_comment_type_value = \'moderator\'';

    $reply_result = db_query($query, array(
      ':cid' => $cid,
    ))->fetchObject();
    
    if ($reply_result) {
      $reply_cid = $reply_result->cid;
      // TODO We need to get the comment body via comment_load, and render it.
      // This method doesn't respect the filter format.
      $reply_body = $reply_result->comment_body_value;
    }
  }

  $survey_data = _smartparticipation_core_get_survey_results ($comment_proposal_nid, $comment_uid);

  return array(
    'cid' => $cid,
    'uid' => $comment_uid, 
    'username' => $comment_username,
    'author_picture' => $comment_author_picture,
    'mail' => $comment_mail,
    'hostname' => $comment_hostname,
    'body' => $comment_body, 
    'comment_proposal' => $comment_proposal, 
    'comment_proposal_nid' => $comment_proposal_nid, 
    'comment_subtopic_title' => $comment_subtopic_title, 
    'comment_subtopic_nid' => $comment->nid, 
    'comment_topic_title' => $comment_topic_title,
    'comment_topic_nid' => $topic_nid, 
    'pid' => $comment_pid, 
    'no_endorsements' => $comment_no_endorsements,
    'endorsement_users' => $comment_endorsement_users,
    'no_replies' => $comment_no_replies,
    'status' => $comment->status,
    'moderation_status' => $comment_moderation_status,  
    'moderation_action' => $comment_moderation_action, 
    'recommended' => $comment_recommended, 
    'original' => $original,
    'original_body' => $original_body,
    'created' => $comment->created,
    'created_display_date' => $created_display_date,
    'permalink' => $permalink,
    'reply_cid' => $reply_cid,
    'reply_body' => $reply_body,
    'notes' => $notes,
    'survey_data' => $survey_data,
  );
}


/**
 * The shorter version of the function which will return the comment details
 * for emails. 
 * 
 */
function smartparticipation_core_comment_data_for_email($cid) {
  
  $comment = comment_load($cid);

  $comment_uid = $comment->uid;  
  $comment_author = user_load($comment_uid);

  $comment_username = $comment_author->name;

  $comment_mail = $comment_author->mail;
  
  // Subtopic details
  $node_subtopic = node_load($comment->nid);
  
  // Topic details
  $topic_nid = smartparticipation_core_field_get_item_value('node', $node_subtopic, 'field_sp_subtopic_topic', 'nid');
  $node_topic = node_load($topic_nid);
  
  // Proposal details
  $proposal_nid = smartparticipation_core_field_get_item_value('node', $node_topic, 'field_sp_topic_proposal', 'nid');
  $node_proposal = node_load($proposal_nid);
  $comment_proposal = $node_proposal->title;
  $comment_proposal_nid = $node_proposal->nid;
  
  return array(
    'uid' => $comment_uid, 
    'username' => $comment_username, 
    'mail' => $comment_mail, 
    'comment_proposal' => $comment_proposal, 
    'comment_proposal_nid' => $comment_proposal_nid, 
  );
}


/**
 * Return an array of comment data for display on user pages.
 *
 * @param $cid - the cid of the comment to be displayed
 * @param $account - the user being displayed
 * @param $show_endorsements - whether endorsements should be displayed. In 
 * some cases this has been determined by the type of page being displayed and
 * who is viewing it. If null, determine here on the basis of the user and 
 * comment.
 * 
 *
 * @return an array of comment data for display on user pages
 */
function smartparticipation_core_user_comment_data($cid, $account, $show_endorsements = null) {

  $comment = comment_load($cid);

  $author_name = smartparticipation_core_comment_author_name($comment);

  // Uses default medium format, as on the topic page
  $display_date = format_date($comment->created); 

  //$body = smartparticipation_core_field_get_item_value('comment', $comment, 'comment_body', 'value');
  $wrapper = entity_metadata_wrapper('comment', $comment);
  $body = $wrapper->comment_body->value();
  $body = $body['safe_value'];

  // Apply glossary markup to comment body
  $proposal_nid = smartparticipation_core_get_comment_proposal_nid($comment);
  $glossary_data = smartparticipation_core_get_proposal_glossary_data($proposal_nid);
  $body = smartparticipation_core_insert_glossary_definitions($body, $glossary_data);

  $permalink = smartparticipation_core_get_comment_permalink_from_cid($cid, 
      t('View this comment in the discussion thread'));

  // Endorsements
  if ($show_endorsements == null) {
    $show_endorsements = smartparticipation_core_show_comment_endorsements($comment);
  }

  $endorsement_count = $show_endorsements ? _smartparticipation_core_comment_endorsement_count($cid) : 0;
  // There are no endorsements, or endorsements should not be displayed
  if ($endorsement_count == 0) {
    $endorsement_count = '';   
  } else {
    $endorsement_count = smartparticipation_core_comment_endorsement_counter_markup($endorsement_count);
  }
  
  // Recommendation
  $recommended = _smartparticipation_core_comment_recommended($cid);

  //$author_thumbnail = theme('user_thumbnail', array('uid' => $account->uid, 'username' => strip_tags($author_name)));
  $author_thumbnail = theme('user_thumbnail', array('uid' => $comment->uid));

  return array(
    'author_name' => $author_name,
    'author_thumbnail' => $author_thumbnail,
    'body' => $body,
    'display_date' => $display_date,
    'endorsement_count' => $endorsement_count,
    'permalink' => $permalink,
    'recommended' => $recommended,
  );
}


/**
 * Return the comment data needed to display a comment in the Recent and
 * Recommended Comment blocks
 * 
 * @param $cid - the cid of the comment to be displayed
 * @return a render array for display of comment data
 */
function smartparticipation_core_featured_comment_data($cid) {
  
  $comment = comment_load($cid); 

  $recommended = _smartparticipation_core_comment_recommended($cid);

  $comment_permalink = smartparticipation_core_get_comment_permalink_from_cid($cid, t('See full comment'));
  
  // TODO Somehow we should be specifying that the date format is the same as
  // used in comment display on topic page. How do we reference that format?
  $comment_date = format_date($comment->created, 'medium');
  //$comment_body = smartparticipation_core_field_get_item_value('comment', $comment, 'comment_body', 'value');
  $wrapper = entity_metadata_wrapper('comment',$comment);
  $comment_body = $wrapper->comment_body->value();
  $comment_body = $comment_body['safe_value'];

  $proposal = smartparticipation_core_get_comment_proposal($comment);

  $proposal_title = htmlspecialchars($proposal->title);

  /*if (drupal_is_front_page()) {*/
  // NB Don't use url('node/' . $proposal->nid, because both url() and l()
  // prepend a slash, so we get two initial slashes.
  $proposal_path = drupal_get_path_alias('node/' . $proposal->nid);
  $proposal_link = l($proposal->title,
      $proposal_path,
      array(
        'attributes' => array(
          'class' => array('proposal-link'),
        ),
      ));
  /*} else {
    $proposal_link = '';
  }*/
  
  $user = user_load($comment->uid);
  $user_profile_path = smartparticipation_core_path_to_user_profile($comment->uid);
  $user_profile_link = l($user->name, 
      $user_profile_path,
      array(
        'attributes' => array(
          'class' => array('user-profile-link'),
        ),
      ));      


  // TODO Does theme('user_picture') automatically fill in the default picture
  // if the user has no picture?
  //$user_picture = theme('user_picture', array('account' => $user));

  $user_thumbnail = theme('user_thumbnail', array('uid' => $user->uid, 'username' => $user->name));

  $featured_comment_truncate_limit = variable_get('smartparticipation_featured_comment_truncate_limit',500);

  return array(
    'body' => $comment_body,
    'date' => $comment_date,
    'permalink' => $comment_permalink,
    'proposal_title' => $proposal_title,
    'proposal_link' => $proposal_link,
    'recommended' => $recommended,  
    //'user_picture' => $user_picture,
    'user_thumbnail' => $user_thumbnail,
    'user_profile_link' => $user_profile_link,
      'featured_comment_truncate_limit' => $featured_comment_truncate_limit,
  );
  
}

/**
 * Return the nid of the proposal a comment is associated with.
 * 
 * @param comment - the comment
 */
function smartparticipation_core_get_comment_proposal_nid($comment) {

  $node_subtopic = node_load($comment->nid);  
  
  $topic_nid = smartparticipation_core_field_get_item_value('node', $node_subtopic, 
      'field_sp_subtopic_topic', 'nid');
  $node_topic = node_load($topic_nid);  
  
  $proposal_nid = smartparticipation_core_field_get_item_value('node', $node_topic, 
      'field_sp_topic_proposal', 'nid');

  return $proposal_nid;
  
}
/**
 * Return the proposal node a comment is associated with.
 * 
 * @param comment - the comment
 */
function smartparticipation_core_get_comment_proposal($comment) {
  
  $proposal_nid = smartparticipation_core_get_comment_proposal_nid($comment);
  return node_load($proposal_nid);
  
}

/**
 * Return the name of the proposal a comment is associated with.
 * 
 * @param comment - the comment
 */
function smartparticipation_core_get_comment_proposal_name($comment) {
  
  $proposal_nid = smartparticipation_core_get_comment_proposal_nid($comment);
  $proposal = node_load($proposal_nid);  
  $proposal_name = $proposal->title;
  
  return $proposal_name;
  
}

/**
 * Return an excerpt from the comment body.
 * 
 * @param $body - the comment body
 * @param $limit - the excerpt character length limit
 * 
 * @return the excerpt
 */
function smartparticipation_core_comment_excerpt($body, $limit) {
  $body = str_replace('<p>', "\n", $body);
  $body = trim($body);
  $body = strip_tags($body);
  $excerpt = substr($body, 0, $limit - 1);
  if (strlen($body) > $limit) {
    $excerpt .= " [...]\n";
  }
  $excerpt = nl2br($excerpt);
  return $excerpt;
  
}

// Remove the text format selection in the form alter function
function _smartparticipation_core_hide_format_comments($form) {
  $form[LANGUAGE_NONE][0]['format']['#access'] = FALSE;
  return $form;
}

/**
 * Configure custom comment settings when module is enabled.
 */
function smartparticipation_core_configure_comments() {
  variable_set('comment_default_per_page_sp_subtopic', 5000);
  variable_set('smartparticipation_default_comment_max_thread_depth', 3);
}

/**
 * Function to get the number of endorsements for a specific comment
 */
function _smartparticipation_core_comment_endorsement_count($cid) {
  $endorsement_service = new \SmartParticipation\EndorsementService;
  return count($endorsement_service->findByCommentId($cid));
}


/**
 * Function to get the number of replies for a specific comment
 */
function _smartparticipation_core_comment_replies($cid) {
  $query = 'SELECT * FROM {comment} WHERE pid = :cid';
  $result = db_query($query, array(':cid' => $cid));
  
  return $result->rowCount();
}


/**
 * Check if a comment is recommended or not
 */
function _smartparticipation_core_comment_recommended($cid) {
  $query = 'SELECT * FROM {comment} c 
            INNER JOIN {flag_content} fc ON c.cid = fc.content_id 
            INNER JOIN {flags} f ON fc.fid = f.fid 
            WHERE c.cid = :cid 
            AND f.name = :recommendation';
  $result = db_query($query, array(':cid' => $cid, ':recommendation' => 'comment_recommendation'));
  
  return $result->rowCount() > 0;

}

/**
 * Implements hook_query_TAG_alter().
 */
function smartparticipation_core_query_comment_filter_alter(QueryAlterableInterface $query) {

  $conditions = $query->conditions();

  // Check to see if the status condition is already set.
  foreach ($conditions as $condition) {
    if (is_array($condition)
      && is_string($condition['field'])
      && $condition['field'] == 'c.status') {
      return;
    }
  }

  // Add the status condition to find only published comments.
  $query->condition('c.status', COMMENT_PUBLISHED);

}

/** 
 * This function modifies the array returned by comment_node_page_additions()
 * as required by custom SmartParticipation functionality. Note that in case the topic's
 * phase does not allow commenting at all, we do not generate a comment panel 
 * in the subtopic display, and we do not reach this point.
 * 
 * Three cases are handled:
 * (1) Generate a comment form for anonymous users. Since anonymous users are 
 * not allowed to post comments, Drupal will not generate a comment form for
 * them.
 * (2) Remove a comment form if the subtopic's topic is not in the current
 * phase. Drupal has generated a comment form for authenticated users in this
 * case, and it must be removed. 
 * (3) If there is no comment form and no comments, we still want to get to
 * the comment-wrapper template to display the comment panel heading.
 * 
 * 
 * @param $comment_node_page_additions - array returned by Drupal's 
 * comment_node_page_additions()
 * @param $subtopic - the subtopic node
 * 
 * @return $comment_node_page_additions, possibly modified
 *
 */
function smartparticipation_core_comment_node_page_additions($comment_node_page_additions, $subtopic) {

  // NB We need to pass in $subtopic rather than getting it this way. 
  // $comment_node_page_additions['#node'] is not set for anonymous users. 
  // $subtopic = $comment_node_page_additions['#node'];
  
  // Topic is not open for commenting
  if (! $subtopic->comment) {
    
    // Remove the comment form if it exists
    if (isset($comment_node_page_additions['comment_form'])) {
      unset($comment_node_page_additions['comment_form']);
    }
    

    $comment_node_page_additions['commenting_closed_message'] = smartparticipation_core_subtopic_commenting_closed_message($subtopic);
    
  // Topic is in current phase, but no comment form exists (e.g., anonymous user)
  } else if (!isset($comment_node_page_additions['comment_form']) || empty($comment_node_page_additions['comment_form'])) {
    
    // Build the comment form and add it to the array
    $comment = new stdClass;
    $comment->nid = $subtopic->nid;
    $form = drupal_get_form('comment_form', $comment);
    $comment_node_page_additions['comment_form'] = $form;
    
  // If user is required to submit the interest survey, remove reply button. A 
  // dummy reply button will be inserted later, as for anonymous users. This
  // prevents the ajax comment module actions from being attached to the reply 
  // button, because unbind() alone doesn't stop the action of retrieving the
  // reply form.
  } else if ($subtopic->interest_survey_required) {
    foreach ($comment_node_page_additions['comments'] as $key=>$value) {
      if (is_int($key) && isset($value['links']['comment']['#links']['comment-reply'])) {
        unset($comment_node_page_additions['comments'][$key]['links']['comment']['#links']['comment-reply']);
      }
    }
  }

  if (!empty($comment_node_page_additions['comments'])) {
    foreach ($comment_node_page_additions['comments'] as $key => $value) {
      if (is_int($key) && !empty($value['comment_body'][0]['#markup'])) {
        $body = $value['comment_body'][0]['#markup'];
        // Apply glossary markup to comment body
        $proposal_nid = smartparticipation_core_get_comment_proposal_nid($value['#comment']);
        $glossary_data = smartparticipation_core_get_proposal_glossary_data($proposal_nid);
        $body = smartparticipation_core_insert_glossary_definitions($body, $glossary_data);
        $comment_node_page_additions['comments'][$key]['comment_body'][0]['#markup'] = $body;
      }
    }
  }

  // Specify the theme and node for the comment form. Then even when there is no
  // form and no comments, we will get to the comment-wrapper template to 
  // display the heading.
  if (!isset($comment_node_page_additions['#theme']) || empty($comment_node_page_additions['#theme'])) {
    $comment_node_page_additions['#theme'] = 'comment_wrapper__node_sp_subtopic';
  }
  
  if (!isset($comment_node_page_additions['#node']) || empty($comment_node_page_additions['#node'])) {
    $comment_node_page_additions['#node'] = $subtopic;
  }

  $comment_node_page_additions['comment_truncate_limit']=variable_get('smartparticipation_comment_truncate_limit',800);

  return $comment_node_page_additions;

}

/**
 * Return a commenting closed message to display on the subtopic comment panel
 * when commenting is closed.
 * 
 * Assumes the topic and subtopic phase closed messages have been entered as
 * full html (specified as the default format in the form alter functions), so
 * that they are enclosed in p tags.
 * 
 * @param $subtopic - the subtopic node
 * 
 * @return string - the message
 */
function smartparticipation_core_subtopic_commenting_closed_message($subtopic) {

  $commenting_closed_message =  t('<p>Commenting is now closed.</p>');
  
  $subtopic_phase_closed_message = smartparticipation_core_subtopic_phase_closed_message($subtopic);
  if ($subtopic_phase_closed_message) {
    $commenting_closed_message .= $subtopic_phase_closed_message;
  }
  
  return $commenting_closed_message;
  
}

/**
 * Return true iff the specified comment has been quarantined.
 *
 * @param $comment - the comment object
 *
 * @return boolean - true iff $comment has been quarantined.
 */
function smartparticipation_core_comment_quarantined($comment) {
  return smartparticipation_core_field_get_item_value('comment', $comment, 'field_sp_comment_mod_action', 'value') == 'quarantine';
}

/**
 * Modify the body of a quarantined comment to report the quarantine.
 * Moderators: append the quarantine statement to the comment body.
 * Non-moderators: replace the body with the quarantine statement.
 * This applies only on the front end; in the moderator interface, there are
 * sufficient other indicators of the quarantined status.
 * 
 * @param $variables - the array of comment variables 
 * 
 * @return the possibly modified array of comment variables
 */
function smartparticipation_core_comment_report_quarantine(&$variables) {
  
  global $user;

  $comment = $variables['comment'];
  $body = $variables['content']['comment_body'][0]['#markup'];
  
  if (smartparticipation_core_comment_quarantined($comment)) {
    $quarantine_statement = t('This comment has been removed for violation of our site use guidelines.');
    $quarantine_message = '<p class="quarantine-message">' . $quarantine_statement . '</p>';
 
    if (smartparticipation_core_user_is_moderator($user)) {
      $body .= $quarantine_message;
    } else {
      $body = $quarantine_message;
    }
    
    $variables['content']['comment_body'][0]['#markup'] = $body;
  }
  
}


/**
 * Return true iff the specified user may view endorsements for the specified
 * comment. A user may view comment endorsements if (i) he is the comment 
 * author; (ii) he has endorsed the comment; or (iii) he is a moderator.
 * 
 * @param $comment - the comment being displayed
 * @param $account (optional) - the user. Defaults to current user.
 * 
 * @return boolean - true iff the user may view the comment
 */
function smartparticipation_core_show_comment_endorsements($comment, $account = null) {

  // Show endorsements for all comments to all users in a private discussion.
  if (smartparticipation_core_is_private_discussion()) {
    return true;
  }

  global $user;
  
  if (is_null($account)) {
    $account = $user;
  }
  
  return ($comment->uid == $account->uid ||
      _smartparticipation_core_comment_endorsed_by_user($comment->cid, $account) ||
      smartparticipation_core_user_is_moderator());
}