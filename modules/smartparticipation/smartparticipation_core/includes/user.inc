<?php

/**
 * @file
 * 
 * Smartparticipation_core custom fields and functions for user entity.
 */
use SmartParticipation\ProfileService;

/**
 * Module function to add custom fields to user entity.
 */
function smartparticipation_core_create_custom_user_fields() {

  // Create the custom user fields.
  foreach (smartparticipation_core_user_installed_fields() as $field) {
    $field['entity_types'] = array('user');
    field_create_field($field);
  }

  // Create all the instances for our fields.
  foreach (smartparticipation_core_user_installed_instances() as $instance) {
    $instance['entity_type'] = 'user';
    $instance['bundle'] = 'user';
    field_create_instance($instance);
  }  
}
 
/**
 * Module function to define custom user fields.
 */
function smartparticipation_core_user_installed_fields() {
  
  return array(
  
    'field_sp_user_accepts_terms' => array(
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'field_name' => 'field_sp_user_accepts_terms',
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
          0 => '',
          1 => '',
        ),
        'allowed_values_function' => '',
      ),
      'translatable' => '0',
      'type' => 'list_boolean',
    ),

    'field_sp_user_comment_emails' => array(
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'field_name' => 'field_sp_user_comment_emails',
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
          0 => '',
          1 => '',
        ),
        'allowed_values_function' => '',
      ),
      'translatable' => '0',
      'type' => 'list_boolean',
    ),

    'field_sp_user_site_emails' => array(
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'field_name' => 'field_sp_user_site_emails',
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
          0 => '',
          1 => '',
        ),
        'allowed_values_function' => '',
      ),
      'translatable' => '0',
      'type' => 'list_boolean',
    ),

  );
}

/**
 * Module function to define custom user field instances.
 */
function smartparticipation_core_user_installed_instances() {
  $t = get_t();
  
  return array(
  
    'field_sp_user_accepts_terms' => array(
      'default_value' => array(
        0 => array(
          'value' => 0,
        ),
      ),
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
      'field_name' => 'field_sp_user_accepts_terms',
      'label' => $t('I agree to the site <a href="@terms-url" target="_blank">Terms & Conditions of Use</a>.', 
                    array('@terms-url' => url('terms'))),
      'required' => 1,
      'settings' => array(
        'user_register_form' => 1,
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'options',
        'settings' => array(
          'display_label' => 1,
        ),
        'type' => 'options_onoff',
        'weight' => '2',
      )
    ),
    

    'field_sp_user_comment_emails' => array(
      'default_value' => array(
        0 => array(
          'value' => 1,
        ),
      ),
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
      'field_name' => 'field_sp_user_comment_emails',
      'label' => $t('Notify me when someone responds to my comments.'),
      'required' => 0,
      'settings' => array(
        'user_register_form' => 0,
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'options',
        'settings' => array(
          'display_label' => 1,
        ),
        'type' => 'options_onoff',
        'weight' => 3,
      )
    ),

    'field_sp_user_site_emails' => array(
      'default_value' => array(
        0 => array(
          'value' => 1,
        ),
      ),
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
      'field_name' => 'field_sp_user_site_emails',
      'label' => $t('Send me updates from this site (for example, when a new discussion opens or a final @proposal_type comes out).',
                    array('@proposal_type' => variable_get('smartparticipation_site_proposal_type'))),
      'required' => 0,
      'settings' => array(
        'user_register_form' => 0,
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'options',
        'settings' => array(
          'display_label' => 1,
        ),
        'type' => 'options_onoff',
        'weight' => 4,
      )
    ),

  );  
}

/**
 * Module function to delete custom user fields on uninstall.
 * 
 */
function smartparticipation_core_delete_custom_user_fields() {

  foreach (array_keys(smartparticipation_core_user_installed_fields()) as $field_name) {
    field_delete_field($field_name);
  }
  
  /* Not needed: field_delete_field() will also delete the instances.
  foreach (smartparticipation_core_user_installed_instances() as $instance_name => $instance) {
    field_delete_instance($instance);
  } 
  */  
}

/**
 * Return the number of the user's comments.
 * 
 * @param $uid - the user's uid
 */
function _smartparticipation_core_number_user_comments($uid) {
  
  $query = 'SELECT c.cid 
      FROM {comment} c 
      WHERE c.uid = :uid 
      AND c.status = 1';
  
  $result = db_query($query,array(':uid' => $uid));
  $count = $result->rowCount();

  return $count;
}


/**
 * Return the number of replies to the user's comments.
 * 
 * @param $uid 
 *   The user's uid
 */
function _smartparticipation_core_number_user_comment_replies($uid) {
  
  // TODO Rewrite query as a string and do a db_query
  $query = db_select('comment', 'c')
    ->fields('c', array('cid'));

  $subquery = db_select('comment', 'c2')
  ->fields('c2', array('cid'));
  
  $subquery->where('c2.uid = :uid', array(':uid' => $uid)); 
  $subquery->where('c2.status = 1');

  $query->condition('c.pid', $subquery, 'IN');
  $query->condition('c.status', 1);

  $result = $query->execute();
  $count = $result->rowCount();

  return $count;
}


/**
 * Return the number of user's comments that other users endorsed.
 * 
 * @param $uid
 *   The user's uid
 */
function _smartparticipation_core_number_user_comments_endorsed($uid) {
  
  $query = 'SELECT c.cid FROM {comment} c 
    LEFT JOIN {flag_content} AS fc ON fc.content_id = c.cid 
    LEFT JOIN {flags} AS f ON  f.fid = fc.fid
    WHERE c.uid = :uid 
    AND c.status = 1
    AND f.name = :name';
  
  $result = db_query($query,array(':uid' => $uid, ':name' => 'comment_endorsement'));
  $count = $result->rowCount();

  return $count;
}


/**
 * Return the number of comments the user endorsed.
 * 
 * @param $uid
 *   The user's uid
 */
function _smartparticipation_core_number_comments_user_endorsed($uid) {

  $query = 'SELECT fc.fcid FROM {flag_content} fc 
    INNER JOIN {comment} c ON c.cid = fc.content_id
    LEFT JOIN {flags} AS f ON f.fid = fc.fid 
    WHERE fc.uid = :uid
    AND c.status = 1
    AND f.name = :name';

  $result = db_query($query,array(':uid' => $uid, ':name' => 'comment_endorsement'));
  $count = $result->rowCount();
  
  return $count;
}


/**
 * Return the number of this user's comments that moderators recommended.
 * 
 * @param $uid
 *   The user's uid.
 */
function _smartparticipation_core_number_user_comments_recommended($uid) {
  
  // TODO Rewrite query as a string and do a db_query
  $query = db_select('comment', 'c')
    ->fields('c', array('cid'));
  $query->where('c.uid = :uid', array(':uid' => $uid));
  $query->where('c.status = 1');

  $query->leftJoin('flag_content', 'fc', 'fc.content_id = c.cid');
  
  $query->leftJoin('flags', 'f', 'f.fid = fc.fid');
  $query->where("f.name = 'comment_recommendation'");

  $result = $query->execute();
  $count = $result->rowCount();

  return $count;
}

