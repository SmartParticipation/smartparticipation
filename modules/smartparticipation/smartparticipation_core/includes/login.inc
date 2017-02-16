<?php

/** 
 * Define custom smartparticipation_core login and registration functionality.
 */
 
global $new_registration;
$new_registration = false;

/**
 * Configure user registration form when module is enabled.
 */
function smartparticipation_core_configure_user_register() {
  variable_set('user_register', 1);
  variable_set('user_email_verification', 0);
}

/**
 * Configure user login when module is enabled.
 */
function smartparticipation_core_configure_logintoboggan() {
  variable_set('logintoboggan_login_with_email','1');
}


/**
 * Unset custom user registration values when module is disabled.
 */
function smartparticipation_core_unconfigure_user_register() {
  variable_set('user_register', 2);
  variable_del('user_email_verification');
}

/**
 * Unset custom user login values when module is disabled.
 */
function smartparticipation_core_unconfigure_logintoboggan() {
  variable_set('logintoboggan_login_with_email','0');
}

/**
 * Unset HybridAuth module variables.
 */
function smartparticipation_core_unconfigure_hybridauth() {
  
  db_delete('variable')
  ->condition('name', 'hybridauth\_%', 'LIKE')
  ->execute();

  smartparticipation_core_revoke_permissions_hybridauth();
  
}


function smartparticipation_core_init_login_and_registration() {
  
  $path = drupal_get_path('module', 'smartparticipation_core');

  if (user_is_anonymous()) {
    $login_path = drupal_get_path_alias('user/login');
    $register_path = drupal_get_path_alias('user/register');
    //$password_path = drupal_get_path_alias('user/password');
    
    // TODO Our current value of site_frontpage is /node, which Drupal redirects
    // to the front page, but in the redirect we lose the fragments set below. 
    // Temporary workaround is to to pass '' to drupal_goto(), but in the long
    // run we should respect the value of site_frontpage. First we need to fix
    // the default value of site_frontpage during the install (also add an 
    // update function). See RR-2222.
    //$front_page = drupal_get_normal_path(variable_get('site_frontpage', 'node'));
    $current_path = current_path();

    if ($current_path == 'user/login' || $current_path == 'user' && user_is_anonymous()) {
      drupal_goto('', array('fragment' => 'login'));
    } else if ($current_path == 'user/register') {
      drupal_goto('', array('fragment' => 'register'));
    } else if ($current_path == 'user/password') {
      drupal_goto('', array('fragment' => 'password'));
    }
     
    $password_path = drupal_get_path_alias('user/password');

    $settings['loginPath'] = $login_path;
    $settings['registerPath'] = $register_path;
    $settings['passwordPath'] = $password_path;

    $settings['destination'] = '?destination=' . $_GET['q'];

    if (isset($_POST['form_id'])
      && ($_POST['form_id'] == 'user_login'
        || $_POST['form_id'] == 'user_register_form'
        || $_POST['form_id'] == 'user_pass')) {

      $settings['submitted'] = $_POST['form_id'];
    }

    drupal_add_js(array('rrLogin' => $settings), 'setting');
    //drupal_add_js($path . '/js/plugins/dialog.js');
    //drupal_add_js($path . '/js/login.js');
    
  } else {

    // For users with advanced roles (admin, moderator, etc.), skip the Complete 
    // Profile form
    if (!smartparticipation_core_user_must_accept_terms()) {

      smartparticipation_complete_profile_set_account_checked(REQUEST_TIME);

      /* global $user;
      $account = user_load($user->uid);

      // If accepts terms hasn't been filled in already, save it now.
      if (empty($account->field_sp_user_accepts_terms)
        || !$account->field_sp_user_accepts_terms['und'][0]['value']) {

        $accepts_terms = array(
          'field_sp_user_accepts_terms' => array(
            'und' => array(
              0 => array(
                'value' => 1
              )
            )
          )
        );
        user_save($account, $accepts_terms);
      }
      */
    } else {
      if (!empty($_SESSION['complete_profile_checked'])) {
        unset($_SESSION['complete_profile_checked']);
      }
    }
  }
}


