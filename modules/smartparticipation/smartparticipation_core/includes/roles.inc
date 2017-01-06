<?php

/**
 * @file
 * 
 * Define custom SmartParticipation roles.
 */

/**
 * Create custom SmartParticipation roles and assign permissions to them.
 */
function smartparticipation_core_create_custom_roles() {
  
  // NB Order matters here. Site editor gets permissions 'assign moderator role,'
  // 'assign report editor role', 'assign content editor role', which doesn't 
  // exist until the corresponding roles have been created. If this ends up 
  // being circular, we can assign role delegation permissions after creating 
  // all custom roles. 
  
  smartparticipation_core_create_moderator_role();
  // smartparticipation_core_create_proposal_editor_role();
  //smartparticipation_core_create_content_editor_role();
  //smartparticipation_core_create_site_editor_role();

}

/*
 * Create the proposal editor role and define its permissions. This role has
 * permissions similar to a content editor, but only in relation to a specific
 * proposal.
 * 
 * TODO Figure out how to implement proposal-level permissions. 
 * 
function smartparticipation_core_create_proposal_editor_role() {

  // Set the permissions for the PROPOSAL EDITOR role. This role can edit
  // documents related to a specified proposal or array of proposals.
  $role_proposal_editor = new stdClass();
  $role_proposal_editor->name = 'proposal editor';
  
  user_role_save($role_proposal_editor);
  user_role_grant_permissions($role_proposal_editor->rid, array(
  
  ));
}
*/
 
/** 
 * Create the content editor role and define its permissions. This role can
 * create both proposal-specific and some sitewide content, can edit proposal-
 * specific content, and edit or delete his or her own content.
 */
function smartparticipation_core_create_content_editor_role() {

  $role = new stdClass();
  $role->name = 'content editor';
  
  user_role_save($role);
  user_role_grant_permissions($role->rid, 
      smartparticipation_core_content_editor_permissions());
}



/**
 * Create the site editor role and define its permissions. This role can edit
 * site content, administer users, taxonomies, webforms, etc.
 */
function smartparticipation_core_create_site_editor_role() {

  $role = new stdClass();
  $role->name = 'site editor';

  user_role_save($role);
  user_role_grant_permissions($role->rid, 
      smartparticipation_core_site_editor_permissions());
}



/**
 * Create the moderator role and define its permissions. 
 */
function smartparticipation_core_create_moderator_role() {

  $role = new stdClass();
  $role->name = 'moderator';

  user_role_save($role);
  user_role_grant_permissions($role->rid, smartparticipation_core_moderator_permissions());
  
}

/**
 * Remove custom SmartParticipation roles when module is uninstalled.
 */
function smartparticipation_core_delete_custom_roles() {

  user_role_delete('content editor');
  //user_role_delete('site editor');
  user_role_delete('moderator');
  //user_role_delete('report editor');
  
}


/**
* Check to see if a user has been assigned a certain role.
*
* @param $role
*   The name of the role.
* @param $account
*   The user object for the user being checked; defaults to the current user.
* @return
*   TRUE if the user object has the role, FALSE if it does not.
*/
function smartparticipation_core_user_has_role($role, $account = null) {
  
  global $user;
  
  if (is_null($account)) {
    $account = $user;
  }

  return is_array($account->roles) && in_array($role, array_values($account->roles));
}

/**
 * Return true iff user is a moderator.
 * 
 * @param $user
 *   The user object for the user being checked; defaults to the current user.
 */
function smartparticipation_core_user_is_moderator($account = null) {
  return smartparticipation_core_user_has_role('moderator', $account);
}

/**
 * Return true iff uid is the uid of a moderator.
 * 
 * @param $uid
 *   The uid of the user being checked.
 */
function smartparticipation_core_uid_is_moderator($uid) {
  $account = user_load($uid);
  return smartparticipation_core_user_is_moderator($account);
}

/**
 * Return true iff user is a "basic authenticated user", which we define as a
 * user that must (a) agree to site terms and conditions, and (b) complete the 
 * proposal interest survey, before participating in a proposal.
 * 
 */
function smartparticipation_core_is_basic_authenticated_user($account = null) {

  global $user;
  
  if (is_null($account)) {
    $account = $user;
  }

  $roles = $account->roles;

  if (!is_array($roles)) {
    return false;
  }
  
  $roles = array_values($roles);
  
  /*
   * We do not currently have custom smartparticipation roles which define other types of
   * "basic authenticated users", but we might have in the future: e.g., an
   * expert contributer would still have to agree to terms and complete the 
   * survey. Then we'll need to make this test more
   */
  return count($roles) == 1 && $roles[0] == 'authenticated user';

}

/** 
 * Return true iff user is an anonymous or basic authenticated user - i.e.,
 * does not have an advanced role.
 * 
 * @param $account - the user object
 */
function smartparticipation_core_is_anonymous_or_basic_authenticated_user($account = null) {

  global $user;
  
  if (is_null($account)) {
    $account = $user;
  }
  
  return ($account->uid == 0 || smartparticipation_core_is_basic_authenticated_user($account));
  
}