/**
 * Return the number of proposals the user has participated in.
 * 
 * Participation is defined as follows:
 *   If a proposal has an interest survey, the user has participated in the 
 *   proposal if and only if he has submitted the survey.
 *   If a proposal has no interest survey, the user has participated in the 
 *   proposal if and only if he has submitted a comment to the proposal.
 * 
 * @param $uid
 *   The user's uid.
 */
function _smartparticipation_core_user_proposal_count($uid) {
  
  $user_proposal_count = 0;
  
  $query = 'SELECT nid FROM {node} n WHERE n.type = :type AND n.status = :status';
  $proposal_nids = db_query($query, array(
    ':type' => 'sp_proposal',
      ':status' => 1
    ))
    ->fetchCol();
  
  foreach ($proposal_nids as $nid) {
    $proposal_node = node_load($nid);
    $survey_id = smartparticipation_core_field_get_item_value('node', $proposal_node, 'field_sp_interest_survey', 'nid');
    
    if(!empty($survey_id)) {
      $uid_submitted_survey = _smartparticipation_core_user_submitted_survey($uid, $survey_id);
      
      // the user completed the survey
      if($uid_submitted_survey) {
        $user_proposal_count++;
      }
    } else {
      $number_comments = _smartparticipation_core_user_comments_on_proposal($uid, $nid);
      
      // the user has added a comment to the proposal
      if($number_comments > 0) {
        $user_proposal_count++;
      }
    }
  }
  
  return $user_proposal_count;
}


/**
 * Return the user profile links block.
 */
function smartparticipation_core_user_profile_links() {

  global $user;
  $uid = $user->uid;
  $links = array();
  
  $uid_view = arg(1);
  
  $user_view = user_load($uid_view);
  if (!empty($user_view->name)) {
    $username = $user_view->name;

    $user_picture = theme('user_picture', array('user' =>$user_view));

    //$block['subject'] = $username;
    $block['content'] = '';

    // TODO Integrate with user menu. Use the menu to provide the links, and
    // theme it differently.
    $proposal_count = theme('profile_nav_comment_count', array(
        'count' => _smartparticipation_core_user_proposal_count($uid_view)
    ));

    $comment_count = theme('profile_nav_comment_count', array(
        'count' => _smartparticipation_core_number_user_comments($uid_view),
    ));

    $endorsed_count = theme('profile_nav_comment_count', array(
        'count' => _smartparticipation_core_number_user_comments_endorsed($uid_view),
    ));

    $recommended_count = theme('profile_nav_comment_count', array(
        'count' => _smartparticipation_core_number_user_comments_recommended($uid_view),
    ));

    $endorsement_count = theme('profile_nav_comment_count', array(
        'count' => _smartparticipation_core_number_comments_user_endorsed($uid_view),
    ));

    $reply_count = theme('profile_nav_comment_count', array(
        'count' => _smartparticipation_core_number_user_comment_replies($uid_view),
    ));

    $user_path = 'user/' . $uid_view;
    $proposals_path = $user_path . '/proposals';
    $comments_path = $user_path . '/comments';
    $endorsed_path = $user_path . '/endorsed';
    $recommended_path = $user_path . '/recommended';
    $endorsements_path = $user_path . '/endorsements';
    $replies_path = $user_path . '/replies';

    // Create multidimensional array for sorting by weight
    // Get menu weights based on path
    $menu_links = menu_load_links('sp-user-menu-logged-in');
    foreach ($menu_links as $ml) {
      if ($ml['plid'] != 0) {
        $path_parts = explode('/', $ml['options']['menu_token_link_path']);
        $weights[$path_parts[2]] = $ml['weight'];
      }
    }

    $profile_link = array('link'=>l(t('Profile'), 'user/' . $uid_view . '/profile'), 'weight'=>$weights['stats']);
    $stats_link = array('link'=>l(t('Stats'), 'user/' . $uid_view . '/stats'), 'weight'=>$weights['stats']);

    // The user is browsing his own profile
    if ($uid == $uid_view) {

      $links = array(
        // l(t('Stats'), 'user/' . $uid_view . '/stats'),

        // l(t('Profile'), 'user/' . $uid_view . '/edit/main'),

        //l(t('Profile'), 'user/' . $uid_view . '/profile'),

          array('link'=>l(t('Account'), 'user/' . $uid_view . '/edit'), 'weight'=> $weights['edit']),
          array('link'=>l(t('Proposals I participated in'), $proposals_path) . $proposal_count, 'weight'=>$weights['proposals']),
          array('link'=>l(t('My comments'), $comments_path) . $comment_count, 'weight'=>$weights['comments']),
          array('link'=>l(t('My endorsed comments'), $endorsed_path) . $endorsed_count, 'weight'=>$weights['endorsed']),
          'rec' => array('link'=>l(t('My recommended comments'), $recommended_path) . $recommended_count, 'weight'=>$weights['recommended']),
          array('link'=>l(t('Replies to my comments'), $replies_path) . $reply_count, 'weight'=>$weights['replies']),
          array('link'=>l(t('Comments I endorsed'), $endorsements_path) . $endorsement_count, 'weight'=>$weights['endorsements']),
      );


      // The user is viewing another user's profile
    } else {

      $links = array(
        // l(t('Stats'), 'user/' . $uid_view . '/stats'),

        //l(t('Profile'), 'user/' . $uid_view . '/profile'),

          array('link'=>l(t('Proposals @user participated in',
              array(
                  '@user' => $username,
              )),
              $proposals_path) . $proposal_count, 'weight'=>$weights['proposals']),

          array('link'=>l(t('@user\'s comments',
              array(
                  '@user' => $username,
              )),
              $comments_path) . $comment_count, 'weight'=>$weights['comments']),

          array('link'=>l(t('@user\'s endorsed comments',
              array(
                  '@user' => $username,
              )),
              $endorsed_path) . $endorsed_count, 'weight'=>$weights['endorsed']),

          'rec' => array('link'=>l(t('@user\'s recommended comments',
              array(
                  '@user' => $username,
              )),
              $recommended_path) . $recommended_count, 'weight'=>$weights['recommended']),

          array('link'=>l(t('Replies to @user\'s comments',
              array(
                  '@user' => $username,
              )),
              $replies_path) . $reply_count, 'weight'=>$weights['replies']),

          array('link'=>l(t('Comments @user endorsed',
              array(
                  '@user' => $username,
              )),
              $endorsements_path) . $endorsement_count, 'weight'=>$weights['endorsements']),
      );

    }

    $recommended_comments_service = new \SmartParticipation\RecommendedCommentsService;
    if (!$recommended_comments_service->getStatus()) {
      unset ($links['rec']);
    }

    if (ProfileService::userProfilesAreActive()) {
      array_unshift($links, $profile_link);
    }
    else {
      array_unshift($links, $stats_link);
    }
  }

  //  Now sort the $links multidimensional array ascending by weight
  foreach ($links as $key => $row) {
    $link[$key] = $row['link'];
    $weight[$key] = $row['weight'];
  }
  array_multisort($weight, SORT_ASC, $links);

  $block['content'] = array(
    'list' => array(
      //'#theme' => 'item_list',
      '#theme' => 'user_profile_links',
      //'#type' => 'ul',
      '#items' => array('username'=>$username, 'user_picture'=>$user_picture, 'links'=>$links),
    ),
  );
      
  return $block;
  
}