/**
 * Implement hook_form_user_login_alter().
 */ 
function smartparticipation_core_form_user_login_alter(&$form, &$form_state, $form_id) {
  
  smartparticipation_core_remove_form_field_descriptions($form);
  
  $form['actions']['submit']['#value'] = 'Log in';

  $login_links = array(
    '#theme' => 'item_list',
    '#items' => array(
      l(t('Lost password?'), 'user/password', array('attributes' => array('title' => t('Request new password.'))))
    ),
  );
  $login_links = drupal_render($login_links);
  $form['actions']['#suffix'] = $login_links;

  $form['name']['#title'] = 'Username or email';

  $form['#validate'][] = '_smartparticipation_core_user_login_validate';

}


/**
 * Module function to generate login and register forms on the page.
 */
function smartparticipation_core_add_login_and_register_forms($page) {
  
  if (user_is_anonymous()) {

    $login_form = drupal_get_form('user_login');
    $register_form = drupal_get_form('user_register_form');
    module_load_include('inc', 'user', 'user.pages'); // needed to get user_pass form
    $password_form = drupal_get_form('user_pass');

    // Add the HybridAuth social login block to the form
    $block = module_invoke('hybridauth', 'block_view', 'hybridauth');
    $hybridauth_output = '<div class="content">' . render($block['content']) . '</div>';

    $social_login_login = theme('social_login',
      array(
        'label' => t('Log in using any of the following services:'),
        'buttons' => $hybridauth_output,
      )
    );
    $social_login_register = theme('social_login',
      array(
        'label'=>t('Register with:'),
        'buttons' => $hybridauth_output,
      )
    );

    $markup = '';

    if (empty($page['#logintoboggan_denied'])) {

      $config = module_invoke('hybridauth', 'get_config');

      $content = array(drupal_render($login_form));
      if (!empty($config['providers'])) {
        $content[] =  $social_login_login;
      }
      $markup =
        theme('dialog', array(
          'dlgId' => 'dlgLogin',
          'title' => t('Log in'),
          'content' => $content
        ));

      $register_content = array(drupal_render($register_form));

      if (!empty($config['providers'])) {
        $register_content[] = $social_login_register;
      }

      $register_title = t('Register');

    } else {

      $register_content = drupal_render($register_form);

      $register_title = t('Register by entering your email address');

    }

    $markup .=
      theme('dialog', array(
        'dlgId' => 'dlgRegister',
        'title' => $register_title,
        'content' => $register_content
      ));

    $markup .=
      theme('dialog', array(
        'dlgId' => 'dlgPassword',
        'title' => t('Lost password?'),
        'content' => drupal_render($password_form)
      ));

    $page['page_bottom']['smartparticipation_core_login'] = array(
      '#markup' => $markup,
    );
  }

  return $page;
}

function smartparticipation_core_prepare_private_discussion_register_form(&$form)
{
  //smartparticipation_core_remove_form_field_descriptions($form['account']);
  $form['#attributes']['class'][] = 'private-discussion';
  unset($form['field_sp_user_accepts_terms']);
  unset($form['account']['name']);
  unset($form['account']['pass']);
  if (!empty($form['field_username_alt'])) {
    unset($form['field_username_alt']);
  }
  //unset($form['account']['mail']['#title']);
  //$form['account']['mail']['#attributes']['placeholder'] = t('Email address');
  $form['account']['mail']['#title'] = t('Email address');
  $form['account']['mail']['#description'] = '';

  $form['actions']['submit']['#value'] = 'Register';
  $form['#validate'] = array('_smartparticipation_core_private_discussion_register_form_validate');
  $form['#submit'] = array('_smartparticipation_core_private_discussion_register_form_submit');
}

