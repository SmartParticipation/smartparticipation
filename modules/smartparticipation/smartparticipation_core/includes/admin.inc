<?php

/**
 * @file
 * 
 * RegRoom functions for admin pages
 */

/**
 * Page argument for the RegRoom module configuration page
 */
function smartparticipation_core_admin_settings() {
  $form = array();
  
  $form['general'] = array(
    '#type' => 'fieldset',
    '#title' => t('General Site Settings'),
    '#collapsible' => true,
    '#collapsed' => true,
    '#weight' => 0,
  ); 
  
  $form['general']['smartparticipation_site_proposal_type'] = array(
    '#type' => 'textfield',
    '#title' => t('Proposal type'),
    '#description' => t('Specify proposal type for this site: rule, plan, recommendation, etc. Use singular and lowercase.'),
    '#required' => true,
    '#default_value' => variable_get('smartparticipation_site_proposal_type', 'proposal'),
    '#size' => 40,
    '#weight' => 5,
  );  
  
  $form['general']['smartparticipation_default_document_label'] = array(
      '#type' => 'textfield',
      '#title' => t('Proposal documents display name'),
      '#description' => t('Specify the proposal documents display name. Use lowercase.'),
      '#required' => true,
      '#default_value' => variable_get('smartparticipation_default_document_label', 'agency documents'),
      '#size' => 40,
      '#weight' => 10,
  );

  $form['general']['smartparticipation_data_truncate_limit'] = array(
      '#type' => 'textfield',
      '#title' => t('Proposal summary truncate value'),
      '#description' => t('How many characters to truncate the proposal summary to.'),
      '#required' => true,
      '#default_value' => variable_get('smartparticipation_data_truncate_limit'),
      '#size' => 10,
      '#weight' => 15,
  );

  $form['general']['smartparticipation_comment_truncate_limit'] = array(
      '#type' => 'textfield',
      '#title' => t('Topic comment truncate value'),
      '#description' => t('How many characters to truncate comments on the topic page to.'),
      '#required' => true,
      '#default_value' => variable_get('smartparticipation_comment_truncate_limit'),
      '#size' => 10,
      '#weight' => 20,
  );

  $form['general']['smartparticipation_featured_comment_truncate_limit'] = array(
      '#type' => 'textfield',
      '#title' => t('Featured comment truncate value'),
      '#description' => t('How many characters to truncate comments in the featured comments section to.'),
      '#required' => true,
      '#default_value' => variable_get('smartparticipation_featured_comment_truncate_limit'),
      '#size' => 10,
      '#weight' => 25,
  );

  $form['general']['smartparticipation_comment_length_limit'] = array(
      '#type' => 'textfield',
      '#title' => t('Comment length limit'),
      '#description' => t('Maximum characters to allow for comments.'),
      '#required' => true,
      '#default_value' => variable_get('smartparticipation_comment_length_limit'),
      '#size' => 10,
      '#weight' => 27,
  );

  $site = variable_get('smartparticipation_site_machine_name');
  $form['general']['smartparticipation_user_placeholder_image'] = array(
      '#type' => 'managed_file',
      '#field_name' => 'smartparticipation_user_placeholder_image',
      '#title' => t('User placeholder image'),
      '#default_value' => variable_get('smartparticipation_user_placeholder_image'),
      '#description' => t('Image displayed when user has not uploaded one 50x50.'),
      '#required' => false,
      '#upload_location' => "public://smartparticipation_core/$site/site/images",
      '#upload_validators' => array(
          'file_validate_extensions' => array('gif png jpg jpeg'),
      ),
      '#weight' => 30,
  );

  $form['general']['smartparticipation_moderator_placeholder_image'] = array(
      '#type' => 'managed_file',
      '#field_name' => 'smartparticipation_moderator_placeholder_image',
      '#title' => t('Moderator placeholder image'),
      '#default_value' => variable_get('smartparticipation_moderator_placeholder_image'),
      '#description' => t('Image displayed when moderator has not uploaded one 50x50.'),
      '#required' => false,
      '#upload_location' => "public://smartparticipation_core/$site/site/images",
      '#upload_validators' => array(
          'file_validate_extensions' => array('gif png jpg jpeg'),
      ),
      '#weight' => 35,
  );

  $form['comment_support_tips'] = array(
    '#type' => 'fieldset',
    '#title' => t('Comment Support Tips'),
    '#collapsible' => true,
    '#collapsed' => true,
    '#weight' => 1,
  );

  $ab_test_service = new \SmartParticipation\ABTestService;
  $ab_tests = $ab_test_service->getOptionsList();
  array_unshift($ab_tests, '-- no active test --');
  $form['comment_support_tips'][\SmartParticipation\ABTestCommentTipsService::ACTIVE_TEST_VAR_NAME] = array(
    '#title' => t('Active A/B Test'),
    '#type' => 'select',
    '#description' => 'The A/B test currently active for comment support tips.',
    '#options' => $ab_tests,
    '#default_value' => variable_get(\SmartParticipation\ABTestCommentTipsService::ACTIVE_TEST_VAR_NAME, ''),
    '#weight' => 10,
  );

  $form['endorsements'] = array(
    '#type' => 'fieldset',
    '#title' => t('Endorsement Settings'),
    '#collapsible' => true,
    '#collapsed' => true,
    '#weight' => 3,
    '#description' => t('The comment endorsement counter link will always display for advanced users (admins, moderators, etc.).  Select the options below to enable this feature for basic site users.'),
  );

  $form['endorsements']['smartparticipation_endorsements_counter_show_all_users'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show comment endorsement counter to all users.'),
    '#required' => false,
    '#default_value' => variable_get('smartparticipation_endorsements_counter_show_all_users'),
    '#weight' => 15,
  );

  $form['endorsements']['smartparticipation_endorsements_users_show_all_users'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show the list of users who endorsed a comment to all users.'),
    '#description' => t('The endorsement counter will be changed to a clickable link.  The link will open a pop-up containing the users who endorsed the comment.  Requires that the comment endorsement counter is also enabled for all users.'),
    '#required' => false,
    '#default_value' => variable_get('smartparticipation_endorsements_users_show_all_users'),
    '#weight' => 20,
    '#states' => array(
      'visible' => array(
        ':input[name="smartparticipation_endorsements_counter_show_all_users"]' => array('checked' => TRUE),
      ),
    ),
  );

  $form['profiles'] = array(
    '#type' => 'fieldset',
    '#title' => t('Profile Settings'),
    '#collapsible' => true,
    '#collapsed' => true,
    '#weight' => 5,
  );

  $form['profiles']['smartparticipation_profile_status'] = array(
    '#type' => 'select',
    '#title' => t('Profile status'),
    '#options' => array(
      0 => 'Disabled',
      1 => 'Enabled',
    ),
    '#default_value' => variable_get('smartparticipation_profile_status'),
    '#description' => t('Specify if user profiles are enabled.'),
    '#weight' => 10,
  );

  $form['profiles']['smartparticipation_profile_reminder'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show profile reminder'),
    '#description' => t('Show profile reminder pop-up if user has not filled out any part of their profile.'),
    '#required' => false,
    '#default_value' => variable_get('smartparticipation_profile_reminder'),
    '#weight' => 15,
    '#states' => array(
      'visible' => array(
        ':input[name="smartparticipation_profile_status"]' => array('value' => 1),
      ),
    ),
  );

  $form['profiles']['smartparticipation_profile_reminder_text'] = array(
    '#type' => 'textarea',
    '#title' => t('Profile reminder text'),
    '#description' => t('Text to show in the reminder pop-up.  Include the token: !profile_form_link'),
    '#required' => false,
    '#default_value' => variable_get(
      'smartparticipation_profile_reminder_text',
      'You haven\'t filled out any of your profile. You can do so on your !profile_form_link.'
    ),
    '#weight' => 20,
    '#rows' => 3,
    '#states' => array(
      'visible' => array(
        ':input[name="smartparticipation_profile_status"]' => array('value' => 1),
        ':input[name="smartparticipation_profile_reminder"]' => array('checked' => TRUE),
      ),
    ),
  );

  $form['profiles']['smartparticipation_profile_form_link_text'] = array(
    '#type' => 'textfield',
    '#title' => t('Profile form link text'),
    '#description' => t('The text to use for the profile form link token: !profile_form_link'),
    '#required' => false,
    '#default_value' => variable_get(
      'smartparticipation_profile_form_link_text',
      'profile form'
    ),
    '#size' => 45,
    '#weight' => 25,
    '#states' => array(
      'visible' => array(
        ':input[name="smartparticipation_profile_status"]' => array('value' => 1),
        ':input[name="smartparticipation_profile_reminder"]' => array('checked' => TRUE),
      ),
    ),
  );

  if (function_exists('profile2_get_types')) {
    $profile_types = profile2_get_types();
    if (!empty($profile_types)) {
      $profile_types = array_keys($profile_types);
      $profile_types = array_combine($profile_types, $profile_types);
      $form['profiles']['smartparticipation_active_profile'] = array(
        '#title' => t('Profile type'),
        '#type' => 'select',
        '#description' => 'The profile type that is currently in use.  This is the machine name of the Profile2 type.',
        '#options' => $profile_types,
        '#default_value' => variable_get('smartparticipation_active_profile'),
        '#weight' => 30,
        '#states' => array(
          'visible' => array(
            ':input[name="smartparticipation_profile_status"]' => array('value' => 1),
            ':input[name="smartparticipation_profile_reminder"]' => array('checked' => TRUE),
          ),
        ),
      );
    }
  }

  // RECOMMENDED COMMENTS
  $form['recommended_comments'] = array(
    '#type' => 'fieldset',
    '#title' => t('Recommended Comments'),
    '#description' => t('Configure recommended comments functionality.'),
    '#collapsible' => true,
    '#collapsed' => true,
    '#weight' => 8,
  );

  $recommended_comments_service =  new \SmartParticipation\RecommendedCommentsService;
  $form['recommended_comments'][$recommended_comments_service::STATUS_VAR] = array(
    '#type' => 'select',
    '#title' => t('Recommended comments status'),
    '#options' => array(
      0 => 'Disabled',
      1 => 'Enabled',
    ),
    '#default_value' => $recommended_comments_service->getStatus(),
    '#description' => t('Specify if recommended comments are enabled.'),
    '#weight' => 10,
  );

  // REGROOM EVENT LOGGING
  $form['smartparticipation_event_logging'] = array(
    '#type' => 'fieldset',
    '#title' => t('Event Logging'),
    '#description' => t('Configure SmartParticipation event logging.'),
    '#collapsible' => true,
    '#collapsed' => true,
    '#weight' => 10,      
  );
  
  $form['smartparticipation_event_logging']['smartparticipation_log_events'] = array(
    '#type' => 'checkbox',
    '#title' => t('Log SmartParticipation events'),
    '#description' => t('Turn SmartParticipation event logging on or off.'),
    '#required' => false,
    '#default_value' => variable_get('smartparticipation_log_events'),
    '#weight' => 0,
  );
    
  // THIRD PARTY SERVICES
  $form['third_party_services'] = array(
    '#type' => 'fieldset',
    '#title' => t('Third party services'),
    '#description' => t('Configuration of third party services.'),
    '#collapsible' => true,
    '#collapsed' => true,
    '#weight' => 20,      
  );

  $form['third_party_services']['smartparticipation_video_player_license_key'] = array(
    '#type' => 'textfield',
    '#title' => t('JW Player license key'),
    '#description' => t('If no license key is specified, the free JW Player will be used.'),
    '#required' => false,
    '#default_value' => variable_get('smartparticipation_video_player_license_key'),
    '#size' => 60,
    '#weight' => 0,
  );
  
  $form['third_party_services']['smartparticipation_addthis_pubid'] = array(
      '#type' => 'textfield',
      '#title' => t('AddThis profile id'),
      '#description' => t('Required in order to provide a share link on comments.'),
      '#required' => false,
      '#default_value' => variable_get('smartparticipation_addthis_pubid'),
      '#size' => 60,
      '#weight' => 5,
  );  
  
  
  // UPCOMING PROPOSALS BLOCK
  // TODO This should be an editable block rather than site configuration fields.
  // Need to figure out how to create an editable block.
  $form['upcoming_proposals'] = array(
    '#type' => 'fieldset',
    '#title' => t('Coming Soon block'),
    '#description' => t('This block appears on the home page featuring any upcoming proposals, if the agency has agreed to this advance announcement. If there are no upcoming or open proposals eligible for display, the following generic image and text will display.'),
    '#collapsible' => true,
    '#collapsed' => true,
    '#weight' => 30,            
  );

  $form['upcoming_proposals']['smartparticipation_coming_soon_heading'] = array(
      '#type' => 'textfield', 
      '#title' => t('Coming Soon heading'),
      '#description' => t('Coming Soon block heading. Case sensitive; enter case as it should be displayed.'),
      '#required' => false,
      '#default_value' => variable_get('smartparticipation_coming_soon_heading'),
      '#weight' => 0,
  );

  $site = variable_get('smartparticipation_site_machine_name');
  $form['upcoming_proposals']['smartparticipation_coming_soon_image'] = array(
    '#type' => 'managed_file',
    '#field_name' => 'smartparticipation_coming_soon_image',
    '#title' => t('Generic Coming Soon image'),
    '#default_value' => variable_get('smartparticipation_coming_soon_image'),
    '#description' => t('Generic image displayed in Coming Soon block when there are no upcoming proposals to display. Size in pixels: 940x250.'),
    '#required' => false,
    '#upload_location' => "public://smartparticipation_core/$site/site/images",
    '#upload_validators' => array(
      'file_validate_extensions' => array('gif png jpg jpeg'),
    ),
    '#weight' => 5,      
  );

  $form['upcoming_proposals']['smartparticipation_coming_soon_text'] = array(
    '#type' => 'textarea',
    '#title' => t('Generic Coming Soon text'),
    '#description' => t('Generic text displayed in Coming Soon block when there are no upcoming proposals to display.'),
    '#required' => false,
    '#default_value' => variable_get('smartparticipation_coming_soon_text'),
    '#weight' => 10,
  );
  
  $form['upcoming_proposals']['smartparticipation_new_proposal_notification_signup_text'] = array(
    '#type' => 'textfield',
    '#title' => t('New proposal notification signup text'),
    '#description' => t('Text to be displayed as a link to sign up for notification of a new proposal.'),
    '#required' => false,
    '#default_value' => variable_get('smartparticipation_new_proposal_notification_signup_text'),
    '#weight' => 15,
    '#size' => 100,
  );
  
  // NOTIFICATIONS
  $form['notifications'] = array(
    '#type' => 'fieldset',
    '#title' => t('Notifications and emails'),
    '#description' => t('Content of email notifications from this site.'),
    '#collapsible' => true,
    '#collapsed' => true,
    '#weight' => 40,
  ); 
  
  $site_name = variable_get('site_name');

  // EMAIL SIGNATURE
  $form['notifications']['mail_signature'] = array(
    '#type' => 'fieldset',
    '#title' => t('Email signature'),
    '#collapsible' => true,
    '#collapsed' => true,
    '#weight' => 0,
  );
  $form['notifications']['mail_signature']['smartparticipation_mail_signature'] = array(
    // TODO Change to text_format when we send HTML emails.
    '#type' => 'textarea', //'text_format',
    '#title' => t('Email signature'),
    '#description' => t('Signature to append to all site emails sent to users.'),
    '#required' => true,
    '#default_value' => variable_get('smartparticipation_mail_signature',
        t('The @site-name Team', array('@site-name' => $site_name))),
    '#weight' => 0,
  );
    
  // REGISTRATION EMAIL
  $form['notifications']['mail_registration'] = array(
    '#type' => 'fieldset',
    '#title' => t('Registration email'),
    '#collapsible' => true,
    '#collapsed' => true,
    '#weight' => 10,      
  );  
  $form['notifications']['mail_registration']['smartparticipation_mail_registration_subject'] = array(
    '#type' => 'textfield', 
    '#title' => t('Registration email subject'),
    '#description' => t('Subject of email sent after user registration.'),
    '#required' => false,
    '#default_value' => variable_get('smartparticipation_mail_registration_subject',
        t('Welcome to @site-name!', array('@site-name' => $site_name))),
    '#size' => 100,
    '#weight' => 0,
  );  
  $form['notifications']['mail_registration']['smartparticipation_mail_registration_body'] = array(
    // TODO Change to text_format when we send HTML emails.
    '#type' => 'textarea', //'text_format',
    '#title' => t('Registration email body'),
    '#description' => t('Text of email sent after user registration. Signature is appended automatically; do not include.'),
    '#required' => false,
    '#default_value' => variable_get('smartparticipation_mail_registration_body'),
    '#weight' => 5,   
    '#rows' => 10,
  );

  // FIRST COMMENT EMAIL
  $form['notifications']['mail_first_comment'] = array(
    '#type' => 'fieldset',
    '#title' => t('First comment email'),
    '#collapsible' => true,
    '#collapsed' => true,
    '#weight' => 20,
  );
  $form['notifications']['mail_first_comment']['smartparticipation_mail_first_comment_subject'] = array(
    '#type' => 'textfield', 
    '#title' => t('First comment email subject'),
    '#description' => t('Subject of email sent after user\'s first comment on a proposal.'),
    '#required' => false,
    '#default_value' => variable_get('smartparticipation_mail_first_comment_subject',
        t('Welcome to the @proposal-name discussion on @site-name!', array(
            '@proposal-name' => '[proposal:name]',
            '@site-name' => $site_name,
        ))),
    '#size' => 100,
    '#weight' => 0,
  );
  $form['notifications']['mail_first_comment']['smartparticipation_mail_first_comment_body'] = array(
    // TODO Change to text_format when we send HTML emails.
    '#type' => 'textarea', //'text_format',
    '#title' => t('First comment email body'),
    '#description' => t('Text of email sent after user\'s first comment on a proposal. Signature is appended automatically; do not include.'),
    '#required' => false,      
    '#default_value' => variable_get('smartparticipation_mail_first_comment_body'),
    '#weight' => 5,
    '#rows' => 10,
  );  
  
  // COMMENT REPLY EMAIL
  $form['notifications']['mail_comment_reply'] = array(
    '#type' => 'fieldset',
    '#title' => t('Comment reply email'),
    '#collapsible' => true,
    '#collapsed' => true,
    '#weight' => 30,
  );
  $form['notifications']['mail_comment_reply']['smartparticipation_mail_comment_reply_subject'] = array(
    '#type' => 'textfield', 
    '#title' => t('Comment reply email subject'),
    '#description' => t('Subject of email sent after reply to user\'s comment.'),
    '#required' => false,
    '#default_value' => variable_get('smartparticipation_mail_comment_reply_subject',
        t('You have received a reply to a comment you made on @site-name', array(
            '@site-name' => $site_name,
        ))),
    '#size' => 100,      
    '#weight' => 0,
  );  
  $form['notifications']['mail_comment_reply']['smartparticipation_mail_comment_reply_body'] = array(
    // TODO Change to text_format when we send HTML emails.
    '#type' => 'textarea', //'text_format',
    '#title' => t('Comment reply email body'),
    '#description' => t('Text of email sent after reply to user\'s comment. Signature is appended automatically; do not include.'),
    '#required' => false,
    '#default_value' => variable_get('smartparticipation_mail_comment_reply_body'),
    '#weight' => 5,
    '#rows' => 15,
  );
  
  // COMMENT ENDORSEMENT EMAIL
  $form['notifications']['mail_comment_endorsement'] = array(
    '#type' => 'fieldset',
    '#title' => t('Comment endorsement email'),
    '#collapsible' => true,
    '#collapsed' => true,
    '#weight' => 40,
  );
  $form['notifications']['mail_comment_endorsement']['smartparticipation_mail_comment_endorsement_subject'] = array(
    '#type' => 'textfield', 
    '#title' => t('Comment endorsement email subject'),
    '#description' => t('Subject of email sent after endorsement of user\'s comment.'),
    '#required' => false,
    '#default_value' => variable_get('smartparticipation_mail_comment_endorsement_subject',
        t('A comment you made on ') . $site_name . t(' has been endorsed')),
    '#size' => 100,
    '#weight' => 0,
  );
  $form['notifications']['mail_comment_endorsement']['smartparticipation_mail_comment_endorsement_body'] = array(
    // TODO Change to text_format when we send HTML emails.
    '#type' => 'textarea', //'text_format',
    '#title' => t('Comment endorsement email body'),
    '#description' => t('Text of email sent after endorsement of user\'s comment. Signature is appended automatically; do not include.'),
    '#required' => false,
    '#default_value' => variable_get('smartparticipation_mail_comment_endorsement_body'),
    '#weight' => 5,
    '#rows' => 10,
  );
  
  // COMMENT RECOMMENDATION EMAIL
  $form['notifications']['mail_comment_recommendation'] = array(
    '#type' => 'fieldset',
    '#title' => t('Comment recommendation email'),
    '#collapsible' => true,
    '#collapsed' => true,
    '#weight' => 50,
  );
  $form['notifications']['mail_comment_recommendation']['smartparticipation_mail_comment_recommendation_subject'] = array(
    '#type' => 'textfield', 
    '#title' => t('Comment recommendation email subject'),
    '#description' => t('Subject of email sent after recommendation of user\'s comment.'),
    '#required' => false,
    '#default_value' => variable_get('smartparticipation_mail_comment_recommendation_subject',
        t('A comment you made on @site-name has been recommended', array(
            '@site-name' => $site_name,
        ))),
    '#size' => 100,
    '#weight' => 0,
  );
  $form['notifications']['mail_comment_recommendation']['smartparticipation_mail_comment_recommendation_body'] = array(
    // TODO Change to text_format when we send HTML emails.
    '#type' => 'textarea', //'text_format',
    '#title' => t('Comment recommendation email body'),
    '#description' => t('Text of email sent after recommendation of user\'s comment. Signature is appended automatically; do not include.'),
    '#required' => false,
    '#default_value' => variable_get('smartparticipation_mail_comment_recommendation_body'),
    '#weight' => 5,
    '#rows' => 15,
  );
  
  /* Commenting out for now. Not sure if it can be implemented.
  // COMMENT SHARED
  $form['notifications']['mail_comment_shared'] = array(
    '#type' => 'fieldset',
    '#title' => t('Comment shared email'),
    '#collapsible' => true,
    '#collapsed' => true,
    '#weight' => 60,
  );
  $form['notifications']['mail_comment_shared']['smartparticipation_core_mail_comment_shared_subject'] = array(
    '#type' => 'textfield', 
    '#title' => t('Comment shared email subject'),
    '#description' => t('Subject of email sent after user\'s comment is shared.'),
    '#required' => false,
    '#default_value' => variable_get('smartparticipation_mail_comment_shared_subject',
        t('A comment you made on ') . $site_name . t(' has been shared')),
    '#size' => 100,
    '#weight' => 0,
  );
  $form['notifications']['mail_comment_shared']['smartparticipation_core_mail_comment_shared_body'] = array(
    // TODO Change to text_format when we send HTML emails.
    '#type' => 'textarea', //'text_format',
    '#title' => t('Comment shared email body'),
    '#description' => t('Text of email sent after user\'s comment is shared. Signature is appended automatically; do not include.'),
    '#required' => false,
    '#default_value' => variable_get('smartparticipation_mail_comment_shared_body'),
    '#weight' => 5,
    '#rows' => 10,
  );
  */


  $form['footer'] = array(
      '#type' => 'fieldset',
      '#title' => t('Footer'),
      '#description' => t('This is the main footer block for the site.'),
      '#collapsible' => true,
      '#collapsed' => true,
      '#weight' => 50,
  );

  $vals = variable_get('smartparticipation_footer_column_1');
  $form['footer']['smartparticipation_footer_column_1'] = array(
      '#type' => 'text_format',
      '#format' => 'full_html',
      '#title' => t('Footer Column 1'),
      '#description' => t('The markup for footer column 1.'),
      '#required' => false,
      '#default_value' => $vals['value'],
      '#weight' => 0,
  );

  $vals = variable_get('smartparticipation_footer_column_2');
  $form['footer']['smartparticipation_footer_column_2'] = array(
      '#type' => 'text_format',
      '#format' => 'full_html',
      '#title' => t('Footer Column 2'),
      '#description' => t('The markup for footer column 2.'),
      '#required' => false,
      '#default_value' => $vals['value'],
      '#weight' => 5,
  );

  $form['footer']['smartparticipation_footer_facebook_url'] = array(
      '#type' => 'textfield',
      '#title' => t('Facebook URL'),
      '#description' => t('The URL to use for Facebook.'),
      '#required' => false,
      '#default_value' => variable_get('smartparticipation_footer_facebook_url'),
      '#size' => 40,
      '#weight' => 10,
  );

  $form['footer']['smartparticipation_footer_twitter_url'] = array(
      '#type' => 'textfield',
      '#title' => t('Twitter URL'),
      '#description' => t('The URL to use for Twitter.'),
      '#required' => false,
      '#default_value' => variable_get('smartparticipation_footer_twitter_url'),
      '#size' => 40,
      '#weight' => 15,
  );

  $form['contact'] = array(
      '#type' => 'fieldset',
      '#title' => t('Contact Information'),
      '#description' => t('Contact information for the Contact Us page.'),
      '#collapsible' => true,
      '#collapsed' => true,
      '#weight' => 60,
  );

  $vals = variable_get('smartparticipation_contact_organization');
  $form['contact']['smartparticipation_contact_organization'] = array(
      '#type' => 'text_format',
      '#format' => 'full_html',
      '#title' => t('Organization'),
      '#description' => t('Organization Name'),
      '#required' => false,
      '#default_value' => $vals['value'],
      '#weight' => 0,
  );

  $form['contact']['smartparticipation_contact_address'] = array(
      '#type' => 'textarea',
      '#title' => t('Address'),
      '#description' => t('Physical Address'),
      '#required' => false,
      '#default_value' => variable_get('smartparticipation_contact_address'),
      '#weight' => 5,
  );

  $form['contact']['smartparticipation_contact_phone'] = array(
      '#type' => 'textfield',
      '#title' => t('Phone number'),
      '#description' => t('Phone Number'),
      '#required' => false,
      '#default_value' => variable_get('smartparticipation_contact_phone'),
      '#size' => 40,
      '#weight' => 10,
  );

  $vals = variable_get('smartparticipation_contact_alternative');
  $form['contact']['smartparticipation_contact_alternative'] = array(
      '#type' => 'text_format',
      '#format' => 'full_html',
      '#title' => t('Alternative Contact'),
      '#description' => t('Alternative Contact Information'),
      '#required' => false,
      '#default_value' => $vals['value'],
      '#weight' => 15,
  );


  $form['#submit'] = array('smartparticipation_core_admin_settings_submit');

  return system_settings_form($form);
}