function smartparticipation_core_theme_profile_link_count($number) {
  
}

/**
 * Implements hook_user_presave()
 */
function smartparticipation_core_user_presave(&$edit, $account, $category) {
  
  if ($account->is_new) {

    // Store URL user registered from
    /*
    $request_path = request_path();

    // Check if the user has logged in with social media.
    if (strpos($request_path,'hybridauth')!==false) {

      // Set the request path to page they are being returned to.
      $request_path = drupal_get_path_alias($_GET['destination']);

      // The home page will be returned as 'node'
      if ($request_path == 'node') {
        $request_path = '';
      }
    }

    $edit['data']['smartparticipation_core_registration_url'] = '/' . $request_path;
    */
    
    // Store registration domain in preparation for shared users table across
    // multiple sites.
    $edit['data']['smartparticipation_core_registration_domain'] = $_SERVER['SERVER_NAME'];
  }
}



/**
 * Helper function to determine whether current user is the same as the user
 * whose profile is being viewed.
 * 
 * @param $account - the profile being viewed
 * 
 * @return boolean - true iff the user is viewing his own profile
 */
function smartparticipation_core_user_viewing_own_profile($account) {
  global $user;
  return $user->uid == $account->uid;
}

/**
 * Title callback for USER PROPOSALS IN page
 *
 * @param $account - the user object associated with the user page being viewed
 *
 * @return the page title
 */
function _smartparticipation_core_user_proposals_title($account) {

  if ($account) {
    if (smartparticipation_core_user_viewing_own_profile($account)) {
      $title = t('Proposals I Participated In');
    } else {
      $title = t('Proposals @user Participated In', array('@user' => $account->name));
    }
  } else {
    $title = '';
  }

  return $title;
}


/**
 * Page callback for USER PROPOSALS IN page: displays a list of proposals the user has participated in.
 *
 * @param $account - the user object
 *
 * @return an array of comment data grouped by proposal
 */
function smartparticipation_core_user_proposals_page($account) {

  $uid = $account->uid;

  // Fetch the user's comments, organized by proposal.
  // TODO Ideally the proposals should be ordered not by create date, but by
  // closing date and opening date, but this is a complex query based on last
  // phase end date and first phase start date, so using create date instead.

  // TODO Proposal status != coming (when proposal status field is changed from
  // 0/1 to coming/open/closed):
  // WHERE ps.field_sp_proposal_status_value != 'coming'
  $query = 'SELECT c.cid, pn.nid AS proposal_nid, pn.title as proposal_title, ps.field_sp_proposal_status_value AS proposal_status
            FROM {comment} c
            INNER JOIN {node} stn ON c.nid = stn.nid
            INNER JOIN {field_data_field_sp_subtopic_topic} st ON c.nid = st.entity_id
            INNER JOIN {node} tn ON tn.nid = st.field_sp_subtopic_topic_nid
            INNER JOIN {field_data_field_sp_topic_proposal} tp ON tp.entity_id = tn.nid
            INNER JOIN {node} pn ON pn.nid = tp.field_sp_topic_proposal_nid
            INNER JOIN {field_data_field_sp_proposal_status} ps ON pn.nid = ps.entity_id
            WHERE c.status = 1
            AND c.uid = :uid
            AND stn.status = 1
            AND tn.status = 1
            AND pn.status = 1
            ORDER BY ps.field_sp_proposal_status_value DESC, pn.created DESC, c.created ASC';

  $comments = db_query($query, array(
      ':uid' => $uid,
  ))
      ->fetchAllAssoc('cid', PDO::FETCH_ASSOC);

  $user_viewing_own_profile = smartparticipation_core_user_viewing_own_profile($account);
  $show_all_endorsements = $user_viewing_own_profile;

  $data = smartparticipation_core_user_comments_by_proposal($comments, $account, $show_all_endorsements);

  $data['#theme'] = 'user_proposals';

  $data['no_comments_message'] =  $user_viewing_own_profile ?
      t('You have not participated in any proposals.') :
      t('@user has not participated in any proposals.', array('@user' => $account->name));

  // Collapse the accordion
  //drupal_add_js('
  //jQuery(window).load(function(){
  //  jQuery( "#multiAccordion h3" ).click();
  //});
  //','inline');

  return $data;

}

/**
 * Title callback for USER COMMENTS page
 * 
 * @param $account - the user object associated with the user page being viewed
 * 
 * @return the page title
 */
function _smartparticipation_core_user_comments_title($account) {
  
  if ($account) {
    if (smartparticipation_core_user_viewing_own_profile($account)) {
      $title = t('My Comments');
    } else {
      $title = t('@user\'s Comments', array('@user' => $account->name));
    }
  } else {
    $title = '';
  }

  return $title;
}


/**
 * Page callback for USER COMMENTS page: displays all of a user's comments,
 * grouped by proposal.
 * 
 * @param $account - the user object
 * 
 * @return an array of comment data grouped by proposal
 */
function smartparticipation_core_user_comments_page($account) {

  $uid = $account->uid;

  // Fetch the user's comments, organized by proposal.
  // TODO Ideally the proposals should be ordered not by create date, but by
  // closing date and opening date, but this is a complex query based on last 
  // phase end date and first phase start date, so using create date instead.
  
  // TODO Proposal status != coming (when proposal status field is changed from
  // 0/1 to coming/open/closed):
  // WHERE ps.field_sp_proposal_status_value != 'coming'
  $query = 'SELECT c.cid, pn.nid AS proposal_nid, pn.title as proposal_title, ps.field_sp_proposal_status_value AS proposal_status 
            FROM {comment} c 
            INNER JOIN {node} stn ON c.nid = stn.nid 
            INNER JOIN {field_data_field_sp_subtopic_topic} st ON c.nid = st.entity_id 
            INNER JOIN {node} tn ON tn.nid = st.field_sp_subtopic_topic_nid
            INNER JOIN {field_data_field_sp_topic_proposal} tp ON tp.entity_id = tn.nid
            INNER JOIN {node} pn ON pn.nid = tp.field_sp_topic_proposal_nid
            INNER JOIN {field_data_field_sp_proposal_status} ps ON pn.nid = ps.entity_id 
            WHERE c.status = 1
            AND c.uid = :uid 
            AND stn.status = 1 
            AND tn.status = 1
            AND pn.status = 1
            ORDER BY ps.field_sp_proposal_status_value DESC, pn.created DESC, c.created ASC';
  
  $comments = db_query($query, array(
    ':uid' => $uid,
  ))
  ->fetchAllAssoc('cid', PDO::FETCH_ASSOC);

  $user_viewing_own_profile = smartparticipation_core_user_viewing_own_profile($account);
  $show_all_endorsements = $user_viewing_own_profile;
  
  $data = smartparticipation_core_user_comments_by_proposal($comments, $account, $show_all_endorsements);
  
  $data['no_comments_message'] =  $user_viewing_own_profile ?
    t('You have not submitted any comments.') :
    t('@user has not submitted any comments.', array('@user' => $account->name));


  return $data;

}