function _smartparticipation_core_private_discussion_register_form_validate($form, &$form_state)
{
  $email = $form_state['values']['mail'];
  if ($error = user_validate_mail($email)) {
    form_set_error('mail', $error);
  } else {
    $uid = db_select('users', 'u')
      ->condition('mail', $email)
      ->fields('u', array('uid'))
      ->execute()
      ->fetchField();
    if (empty($uid)) {
      $query = "subject=Discussion Access Request&body=Full Name:";
      form_set_error(
        'mail',
        '<p>Oops, we can’t confirm that this email address belongs to someone eligible to join the discussion.</p>'
        . '<p>Let’s try something different.</p>'
        . '<p>Please email <b>your full name</b> to '
        . '<a href="mailto:'
        . variable_get('site_mail')
        . '?'
        . $query
        . '">'
        . variable_get('site_mail')
        . '</a>'
        . '.</p><p>We’ll get back to you with a login link as soon as we can confirm your information.</p>');
      //unset($form_state['complete form']['actions']['submit']);
      $form_state['complete form']['#attributes']['class'][] = 'email-not-found';
      $form_state['rebuild'] = TRUE;
      smartparticipation_core_log_server_event(array(
        'name' => 'private register email not found',
        'detail' => 'email: ' . $email,
      ));
    }
  }
}

function _smartparticipation_core_private_discussion_register_form_submit($form, &$form_state)
{
  // Do we have a user with this email?
  $email = $form_state['values']['mail'];
  $uid = db_select('users' , 'u')
    ->condition('mail' , $email)
    ->fields('u', array('uid'))
    ->execute()
    ->fetchField();
  if (!empty($uid)) {
    // Found: Send the welcome email
    $account = user_load($uid);
    _user_mail_notify('register_admin_created', $account);
    drupal_set_message(t('We’ve sent a login link to the email address you gave us.  Use this link to finish registering and join the discussion.'));
    smartparticipation_core_log_server_event(array(
      'name' => 'private register success',
      'detail' => 'email: ' . $email,
    ));
  } else {
    // Not found: Redirect them to the request access form
    $encoded_email = base64_encode($email);
    //drupal_goto('register/request');
  }

}

function smartparticipation_core_private_discussion_register_request_form($encoded_email=null)
{
  $email = '';
  if (!empty($email)) {
    $email = htmlspecialchars(base64_decode($encoded_email));
  }

  $form = array();

  $form['email'] = array(
    '#type' => 'textfield',
    '#title' => t('Email'),
    '#default_value' => $email,
  );

  $form['full_name'] = array(
    '#type' => 'textfield',
    '#title' => t('Full Name'),
  );

  $form['building'] = array(
    '#type' => 'textfield',
    '#title' => t('CCSD Building'),
  );

  $form['actions']['send'] = array(
    '#type' => 'submit',
    '#value' => t('Send request'),
  );

  $form['#submit'] = array('_smartparticipation_core_private_discussion_register_request_form_submit');

  return $form;
}

/**
 * Implements hook_form_FORM_ID_alter().
 * 
 * NB: The user_register_form applies both to users creating a new account on 
 * the front end, and account creation by an admin user on the back end. 
 */
