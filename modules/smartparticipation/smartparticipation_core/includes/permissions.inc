<?php

/**
 * Assign permissions to system roles during module install. Includes changing
 * default permissions, and assigning smartparticipation_core-specific permissions.
 */
function smartparticipation_core_assign_permissions_to_system_roles() {

  // Change default permissions for system roles
  
  // ANONYMOUS USER
  user_role_grant_permissions(DRUPAL_ANONYMOUS_RID, array(
    'access content',
  ));

  // AUTHENTICATED USER
  user_role_grant_permissions(DRUPAL_AUTHENTICATED_RID, array(
    'access content',    
  ));
  
  // ADMINISTRATOR
  $role_administrator = user_role_load_by_name('administrator');
  $rid_administrator = $role_administrator->rid;
  $administrator_permissions = array(
    'administer comments', // includes comment settings
    'administer smartparticipation_core configuration',
      
    'assign node weight', // weight module

    'create sp_about content',
    'create sp_announcement content',
    'create sp_document content',
    'create sp_faq content',
    'create sp_interest_survey content',
    'create sp_learn content',
    'create sp_news content',
    'create sp_proposal content',
    'create sp_publication content',
    'create sp_research content',
    'create sp_sponsor_partner content',
    'create sp_subtopic content',
    'create sp_team_member content',
    'create sp_topic content',
    'create webform content',

    'delete any sp_about content',
    'delete any sp_announcement content',
    'delete any sp_document content',
    'delete any sp_faq content',
    'delete any sp_interest_survey content',
    'delete any sp_learn content',
    'delete any sp_news content',
    'delete any sp_proposal content',
    'delete any sp_publication content',
    'delete any sp_research content',
    'delete any sp_sponsor_partner content',
    'delete any sp_subtopic content',
    'delete any sp_team_member content',
    'delete any sp_topic content',
    'delete any webform content',

    'edit any sp_about content',
    'edit any sp_announcement content',
    'edit any sp_document content',
    'edit any sp_faq content',
    'edit any sp_interest_survey content',
    'edit any sp_learn content',
    'edit any sp_news content',
    'edit any sp_proposal content',
    'edit any sp_publication content',
    'edit any sp_research content',
    'edit any sp_sponsor_partner content',
    'edit any sp_subtopic content',
    'edit any sp_team_member content',
    'edit any sp_topic content',
    'edit any webform content',
    
  );
     
  // Add permissions to edit and delete terms in built-in smartparticipation_core vocabularies.
  $smartparticipation_core_vocabularies = array(
    'sp_faq_category',
    'sp_proposal_phase',
    'sp_glossary',
  );
    
  $administrator_permissions = array();
  foreach ($smartparticipation_core_vocabularies as $name) {
    $vocabulary = taxonomy_vocabulary_machine_name_load($name);
    if ($vocabulary) {
      $vid = $vocabulary->vid;
      $administrator_permissions[] = 'edit terms in ' . $vid;
      $administrator_permissions[] = 'delete terms in ' . $vid;
    }
  }
    
  user_role_grant_permissions($rid_administrator, $administrator_permissions);

  // We still have to test smartparticipation_core_is_moderator() rather than testing specific
  // moderator-related permissions, because user_access() gives all 
  // permissions to user #1.
  user_role_revoke_permissions($rid_administrator, array(
    'administer comments',
  ));
 
}

/** 
 * Implements hook_permission().
 */
function smartparticipation_core_permission() {

  $permissions = array();
  
  $permissions['administer smartparticipation_core configuration'] = array(
    'title' => ('Administer Smartparticipation Configuration'),
    'description' => t('Allow users to access the SmartParticipation configuration page'),      
  );
  
  $permissions['export smartparticipation_core data'] = array(
    'title' => ('Export Smartparticipation Data'),
    'description' => t('Allow users to export SmartParticipation data'),
  );

  $permissions['view sp_announcement'] = array(
    'title' => t('View SmartParticipation Announcements'),
    'description' => t('Allow users to view SmartParticipation announcement nodes'),
  );

  $permissions['view sp_faq'] = array(
    'title' => t('View SmartParticipation FAQ'),
    'description' => t('Allow users to view SmartParticipation faq nodes'),
  );

  $permissions['view sp_news'] = array(
    'title' => t('View SmartParticipation News'),
    'description' => t('Allow users to view SmartParticipation news nodes'),
  );

  $permissions['view sp_interest_survey'] = array(
      'title' => t('View SmartParticipation Interest Survey'),
      'description' => t('Allow users to view SmartParticipation interest_survey nodes'),
  );

  $permissions['view sp_publication'] = array(
    'title' => t('View SmartParticipation Publication'),
    'description' => t('Allow users to view SmartParticipation publication nodes'),
  );

  $permissions['view sp_research'] = array(
    'title' => t('View SmartParticipation Research'),
    'description' => t('Allow users to view SmartParticipation research nodes'),
  );

  $permissions['view sp_sponsor_partner'] = array(
    'title' => t('View SmartParticipation Sponsor/Partner'),
    'description' => t('Allow users to view SmartParticipation sponsor_partner nodes'),
  );

  $permissions['view sp_subtopic'] = array(
    'title' => t('View SmartParticipation Subtopic'),
    'description' => t('Allow users to view SmartParticipation subtopic nodes'),
  );

  $permissions['view sp_team_member'] = array(
    'title' => t('View SmartParticipation Team Member'),
    'description' => t('Allow users to view SmartParticipation team_member nodes'),
  );

  return $permissions;
}