/**
 * Title callback for USER ENDORSED page.
 * 
 * @param $account - the user object associated with the user page being viewed
 * 
 * @return the page title
 */
function _smartparticipation_core_user_endorsed_title($account) {
  
  if ($account) {
    if (smartparticipation_core_user_viewing_own_profile($account)) {
      $title = t('My Endorsed Comments');
    } else {
      $title = t('@name\'s Endorsed Comments', array('@name' => $account->name));
    }
  } else {
    $title = '';
  }
  
  return $title;
  
}


/**
 * Page callback for USER ENDORSED page: displays all users endorsed comments,
 * grouped by proposal.
 * 
 * @param $account - the user object associated with the user page being viewed
 * 
 * @return an array of comment data grouped by proposal
 */
function smartparticipation_core_user_endorsed_page($account) {
  
  $uid = $account->uid;
  
  // Fetch the user's comments, organized by proposal.
  // TODO Ideally the proposals should be ordered not by create date, but by
  // closing date and opening date, but this is a complex query based on last
  // phase end date and first phase start date, so using create date instead.
  
  // TODO Proposal status != coming (when proposal status field is changed from
  // 0/1 to coming/open/closed):
  // WHERE ps.field_sp_proposal_status_value != 'coming'
  $query = 'SELECT c.cid, pn.nid AS proposal_nid, pn.title as proposal_title, ps.field_sp_proposal_status_value AS proposal_status
            FROM {comment} c
            INNER JOIN {flag_content} fc ON fc.content_id = c.cid  
            INNER JOIN {flags} f ON f.fid = fc.fid 
            INNER JOIN {node} stn ON c.nid = stn.nid
            INNER JOIN {field_data_field_sp_subtopic_topic} st ON c.nid = st.entity_id
            INNER JOIN {node} tn ON tn.nid = st.field_sp_subtopic_topic_nid
            INNER JOIN {field_data_field_sp_topic_proposal} tp ON tp.entity_id = tn.nid
            INNER JOIN {node} pn ON pn.nid = tp.field_sp_topic_proposal_nid
            INNER JOIN {field_data_field_sp_proposal_status} ps ON pn.nid = ps.entity_id
            WHERE c.status = 1
            AND c.uid = :uid
            AND f.name = :name
            AND stn.status = 1
            AND tn.status = 1
            AND pn.status = 1
            ORDER BY ps.field_sp_proposal_status_value DESC, pn.created DESC, c.created ASC';

  $comments = db_query($query, array(
      ':uid' => $uid,
      ':name' => 'comment_endorsement',
  ))
  ->fetchAllAssoc('cid', PDO::FETCH_ASSOC);

  $user_viewing_own_profile = smartparticipation_core_user_viewing_own_profile($account);
  $show_all_endorsements = $user_viewing_own_profile;
  
  $data = smartparticipation_core_user_comments_by_proposal($comments, $account, $show_all_endorsements);
  
  $data['no_comments_message'] = $user_viewing_own_profile ?
    t('You have no endorsed comments.') :
    t('@user has no endorsed comments.', array('@user' => $account->name));
  
  return $data;
  
}

/**
 * Title callback for USER RECOMMENDED page.
 *
 * @param $account - the user object associated with the user page being viewed
 *
 * @return the page title
 */
function _smartparticipation_core_user_recommended_title($account) {

  if ($account) {
    if (smartparticipation_core_user_viewing_own_profile($account)) {
      $title = t('My Recommended Comments');
    } else {
      $title = t('@name\'s Recommended Comments', array('@name' => $account->name));
    }
  } else {
    $title = '';
  }

  return $title;

}


/**
 * Page callback for USER RECOMMENDED page: displays all users recommended comments,
 * grouped by proposal.
 *
 * @param $account - the user object associated with the user page being viewed
 *
 * @return an array of comment data grouped by proposal
 */
function smartparticipation_core_user_recommended_page($account) {

  $uid = $account->uid;

  // Fetch the user's comments, organized by proposal.
  // TODO Ideally the proposals should be ordered not by create date, but by
  // closing date and opening date, but this is a complex query based on last
  // phase end date and first phase start date, so using create date instead.

  // TODO Proposal status != coming (when proposal status field is changed from
  // 0/1 to coming/open/closed):
  // WHERE ps.field_sp_proposal_status_value != 'coming'
  $query = 'SELECT c.cid, pn.nid AS proposal_nid, pn.title as proposal_title, ps.field_sp_proposal_status_value AS proposal_status
            FROM {comment} c
            INNER JOIN {flag_content} fc ON fc.content_id = c.cid
            INNER JOIN {flags} f ON f.fid = fc.fid
            INNER JOIN {node} stn ON c.nid = stn.nid
            INNER JOIN {field_data_field_sp_subtopic_topic} st ON c.nid = st.entity_id
            INNER JOIN {node} tn ON tn.nid = st.field_sp_subtopic_topic_nid
            INNER JOIN {field_data_field_sp_topic_proposal} tp ON tp.entity_id = tn.nid
            INNER JOIN {node} pn ON pn.nid = tp.field_sp_topic_proposal_nid
            INNER JOIN {field_data_field_sp_proposal_status} ps ON pn.nid = ps.entity_id
            WHERE c.status = 1
            AND c.uid = :uid
            AND f.name = :name
            AND stn.status = 1
            AND tn.status = 1
            AND pn.status = 1
            ORDER BY ps.field_sp_proposal_status_value DESC, pn.created DESC, c.created ASC';

  $comments = db_query($query, array(
      ':uid' => $uid,
      ':name' => 'comment_recommendation',
  ))
      ->fetchAllAssoc('cid', PDO::FETCH_ASSOC);

  $user_viewing_own_profile = smartparticipation_core_user_viewing_own_profile($account);
  $show_all_endorsements = $user_viewing_own_profile;

  $data = smartparticipation_core_user_comments_by_proposal($comments, $account, $show_all_endorsements);

  $data['no_comments_message'] = $user_viewing_own_profile ?
      t('You have no recommended comments.') :
      t('@user has no recommended comments.', array('@user' => $account->name));

  return $data;

}