function smartparticipation_core_form_user_register_form_alter(&$form, $form_state, $form_id) {

  if (user_is_anonymous() && smartparticipation_core_is_private_discussion()) {
    smartparticipation_core_prepare_private_discussion_register_form($form);
    return;
  }
  
  global $user;
  
  // A USER ACCOUNT IS BEING CREATED FROM THE BACK END BY ANOTHER USER
  if ($user->uid > 0) {
    // Remove Terms & Conditions checkbox from user account form
    $form['field_sp_user_accepts_terms']['#access'] = FALSE;
    // Must also make the field optional, else submission won't validate.
    $form['field_sp_user_accepts_terms'][LANGUAGE_NONE]['#required'] = 0;

    // Notify user of new account by default
    $form['account']['notify']['#default_value'] = 1;
    
  // A NEW USER IS REGISTERING AN ACCOUNT
  } else {
  
    smartparticipation_core_remove_form_field_descriptions($form['account']);
  
    // IMPORTANT: Do NOT change this value on the admin account creation form!
    // It prevents the form from submitting.
    $form['actions']['submit']['#value'] = 'Register';

    // Custom validation and submission handlers apply only to front end
    // registration form.
    $form['#validate'][] = '_smartparticipation_core_user_register_form_validate';
    $form['#submit'][] = '_smartparticipation_core_user_register_form_submit';
    
    // If user registers from topic page: present survey immediately after completion of login/registration.
    if ( (arg(0) == 'node') && is_numeric(arg(1)) ) {
      $current_node = node_load(arg(1));
      if($current_node->type == 'sp_topic') {
        $proposal_nid = smartparticipation_core_field_get_item_value('node', $current_node, 'field_sp_topic_proposal', 'nid', $langcode = 'LANGUAGE_NONE');
        $proposal = node_load($proposal_nid);
    
        $survey_id = smartparticipation_core_field_get_item_value('node', $proposal, 'field_sp_interest_survey', 'nid', $langcode = 'LANGUAGE_NONE');
        if (isset($survey_id)) {
          $form['survey_id'] = array(
              '#type' => 'hidden',
              '#value' => $survey_id,
          );
          $form['#submit'][] = '_smartparticipation_core_interest_survey_submit';
        }
      }
    }
  }

  $form['field_sp_user_site_emails']['#access'] = FALSE;
  $form['field_sp_user_comment_emails']['#access'] = FALSE;

  if (!empty($form['field_username_alt'])) {
    $form['field_username_alt']['#access'] = FALSE;
    $form['field_username_alt'][LANGUAGE_NONE]['#required'] = 0;
  }
  
  $form['#attributes']['autocomplete'] = 'off';

}


/**
 * Implement hook_form_user_pass_alter().
 */
function smartparticipation_core_form_user_pass_alter(&$form, $form_state, $form_id) {
  
  smartparticipation_core_remove_form_field_descriptions($form);

  $form['name']['#title'] = 'Username or email';

  $form['#submit'][] = '_smartparticipation_core_user_pass_submit';

  $form['#validate'][] = '_smartparticipation_core_user_pass_validate';
}

/**
 * Customize Lost Password form submit handler
 * Prevents login dialog from appearing after lost password for is submitted
 */
function _smartparticipation_core_user_pass_submit($form, &$form_state) {
  $form_state['redirect'] = current_path();
}

/**
 * Customize Lost Password form validation
 */
function _smartparticipation_core_user_pass_validate($form, $form_state) {
  $errors = form_get_errors();

  // Customize the validation error message when the username or e-mail is not found.
  if ($errors) {
    form_clear_error();

    foreach ($errors as $field => $error_message) {
      if ($field == 'name') {
        $register = l(t('register'), 'user/register', array('attributes' => array('title' => t('Register a new account'))));
        $error_message = t('We don\'t recognize that username or e-mail address. Please <a href="@register">register a new account</a>.',
            array(
              '@register' => url('user/register'),
            )
        ); 
      }
      form_set_error($field, $error_message);
    }
  }

  drupal_get_messages('error', TRUE);
}

/**
 * Custom registration form validation handler.
 */
function _smartparticipation_core_user_register_form_validate($form, &$form_state) {

  global $user;

  $errors = form_get_errors();

  // Customize the validation error message for the Terms & Conditions checkbox.
  if ($errors) {
    form_clear_error();

    foreach ($errors as $field => $error_message) {
      if ($field == 'field_sp_user_accepts_terms][und') {
        $error_message = t('You must agree to the site terms and conditions of use in order to register.');
      }
      form_set_error($field, $error_message);
    }
  }

  // Clear the Drupal error message stack
  drupal_get_messages('error', TRUE);

}