/**
 * Custom submit handler for the admin settings form. 
 */
function smartparticipation_core_admin_settings_submit($form, &$form_state) {


  $form_state['values']['smartparticipation_site_proposal_type'] =
    strtolower($form_state['values']['smartparticipation_site_proposal_type']);
    
  $plural = $form_state['values']['smartparticipation_site_proposal_type'] . 's';
  variable_set('smartparticipation_site_proposal_type_plural', $plural);
  
  $form_state['values']['smartparticipation_default_document_label'] =
    strtolower($form_state['values']['smartparticipation_default_document_label']);
  
  // Change image file status to permanent and record its usage in the database
  $file = file_load($form_state['values']['smartparticipation_coming_soon_image']);
  if ($file) {
    $file->status = FILE_STATUS_PERMANENT;
    file_save($file);
    file_usage_add($file, 'smartparticipation_core', 'variable:smartparticipation_coming_soon_image', $file->fid, 1);
  }

  $file = file_load($form_state['values']['smartparticipation_user_placeholder_image']);
  if ($file) {
    $file->status = FILE_STATUS_PERMANENT;
    file_save($file);
    file_usage_add($file, 'smartparticipation_core', 'variable:smartparticipation_user_placeholder_image', $file->fid, 1);
  }

}

  /**
 * Customize the node add list at /node/add.
 */