/**
 * Title callback for USER ENDORSEMENTS page.
 *
 * @param $account - the user object associated with the user page being viewed
 *
 * @return the page title
 */
function _smartparticipation_core_user_endorsements_title($account) {

  if ($account) {
    if (smartparticipation_core_user_viewing_own_profile($account)) {
      $title = t('Comments I Endorsed');
    } else {
      $title = t('Comments @name Endorsed', array('@name' => $account->name));
    }
  } else {
    $title = '';
  }

  return $title;

}


/**
 * Page callback for USER ENDORSEMENTS page: displays all comments the user
 * endorsed, grouped by proposal.
 *
 * @param $account - the user object associated with the user page being viewed
 *
 * @return an array of comment data grouped by proposal
 */
function smartparticipation_core_user_endorsements_page($account) {

  $uid = $account->uid;

  // Fetch the user's comments, organized by proposal.
  // TODO Ideally the proposals should be ordered not by create date, but by
  // closing date and opening date, but this is a complex query based on last
  // phase end date and first phase start date, so using create date instead.

  // TODO Proposal status != coming (when proposal status field is changed from
  // 0/1 to coming/open/closed):
  // WHERE ps.field_sp_proposal_status_value != 'coming'
  $query = 'SELECT c.cid, pn.nid AS proposal_nid, pn.title as proposal_title, ps.field_sp_proposal_status_value AS proposal_status
            FROM {comment} c
            INNER JOIN {flag_content} fc ON fc.content_id = c.cid
            INNER JOIN {flags} f ON f.fid = fc.fid
            INNER JOIN {node} stn ON c.nid = stn.nid
            INNER JOIN {field_data_field_sp_subtopic_topic} st ON c.nid = st.entity_id
            INNER JOIN {node} tn ON tn.nid = st.field_sp_subtopic_topic_nid
            INNER JOIN {field_data_field_sp_topic_proposal} tp ON tp.entity_id = tn.nid
            INNER JOIN {node} pn ON pn.nid = tp.field_sp_topic_proposal_nid
            INNER JOIN {field_data_field_sp_proposal_status} ps ON pn.nid = ps.entity_id
            WHERE c.status = 1
            AND fc.uid = :uid
            AND f.name = :name
            AND stn.status = 1
            AND tn.status = 1
            AND pn.status = 1
            ORDER BY ps.field_sp_proposal_status_value DESC, pn.created DESC, c.created ASC';

  $comments = db_query($query, array(
      ':uid' => $uid,
      ':name' => 'comment_endorsement',
      ))
      ->fetchAllAssoc('cid', PDO::FETCH_ASSOC);

  $user_viewing_own_profile = smartparticipation_core_user_viewing_own_profile($account);
  $show_all_endorsements = $user_viewing_own_profile;

  $data = smartparticipation_core_user_comments_by_proposal($comments, $account, $show_all_endorsements);

  $data['no_comments_message'] = $user_viewing_own_profile ?
      t('You have not endorsed any comments.') :
      t('@user has not endorsed any comments.', array('@user' => $account->name));

  return $data;

}

/**
 * Title callback for USER REPLIES page.
 * 
 * @param $account - the user object associated with the user page being viewed
 * 
 * @return the page title

 */
function _smartparticipation_core_user_replies_title($account) {
  
  if ($account) {
    if (smartparticipation_core_user_viewing_own_profile($account)) {
      $title = t('Replies to My Comments');
    } else {
      $title = t('Replies to @name\'s Comments', array('@name' => $account->name));
    }
  } else {
    $title = '';
  }
  
  return $title;
}


/**
 * Page callback for USER REPLIES page: displays all replies to the specified
 * user's comments, grouped by proposal.
 
 * TODO - also grouped by parent comment by the user??? ******** CHECK WIREFRAMES
 * 
 * @param $account - the user object associated with the user page being viewed
 * 
 * @return an array of comment data grouped by proposal
 */
function smartparticipation_core_user_replies_page($account) {
  
  $uid = $account->uid;
  
  // Fetch the user's comments, organized by proposal.
  // TODO Ideally the proposals should be ordered not by create date, but by
  // closing date and opening date, but this is a complex query based on last
  // phase end date and first phase start date, so using create date instead.
  
  // TODO Proposal status != coming (when proposal status field is changed from
  // 0/1 to coming/open/closed):
  // WHERE ps.field_sp_proposal_status_value != 'coming'
  $query = 'SELECT c.cid, pn.nid AS proposal_nid, pn.title as proposal_title, ps.field_sp_proposal_status_value AS proposal_status
            FROM {comment} c
            INNER JOIN {comment} cp ON cp.cid = c.pid
            INNER JOIN {node} stn ON c.nid = stn.nid
            INNER JOIN {field_data_field_sp_subtopic_topic} st ON c.nid = st.entity_id
            INNER JOIN {node} tn ON tn.nid = st.field_sp_subtopic_topic_nid
            INNER JOIN {field_data_field_sp_topic_proposal} tp ON tp.entity_id = tn.nid
            INNER JOIN {node} pn ON pn.nid = tp.field_sp_topic_proposal_nid
            INNER JOIN {field_data_field_sp_proposal_status} ps ON pn.nid = ps.entity_id
            WHERE c.status = 1
            AND c.pid != 0
            AND cp.uid = :uid
            AND stn.status = 1
            AND tn.status = 1
            AND pn.status = 1
            ORDER BY ps.field_sp_proposal_status_value DESC, pn.created DESC, c.created ASC';
  
  $comments = db_query($query, array(
    ':uid' => $uid,
  ))
  ->fetchAllAssoc('cid', PDO::FETCH_ASSOC);
  
  $data = smartparticipation_core_user_comments_by_proposal($comments, $account);

  $data['no_comments_message'] = smartparticipation_core_user_viewing_own_profile($account) ?
    t('There are no replies to your comments.') :
    t('There are no replies to @user\'s comments.', array('@user' => $account->name));
  
  return $data;
  
}

/**
 * Return a render array for display by the user comments template.
 *
 * @param $comments - the comment data returned by the database query. The
 * data is assumed to be structured as in the query in smartparticipation_core_user_comments_
 * page().
 * @param $account - the user being viewed
 * @param $show_all_endorsements - true if user is viewing his own comments or
 * his own endorsements. Otherwise, must be determined for each comment.
 * 
 * @return a render array including comment data grouped by proposal,
 * and the theme specification.
 */