/**
 * Determine whether to display subtopic or topic editing tabs on node pages.
 */
function smartparticipation_core_access_check($node) {

  switch ($node->type) {

    case 'sp_topic':
      return user_access('create sp_subtopic content', $account = NULL);

    case 'sp_proposal':
      return user_access('create sp_topic content', $account = NULL);

    default:
      return FALSE;      
  }
}


/**
 * Determine whether to display document editing tab on proposal pages.
 */
function smartparticipation_core_access_check_documents($node) {

  if ($node->type == 'sp_proposal') {
    return user_access('create sp_document content', $account = NULL);
  }

  return FALSE;
}


/**
 * Determine whether to display menu tab on node pages.
 */
function smartparticipation_core_access_check_menus($node) {

  switch ($node->type) {

    case 'sp_about':
      return user_access('create sp_about content', $account = NULL);

    case 'sp_learn':
      return user_access('create sp_learn content', $account = NULL);
      
    default:
      return FALSE;    
  }
}


/**
 * Determine whether to display survey tab on proposal page.
 */
function smartparticipation_core_access_check_surveys($node) {

  if ($node->type == 'sp_proposal') {
    return user_access('create sp_interest_survey content', $account = NULL);
  }
  
  return FALSE;
}

/**
 * Determine whether to display data export tab on proposal page.
 */
function smartparticipation_core_access_check_export($node) {

  if ($node->type == 'sp_proposal') {
    return user_access('export smartparticipation_core data', $account = NULL);
  }

  return FALSE;
}

/**
 * Return true iff user can export smartparticipation_core user data.
 */
function smartparticipation_core_access_check_export_users() {
  return user_access('export smartparticipation_core data', $account = NULL);
}


/**
 * Return true if user can delete their own account.
 */
function _smartparticipation_core_access_user_can_delete_own_account() {
  return _smartparticipation_core_user_can_delete_own_account();
}

/**
 * Assign permissions to manage the glossaries
 *
 */
function smartparticipation_core_assign_permissions_glossary($vid) {
  
  $glossary_permissions = array(
    'edit terms in '. $vid,
    'delete terms in '. $vid,
  );
  
  $roles_names = array('administrator');
  foreach ($roles_names as $role_name) {
    $role = user_role_load_by_name($role_name);
    $rid = $role->rid;
    
    user_role_grant_permissions($rid, $glossary_permissions);
  }
}

/**
 * Revoke permissions to the glossaries when the vocabulary is deleted
 */
function smartparticipation_core_revoke_permissions_glossary($vid) {
  
  $glossary_permissions = array(
    'edit terms in '. $vid,
    'delete terms in '. $vid,
  );
  
  $roles_names = array('administrator', 'site editor', 'content editor');
  foreach ($roles_names as $role_name) {
    $role = user_role_load_by_name($role_name);
    $rid = $role->rid;
    
    user_role_revoke_permissions($rid, $glossary_permissions);
  }  
}

/**
 * Restore default permissions to system roles that were revoked during module
 * install. Runs during module uninstall.
 */
function smartparticipation_core_restore_permissions_to_system_roles() {
  
  // ADMINISTRATOR
  $role_administrator = user_role_load_by_name('administrator');
  $rid_administrator = $role_administrator->rid;
  user_role_grant_permissions($rid_administrator, array(
    'administer comments',
  ));
}

/**
 * Return an array of permissions for the moderator role.
 *
 */
function smartparticipation_core_moderator_permissions() {
  
  return array(
    'access all webform results',  
    'administer comments',
    'create sp_comment_note content',
    'delete own sp_comment_note content',
    'edit own comments',
    'edit own sp_comment_note content',
  );
}

/**
 * Revoke permissions when HybridAuth settings are deleted.
 */
function smartparticipation_core_revoke_permissions_hybridauth() {

  $roles = array(
    user_role_load_by_name('anonymous user'),
    user_role_load_by_name('authenticated user'),
  );

  foreach ($roles as $role) {
    user_role_revoke_permissions($role->rid, array('use hybridauth'));
  }

}

