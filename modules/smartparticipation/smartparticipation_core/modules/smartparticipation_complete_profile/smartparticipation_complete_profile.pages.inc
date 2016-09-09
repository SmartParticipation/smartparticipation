<?php

function smartparticipation_complete_profile_form($form, &$form_state, $account) {
  $form_state['user'] = $account;
  $form['#user'] = $account;
  // Category used by profile module.
  $form['#user_category'] = 'register';

  // Attach the regular user form.
  user_account_form($form, $form_state);

  // Attach the user field form elements.
  $langcode = function_exists('entity_language') ? entity_language('user', $form['#user']) : NULL;
  field_attach_form('user', $account, $form, $form_state, $langcode);

  $instances = field_info_instances('user', 'user');
  foreach (element_children($form) as $key) {
    if (!isset($instances[$key])) {
      $form[$key]['#access'] = FALSE;
    }
    else {
      if (!empty($instances[$key]['required'])) {
        // If the user has already filled out a value for this field, hide it.
        if (!smartparticipation_complete_profile_field_is_empty('user', $account, $key)) {
          $form[$key]['#access'] = FALSE;
        }
      }
      elseif (empty($instances[$key]['user_register_form'])) {
        // Hide any fields not configured to show up on the registration form.
        $form[$key]['#access'] = FALSE;
      }
    }
  }

  // Support the deprecated profile module.
  if (module_exists('profile')) {
    profile_form_alter($form, $form_state, 'user_register_form');
    // Determine if each profile field should actually be shown or not.
    foreach (_profile_get_fields($form['#user_category'], TRUE) as $profile_field) {
      if (!smartparticipation_complete_profile_profile_field_is_empty($account, $profile_field)) {
        $form[$profile_field->category][$profile_field->name]['#access'] = FALSE;
        $form[$profile_field->category]['#access'] = (bool) element_get_visible_children($form[$profile_field->category]);
      }
    }
  }

  if (empty($account->mail)) {
    $form['account'] = array(
      '#type' => 'container',
      '#weight' => -10,
    );
    $form['account']['mail'] = array(
      '#type' => 'textfield',
      '#title' => t('E-mail address'),
      '#maxlength' => EMAIL_MAX_LENGTH,
      '#description' => t('A valid e-mail address. All e-mails from the system will be sent to this address. The e-mail address is not made public and will only be used if you wish to receive a new password or wish to receive certain news or notifications by e-mail.'),
      '#required' => TRUE
    );
  }

  $form['help'] = array(
    '#markup' => '<p>' . t('Fill out the following fields to complete your profile.') . '</p>',
    '#weight' => -100,
  );

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );

  form_load_include($form_state, 'inc', 'user', 'user.pages');
  $form['#validate'][] = 'user_profile_form_validate';
  $form['#submit'][] = 'user_profile_form_submit';

  return $form;
}