/**
 * Custom login form validation handler.
 */
function _smartparticipation_core_user_login_validate($form, &$form_state) {
  
  // Clear the Drupal error message stack
  drupal_get_messages('error', TRUE);
}


/**
 * Custom registration form submit handler.
 */
function _smartparticipation_core_user_register_form_submit($form, &$form_state) {

  // A pretty ugly hack to substitute logintoboggan's hard-coded registration 
  // message with our own.
  $messages = drupal_get_messages('status');
  $status_messages = $messages['status'];
  foreach ($status_messages as $message) {
    if ($message == t('Further instructions have been sent to your e-mail address.')) {
      $message = t('Welcome! Your @site-name account has been created.', array(
          '@site-name' => variable_get('site_name'),
      ));
    } 
    drupal_set_message($message, 'status');
  }

}

/**
 * Return true if and only if user has a role that requires accepting terms and
 * conditions in order to complete account creation.
 */
function smartparticipation_core_user_must_accept_terms() {
  // Currently the same as an authenticated user, but in the future we might
  // have custom roles such as "expert" which still require agreeing to terms.
  return smartparticipation_core_is_basic_authenticated_user();
}

/**
 * Implement hook_form_complete_profile_form_alter().
 *
 * Alter the complete profile form for completion of registration.
 */
function smartparticipation_core_form_complete_profile_form_alter(&$form, $form_state, $form_id) {
  // The accepts terms field is present so the user is completing registration.
  if (isset($form['field_sp_user_accepts_terms'])) {

    // Change the page title.
    drupal_set_title(t('Complete your registration'));

    $vars = array();
    $has_email_field = empty($form['account']['mail']['#default_value']);
    // Check if the email field is present (Twitter doesn't send emails).
    if ($has_email_field) {
      $vars['email'] = TRUE;
    }
    $has_username_field = FALSE;
    $username_field_exists = smartparticipation_core_username_field_exists();
    if ($username_field_exists) {
      $has_username_field = empty($form['field_username_alt'][LANGUAGE_NONE][0]['value']['#default_value']);
    }
    if ($has_username_field) {
      $vars['username'] = TRUE;
    }

    // Add the complete registration notice.
    $form['help']['#markup'] = theme('accept_terms_notice', $vars);

    // Add the delete account link.
    $form['actions']['#suffix'] = theme('does_not_accept_terms');

    // Use the custom validation used on the registration form.
    // TODO should this not use the same validation function as the registration form?
    $form['#validate'][] = '_smartparticipation_core_complete_profile_form_validate';
    $form['#validate'][] = '_smartparticipation_core_user_register_form_validate';
    $form['#submit'][] = '_smartparticipation_core_complete_profile_form_submit';
    if ($username_field_exists) {
      $form['#submit'][] = '_smartparticipation_core_complete_profile_username_field_exists_submit';
    }
    if ($has_email_field) {
      $form['#submit'][] = '_smartparticipation_core_user_register_complete_profile_form_with_email_submit';
    }

  }

}

function _smartparticipation_core_complete_profile_username_field_exists_submit($form, &$form_state)
{
  _smartparticipation_core_send_registration_email($form['#user'], true);
}

function _smartparticipation_core_complete_profile_form_submit($form, &$form_state)
{
  smartparticipation_core_update_username($form_state);
}

