<?php

/**
 * @file 
 * Define the faq node type.
 * 
 * @ingroup smartparticipation_core
 */

/**
 * Page callback for Contact Us page
 */
function smartparticipation_core_contact_page() {


  $data['site_name'] = variable_get('site_name');
  $data['site_email'] = variable_get('site_mail');

  $vals = variable_get('smartparticipation_contact_organization');
  $data['organization'] = $vals['value'];

  $data['address'] = nl2br(variable_get('smartparticipation_contact_address'));
  $data['phone'] = variable_get('smartparticipation_contact_phone');

  $vals = variable_get('smartparticipation_contact_alternative');
  $data['alternative_contact'] = $vals['value'];

  //$data['form'] = drupal_get_form('smartparticipation_core_submit_faq_form');

  // Return a render array
  return array(
    '#items' => $data,
    '#theme' => 'contact',
  );  

}

/**
 * The Contact form.
 */
function smartparticipation_core_contact_form() {

  $form = array();

  $form['name'] = array(
      '#type' => 'textfield',
      '#title' => t('Your name'),
      '#required' => TRUE,
      '#size' => 20,
      '#weight' => 0,
  );

  $form['email'] = array(
      '#type' => 'textfield',
      '#title' => t('Email address'),
      '#required' => TRUE,
      '#size' => 20,
      '#weight' => 5,
  );

  $form['subject'] = array(
      '#type' => 'textfield',
      '#title' => t('Subject'),
      '#required' => TRUE,
      '#size' => 20,
      '#weight' => 10,
  );

  $form['message'] = array(
      '#type' => 'textarea',
      '#title' => t('Message'),
      '#required' => TRUE,
      '#resizable' => FALSE,
      '#weight' => 15,
  );

  $form['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Send'),
      '#weight' => 20,

  );

  return $form;
}

/**
 * Implements hook_form_FORM_ID_alter().
 */
function smartparticipation_core_form_sp_contact_form_alter(&$form, &$form_state, $form_id) {
  honeypot_add_form_protection($form, $form_state, array('honeypot', 'time_restriction'));
}

/**
 * Form validation logic for the contact form.
 */
function smartparticipation_core_contact_form_validate($form, &$form_state) {
  if (!valid_email_address($form_state['values']['email'])) {
    form_set_error('email', t('Please enter a valid email address.'));
  }

  // Clear the error message array, since these will be displayed with the
  // individual form fields.
  drupal_get_messages('error', TRUE);
}

/**
 * Form submission logic for the contact form.
 */
function smartparticipation_core_contact_form_submit($form, &$form_state) {

  smartparticipation_core_contact_mail_send($form_state['values']);

}


/**
 * Sends an e-mail in response to the submit faq form.
 *
 * @param $form_values
 *   An array of values from the contact form fields that were submitted.
 *   There are just two relevant items: $form_values['email'] and
 *   $form_values['message'].
 */
function smartparticipation_core_contact_mail_send($form_values) {

  // Specify 'to' and 'from' addresses.
  $to = variable_get('site_mail');
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

  $site_name = variable_get('site_name', 'SmartParticipation');
  $options = array('langcode' => $language->language,);

  $subject = t('Someone has submitted a Contact message to @site-name', array('@site-name' => $site_name), $options);

  $body = t("Someone has submitted a Contact message to @site-name.\n\n",
      array('@site-name' => $site_name), $options);

  // Because this is just user-entered text, we do not need to translate it.
  // Since user-entered text may have unintentional HTML entities in it like
  // '<' or '>', we need to make sure these entities are properly escaped,
  // as the body will later be transformed from HTML to text, meaning
  // that a normal use of '<' will result in truncation of the message.
  $body .= t('Name: ') . check_plain($params['name']) . "\n\n";

  $body .= t("Email address: @email\n\n",
      array('@email' => $params['email']), $options);

  $body .= t('Subject: ') . check_plain($params['subject']) . "\n\n";

  $body .= t('Message: ') . nl2br(check_plain($params['message']));

  $result = _smartparticipation_core_drupal_mail($from, $to, $subject, $body, array());

  if ($result) {
    drupal_set_message(t('Your message has been sent. Thank you for your interest in @site-name.',
        array('@site-name' => variable_get('site_name'))));
  }
  else {
    drupal_set_message(t('There was a problem sending your message and it was not sent.'), 'error');
  }
}