function smartparticipation_core_user_comments_by_proposal($comments, $account, $show_all_endorsements = false) {

  $proposal_type = ucfirst(variable_get('smartparticipation_site_proposal_type'));

  // Create a data structure keyed on proposals for display
  $proposals = array();
  $last_proposal_nid = 0;
  $comment_number = 0;

  foreach ($comments as $cid=>$data) {

    $comment_number++;

    $proposal_nid = $data['proposal_nid'];

    if ($proposal_nid != $last_proposal_nid) {

      $status = $data['proposal_status'] == 1 ? 'Open' : 'Closed';
      $status = t($status . ' @proposal_type', array('@proposal_type' => $proposal_type));
      $url = url('node/' . $proposal_nid);
      $proposals[$proposal_nid] = array(
        'url' => $url,
        'status' => $status,
        'title' => $data['proposal_title'],
      );

      $last_proposal_nid = $proposal_nid;
      $comment_number = 1;
    }

    $show_endorsements = $show_all_endorsements ? true : null;
    
    $comment_data = smartparticipation_core_user_comment_data($cid, $account, $show_endorsements);
    $comment_data['number'] = $comment_number;
    $proposals[$proposal_nid]['comments'][] = $comment_data;

  }

  // Return a render array
  return array(
    '#items' => $proposals,
    '#theme' => 'user_comments_by_proposal',
  );

}

/**
 * Title callback for USER STATS page
 */
function _smartparticipation_core_user_stats_title($account) {

  if ($account) {
    if (smartparticipation_core_user_viewing_own_profile($account)) {
      $title = t('My Stats');
    } else {
      $title = t('@name\'s Stats', array('@name' => $account->name));
    }
  } else {
    $title = '';
  }
  
  return $title;
}

function _smartparticipation_core_user_profile_edit_page($account)
{
  $form = drupal_get_form('smartparticipation_core_profile_form');

  return array(
    '#theme' => 'user_regoom_profile_edit',
    'form' => $form,
  );
}

function _smartparticipation_core_user_profile_title($account)
{
  if ($account) {
    if (smartparticipation_core_user_viewing_own_profile($account)) {
      $title = t('My Profile');
    } else {
      $title = t('@name\'s Profile', array('@name' => $account->name));
    }
  }
  return $title;
}

function smartparticipation_core_profile_is_empty($profile = null, $account = null)
{
  if (!function_exists('profile2_load_by_user')) {
    return true;
  }
  // Get the profile for this user if not provided
  if (empty($profile)) {
    if (empty($account)) {
      $account = user_load($GLOBALS['user']->uid);
    }
    $profile = profile2_load_by_user($account);
  }
  // Determine if active profile is empty.
  $profile_type = variable_get('smartparticipation_active_profile','main');
  $profile_empty = true;
  if (!empty($profile) && !empty($profile[$profile_type])) {
    $active_profile = $profile[$profile_type];
    $profile_fields = field_info_instances('profile2', $profile_type);
    foreach ($profile_fields as $field_name => $field) {
      if (!empty($active_profile->$field_name)) {
        $profile_empty = false;
        break;
      }
    }
  }
  return $profile_empty;
}

function _smartparticipation_core_user_profile_page($account)
{
  if (!ProfileService::userProfilesAreActive()) {
    // TODO instead of relying on this redirect, can we replace the profile links with the stats link?
    drupal_goto(drupal_get_path_alias('user/' . $account->uid . '/stats'));
  }
  //$profile = SmartParticipation\ProfileFactory::build($account->uid);
  $profile_service = new SmartParticipation\ProfileService;
  $profile = $profile_service->find($account->uid);

  $profile_empty = $profile_service->isProfileEmpty($profile);

  $user_viewing_own_profile = smartparticipation_core_user_viewing_own_profile($account);

  // Redirect user to profile form if viewing their own empty profile.
  if ($user_viewing_own_profile && $profile_empty) {
    drupal_goto('user/' . $account->uid . '/profile/edit');
  }

  $no_content_message = t('@name has not provided any profile information.', array('@name' => $account->name));

  if ($user_viewing_own_profile) {
    $no_content_message = t(
      'You haven\'t provided any profile information - !edit_your_profile to share your information.', array(
        '!edit_your_profile' => l(
          t('edit your profile'),
          'user/'.$account->uid.'/profile/edit'
        )
      )
    );
  }

  $active_profile = $profile_service->getActiveProfile();
  $profile_theme = 'user_smartparticipation_core_profile_' . $active_profile;

  return array(
    '#markup' => theme(
      'user_smartparticipation_core_profile',
      array(
        'profile' => $profile,
        'profile_theme' => $profile_theme,
        'page_vars' => array('profile_empty' => $profile_empty, 'no_content_message' => $no_content_message)
      )
    )
  );
}

/**
 * Page callback for USER STATS page
 */
function _smartparticipation_core_user_stats_page($account) {
  
  $uid = $account->uid;
  
  $viewing_my_profile = smartparticipation_core_user_viewing_own_profile($account);

  $user_data = array();
  
  $user_data['user_picture'] = theme('user_picture', array('account' => $account));
  
  // Year of user registration
  $registration_year = date('Y', $account->created);
  $user_data['registration_year'] = t('Participant since ' . $registration_year);

  // Proposals user participated in
  $count = _smartparticipation_core_user_proposal_count($uid);
  $proposal_type = variable_get('smartparticipation_site_proposal_type_plural');
  $base_text = 'participated in';
  $text = $viewing_my_profile ?
      t('@proposal_type I ' . $base_text, array(
       '@proposal_type' => $proposal_type,
      )) :
      t('@proposal_type @name ' . $base_text, array(
        '@name' => $account->name,
        '@proposal_type' => $proposal_type,
      ));
  $href = 'user/' . $uid . '/proposals';
  $user_data['proposals'] = array(
      'count' => $count,
      'text' => $text,
      'link' => l(t($text), $href, array('attributes' => array('class' => 'stats-link'))),
      'href' => '/' . $href,
  );
  
  // User's comments
  $count = _smartparticipation_core_number_user_comments($uid);
  $base_text = 'comments';
  $text = $viewing_my_profile ?
      t('my ' . $base_text) :
      t('@name\'s ' . $base_text, array('@name' => $account->name));
  $href = 'user/' . $uid . '/comments';
  $user_data['user_comments'] = array(
      'count' => $count,
      'text' => $text,
      'link' => l(t($text), $href, array('attributes' => array('class' => 'stats-link'))),
      'href' => '/' . $href,
  );
  
  // User's comments that have been endorsed
  $count = _smartparticipation_core_number_user_comments_endorsed($uid);
  $base_text = 'comments that people have endorsed';
  $text = $viewing_my_profile ?
      t('my ' . $base_text) :
      t('@name\'s ' . $base_text, array('@name' => $account->name));
  $href = 'user/' . $uid . '/endorsed';
  $user_data['user_comments_endorsed'] = array(
      'count' => $count,
      'text' => $text,
      'link' => l(t($text), $href,array('attributes' => array('class' => 'stats-link'))),
      'href' => '/' . $href,
  );
    
  // User's recommended comments
  $count = _smartparticipation_core_number_user_comments_recommended($uid);
  $base_text = 'comments that moderators have recommended';
  $text = $viewing_my_profile ?
      t('my ' . $base_text) :
      t('@name\'s ' . $base_text, array('@name' => $account->name));
  $href = 'user/' . $uid . '/recommended';
  $user_data['user_comments_recommended'] = array(
      'count' => $count,
      'text' => $text,
      'link' => l(t($text), $href,array('attributes' => array('class' => 'stats-link'))),
      'href' => '/' . $href,
  );
  $recommended_comments_service = new \SmartParticipation\RecommendedCommentsService;
  $user_data['recommended_comments_enabled'] = $recommended_comments_service->getStatus();


  // Replies to user's comments
  $count = _smartparticipation_core_number_user_comment_replies($uid);
  $base_text1 = 'replies to';
  $base_text2 = 'comments';
  $text = $viewing_my_profile ?
      t($base_text1 . ' my ' . $base_text2) :
      t($base_text1 . ' @name\'s ' . $base_text2, array('@name' => $account->name));
  $href = 'user/' . $uid . '/replies';
  $user_data['replies_to_user_comments'] = array(
      'count' => $count,
      'text' => $text,
      'link' => l(t($text), $href,array('attributes' => array('class' => 'stats-link'))),
      'href' => '/' . $href,
  );

  // Comments user endorsed
  $count = _smartparticipation_core_number_comments_user_endorsed($uid);
  $base_text1 = 'comments';
  $base_text2 = 'endorsed';
  $text = $viewing_my_profile ?
      t($base_text1 . ' I ' . $base_text2) :
      t($base_text1 . ' @name ' . $base_text2, array('@name' => $account->name));
  $href = 'user/' . $uid . '/endorsements';
  $user_data['comments_user_endorsed'] = array(
      'count' => $count,
      'text' => $text,
      'link' => l(t($text), $href,array('attributes' => array('class' => 'stats-link'))),
      'href' => '/' . $href,
  );

  // Return a render array
  return array(
    '#items' => $user_data, 
    '#theme' => 'user_stats',
  );  
}

