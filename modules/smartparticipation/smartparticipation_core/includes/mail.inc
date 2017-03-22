<?php 

/**
 * SmartParticipation mail functions.
 */

/**
 * Implements hook_mail().
 *
 * This hook defines a list of possible e-mail templates that this module can
 * send. Each e-mail is given a unique identifier, or 'key'.
 *
 * $message comes in with some standard properties already set: 'to' address,
 * 'from' address, and a set of default 'headers' from drupal_mail(). The goal
 * of hook_mail() is to set the message's 'subject' and 'body' properties, as
 * well as make any adjustments to the headers that are necessary.
 *
 * The $params argument is an array which can hold any additional data required
 * to build the mail subject and body; for example, user-entered form data, or
 * some context information as to where the mail request came from.
 *
 * Note that hook_mail() is not actually a hook. It is only called for a single
 * module, the module named in the first argument of drupal_mail(). So it's
 * a callback of a type, but not a hook.
 */
function smartparticipation_core_mail($key, &$message, $params) {
  global $user;

  // Each message is associated with a language, which may or may not be the
  // current user's selected language, depending on the type of e-mail being
  // sent. This $options array is used later in the t() calls for subject
  // and body to ensure the proper translation takes effect.
  $options = array(
    'langcode' => $message['language']->language,
  );

  // Construct subject and body based on $key.
  switch ($key) {

    case 'smartparticipation_core_submit_faq_form':
      $message = smartparticipation_core_submit_faq_mail($message, $params, $options);
      break;
  }
}

/**
 * Custom SmartParticipation mail wrapper function.
 * 
 * @param $from - the email from address, or 'default_from' to get the site
 * default from address
 * @param $to - the email recipient address
 * @param $subject - the email subject
 * @param $body - the email body
 * @param $data - an array of keyed objects for token replacement. Each
 * array element maps object type to object.
 * 
 */
function _smartparticipation_core_drupal_mail($from, $to, $subject, $body, $data = null) {
  $my_module = 'smartparticipation_core';
  $my_mail_token = microtime();

  if ($from == 'default_from') {
    $from = variable_get('smtp_from', variable_get('site_mail'));
  }
 
  $body = smartparticipation_core_append_mail_signature($body);
  
  // If $data includes a comment:
  // Get the comment's proposal node, but don't override a node already 
  // specified in data.
  // Also do comment permalink replacement.
  if (array_key_exists('comment', $data)) {
    $comment = $data['comment'];
    
    $body = smartparticipation_core_token_replace_comment_permalink($body, $comment);
  
    if (! array_key_exists('node', $data)) {
      $subtopic_nid = $comment->nid;
      $proposal = smartparticipation_core_get_subtopic_proposal($subtopic_nid);
      $data['node'] = $proposal;
    }
  }

  $subject = token_replace($subject, $data);
  $body = token_replace($body, $data, array('sanitize'=>false));
  // CKEditor inserts paragraph tags.  Use a double newline instead for
  // the plain text email.
  $body = str_replace('</p>', "\n\n", $body);
  // Remove HTML from things like comment text.
  $body = strip_tags($body);
  // Replace any converted entities.
  $body = html_entity_decode($body, ENT_QUOTES, "UTF-8");

  $message = array(
    'id' => $my_module . '_' . $my_mail_token,
    'from' => $from,
    'to' => $to,
    'subject' => $subject,
    'body' => array($body),
    'headers' => array(
    'Content-Type' => 'text/plain; charset=UTF-8;',
    //'Content-Type' => 'text/html; charset=utf-8',
    'From' => $from,
    'Sender' => $from,
    'Return-Path' => $from,
    ),
  );
  
  $system = drupal_mail_system($my_module, $my_mail_token);
  $message = $system->format($message);

  return $system->mail($message) ? TRUE : FALSE;

}


/**
 * Append site email signature to an email body.
 */