function smartparticipation_core_update_username($form_state)
{
  if (!empty($form_state['values']['field_username_alt'][LANGUAGE_NONE][0]['value'])) {
    global $user;
    $username = $form_state['values']['field_username_alt'][LANGUAGE_NONE][0]['value'];
    $result = db_update('users')
      ->fields(array('name' => $username))
      ->condition('uid', $user->uid)
      ->execute();
    module_load_include('inc', 'pathauto'); // include the inc file for Pathauto module
    /*
       pathauto_create_alias($module, $op, $source, $data, $type, $language)
       @param $module: Your module name
       @param $op: The operation (insert, update, return, bulkupdate)
       @param $source: Internal Drupal path (i.e.: node/1)
       @param $data: Addition data used to generate a pathalias, in this case the node object
       @param $type: Type of content that will be used to match a pattern, defined in the backend url alias patterns
       @param $language: If you have a multlingual website, you need to pass the language for which you want to generate the url alias
    */
    //$user->name = $username;
    //pathauto_create_alias('smartparticipation_core', 'update', 'node/'.$user->uid, array('user' => $user));
    $alias = 'users/'.$username;
    $source = 'user/'.$user->uid;
    $alias = pathauto_clean_alias($alias);
    pathauto_alias_uniquify($alias, $source, LANGUAGE_NONE);
    $result = db_update('url_alias')
      ->fields(array('alias' => $alias))
      ->condition('source', $source)
      ->execute();
  }
}

function _smartparticipation_core_complete_profile_form_validate($form, &$form_state)
{
  smartparticipation_core_validate_username($form_state);
}

function smartparticipation_core_validate_username($form_state)
{
  if (!empty($form_state['values']['field_username_alt'][LANGUAGE_NONE][0]['value'])) {
    global $user;
    $username = $form_state['values']['field_username_alt'][LANGUAGE_NONE][0]['value'];
    $uid = db_select('users' , 'u')
      ->condition('name' , $username)
      ->condition('name', $user->name, '!=')
      ->fields('u', array('uid'))
      ->execute()
      ->fetchField();
    if (!empty($uid)) {
      form_set_error('field_username_alt][und][0][value', t('This username is already in use.'));
    }
  }
}

/**
 * A submit handler for the complete profile form when a user must enter their email.
 *
 * The registration welcome email is delayed since the social media service did not provide an email address.
 */
function _smartparticipation_core_user_register_complete_profile_form_with_email_submit($form, &$form_state) {
  _smartparticipation_core_send_registration_email($form['#user'], true);
}

/**
 * If a user registers using a social media account or their account is created
 * by the admin they are asked to accept terms or delete their account before
 * using the site.  This action handles deletion of their account and redirects
 * them to the front page.
 */
function smartparticipation_core_user_does_not_accept_terms($action = null) {

  global $user;
  
  // The front page.
  $redirect_path = drupal_get_normal_path(variable_get('site_frontpage', 'node'));

  // This success action is a hack to to display the success message after
  // deleting the account.  Normally it seems we can use drupal_set_message
  // immediately before drupal_goto and the message will display on the
  // redirected page.  Some sort of conflict seems to be preventing the
  // message display in this case (possibly the Complete Profile module).
  if ($action == 'success') {

    drupal_set_message(t('Your account has been deleted.'));

  } else {

    // Delete the user account if permitted.
    // This check is also done through the access callback but I thought I'd
    // "double-up" on the check to prevent it from being called internally
    // and unintentionally wiping out a user account.
    if (_smartparticipation_core_user_can_delete_own_account()) {

      user_delete($user->uid);

      // Return to this function with the success action set.  See above.
      $redirect_path = 'user_does_not_accept_terms/success';

    }
  }

  drupal_goto($redirect_path);
}

/**
 * Return true iff the user account is in a state in which it can be deleted.
 *
 * @return bool
 */
function _smartparticipation_core_user_can_delete_own_account() {

  global $user;
  
  if (!user_is_anonymous()) {

    // Make sure they are a basic user who would normally be required to accept terms.
    if (smartparticipation_core_user_must_accept_terms()) {

      // TODO Isn't this just $user?
      $account = user_load($user->uid);

      // Make sure the user hasn't already accepted the terms.
      if (empty($account->field_sp_user_accepts_terms)
          || !$account->field_sp_user_accepts_terms[LANGUAGE_NONE][0]['value']) {

        return TRUE;

      }
    }
  }

  return FALSE;
 
}