/*
function _smartparticipation_core_user_profile_page($account) {
  drupal_goto('/users/' . $account->name . '/stats');
}
*/

/**
 * Get the number of comments added by one user to a specific proposal
 */
function _smartparticipation_core_user_comments_on_proposal($uid, $proposal_nid) {
  $query = 'SELECT * FROM {field_data_field_sp_proposal_topic} t
            INNER JOIN {field_data_field_sp_topic_subtopic} s ON t.field_sp_proposal_topic_nid = s.entity_id
            INNER JOIN {comment} c ON s.field_sp_topic_subtopic_nid = c.nid
            WHERE t.entity_id = :proposal_nid
            AND c.uid = :uid';

  $result = db_query($query, array(':proposal_nid' => $proposal_nid, ':uid' => $uid));

  return $result->rowCount();
}

/**
 * Get the number of endorsements added by one user to a specific proposal
 */
function _smartparticipation_core_user_endorsements_on_proposal($uid, $proposal_nid) {
  $query = 'SELECT * FROM {field_data_field_sp_proposal_topic} t
            INNER JOIN {field_data_field_sp_topic_subtopic} s ON t.field_sp_proposal_topic_nid = s.entity_id
            INNER JOIN {comment} c ON s.field_sp_topic_subtopic_nid = c.nid
            INNER JOIN {flag_content} fc ON c.cid = fc.content_id
            INNER JOIN {flags} f ON fc.fid = f.fid
            WHERE t.entity_id = :proposal_nid
            AND c.uid = :uid
            AND f.name = :endorsements';

  $result = db_query($query, array(':proposal_nid' => $proposal_nid, ':uid' => $uid, ':endorsements' => 'comment_endorsement'));

  return $result->rowCount();
}


/**
 * Implements hook_user_insert().
 */
function smartparticipation_core_user_insert(&$edit, $account, $category) {

  _smartparticipation_core_send_registration_email($account);

  smartparticipation_core_log_user_registration($account);

}


/**
 * Send the registration welcome email.
 */
function _smartparticipation_core_send_registration_email($account, $ignore_user_check = false) {

  global $user;

  // If account created by another user, Drupal sends a message with a login
  // link, so don't send two messages.
  // Note that in this case, $user->uid hasn't been set yet.
  // if ($user->uid == $account->uid)    
  // The account email may not be set if the user is coming from a social
  // media service that does not provide the his or her email address.
  if (($user->uid == 0 || $ignore_user_check) && !empty($account->mail)) {
    $subject = variable_get('smartparticipation_mail_registration_subject', '');
    $message = variable_get('smartparticipation_mail_registration_body', '');

    if ($subject && $message) {

      $to = $account->mail;
      $from = 'default_from';
      
      _smartparticipation_core_drupal_mail($from, $to, $subject, $message, array('user' => $account));
    }
  }
}

/**
 * Implements hook_form_FORM_ID_alter().
 * 
 * NB This form is used both when a user is editing his own account, and when 
 * an admin user is editing another user's account.
 */
function smartparticipation_core_form_user_profile_form_alter(&$form, $form_state, $form_id) {

  global $user;

  $form['picture']['picture_upload']['#description'] =
    t('Pictures larger than @dimensions pixels will be scaled down.',
      array('@dimensions' => variable_get('user_picture_dimensions', '85x85')));
  
  // Remove the current password field
  unset($form['account']['current_pass']);
  unset($form['account']['current_pass_required_values']);
  $index = array_search('user_validate_current_pass', $form['#validate']);
  if ($index !== FALSE) {
    unset($form['#validate'][$index]);
  }
  
  // Remove Terms & Conditions checkbox. Once made, the agreement cannot be
  // rescinded.
  $form['field_sp_user_accepts_terms']['#access'] = FALSE;
  // Must also make the field optional, else submission won't validate.
  $form['field_sp_user_accepts_terms'][LANGUAGE_NONE]['#required'] = 0;

    $form['picture']['picture_upload']['#title'] = t('Upload picture:');

  $form['picture']['picture_delete']['#description'] = '';
  
  $edited_user_id = $form_state['build_info']['args'][0]->uid;
  
  // USER EDITING ANOTHER USER'S ACCOUNT
  if (isset($edited_user_id) && $user->uid != $edited_user_id) {
    
    // Advanced users need access to these fields so that they can unsubscribe
    // basic users. 
    $form['field_sp_user_site_emails'][LANGUAGE_NONE]['#title'] =
        'Notify user when someone responds to his/her comments.';
    $form['field_sp_user_comment_emails'][LANGUAGE_NONE]['#title'] =
        'Send user updates from this site (for example, when a new discussion opens or a final rule comes out).';

    smartparticipation_core_remove_field_username_alt($form);
    
  // USER EDITING OWN ACCOUNT
  } else {
    
    if (smartparticipation_core_is_basic_authenticated_user($user)) {
      smartparticipation_core_remove_form_field_descriptions($form['account']);
      if (!empty($form['field_username_alt'][LANGUAGE_NONE][0]['value']['#default_value'])) {
        smartparticipation_core_remove_field_username_alt($form);
      } elseif (!empty($form['field_username_alt'])) {
        $form['#prefix'] = '<h3>Please create a username and set your password.</h3>';
        $form['#validate'][] = '_smartparticipation_core_username_validate';
        $form['#submit'][] = '_smartparticipation_core_username_submit';
      }
    } else {
      smartparticipation_core_remove_field_username_alt($form);
    }
    
    $form['field_sp_user_comment_emails']['#weight'] = 7;
    $form['field_sp_user_site_emails']['#weight'] = 8;
  }
}

