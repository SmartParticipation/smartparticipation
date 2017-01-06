<?php

/**
 * SmartParticipation form functions.
 */
 
/**
 * Implements hook_form_alter().
 */
function smartparticipation_core_form_alter(&$form, &$form_state, $form_id) {

  if ($form_id == 'comment_form' || $form_id == 'comment_node_sp_subtopic_form') {
    if (!user_access('administer comments')) {
      $form['field_sp_comment_mod_status']['#access'] = FALSE;
      $form['field_sp_comment_mod_action']['#access'] = FALSE;
      $form['field_sp_comment_original']['#access'] = FALSE;
    }

    if (!isset($form['cid']['#value'])) {
      $form['field_sp_comment_original']['#access'] = FALSE;
    } else {
      $comment = comment_load($form['cid']['#value']);
      $comment_body = $comment->comment_body[LANGUAGE_NONE][0]['value'];
      if (!isset($comment->field_sp_comment_original[LANGUAGE_NONE][0]['value'])) {
        $comment_original = $comment_body;
      } else {
        $comment_original = $comment->field_sp_comment_original[LANGUAGE_NONE][0]['value'];
      }

      $form['field_sp_comment_original'][LANGUAGE_NONE][0]['value']['#default_value'] = $comment_original;
      $form['field_sp_comment_original']['#disabled'] = TRUE;
    }

    $form['comment_body']['#after_build'][] = '_smartparticipation_core_hide_format_comments';
  }
}

/**
 * Implements hook_element_info_alter().
 */
function smartparticipation_core_element_info_alter(&$type) {
  // Remove user_form_process_password_confirm
  array_pop($type['password_confirm']['#process']);

  if (isset($type['text_format']['#process'])) {
    foreach ($type['text_format']['#process'] as &$callback) {
      if ($callback === 'filter_process_format') {
        $callback = 'smartparticipation_core_filter_process_format';
      }
    }
  }
}

/**
 * Callback specifying default text filter formats.
 */
function smartparticipation_core_filter_process_format($element) {
  $element = filter_process_format($element);
  // Change default format for body field of all node types to full_html.
  if (isset($element['#entity_type']) && $element['#entity_type'] == 'node' && 
      $element['#field_name'] == 'body') {
    $element['format']['format']['#default_value'] = 'full_html';
  }
  
  return $element;
}

/**
 * Remove form field descriptions from a form. Doesn't handle fields nested 
 * inside fieldsets or other elements, so the function should be passed only the 
 * part of the form array directly containing the fields to be altered.
 * 
 * @param $form - the form or part of a form containing the fields
 * @param $exceptions - an array of field names to be exempted 
 * 
 * @return the modified form
 */
function smartparticipation_core_remove_form_field_descriptions(&$form, $exceptions = array()) {

  if (!empty($form)) {
    foreach ($form as $key => $value) {
      if ($key[0] != '#' && is_array($value) && isset($value['#description']) &&
        !in_array($key, $exceptions)
      ) {
        unset($form[$key]['#description']);
      }
    }
  }

}

/**
 * Implements hook_forms().
 *
 * @param $form_id
 * @param $args
 * @return array
 */
function smartparticipation_core_forms($form_id, $args)
{
  $forms = array();
  switch ($form_id) {
    case 'smartparticipation_core_profile_form':
      $forms[$form_id] = array(
        'callback' => $form_id,
      );
  }
  if (! empty($forms)) {
    return $forms;
  }
}