/**
 * Implements hook_user_login().
 */
function smartparticipation_core_user_login(&$edit, $account) {
  
  smartparticipation_core_log_user_login($account);

}

/**
 * Record the user login event in the event log. Note that logging this event
 * cannot be requested by the client, or in a custom login form submit handler,
 * because at those stages we don't yet know the uid. 
 * 
 * @return boolean
 */
function smartparticipation_core_log_user_login() {
  
  global $new_registration;
  
  // Do not record the login event that immediately follows a new registration.
  if ($new_registration) {
    return false;
  }
  
  // Recording the login event cannot be triggered by the client, or in 
  // a custom login form submit handler, because we don't yet know the uid.
  $event = array(
    'name' => 'user_login',  
  ) + smartparticipation_core_login_or_registration_path();

  return smartparticipation_core_log_server_event($event);
}

/**
 * Return the path of the page at which the user registers or logs in. 
 * Registration/login via social media requires a modification of the 
 * request path; the social media service is then stored in the detail field.
 * 
 * @return array containing the event path and possibly an event detail
 */
function smartparticipation_core_login_or_registration_path() {
  
  $event = array();
  
  $path = request_path();

  // Check if the user has logged in with social media.
  if (strpos($path,'hybridauth') !== false) {
    
    // Add the social media service as an event detail.
    $path_parts = explode('/', $path);
    $event['detail'] = $path_parts[2];

    // Set the path to page user will be returned to.
    $path = drupal_get_path_alias($_GET['destination']);

    // The home page will be returned as 'node'
    if ($path == 'node') {
      $path = '';
    }
 
  }
  
  $event['path'] = '/' . $path;

  return $event;

}

/**
 * Log when a user registers a new account. Must be triggered from the server
 * rather than the client in order to retrieve the new uid, which is not yet
 * stored in global $user. By the same token, this function is called by 
 * smartparticipation_core_user_insert() rather than by a custom registration form submit 
 * handler, because in the submit handler we do not yet have the new uid. 
 * (There is no hook_user_register().)
 * 
 * @param $account - the newly registered account
 * 
 * @return false if the account has been created by another user; otherwise, 
 * the return value of smartparticipation_core_log_server_event().
 */
function smartparticipation_core_log_user_registration($account) {
  
  global $user, $new_registration;

  $result = false;
  
  // Log the registration event only when a user registers his own account, not
  // someone else's. Note that in this case $user->uid is still 0 (and therefore 
  //  logging must be handled from the server side to retrieve the new uid).
  if ($user->uid == 0) {
    
    // Set a flag to block logging of the login event that immediately follows
    // registration.
    $new_registration = true;
    
    $detail = '';
    if (isset($_COOKIE['Drupal_visitor_oid'])) {
      $detail = 'oid: ' . $_COOKIE['Drupal_visitor_oid'];
    }
    
    $event = array(
      'detail' => $detail,
      'name' => 'user registration',
      'uid' => $account->uid,       
    ) + smartparticipation_core_login_or_registration_path();
    
    $result = smartparticipation_core_log_server_event($event);
  }
  
  return $result;
}

function smartparticipation_core_form_user_pass_reset_alter(&$form, $form_state, $form_id)
{
  $uid = intval(arg(2));
  $account = null;
  if (!empty($uid)) {
    $account = user_load($uid);
  }
  if (!empty($account)) {
    if (smartparticipation_core_user_must_provide_username($account)) {
      drupal_set_title(t('Register account'));
      $form['message'] = array('#markup' => t('<p>Click on this button to log in to the site where you will be asked to create a username and set your password.</p>'));
    }
  }
}

function smartparticipation_core_is_private_discussion()
{
  if (variable_get('site_403', '') == 'toboggan/denied') {
    return true;
  }
  return false;
}