function _smartparticipation_core_username_validate($form, &$form_state)
{
  smartparticipation_core_validate_username($form_state);
}

function _smartparticipation_core_username_submit($form, &$form_state)
{
  smartparticipation_core_update_username($form_state);
  // Go to the front page
  drupal_goto();
}

function smartparticipation_core_user_must_provide_username($account=null)
{
  if (empty($account)) {
    global $user;
    $account = $user;
  }
  if (!empty($account) && smartparticipation_core_is_basic_authenticated_user($account)) {
    $username_field_exists = smartparticipation_core_username_field_exists();
    if ($username_field_exists) {
      /** @var EntityDrupalWrapper $account */
      $account = entity_metadata_wrapper('user', $account);
      $user_provided_username = $account->field_username_alt->value();
      if (empty($user_provided_username)) {
        return true;
      }
    }
  }
  return false;
}

function smartparticipation_core_username_field_exists()
{
  $field_instance = field_info_instance('user','field_username_alt','user');
  return !empty($field_instance);
}

function smartparticipation_core_remove_field_username_alt(&$form)
{
  if (!empty($form['field_username_alt'])) {
    $form['field_username_alt']['#access'] = FALSE;
    $form['field_username_alt'][LANGUAGE_NONE]['#required'] = 0;
  }
}

/**
 * Return the path to the main user profile page, to create a link from comments,
 * etc. Define a function so that we can change the primary user destination in
 * one fell swoop. We are removing the main display profile in release 6.0, so 
 * everything should link to the stats page. In a later version we can restore
 * the main profile page and relink to it here.
 * 
 * @param $uid - the user uid
 */
function smartparticipation_core_path_to_user_profile($uid) {
  // NB Not url('user/' . $uid . '/stats'). Both url() and l() prepend a slash,
  // so if this path gets sent to l() we'll get two initial slashes. Note that
  // the name of this function contains 'path', not 'url'. url() would be used
  // to send a value directly to the template, not to an intermediate function.
  return drupal_get_path_alias('user/' . $uid . '/profile');
  
}


/**
 * Implements hook_user_view()
 */
function smartparticipation_core_user_view($account, $view_mode, $langcode) {
  // redirect the user profile page to the stats page
  drupal_goto(smartparticipation_core_path_to_user_profile($account->uid));
}

/**
 * Determines access to user profile pages. All users have access to profiles of
 * basic authenticated users. Only non-basic users have access to profiles of
 * non-basic users. (This last rule may need refinement in the future, but it 
 * will do for now. We are basically trying to block public access to profiles
 * of editors, admins, moderators, etc.)
 * 
 * @param $account - the user whose profile is being requested
 * 
 * @return boolean - true iff the current user has access to the requested 
 * profile
 */
function smartparticipation_core_access_check_user($account) {
  
  global $user;

  // Redundant, but fast, since this function is accessed to build user menu
  if ($account->uid == $user->uid && $user->uid > 0) {
    $allow = true;
  } else if (smartparticipation_core_is_basic_authenticated_user($account)) {
    // All users have access to profile of basic authenticated user
    if (user_access('access content', $user)) {
      $allow = TRUE;
    }
  } else {
    // Only non-basic authenticated users have access to profiles of non-basic
    // authenticated users (this is a rough cut).
    $allow = (! smartparticipation_core_is_anonymous_or_basic_authenticated_user($user));
  }

  return $allow;
}

/**
 * Return true iff user has opted in to comment response email notifications.
 */
function smartparticipation_core_user_accepts_comment_emails($account) {

  return smartparticipation_core_field_get_item_value('user', $account,
      'field_sp_user_comment_emails', 'value');
  
}

function smartparticipation_core_profile_form($form, &$form_state)
{
  global $user;
  if (! isset($form_state['profiles'])) {
    $profile_type = variable_get('smartparticipation_active_profile','main');
    $profile = profile2_load_by_user($user, $profile_type);
    if (! $profile) {
      $profile = profile_create(array(
        'type' => $profile_type,
        'uid' => $user->uid
      ));
    }
    $form_state['profiles'][$profile->type] = $profile;
  }

  // Use field attach form and handle the fields yourself:
  //field_attach_form('profile2', $profile, $form, $form_state);
  // Or use profile2 API which is simpler
  profile2_attach_form($form, $form_state);

  $form['#prefix'] =
    '<h3 class="profile-form-intro">'
    . 'Share some info about yourself with your CCSD colleagues!<br>(<em>All info is voluntary.</em>)'
    . '</h3>';

  // Picture/avatar.
  $form['picture'] = array(
    '#type' => 'fieldset',
    '#title' => t('Share a photo of yourself or some other image that will appear next to your username in your Profile and your comments.'),
    '#weight' => 1,
    '#access' => (variable_get('user_pictures', 0)),
  );
  $form['picture']['picture'] = array(
    '#type' => 'value',
    '#value' => isset($user->picture) ? $user->picture : NULL,
  );
  $form['picture']['picture_current'] = array(
    '#markup' => theme('user_picture', array('account' => $user)),
  );
  $form['picture']['picture_delete'] = array(
    '#type' => 'checkbox',
    '#title' => t('Delete picture'),
    '#access' => !empty($user->picture->fid),
    '#description' => t('Check this box to delete your current picture.'),
  );
  $form['picture']['picture_upload'] = array(
    '#type' => 'file',
    '#title' => t('Upload picture'),
    '#size' => 48,
    '#description' => t('Pictures larger than @dimensions pixels will be scaled down.', array('@dimensions' => variable_get('user_picture_dimensions', '85x85'))) . ' ' . filter_xss_admin(variable_get('user_picture_guidelines', '')),
  );
  $form['#validate'][] = 'user_validate_picture';

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
    '#weight' => 2,
  );

  $form['#submit'][] = '_smartparticipation_core_profile_form_submit';

  return $form;
}

function _smartparticipation_core_profile_form_submit($form, &$form_state)
{
  global $user;
  $edit = array();
  $edit['picture_upload'] = $form_state['values']['picture_upload'];
  $edit['picture_delete'] = $form_state['values']['picture_delete'];
  user_save($user, $edit);
  drupal_set_message(t('Your profile changes have been saved.'));
  drupal_goto('user/'.$user->uid.'/profile');
}