function smartparticipation_core_preprocess_node_add_list(&$variables) {

  foreach ($variables['content'] as $key => $item) {
    $type = str_replace('-', '_', preg_replace('/node\/add\//', '', $item['path']));

    switch ($type) {
      
      /* This case is handled via permissions instead.
      case 'node_export':
        unset($variables['content'][$key]);
        break;
      */
            
      case 'page':
        // Modify Basic page description.
        $variables['content'][$key]['description'] = 'Static pages that don\'t belong to a specific group, such as Terms of Use, Contact Us, etc.';
        break;        

      /* This case can't be handled correctly by permissions. This node type 
       * should never be created from here.
       */
      case 'sp_comment_note':
        // Seems to be safe in PHP due to copy-on-write: PHP makes a copy of the
        // array if it is modified during iteration. See 
        // http://stackoverflow.com/questions/10057671/how-foreach-actually-works
        unset($variables['content'][$key]);
        break;
        
      /* Also can't be handled from permissions, since some users do have 
       * permission to create a webform. They just shouldn't do it from here.
       */
      case 'webform':
        // Seems to be safe in PHP due to copy-on-write: PHP makes a copy of the
        // array if it is modified during iteration. See 
        // http://stackoverflow.com/questions/10057671/how-foreach-actually-works
        unset($variables['content'][$key]);
        break;
    }
    
  }     
}