function smartparticipation_core_append_mail_signature($message) {
  
  $signature = variable_get('smartparticipation_mail_signature');
  
  /*
   * Can't do this right now because sending $message back as an array
   * breaks some of the mail functions. These need to be re-implemented to
   * allow $message to be an array.
  if ($signature) {
    if (!is_array($message)) {
      $message = array($message);
    }
    $message[] = $signature;
  }
  */
  
  if ($signature) {
    if (is_array($message)) {
      $message[] = $signature;
    } else {
      $message .= "\n\n\n$signature";
    }
  }
  
  return $message;
}

/**
 * Set default SmartParticipation email templates.
 *
 * Variables set:
 *
 *  smartparticipation_mail_signature
 *  smartparticipation_mail_registration_subject
 *  smartparticipation_mail_registration_body
 *  smartparticipation_mail_first_comment_subject
 *  smartparticipation_mail_first_comment_body
 *  smartparticipation_mail_comment_reply_subject
 *  smartparticipation_mail_comment_reply_body
 *  smartparticipation_mail_comment_endorsement_subject
 *  smartparticipation_mail_comment_endorsement_body
 *  smartparticipation_mail_comment_recommendation_subject
 *  smartparticipation_mail_comment_recommendation_body
 *
 */
function smartparticipation_core_set_default_email_templates()
{
  // SIGNATURE

  variable_set(
    "smartparticipation_mail_signature",
    "Regards,
The [site:name] Team"
  );

  // REGISTRATION

  variable_set(
    "smartparticipation_mail_registration_subject",
    "Welcome to [site:name]!"
  );

  variable_set(
    "smartparticipation_mail_registration_body",
    "Dear [user:name],

Welcome! You're now registered as part of the to join the discussion on [site:name]. 

Your username is [user:name]. You can change your password and make other modifications to your account at any time at [site:url]user/[user:uid]/edit. If you forget your password, you can reset it at [site:url]user/password.

You can find the Site Use Guidelines and other important information at [site:url]terms.

We look forward to your joining the discussion!"
  );

  // FIRST COMMENT

  variable_set(
    "smartparticipation_mail_first_comment_subject",
    "Welcome to the [node:title] discussion on [site:name]!"
  );

  variable_set(
    "smartparticipation_mail_first_comment_body",
    "Dear [comment:author:name],

Welcome to the [node:title] discussion on [site:name]!  We're glad you have decided to join the discussion, and we hope you check back frequently to watch how it evolves."
  );

  // COMMENT REPLY

  variable_set(
    "smartparticipation_mail_comment_reply_subject",
    "Someone has replied to a comment you made on [site:name]"
  );

  variable_set(
    "smartparticipation_mail_comment_reply_body",
    "Dear [comment:parent:author:name],

Someone has replied to one of your comments on the [node:title] discussion at [site:name].  Your comment reads:

[comment:parent:body]

On [comment:created:long], [comment:author:name] replied:

[comment:body]

You can view the discussion thread at [comment:parent:permalink].

You can see all the replies your comments have received on your profile at [comment:parent:author:url].

Keep the discussion going at [site:url]!"
  );

  // COMMENT ENDORSEMENT

  variable_set(
    "smartparticipation_mail_comment_endorsement_subject",
    "Someone endorsed a comment you made on [site:name]"
  );

  variable_set(
    "smartparticipation_mail_comment_endorsement_body",
    "Dear [comment:author:name],

Someone has endorsed one of your comments on in the [node:title] discussion at [site:name] for doing a good job of making a good point! Your endorsed comment reads:

[comment:body]

You can view the discussion thread at [comment:permalink].

You can see all your comment and endorsements on your profile at [comment:author:url]."
  );

  // COMMENT RECOMMENDATION

  variable_set(
    "smartparticipation_mail_comment_recommendation_subject",
    "The Moderator recommended a comment you made on [site:name]"
  );

  variable_set(
    "smartparticipation_mail_comment_recommendation_body",
    "Dear [comment:author:name],

The [site:name] moderator has recommended one of your comments as a good example of effective commenting.  Thanks for helping new participants learn how to make comments that count!  

Your recommended comment reads:

[comment:body]

You can view the discussion thread at [comment:permalink].

You can see all your comments and moderator recommendations on your profile at [comment:author:url]."
  );

}

