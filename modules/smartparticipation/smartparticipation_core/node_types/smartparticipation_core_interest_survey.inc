<?php

/**
 * @file 
 * Define the interest survey node type.
 * 
 * @ingroup smartparticipation_core
 */
 
/**
 * Create the interest survey node type.
 */
function smartparticipation_core_create_interest_survey_node_type() {
  
  $t = get_t();
  
  $interest_survey = array(
    'type' => 'sp_interest_survey',
    'name' => $t('Interest survey'),
    // Drupal will handle this node as if we had designed it in the UI.
    'base' => 'node_content',
    'has_title' => TRUE,
    'title_label' => $t('Title'),
    'description' => $t('Interest survey for a proposal that a user must complete before joining the discussion.'),
    'custom' => TRUE,
    'help' => '',
  );  
  
  // Set defaults not explicitly declared above.
  $node_type = node_type_set_defaults($interest_survey);
  
  node_type_save($node_type);

  // Interest surveys do not receive comments directly.
  variable_set('comment_sp_interest_survey', COMMENT_NODE_HIDDEN);
 
}


/**
 * Delete the interest survey node type.
 */
function smartparticipation_core_delete_interest_survey_node_type() {
  smartparticipation_core_delete_node_type('sp_interest_survey');
}


function _smartparticipation_core_interest_survey_submit($form, &$form_state) {
  $form_state['redirect'] = 'node/'. $form['survey_id']['#value'];
}

/**
 * Load the interest survey into a dialog, and add the required JavaScript to 
 * the page.
 * 
 * @param $interest_survey_nid - the nid of the interest survey node
 * 
 * @return the themed interest survey dialog
 */
function smartparticipation_core_get_rendered_interest_survey($interest_survey_nid) {

  // Load the JS needed to open the survey dialog.
  $module_path = drupal_get_path('module', 'smartparticipation_core');
  $js_path = $module_path . '/js/';
//  drupal_add_js($js_path . 'plugins/dialog.js');
//  drupal_add_js($js_path . 'survey.js');
  
  // Store the survey rendered inside of a dialog.
  $survey = node_view(node_load($interest_survey_nid));
  
  return theme('dialog', array(
    'dlgId' => 'dlgInterestSurvey',
    'content' => array(
      drupal_render($survey),
    )
  ));
  
}


/**
 * Return the rendered interest survey iff the specified user must submit the 
 * interest survey for the specified proposal.
 * 
 * @param $account - the user object
 * @param $proposal - the proposal node
 * 
 * @return if the user must submit the survey, return the rendered survey; else
 * return the empty string.
 */
function smartparticipation_core_load_proposal_interest_survey($account, $proposal) {
  
  $survey = '';
  
  if (smartparticipation_core_is_basic_authenticated_user($account)) {

    // Returns null if there is no interest survey for the proposal, or if the
    // survey is not published.
    $interest_survey_nid = smartparticipation_core_proposal_interest_survey($proposal);

    if (isset($interest_survey_nid) &&
        !_smartparticipation_core_user_submitted_survey($account->uid, $interest_survey_nid)) {
      $survey = smartparticipation_core_get_rendered_interest_survey($interest_survey_nid);
    } 
  }
  
  return $survey;

}

/**
 * Implements hook_webform_node_defaults_alter().
 * Set default webform configuration settings
 *
 * @param $defaults - Default values for the form which are overridden here
 */
function smartparticipation_core_webform_node_defaults_alter(&$defaults){

  $defaults['redirect_url'] = '<none>';
  $defaults['submit_limit'] = '1';
  $defaults['roles'] = array ('2');

}