/**
 * Alter the admin/content list.
 */
function smartparticipation_core_form_node_admin_content_alter(&$form, &$form_state, $form_id) {

  // Remove comment note content type from "Find Content" page
  unset ($form['filter']['filters']['status']['filters']['type']['#options']['sp_comment_note']);
  foreach ($form['admin']['nodes']['#options'] as $key => $options) {
    if ($options['type'] == 'Comment note') {
      unset ($form['admin']['nodes']['#options'][$key]);
    }
  }

  // Add an extra column for the phase (only applies to some content types)
  $form['admin']['nodes']['#header']['phase'] = array('data' => t('Phase'));
  foreach ($form['admin']['nodes']['#options'] as $nid => $options) {
    $phase_name = '';
    $phase_tid = '';
    switch ($options['type']) {
      case'Comment tips':
        $node = node_load($nid);
        if(!empty($node->field_sp_comment_tips_phase[LANGUAGE_NONE][0]['tid'])) {
          $phase_tid = $node->field_sp_comment_tips_phase[LANGUAGE_NONE][0]['tid'];
        }
        break;
      case 'Proposal topic':
        $node = node_load($nid);
        if(!empty($node->field_sp_topic_phase[LANGUAGE_NONE][0]['tid'])) {
          $phase_tid = $node->field_sp_topic_phase[LANGUAGE_NONE][0]['tid'];
        }
        break;
      case 'Proposal subtopic':
        $node = node_load($nid);
        if (!empty($node->field_sp_subtopic_topic[LANGUAGE_NONE][0]['nid'])) {
          $node = node_load($node->field_sp_subtopic_topic[LANGUAGE_NONE][0]['nid']);
        }
        if (!empty($node->field_sp_topic_phase[LANGUAGE_NONE][0]['tid'])) {
          $phase_tid = $node->field_sp_topic_phase[LANGUAGE_NONE][0]['tid'];
        }
        break;
    }
    if (!empty($phase_tid)) {
      $phase=taxonomy_term_load($phase_tid);
      $phase_name=$phase->name;
    }
    $form['admin']['nodes']['#options'][$nid]['phase'] = $phase_name;
  }
}
