<?php
/**
 * @file 
 * New FAQ submission form
 */

/**
 * Sends an e-mail in response to the submit faq form.
 *
 * @param $form_values
 *   An array of values from the contact form fields that were submitted.
 *   There are just two relevant items: $form_values['email'] and
 *   $form_values['message'].
 */
function smartparticipation_core_submit_faq_mail_send($form_values) {
  // All system mails need to specify the module and template key (mirrored from
  // hook_mail()) that the message they want to send comes from.
  $module = 'smartparticipation_core';
  $key = 'smartparticipation_core_submit_faq_form';

  // Specify 'to' and 'from' addresses.
  $to = variable_get('site_mail', 'regulationroom-admin@cornell.edu');
  $from = $to;

  // "params" loads in additional context for email content completion in
  // hook_mail(). In this case, we want to pass in the values the user entered
  // into the form, which include the message body in $form_values['message'].
  $params = $form_values;

  // The language of the e-mail. This will one of three values:
  // * user_preferred_language(): Used for sending mail to a particular website
  //   user, so that the mail appears in their preferred language.
  // * global $language: Used when sending a mail back to the user currently
  //   viewing the site. This will send it in the language they're currently
  //   using.
  // * language_default(): Used when sending mail to a pre-existing, 'neutral'
  //   address, such as the system e-mail address, or when you're unsure of the
  //   language preferences of the intended recipient.

  // Since in our case, we are sending a message to a random e-mail address that
  // is not necessarily tied to a user account, we will use the site's default
  // language.
  $language = language_default();

  // Whether or not to automatically send the mail when drupal_mail() is
  // called. This defaults to TRUE, and is normally what you want unless you
  // need to do additional processing before drupal_mail_send() is called.
  $send = TRUE;
  // Send the mail, and check for success. Note that this does not guarantee
  // message delivery; only that there were no PHP-related issues encountered
  // while sending.

  //$result = drupal_mail($module, $key, $to, $language, $params, $from, $send);

  $site_name = variable_get('site_name', 'SmartParticipation');
  $options = array('langcode' => $language->language,);

  $subject = t('A new FAQ has been submitted to @site-name', array('@site-name' => $site_name), $options);

  // Note that the message body is an array, not a string.
  $body = t("A user has submitted a question to the @site-name FAQ page.\n\n",
      array('@site-name' => $site_name), $options);

  $body .= t("Email address: @email\n\n",
      array('@email' => $params['email']), $options);

  // Because this is just user-entered text, we do not need to translate it.
  // Since user-entered text may have unintentional HTML entities in it like
  // '<' or '>', we need to make sure these entities are properly escaped,
  // as the body will later be transformed from HTML to text, meaning
  // that a normal use of '<' will result in truncation of the message.
  $body .= t('Question: ') . check_plain($params['question']);

  $result = _smartparticipation_core_drupal_mail($from, $to, $subject, $body, array());

  if ($result) {
    drupal_set_message(t('Your question has been submitted. Thank you for your interest in @site-name.',
      array('@site-name' => variable_get('site_name', 'SmartParticipation'))));
  }
  else {
    drupal_set_message(t('There was a problem sending your message and it was not sent.'), 'error');
  }
}

/**
 * Construct the subject and body for the submit faq email.
 */
function smartparticipation_core_submit_faq_mail($message, $params, $options) {

  $site_name = variable_get('site_name', 'SmartParticipation');
  $message['subject'] = t('A new FAQ has been submitted to @site-name', array('@site-name' => $site_name), $options);
  
  // Note that the message body is an array, not a string.
  $message['body'][] = t('A user has submitted a question to the @site-name FAQ page.',
    array('@site-name' => $site_name), $options);
    
  $message['body'][] = t('Email address: @email', 
    array('@email' => $params['email']), $options);
    
  // Because this is just user-entered text, we do not need to translate it.
  // Since user-entered text may have unintentional HTML entities in it like
  // '<' or '>', we need to make sure these entities are properly escaped,
  // as the body will later be transformed from HTML to text, meaning
  // that a normal use of '<' will result in truncation of the message.
  $message['body'][] = t('Question: ') . check_plain($params['question']);
  
  return $message;
}

/**
 * The faq submission form.
 */
function smartparticipation_core_submit_faq_form() {

  $form = array();

  $form['email'] = array(
    '#type' => 'textfield',
    '#title' => t('Email address'),
    '#required' => TRUE,
    '#size' => 20,
    '#weight' => 0,
  );
  $form['question'] = array(
    '#type' => 'textarea',
    '#title' => t('Question'),
    '#required' => TRUE,
    '#resizable' => FALSE,
    '#weight' => 5,    
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Ask'),
    '#weight' => 10,  
      
    /* Ajax submission
    '#ajax' => array(
      'callback' => 'smartparticipation_core_submit_faq_form_submit',
      'wrapper' => // Add a div to hold the message,  
    ),  
    */
  );
  
  return $form;
}

/**
 * Implements hook_form_FORM_ID_alter().
 */
function smartparticipation_core_form_sp_submit_faq_form_alter(&$form, &$form_state, $form_id) {
  honeypot_add_form_protection($form, $form_state, array('honeypot', 'time_restriction'));
}



/**
 * Form validation logic for the faq submission form.
 */
function smartparticipation_core_submit_faq_form_validate($form, &$form_state) {
  if (!valid_email_address($form_state['values']['email'])) {
    form_set_error('email', t('Please enter a valid email address.'));
  }
  
  // Clear the error message array, since these will be displayed with the
  // individual form fields.
  drupal_get_messages('error', TRUE);
}

/**
 * Form submission logic for the faq submission form.
 */
function smartparticipation_core_submit_faq_form_submit($form, &$form_state) {
  
  // TODO Later: save the FAQ to the db.
  
  smartparticipation_core_submit_faq_mail_send($form_state['values']);
}

  