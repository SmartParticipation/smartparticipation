<?php
/**
 * @file 
 * SmartParticipation menu functions.
 */
 
/**
 * Implements hook_menu().
 */
function smartparticipation_core_menu() {
  $items = array();

  // ADMIN PAGES
  // Module configuration page
  $items['admin/config/system/smartparticipation_core'] = array(
    'title' => 'SmartParticipation Site Configuration',
    'description' => 'Configure SmartParticipation properties for the site.',
    'page callback' => 'drupal_get_form',
    'page arguments' => array('smartparticipation_core_admin_settings'),
    'file' => 'includes/admin.inc',
    'access arguments' => array('administer smartparticipation_core configuration'),
    'weight' => 0,   
  );

  $items['admin/people/export-users'] = array(
    'title' => 'Export',
    'page callback' => '_smartparticipation_core_export_users_csv',
    //'page arguments' => array(1),
    'access callback' => 'smartparticipation_core_access_check_export_users',
    //'access arguments' => array(1),
    'file' => 'includes/export.inc',
    'type' => MENU_LOCAL_TASK,
    'weight' => 2,
  );

  // NODE TABS
  $items['node/%node/menus'] = array(
    'title' => 'Menus',
    'page callback' => 'smartparticipation_core_manage_menus',
    'page arguments' => array(1), 
    'access callback' => 'smartparticipation_core_access_check_menus',
    'access arguments' => array(1),
    'type' => MENU_LOCAL_TASK,
    'weight' => 2,
  );
  
  $items['node/%node/%smartparticipation_core_child_node_type'] = array(
    'title callback' => 'smartparticipation_core_tab_title',
	  'title arguments' => array(1),
    'page callback' => 'drupal_get_form',
    'page arguments' => array('smartparticipation_core_order_form', 1),
    'access callback' => 'smartparticipation_core_access_check',
    'access arguments' => array(1),
    'file' => 'includes/node_order.inc',
    'type' => MENU_LOCAL_TASK,
    'weight' => 5,
  );

  $items['node/%node/documents'] = array(
    'title' => 'Documents',
    'page callback' => 'drupal_get_form',
    'page arguments' => array('sp_document_order_form', 1),
    'access callback' => 'smartparticipation_core_access_check_documents',
    'access arguments' => array(1),
    'file' => 'includes/node_order.inc',
    'type' => MENU_LOCAL_TASK,
    'weight' => 10,
  );

  $items['node/%node/surveys'] = array(
    'title' => 'Surveys',
    'page callback' => 'drupal_get_form',
    'page arguments' => array('smartparticipation_core_surveys_form', 1),
    'access callback' => 'smartparticipation_core_access_check_surveys',
    'access arguments' => array(1),
    'file' => 'includes/surveys.inc',
    'type' => MENU_LOCAL_TASK,
    'weight' => 15,
  );

  $items['node/%node/glossary'] = array(
    'title' => 'Glossary',
    'page callback' => 'smartparticipation_core_manage_glossary',
    'page arguments' => array(1), 
    'access callback' => 'smartparticipation_core_access_check_glossary',
    'access arguments' => array(1),
    'type' => MENU_LOCAL_TASK,
    'weight' => 20,
  );

  $items['node/%node/export'] = array(
    'title' => 'Data export',
    'page callback' => 'drupal_get_form',
    'page arguments' => array('smartparticipation_core_export_form', 1),
    'access callback' => 'smartparticipation_core_access_check_export',
    'access arguments' => array(1),
    'file' => 'includes/export.inc',
    'type' => MENU_LOCAL_TASK,
    'weight' => 25,
  );

  $items['node/%node/export-survey'] = array(
    'title' => 'Data export',
    'page callback' => '_smartparticipation_core_export_survey_csv',
    'page arguments' => array(1),
    'access callback' => 'smartparticipation_core_access_check_export',
    'access arguments' => array(1),
    'file' => 'includes/export.inc',
  );

  $items['node/%node/export-comments'] = array(
    //'title' => 'Data Export',
    'page callback' => '_smartparticipation_core_export_comments_csv',
    'page arguments' => array(1),
    'access callback' => 'smartparticipation_core_access_check_export',
    'access arguments' => array(1),
    'file' => 'includes/export.inc',
  );

  $items['node/%node/export-endorsements'] = array(
    //'title' => 'Export Endorsements',
    'page callback' => '_smartparticipation_core_export_endorsements_csv',
    'page arguments' => array(1),
    'access callback' => 'smartparticipation_core_access_check_export',
    'access arguments' => array(1),
    'file' => 'includes/export.inc',
  );

  $items['node/%node/export-event-log'] = array(
    'page callback' => '_smartparticipation_core_export_event_log_csv',
    'page arguments' => array(1),
    'access callback' => 'smartparticipation_core_access_check_export',
    'access arguments' => array(1),
    'file' => 'includes/export.inc',
  );

  $items['node/%node/export-access-log'] = array(
    'page callback' => '_smartparticipation_core_export_access_log_csv',
    'page arguments' => array(1),
    'access callback' => 'smartparticipation_core_access_check_export',
    'access arguments' => array(1),
    'file' => 'includes/export.inc',
  );



  $items['node/%node/export-bot-log'] = array(
    'page callback' => '_smartparticipation_core_export_bot_log_csv',
    'page arguments' => array(1),
    'access callback' => 'smartparticipation_core_access_check_export',
    'access arguments' => array(1),
    'file' => 'includes/export.inc',
  );

  // USER PAGES
  
  /*
   * Trying to redirect all users from here to stats page, but doesn't work. 
   * We'll just live with the current situation: any authenticated user gets a 
   * redirect to stats page, anonymous users a 403. Not sure where either comes 
   * from. We use the default pathauto pattern 'users/[user:name]' for the
   * user path, but I don't know where we accomplish the redirect or the access
   * check.
  $items['users/%user'] = array(
    'title' => '',
    'page callback' => '_smartparticipation_core_user_profile_page',
    'page arguments' => array(1),
    'access callback' => 'smartparticipation_core_access_check_user', 
    'access arguments' => array(1),
  );
  */

//  $items['user/%user/profile'] = array(
//    'title' => '',
//    'title callback' => '_smartparticipation_core_user_profile_title',
//    'title arguments' => array(1),
//    'page callback' => '_smartparticipation_core_user_profile_page',
//    'page arguments' => 'smartparticipation_core_access_check_user',
//    'access arguments' => array(1),
//  );
  
  $items['user/%user/stats'] = array(
    'title' => '',
    'title callback' => '_smartparticipation_core_user_stats_title',
    'title arguments' => array(1),
    'page callback' => '_smartparticipation_core_user_stats_page',
    'page arguments' => array(1),
    'access callback' => 'smartparticipation_core_access_check_user', 
    'access arguments' => array(1),
  );

  $items['user/%user/proposals'] = array(
      'title' => '',
      'title callback' => '_smartparticipation_core_user_proposals_title',
      'title arguments' => array(1),
      'page callback' => 'smartparticipation_core_user_proposals_page',
      'page arguments' => array(1),
      'access callback' => 'smartparticipation_core_access_check_user',
      'access arguments' => array(1),
  );

  $items['user/%user/comments'] = array(
      'title' => '',
      'title callback' => '_smartparticipation_core_user_comments_title',
      'title arguments' => array(1),
      'page callback' => 'smartparticipation_core_user_comments_page',
      'page arguments' => array(1),
      'access callback' => 'smartparticipation_core_access_check_user',
      'access arguments' => array(1),
  );

  $items['user/%user/endorsed'] = array(
      'title' => '',
      'title callback' => '_smartparticipation_core_user_endorsed_title',
      'title arguments' => array(1),
      'page callback' => 'smartparticipation_core_user_endorsed_page',
      'page arguments' => array(1),
      'access callback' => 'smartparticipation_core_access_check_user',
      'access arguments' => array(1),
  );

  $items['user/%user/recommended'] = array(
      'title' => '',
      'title callback' => '_smartparticipation_core_user_recommended_title',
      'title arguments' => array(1),
      'page callback' => 'smartparticipation_core_user_recommended_page',
      'page arguments' => array(1),
      'access callback' => 'smartparticipation_core_access_check_user',
      'access arguments' => array(1),
  );

  $items['user/%user/endorsements'] = array(
      'title' => '',
      'title callback' => '_smartparticipation_core_user_endorsements_title',
      'title arguments' => array(1),
      'page callback' => 'smartparticipation_core_user_endorsements_page',
      'page arguments' => array(1),
      'access callback' => 'smartparticipation_core_access_check_user',
      'access arguments' => array(1),
  );

  $items['user/%user/replies'] = array(
    'title' => '',
    'title callback' => '_smartparticipation_core_user_replies_title',
    'title arguments' => array(1),
    'page callback' => 'smartparticipation_core_user_replies_page',
    'page arguments' => array(1), 
    'access callback' => 'smartparticipation_core_access_check_user', 
    'access arguments' => array(1),
  );
  
  
  // AJAX COMMENT ACTIONS
  $items['comment/%/in_progress'] = array(
    'title' => 'No Reply',
    'page callback' => 'smartparticipation_core_comment_in_progress',
    'page arguments' => array(1),
    'access arguments' => array('administer comments'),
  );
  
  
  $items['comment/%/quarantine'] = array(
    'title' => 'Quarantine',
    'page callback' => 'smartparticipation_core_comment_quarantine',
    'page arguments' => array(1),
    'access arguments' => array('administer comments'),
    'weight' => 1,
  );

  $items['comment/%/unquarantine'] = array(
    'title' => 'Unquarantine',
    'page callback' => 'smartparticipation_core_comment_unquarantine',
    'page arguments' => array(1),
    'access arguments' => array('administer comments'),
    'weight' => 1,
  );

  $items['comment/%/no_reply'] = array(
    'title' => 'No Reply',
    'page callback' => 'smartparticipation_core_comment_no_reply',
    'page arguments' => array(1),
    'access arguments' => array('administer comments'),
  );

  $items['comment/%/recommend'] = array(
    'title' => 'Recommend',
    'page callback' => 'smartparticipation_core_comment_recommend_or_unrecommend',
    'page arguments' => array(1),
    'access arguments' => array('administer comments'),
  );

  $items['comment/%/redact'] = array(
    'title' => 'Redact',
    'page callback' => 'smartparticipation_core_comment_redact',
    'page arguments' => array(1),
    'access arguments' => array('administer comments'),
  );

  $items['comment/%/add_note'] = array(
    'title' => 'Add note',
    'page callback' => 'smartparticipation_core_comment_add_note',
    'page arguments' => array(1),
    'access arguments' => array('administer comments'),
  );

  $items['comment/%/render'] = array(
    'title' => 'Render comment',
    'page callback' => 'smartparticipation_core_comment_ajax_render',
    'page arguments' => array(1),
    'access arguments' => array('administer comments'),
  );

  $items['comment/%/get'] = array(
    'title' => 'Get comment data',
    'page callback' => 'smartparticipation_core_get_comment_data',
    'page arguments' => array(1),
    'access arguments' => array('administer comments'),
  );
  

  // CUSTOM PAGES
  $items['announcements'] = array(
    'title' => 'Announcements',
    'page callback' => 'smartparticipation_core_announcements_page',
    'access arguments' => array('access content'),  
  );

  $items['contact'] = array(
      'title' => 'Contact Us',
      'page callback' => 'smartparticipation_core_contact_page',
      'access arguments' => array('access content'),
  );

  $items['faq'] = array(
    'title' => 'Frequently Asked Questions',
    'page callback' => 'smartparticipation_core_faq_page',
    'access arguments' => array('access content'),  
  );

  $items['moderator'] = array(
    'title' => 'Moderator Interface',
    'page callback' => 'smartparticipation_core_moderator_page',
    'access callback' => 'smartparticipation_core_moderator_access'
  );  
  
  $items['moderator/ajax'] = array(
    'page callback' => 'smartparticipation_core_ajax_moderator',
    'access callback' => 'smartparticipation_core_moderator_access',
  );

  $items['moderator/comment/%/action'] = array(
    'page callback' => 'smartparticipation_core_moderator_comment_action',
    'access callback' => 'smartparticipation_core_moderator_access',
    'page arguments' => array(2),
  );

  /*$items['register/request'] = array(
    'page callback' => 'drupal_get_form',
    'page arguments' => array('smartparticipation_core_private_discussion_register_request_form'),
    'access callback' => 'user_is_anonymous',
  );

  $items['register/request/%'] = array(
    'page callback' => 'drupal_get_form',
    'page arguments' => array('smartparticipation_core_private_discussion_register_request_form', 2),
    'access callback' => 'user_is_anonymous',
  );*/

  $items['user_does_not_accept_terms'] = array(
    'title' => 'Does not accept terms',
    'page callback' => 'smartparticipation_core_user_does_not_accept_terms',
    'access callback' => '_smartparticipation_core_access_user_can_delete_own_account',
  );

  $items['user_does_not_accept_terms/%'] = array(
    'title' => 'Account deleted',
    'page callback' => 'smartparticipation_core_user_does_not_accept_terms',
    'page arguments' => array(1),
    'access callback' => TRUE,
  );
  

  // AJAX EVENT LOGGING
  $items['ajax/log_event'] = array(
    'title' => 'Log event',
    'page callback' => 'smartparticipation_core_log_client_event',
    //'access arguments' => array('access content'),
    'access callback' => TRUE,
  );

  // AJAX FEATURED COMMENTS SHOW MORE
  $items = array_merge($items, \SmartParticipation\ShowMoreFeaturedComments::getMenuRoute());

  return $items;
}

function smartparticipation_show_more_featured_comments($comment_type)
{
  $show_more_featured_comments = new \SmartParticipation\ShowMoreFeaturedComments;
  return $show_more_featured_comments->request($comment_type);
}

function smartparticipation_core_tab_title($node) {
  $type = smartparticipation_core_get_nodereferences_type($node->type);
  return ucfirst($type) . 's';
}

function smartparticipation_core_child_node_type_to_arg($arg, $map, $index) {

  $node = menu_get_object();
  
  // Adding test for $node to prevent errors. Or should we be doing something
  // else in this case?
  if ($node) {
    switch ($node->type) {
      case 'sp_proposal':
        return 'topics';
        break;
      case 'sp_topic':
        return 'subtopics';
        break;
      default:
        return 'nodereference';
        break;
    }
  }
}


function smartparticipation_core_create_custom_menus() {
  smartparticipation_core_create_user_menu();
  smartparticipation_core_customize_main_menu();
  smartparticipation_core_create_footer_menu();
}


function smartparticipation_core_create_user_menu() {
  global $user;
  $uid = $user->uid;
  
  $menu = array (
    'menu_name' => 'sp-user-menu-logged-out',
    'title' => 'SmartParticipation User Menu Logged Out',
    'description' => 'A user menu for SmartParticipation sites when the user is logged out.'
  );

  menu_save($menu);

  $menu = array (
    'menu_name' => 'sp-user-menu-logged-in',
    'title' => 'SmartParticipation User Menu Logged In',
    'description' => 'A user menu for SmartParticipation sites when the user is logged in.'
  );

  menu_save($menu);

  // LOGGED OUT MENU LINKS

  $menu_links['sp-user-menu-logged-out:user/login'] = array(
    'menu_name' => 'sp-user-menu-logged-out',
    'link_path' => 'user/login',
    'router_path' => 'user/login',
    'link_title' => 'Log in',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
    ),
    'module' => 'menu',
    'hidden' => '0',
    'external' => '0',
    'has_children' => '0',
    'expanded' => '0',
    'weight' => '10',
  );

  $menu_links['sp-user-menu-logged-out:user/register'] = array(
    'menu_name' => 'sp-user-menu-logged-out',
    'link_path' => 'user/register',
    'router_path' => 'user/register',
    'link_title' => 'Register',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
    ),
    'module' => 'menu',
    'hidden' => '0',
    'external' => '0',
    'has_children' => '0',
    'expanded' => '0',
    'weight' => '5',
  );

  // LOGGED IN MENU LINKS

  $menu_links['sp-user-menu-logged-in:user'] = array(
    'menu_name' => 'sp-user-menu-logged-in',
    'link_path' => 'user',
    'router_path' => 'user',
    'link_title' => '[current-user:name]',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'menu_token_link_path' => 'user/[current-user:uid]/stats',
      'menu_token_data' => array(),
      'menu_token_options' => array(
        'clear' => 0,
      ),
      'alter' => TRUE,
    ),
    'module' => 'menu',
    'hidden' => '0',
    'external' => '0',
    'has_children' => '1',
    'expanded' => '0',
    'weight' => '20',
  );

  // Exported menu link: sp-user-menu-logged-in:menutoken/519d0e553646e
  $menu_links['sp-user-menu-logged-in:menutoken/519d0e553646e'] = array(
    'menu_name' => 'sp-user-menu-logged-in',
    'link_path' => 'menutoken/519d0e553646e',
    'router_path' => 'menutoken/%',
    'link_title' => 'Stats',
    'options' => array(
      'menu_token_link_path' => 'user/[current-user:uid]/stats',
      'menu_token_data' => array(),
      'menu_token_options' => array(
        'clear' => 0,
      ),
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
    ),
    'module' => 'menu',
    'hidden' => '0',
    'external' => '0',
    'has_children' => '0',
    'expanded' => '0',
    'weight' => '25',
    'parent_path' => 'user',
  );

  // Exported menu link: sp-user-menu-logged-in:menutoken/519d1ae326b65
  /*
  $menu_links['sp-user-menu-logged-in:menutoken/519d1ae326b65'] = array(
    'menu_name' => 'sp-user-menu-logged-in',
    'link_path' => 'menutoken/519d1ae326b65',
    'router_path' => 'menutoken/%',
    'link_title' => 'Profile',
    'options' => array(
      'menu_token_link_path' => 'user/[current-user:uid]/edit/main',
      'menu_token_data' => array(),
      'menu_token_options' => array(
        'clear' => 0,
      ),
      'attributes' => array(
        'title' => '',
      ),
      'alter' => TRUE,
    ),
    'module' => 'menu',
    'hidden' => '0',
    'external' => '0',
    'has_children' => '0',
    'expanded' => '0',
    'weight' => '30',
    'parent_path' => 'user',
  );
  */

  // Exported menu link: sp-user-menu-logged-in:menutoken/51c37684f1e7c
  $menu_links['sp-user-menu-logged-in:menutoken/51c37684f1e7c'] = array(
    'menu_name' => 'sp-user-menu-logged-in',
    'link_path' => 'menutoken/51c37684f1e7c',
    'router_path' => 'menutoken/%',
    'link_title' => 'Account',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'menu_token_link_path' => 'user/[current-user:uid]/edit',
      'menu_token_data' => array(),
      'menu_token_options' => array(
        'clear' => 0,
      ),
      'alter' => TRUE,
    ),
    'module' => 'menu',
    'hidden' => '0',
    'external' => '0',
    'has_children' => '0',
    'expanded' => '0',
    'weight' => '35',
    'parent_path' => 'user',
  );

  $menu_links['sp-user-menu-logged-in:menutoken/5686796ade321'] = array(
      'menu_name' => 'sp-user-menu-logged-in',
      'link_path' => 'menutoken/5686796ade321',
      'router_path' => 'menutoken/%',
      'link_title' => 'Proposals I participated in',
      'options' => array(
          'attributes' => array(
              'title' => '',
          ),
          'menu_token_link_path' => 'user/[current-user:uid]/proposals',
          'menu_token_data' => array(),
          'menu_token_options' => array(
              'clear' => 0,
          ),
          'alter' => TRUE,
      ),
      'module' => 'menu',
      'hidden' => '0',
      'external' => '0',
      'has_children' => '0',
      'expanded' => '0',
      'weight' => '37',
      'parent_path' => 'user',
  );

  // Exported menu link: sp-user-menu-logged-in:menutoken/519d16e9e2ef4
  $menu_links['sp-user-menu-logged-in:menutoken/519d16e9e2ef4'] = array(
    'menu_name' => 'sp-user-menu-logged-in',
    'link_path' => 'menutoken/519d16e9e2ef4',
    'router_path' => 'menutoken/%',
    'link_title' => 'My comments',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'menu_token_link_path' => 'user/[current-user:uid]/comments',
      'menu_token_data' => array(),
      'menu_token_options' => array(
        'clear' => 0,
      ),
      'alter' => TRUE,
    ),
    'module' => 'menu',
    'hidden' => '0',
    'external' => '0',
    'has_children' => '0',
    'expanded' => '0',
    'weight' => '40',
    'parent_path' => 'user',
  );

  $menu_links['sp-user-menu-logged-in:menutoken/56867fe0c406f'] = array(
      'menu_name' => 'sp-user-menu-logged-in',
      'link_path' => 'menutoken/56867fe0c406f',
      'router_path' => 'menutoken/%',
      'link_title' => 'My endorsed comments',
      'options' => array(
          'attributes' => array(
              'title' => '',
          ),
          'menu_token_link_path' => 'user/[current-user:uid]/endorsed',
          'menu_token_data' => array(),
          'menu_token_options' => array(
              'clear' => 0,
          ),
          'alter' => TRUE,
      ),
      'module' => 'menu',
      'hidden' => '0',
      'external' => '0',
      'has_children' => '0',
      'expanded' => '0',
      'weight' => '42',
      'parent_path' => 'user',
  );

  $menu_links['sp-user-menu-logged-in:menutoken/568681abcff2e'] = array(
      'menu_name' => 'sp-user-menu-logged-in',
      'link_path' => 'menutoken/568681abcff2e',
      'router_path' => 'menutoken/%',
      'link_title' => 'My Recommended comments',
      'options' => array(
          'attributes' => array(
              'title' => '',
          ),
          'menu_token_link_path' => 'user/[current-user:uid]/recommended',
          'menu_token_data' => array(),
          'menu_token_options' => array(
              'clear' => 0,
          ),
          'alter' => TRUE,
      ),
      'module' => 'menu',
      'hidden' => '0',
      'external' => '0',
      'has_children' => '0',
      'expanded' => '0',
      'weight' => '43',
      'parent_path' => 'user',
  );


  // Exported menu link: sp-user-menu-logged-in:menutoken/519d16f8a10a1
  $menu_links['sp-user-menu-logged-in:menutoken/519d16f8a10a1'] = array(
    'menu_name' => 'sp-user-menu-logged-in',
    'link_path' => 'menutoken/519d16f8a10a1',
    'router_path' => 'menutoken/%',
    'link_title' => 'Comments I endorsed',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'menu_token_link_path' => 'user/[current-user:uid]/endorsements',
      'menu_token_data' => array(),
      'menu_token_options' => array(
        'clear' => 0,
      ),
      'alter' => TRUE,
    ),
    'module' => 'menu',
    'hidden' => '0',
    'external' => '0',
    'has_children' => '0',
    'expanded' => '0',
    'weight' => '50',
    'parent_path' => 'user',
  );
  
  // Exported menu link: sp-user-menu-logged-in:menutoken/519d170565c2c
  $menu_links['sp-user-menu-logged-in:menutoken/519d170565c2c'] = array(
    'menu_name' => 'sp-user-menu-logged-in',
    'link_path' => 'menutoken/519d170565c2c',
    'router_path' => 'menutoken/%',
    'link_title' => 'Replies to my comments',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'menu_token_link_path' => 'user/[current-user:uid]/replies',
      'menu_token_data' => array(),
      'menu_token_options' => array(
        'clear' => 0,
      ),
      'alter' => TRUE,
    ),
    'module' => 'menu',
    'hidden' => '0',
    'external' => '0',
    'has_children' => '0',
    'expanded' => '0',
    'weight' => '45',
    'parent_path' => 'user',
  );

  $menu_links['sp-user-menu-logged-in:user/logout'] = array(
    'menu_name' => 'sp-user-menu-logged-in',
    'link_path' => 'user/logout',
    'router_path' => 'user/logout',
    'link_title' => 'Log out',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
    ),
    'module' => 'menu',
    'hidden' => '0',
    'external' => '0',
    'has_children' => '0',
    'expanded' => '0',
    'weight' => '55',
  );

  $mlid = null;
  $parent = array();
  foreach ($menu_links as $menu_link) {
    // set the parent link id if this link has a parent path
    if (count($parent) && isset($menu_link['parent_path'])) {
      if (isset($parent[$menu_link['parent_path']])) {
        $menu_link['plid'] = $parent[$menu_link['parent_path']];
      }
    }
    $mlid = menu_link_save($menu_link);
    menu_cache_clear_all();
    // store the link id if it has children
    if ($menu_link['has_children']) {
      $parent[$menu_link['link_path']] = $mlid;
    }
  }

  menu_rebuild();

  //variable_set('menu_secondary_links_source','sp-user-menu');
}


function smartparticipation_core_create_footer_menu() {
  $menu = array (
    'menu_name' => 'sp-footer-menu',
    'title' => 'SmartParticipation Footer Menu',
    'description' => 'A footer menu for SmartParticipation sites.'
  );

  menu_save($menu);

  // FOOTER MENU LINKS
  // These nodes are created on module install.
  $path_about = drupal_get_normal_path('about/overview');
  $path_contact = drupal_get_normal_path('contact');
  $path_faq = drupal_get_normal_path('faq');
  $path_terms = drupal_get_normal_path('terms');
    
  $menu_links['sp-footer-menu:about/overview'] = array(
    'menu_name' => 'sp-footer-menu',
    'link_path' => $path_about,
    'router_path' => $path_about,
    'link_title' => 'About',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
    ),
    'module' => 'menu',
    'hidden' => '0',
    'external' => '0',
    'has_children' => '0',
    'expanded' => '0',
    'weight' => '2',
  );
  
  $menu_links['sp-footer-menu:faq'] = array(
    'menu_name' => 'sp-footer-menu',
    'link_path' => $path_faq,
    'router_path' => $path_faq,
    'link_title' => 'FAQ',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
    ),
    'module' => 'menu',
    'hidden' => '0',
    'external' => '0',
    'has_children' => '0',
    'expanded' => '0',
    'weight' => '4',
  );
  
  $menu_links['sp-footer-menu:contact'] = array(
    'menu_name' => 'sp-footer-menu',
    'link_path' => $path_contact,
    'router_path' => $path_contact,
    'link_title' => 'Contact Us',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
    ),
    'module' => 'menu',
    'hidden' => '0',
    'external' => '0',
    'has_children' => '0',
    'expanded' => '0',
    'weight' => '6',
  );
  
  $menu_links['sp-footer-menu:terms'] = array(
    'menu_name' => 'sp-footer-menu',
    'link_path' => $path_terms,
    'router_path' => $path_terms,
    'link_title' => 'Terms of Use & Privacy',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
    ),
    'module' => 'menu',
    'hidden' => '0',
    'external' => '0',
    'has_children' => '0',
    'expanded' => '0',
    'weight' => '8',
  );

  $mlid = null;
  $parent = array();
  foreach ($menu_links as $menu_link) {
    // set the parent link id if this link has a parent path
    if (count($parent) && isset($menu_link['parent_path'])) {
      if (isset($parent[$menu_link['parent_path']])) {
        $menu_link['plid'] = $parent[$menu_link['parent_path']];
      }
    }
    $mlid = menu_link_save($menu_link);
    menu_cache_clear_all();
    // store the link id if it has children
    if ($menu_link['has_children']) {
      $parent[$menu_link['link_path']] = $mlid;
    }
  }

  menu_rebuild();
}


function smartparticipation_core_customize_main_menu() {

  $proposals_open = _smartparticipation_core_get_proposal_list('1');
  $proposals_closed = _smartparticipation_core_get_proposal_list('0');
  $proposals_name = ucfirst(variable_get('smartparticipation_site_proposal_type_plural'));
 
  // OPEN PROPOSALS menu 
  if( count($proposals_open) > 0 ) {
    $menu_links_open['main-menu:open'] = array(
      'menu_name' => 'main-menu',
      'link_path' => '<nolink>',
      //'router_path' => '',
      'link_title' => 'Open ' . $proposals_name,
      'options' => array(
        'attributes' => array(
          'title' => '',
        ),
      ),
      'module' => 'menu',
      'hidden' => '0',
      'external' => '0',
      'has_children' => '1',
      'expanded' => '1',
      'weight' => '5',
    );
    
    $mlid_open = menu_link_save($menu_links_open['main-menu:open']);
    
    
    foreach($proposals_open as $nid => $title) {
      $menu_links_name = 'main-menu:'.$nid;
      //dsm($menu_links_name);
      $menu_links[$menu_links_name] = array(
        'menu_name' => 'main-menu',
        'link_path' => 'node/'.$nid,
        //'router_path' => '',
        'link_title' => $title,
        'options' => array(
          'attributes' => array(
            'title' => '',
          ),
        ),
        'module' => 'menu',
        'hidden' => '0',
        'external' => '0',
        'has_children' => '0',
        'expanded' => '0',
        'plid' => $mlid_open,
      );
    }
  }
  
  // CLOSED PROPOSALS menu
  if( count($proposals_closed) > 0 ) {
    $menu_links_closed['main-menu:closed'] = array(
      'menu_name' => 'main-menu',
      'link_path' => '<nolink>',
      'link_title' => 'Closed ' . $proposals_name,
      'options' => array(
        'attributes' => array(
          'title' => '',
        ),
      ),
      'module' => 'menu',
      'hidden' => '0',
      'external' => '0',
      'has_children' => '1',
      'expanded' => '1',
      'weight' => '10',
    );
    
    $mlid_closed = menu_link_save($menu_links_closed['main-menu:closed']);
    
    foreach($proposals_closed as $nid => $title) {
      $menu_links_name = 'main-menu:'.$nid;
      $menu_links[$menu_links_name] = array(
        'menu_name' => 'main-menu',
        'link_path' => 'node/'.$nid,
        'link_title' => $title,
        'options' => array(
          'attributes' => array(
            'title' => '',
          ),
        ),
        'module' => 'menu',
        'hidden' => '0',
        'external' => '0',
        'has_children' => '0',
        'expanded' => '0',
        'plid' => $mlid_closed,
      );
    }
  }

  // LEARN MORE menu
  $learn_list = _smartparticipation_core_get_published_nodes_by_type('sp_learn');
  if( count($learn_list) > 0 ) {
    $menu_links_learn['main-menu:learn'] = array(
      'menu_name' => 'main-menu',
      'link_path' => '<nolink>',
      'link_title' => 'Learn',
      'options' => array(
        'attributes' => array(
          'title' => '',
        ),
      ),
      'module' => 'menu',
      'hidden' => '0',
      'external' => '0',
      'has_children' => '1',
      'expanded' => '1',
      'weight' => '15',
    );
    
    $mlid_learn = menu_link_save($menu_links_learn['main-menu:learn']);
    
    foreach($learn_list as $nid => $title) {
      $menu_links_name = 'main-menu:'.$nid;
      $menu_links[$menu_links_name] = array(
        'menu_name' => 'main-menu',
        'link_path' => 'node/' . $nid,
        'link_title' => $title,
        'options' => array(
          'attributes' => array(
            'title' => '',
          ),
        ),
        'module' => 'menu',
        'hidden' => '0',
        'external' => '0',
        'has_children' => '0',
        'expanded' => '0',
        'plid' => $mlid_learn,
      );
    }
  }
  
  // ABOUT menu
  $about_nodes = _smartparticipation_core_get_published_nodes_by_type('sp_about');
  if (count($about_nodes) > 0) {
    $menu_links_about['main-menu:about'] = array(    
      'menu_name' => 'main-menu',
      'link_path' => '<nolink>',
      'link_title' => t('About'),
      'options' => array(
        'attributes' => array(
          'title' => '',
        ),
      ),
      'module' => 'menu',
      'hidden' => '0',
      'external' => '0',
      'has_children' => '1',
      'expanded' => '1',
      'weight' => '20',
    );    

    $mlid_about = menu_link_save($menu_links_about['main-menu:about']);
    
    $j = 0;
    foreach($about_nodes as $nid => $title) {
      $menu_links_name = 'main-menu:'.$nid;
      $menu_links[$menu_links_name] = array(
        'menu_name' => 'main-menu',
        'link_path' => 'node/' . $nid,
        'link_title' => $title,
        'options' => array(
          'attributes' => array(
            'title' => '',
          ),
        ),
        'module' => 'menu',
        'hidden' => '0',
        'external' => '0',
        'has_children' => '0',
        'expanded' => '0',
        'plid' => $mlid_about,
        'weight' => $j,
      );
      $j++;
    }
  }
  
  // ANNOUNCEMENTS menu item
  $path_announcements = drupal_get_normal_path('announcements');
  $menu_links['main-menu:announcements'] = array(
    'menu_name' => 'main-menu',
    'link_path' => $path_announcements,
    'router_path' => $path_announcements,
    'link_title' => 'Announcements',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
    ),
    'module' => 'menu',
    'hidden' => '0',
    'external' => '0',
    'has_children' => '0',
    'expanded' => '0',
    'weight' => '25',
  );

  // MODERATOR menu item 
  $path_moderator = drupal_get_normal_path('moderator');
  $menu_links['main-menu:moderator'] = array(
    'menu_name' => 'main-menu',
    'link_path' => $path_moderator,
    'router_path' => $path_moderator,
    'link_title' => 'Moderator',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
    ),
    'module' => 'menu',
    'hidden' => '0',
    'external' => '0',
    'has_children' => '0',
    'expanded' => '0',
    'weight' => '35',
  );
  
  $mlid = null;
  $parent = array();
  foreach ($menu_links as $menu_link) {
    // set the parent link id if this link has a parent path
    if (count($parent) && isset($menu_link['parent_path'])) {
      if (isset($parent[$menu_link['parent_path']])) {
        $menu_link['plid'] = $parent[$menu_link['parent_path']];
      }
    }
    $mlid = menu_link_save($menu_link);
    menu_cache_clear_all();
    // store the link id if it has children
    if ($menu_link['has_children']) {
      $parent[$menu_link['link_path']] = $mlid;
    }
  }

  menu_rebuild();
}


function smartparticipation_core_delete_custom_menus() {
  smartparticipation_core_delete_user_menu();
  smartparticipation_core_delete_footer_menu();
  smartparticipation_core_restore_main_menu();
}


function smartparticipation_core_delete_user_menu() {
  menu_delete(array('menu_name'=>'sp-user-menu-logged-out'));
  menu_delete(array('menu_name'=>'sp-user-menu-logged-in'));
}

/**
 * Delete main-menu links added by smartparticipation_core module.
 */
function smartparticipation_core_restore_main_menu() {
  
  $sql = "SELECT mlid
          FROM menu_links
          WHERE menu_name='main-menu' AND link_title != 'Home'";
  $menu_links = db_query($sql);
  foreach ($menu_links as $link) {
    menu_link_delete($link->mlid);
  }
}

/**
 * Update menu link text when the title of a node in the main menu is changed.
 * @param $node - the node that was just updated
 */
function smartparticipation_core_update_menu_link_text($node) {
  
  /*
   * 
   * menu_node: nid, mlid
   * menu_links: mlid (same as menu_node.mlid) - update menu_links.link_title
   */
  
}


function smartparticipation_core_delete_footer_menu() {
  menu_delete(array('menu_name'=>'sp-footer-menu'));
}


function smartparticipation_core_manage_menus($node) {
  $path['destination'] = 'node/'.$node->nid;
  $admin_menu_page = 'admin/structure/menu/manage/main-menu';
  drupal_goto($admin_menu_page, array('query' => $path));
}


// TODO this function should be eliminated and replaced with separate
// functions. See smartparticipation_core_node_insert(). I've added a sketch of the functions
// to about.inc, learn.inc, and proposal.inc, but you'll need to make the
// code functional.
// TODO Do we need to check whether the node is published? Maybe the menu
// module handles that automatically.
function smartparticipation_core_autocreatemenu($node) {

  if ($node->type == 'sp_about') {
    
    if ($node->sticky == 0) {
      
      $last_weight = _smartparticipation_core_get_last_menu_item_weight($node->type);
      $params = smartparticipation_core_automenu_automenu_create($node);
      smartparticipation_core_automenu_add_menu_link($params, $last_weight);
      
    }
  } else {
    
    $last_weight = _smartparticipation_core_get_last_menu_item_weight($node->type);
    $params = smartparticipation_core_automenu_automenu_create($node);
    smartparticipation_core_automenu_add_menu_link($params, $last_weight);
    
  }
}


/**
 * Save a menu link.
 */
function smartparticipation_core_automenu_add_menu_link($params, $last_weight) {
  // TODO $params could be null. Doesn't that generate an error? See
  // notes in smartparticipation_core_automenu_automenu_create().
  // dd($params);
  if (!empty($params)) {
    $new_menu = array(
      'menu_name' => $params['menu_name'],
      'link_path' => $params['link_path'],
      'link_title' => $params['link_title'],
      'plid' => $params['plid'],
      'hidden' => $params['hidden'],
      'expanded' => $params['expanded'],
      'options' => array('alter' => TRUE),
    );

    if($params['options'] != NULL) {
        $new_menu['options'] = $params['options'];
    }
    
    // TODO Do we need the test when we've just set the value? Can this
    // all be replaced with one line:
    // $new_menu['weight'] = 0;
    //$params['weight'] = 0;
    $params['weight'] = $last_weight + 1;
    if ($params['weight'] != NULL) {
      $new_menu['weight']  = $params['weight'];
    }
    
    // TODO Do we need the test when we've just set the value? Can this
    // all be replaced with one line?
    // $new_menu['mlid'] = ''; 
    $params['mlid'] = '';
    if ($params['mlid'] != NULL) {
      $new_menu['mlid'] = $params['mlid'];
    }

    if(!menu_link_save($new_menu)) {
      drupal_set_message(t('There was an error saving the auto-menu link.'), 'error');
    }
    else {
      drupal_set_message(t('The page was automatically added/updated to: !menu.',
        array('!menu' => $params['menu_name'])));
    }
  }
}

// TODO The whole switch statement can be removed if we use separate
// function to pass in the $link_title. See notes in proposal.inc, about.inc, 
// learn.inc. New function signature:
// function smartparticipation_core_automenu_automenu_create($node, $link_title) {
// TODO These weights are now hard-coded in two places. Combine.
function smartparticipation_core_automenu_automenu_create($node) {
  
  switch($node->type) {

    case 'sp_about':
      $link_title = t('About');
      $weight = 20;
      break;

    case 'sp_announcement':
      $link_title = t('Announcements');
      $weight = 25;
      break;

    case 'sp_learn':
      $link_title = t('Learn');
      $weight = 15;
      break;

    case 'sp_proposal':
      $proposal_status = smartparticipation_core_field_get_item_value('node', $node, 'field_sp_proposal_status', 'value');
      $proposal_type = ucfirst(variable_get('smartparticipation_site_proposal_type_plural'));

      // CLOSED proposal
      if ($proposal_status == 0) {
        $link_title = t('Closed @proposal_type', array('@proposal_type' => $proposal_type));
        $weight = 10;
        
      // OPEN proposal        
      } else {
        $link_title = t('Open @proposal_type', array('@proposal_type' => $proposal_type));
        $weight = 5;
      }
      break;      
  }
  
  $menu_name = 'main-menu';
  $result = array();
  
  // checking if the parent menu item is built
  smartparticipation_core_menu_parent($link_title, $node->status, $weight);
  
  // getting the parent menu item ID
  $query = db_select('menu_links', 'l')
    ->fields('l', array('mlid'));
  $query->where("l.link_title = :link_title AND menu_name = :menu_name", array(':link_title' => $link_title, ':menu_name' => $menu_name));

  $result = $query->execute();
  $count = $result->rowCount();

  if($count > 0) {
    foreach ($result as $row) {
      $mlid = $row->mlid; // parent menu item ID
    }
  } else {
    $mlid = 0;
  }
  
  $params = array();
  
  if (!smartparticipation_core_menu_exists($node->title, $menu_name)) {
    
    $parent_menu = explode(":", "main-menu:".$mlid);
    
    if ($parent_menu[0] != '0') {
      //if($node->status == 1) {
        if($node->language) {
          $new_menu['options'] = array('langcode' => $node->language);
        }
  
        $params = array(
          'success' => TRUE,
          'menu_name' => $parent_menu[0],
          'link_path' => 'node/' . $node->nid,
          'link_title' => $node->title,
          'plid' => $parent_menu[1],
          'hidden' => ($node->status == 1)?0:1, 
          'options' => $new_menu['options'],
          'expanded' => 0,
        );
        
        $new_menu['plid']="";
        
        $cmlid = db_select('menu_links', 'ml')
          ->condition('ml.link_path', 'node/' . $node->nid)
          ->condition('ml.plid', $new_menu['plid'])
          ->fields('ml', array('mlid'))
          ->execute()
          ->fetchField(); // the current mlid
        
        if ($cmlid) {
          $new_menu['mlid'] = $cmlid;
        }
    }
  }

  return $params;
}


function smartparticipation_core_menu_parent($link_title, $node_status, $weight = 0) {

  // This function will check if the menu parent item exists.
  // If not it will create it.
  $menu_name = 'main-menu';

  $query = 'SELECT l.mlid FROM {menu_links} l WHERE l.link_title = :link_title AND menu_name = :menu_name';
  $result = db_query($query,array(':link_title' => $link_title, ':menu_name' => $menu_name));
  $record = $result->fetchObject();
  $count = $result->rowCount();
  
  $menu_item = array(
    'link_title' => $link_title,
    'menu_name'  => $menu_name, 
    'link_path'  => '<nolink>',
    'weight' => $weight, 
  );
  
  if($count == 0) {
    if(!smartparticipation_core_menu_exists($link_title, $menu_name)) {
      $mlid = menu_link_save($menu_item);
    }
  } elseif($node_status == 1) {
    $mlid = $record->mlid;
    $menu_item = menu_link_load($mlid);    
    $menu_item['hidden'] = 0;
    //dsm($menu_item);
    menu_link_save($menu_item);
  }
  
  menu_cache_clear_all();
}


function smartparticipation_core_menu_exists($link_title, $menu_name) {
  // This function will check if the menu item exists.
  $query = 'SELECT l.mlid FROM {menu_links} l WHERE l.link_title = :link_title AND menu_name = :menu_name';
  $result = db_query($query,array(':link_title' => $link_title, ':menu_name' => $menu_name));
  $count = $result->rowCount();
  return $count != 0;
}


function smartparticipation_core_set_special_menu_items() {
  variable_set('special_menu_items_nolink_tag', '<a>');
}


/**
 * @return the maximum weight value for a menu item of a certain content type
 */
function _smartparticipation_core_get_last_menu_item_weight($dist) {
  $query = 'SELECT n.*, mn.*, ml.* FROM {node} n, {menu_node} mn, {menu_links} ml WHERE n.type = :dist AND n.status = 1 AND n.nid = mn.nid AND mn.mlid = ml.mlid ORDER BY n.nid DESC LIMIT 1';
  $result = db_query($query,array(':dist' => $dist));
  $record = $result->fetchObject();
  $count = $result->rowCount();
  
  if($count > 0) {
    return $record->weight;
  }
  return 0;
}


function smartparticipation_core_menu_update_parent($node) {
  $menu_name = 'main-menu';
  $item = menu_get_active_trail();
  $old_mlid = $item[2]['mlid'];
  $plid = $item[1]['mlid'];

  $old_link_title = $item[1]['link_title'];
  
  $proposal_status = smartparticipation_core_field_get_item_value('node', $node, 'field_sp_proposal_status', 'value');
  $proposal_type = ucfirst(variable_get('smartparticipation_site_proposal_type_plural'));

  // CLOSED proposal
  if ($proposal_status == 0 && $old_link_title == 'Open ' . $proposal_type) {
    
    $new_link_title = 'Closed ' . $proposal_type;
    if(!smartparticipation_core_menu_exists($new_link_title, $menu_name)) {
      smartparticipation_core_menu_parent($new_link_title, $proposal_status);
    }
    $new_mlid = smartparticipation_core_menu_get_mlid($new_link_title);
    smartparticipation_core_toggle_main_menu_closed_proposals_link(0);

    // OPEN proposal
  } else if ($proposal_status == 1 && $old_link_title == 'Closed ' . $proposal_type) {

    $new_link_title = 'Open ' . $proposal_type;
    if (!smartparticipation_core_menu_exists($new_link_title, $menu_name)) {
      smartparticipation_core_menu_parent($new_link_title, $proposal_status);
    }
    $new_mlid = smartparticipation_core_menu_get_mlid($new_link_title);
    smartparticipation_core_toggle_main_menu_open_proposals_link(0);
    
  } else {
    $new_mlid = $old_mlid;
  }

  if($new_mlid != $old_mlid) {
    $menu_item = menu_link_load($old_mlid);
    $menu_item['plid'] = $new_mlid;
    menu_link_save($menu_item);
  }
  
  $title_new = $node->title;
  $title_old = $node->original->title;
  
  if($title_new != $title_old) {
    $menu_item = menu_link_load($old_mlid);
    $menu_item['link_title'] = $title_new;
    menu_link_save($menu_item);
  }
  
  if($node->status == 1){
    $menu_item = menu_link_load($old_mlid);
    $menu_item['hidden'] = 0;
    menu_link_save($menu_item);
  }

  smartparticipation_core_sort_proposal_menu($plid);

}

/**
 * Sorts either open or closed proposal menu items
 * Sets the weight on the menu links appropriately and updates the menu
 *
 * @param $plid - The menu link ID of either the open or closed proposal menu link
 */
function smartparticipation_core_sort_proposal_menu($plid) {

  $menu = menu_link_load($plid);

  if(preg_match('/^Open /', $menu['link_title'])) {
    $proposals_to_sort = 'open';
  }
  else {
    $proposals_to_sort = 'close';
  }


  /// Get data for all child menu links of Open or Closed Proposal menu link
  $child_menu_data = db_select('menu_links' , 'ml')
      ->condition('ml.plid' , $plid)
      ->fields('ml',array('mlid','link_title','link_path'))
      ->execute()
      ->fetchAllAssoc('mlid',PDO::FETCH_ASSOC);

  // Get phase data for each proposal
  // For open proposals find min. start date
  // For closed proposals find max. end date
  $dates = array();
  foreach ($child_menu_data as $data) {
    preg_match("/node\/(\d+)$/", $data['link_path'], $matches);
    $nid = $matches[1];
    $node = node_load($nid);
    $phases = _smartparticipation_core_get_proposal_phase_data($node);
    if ($proposals_to_sort == 'open') {
      $start_dates = array();
      foreach ($phases as $phase) {
        $start_dates[] = strtotime($phase['dates']['start']);
      }
      $dates[$data['mlid']] = min($start_dates);
    }
    else {
      $end_dates = array();
      foreach ($phases as $phase) {
        $end_dates[] = strtotime($phase['dates']['end']);
      }
      $dates[$data['mlid']] = max($end_dates);
    }
  }

  // Sort descending for both open and closed proposals
  // Set weight on menu link accordingly
  arsort($dates);
  $weight=0;
  foreach ($dates as $mlid=>$date) {
    $menu = menu_link_load($mlid);
    $menu['weight'] = $weight++;
    menu_link_save($menu);
  }

}

function smartparticipation_core_menu_get_mlid($link_title) {
  $query = 'SELECT * FROM {menu_links} WHERE link_title = :link_title';
  $result = db_query($query,array(':link_title' => $link_title));
  $record = $result->fetchObject();
  $count = $result->rowCount();
  
  if($count > 0) {
    return $record->mlid;
  }
}


/*
 * This function will get a list of mlid for the child items for one menu item 
 */
function smartparticipation_core_menu_item_childs($plid) {
  $query = 'SELECT * FROM {menu_links} WHERE plid = :plid';
  $items = db_query($query,array(':plid' => $plid))
    ->fetchAllAssoc('mlid', PDO::FETCH_ASSOC);
  
  $items_list = array();
  foreach ($items as $item) {
    $items_list[] = $item['mlid'];
  }
  return $items_list;
}


/*
 * This function will disable the coresponding menu items if there are no open / closed proposals
 */
function smartparticipation_core_disable_unused_menus() {
  $proposal_type = ucfirst(variable_get('smartparticipation_site_proposal_type_plural'));
  $open_link_title = 'Open ' . $proposal_type;
  $closed_link_title = 'Closed ' . $proposal_type;
  
  $open_mlid = smartparticipation_core_menu_get_mlid($open_link_title);
  $closed_mlid = smartparticipation_core_menu_get_mlid($closed_link_title);

  $open_proposals = _smartparticipation_core_get_proposal_list('1');
  $closed_proposals = _smartparticipation_core_get_proposal_list('0');

  // update the menu item for the Open Proposals
  if($open_mlid != '') {
    $open_menu_item = menu_link_load($open_mlid);
    $open_menu_item['hidden'] = 0; // bt default the menu item is active
  
    if(count($open_proposals) == 0) {
      // No OPEN proposals
      //menu_link_delete($open_mlid, $path = NULL);
      $open_menu_item['hidden'] = 1; // the menu item is disabled
    
      // Save the item to database.
      menu_link_save($open_menu_item);
    }
  }

  // update the menu item for the Closed Proposals
  if($closed_mlid != '') {
    $closed_menu_item = menu_link_load($closed_mlid);
    $closed_menu_item['hidden'] = 0; // bt default the menu item is active
    if(count($closed_proposals) == 0) {
      // No CLOSED proposals
      //menu_link_delete($closed_mlid, $path = NULL);
      $closed_menu_item['hidden'] = 1; // the menu item is disabled
    
      // Save the item to database.
      menu_link_save($closed_menu_item);
    }
  }
}


/**
 * Implement hook_menu_alter(). 
 * 
 * Runs on install and update.
 */
function smartparticipation_core_menu_alter(&$items) {

  // Hide hybridauth tab on user account form
  $items['user/%/hybridauth'] = array(
    'type' => MENU_LOCAL_TASK,
  );
  
  // Grant those with "edit terms in [vid]" permission access to list the terms
  // and add new ones, per vocabulary. "edit terms in [vid]" alone does not 
  // grant this access.
  $items['admin/structure/taxonomy/%taxonomy_vocabulary_machine_name']['access callback'] = 'taxonomy_term_edit_access';
  $items['admin/structure/taxonomy/%taxonomy_vocabulary_machine_name']['access arguments'] = array(3);
  $items['admin/structure/taxonomy/%taxonomy_vocabulary_machine_name/add']['access callback'] = 'taxonomy_term_edit_access';
  $items['admin/structure/taxonomy/%taxonomy_vocabulary_machine_name/add']['access arguments'] = array(3);
  
  // The permission 'access administration pages' is given to site and content
  // editors in order to access the site glossary, but it also gives them access
  // to some Configuration pages. Remove that access here.
  // TODO Blocking access to the URL is a workaround. We should instead create
  // new permissions to define access to those Configuration pages, and revoke
  // that access from site and content editors.
  $items['admin/config']['access arguments'] = array('administer site configuration');
  // This one doesn't work - access still allowed. ??
  //$items['admin/config/development/devel_demo']['access arguments'] = array('administer site configuration');
  $items['admin/config/people/accounts']['access arguments'] = array('administer site configuration');

}

/**
 * Add shortcuts on module install.
 * 
 * Other shortcuts may be added in functions for individual module configuration
 * (e.g., forena). 
 * 
 */
function smartparticipation_core_add_shortcuts() {
  smartparticipation_core_add_smartparticipation_core_config_shortcut();
  smartparticipation_core_add_metatag_shortcut();
}

/**
 * Add a shortcut to the default shortcut set to the SmartParticipation configuration page,
 * for users who have access to this page but not to the main configuration
 * page. Unfortunately, Drupal doesn't handle this by default, because the page
 * is two levels down from admin/config. Users with access to
 * admin/config/system/smartparticipation_core can access admin/config/system, but not
 * admin/config, and therefore they don't get the toolbar menu item.
 *
 * NB Do NOT add this to smartparticipation_core_menu_alter(), because then the module assigned
 * is 'system' rather than 'menu', and no one has access to either the shortcut
 * or the page.
 */
function smartparticipation_core_add_smartparticipation_core_config_shortcut() {
  
  $shortcut = array(
    'link_path' => 'admin/config/system/smartparticipation_core',
    'link_title' => 'SmartParticipation site configuration',
    'menu_name' => 'shortcut-set-1',
    'weight' => 0,
  );
  menu_link_save($shortcut);
}

/**
 * Add a shortcut to the default shortcut set to the Metatag module 
 * configuration page.
 */
function smartparticipation_core_add_metatag_shortcut() {
  
  $shortcut = array(
    'link_path' => 'admin/config/search/metatags',
    'link_title' => 'Meta tags',
    'menu_name' => 'shortcut-set-1',
    'weight' => 10,
  );
  menu_link_save($shortcut); 
}

/**
 * Delete shortcuts added by the SmartParticipation module.
 */
function smartparticipation_core_delete_shortcuts() {
  
  menu_link_delete(null, 'admin/config/system/smartparticipation_core');
  
  $shortcut_set = shortcut_set_load('shortcut-set-1');
  $mlid = 0;
  foreach ($shortcut_set->links as $link) {
    if ($link['link_path'] == 'admin/config/search/metatags') {
      menu_link_delete($link['mlid']);
      break;
    }
  }


}

/**
 * When a sp_about node is added, deleted or updated, check whether there
 * are any published sp_about nodes. If not, remove the About item from the
 * main menu. If so, add the About item if it is not already present.
 * Note that this function runs BEFORE node_save() saves the changes to the 
 * database. There's no way to handle a subsequent failure of node_save(). This
 * is a weakness in the Drupal API: there should be a hook that runs 
 * AFTER node_save().
 * 
 * @param $node - the node object being modified
 * @param $action - the action being taken on the node. Possible values: 
 * 'delete', 'update'. 
 * 
 * TODO Determine whether 'add' case needs to be included. We already call
 * smartparticipation_core_autocreate_menu(), which may be sufficient. Leaving this case for
 * later.
 * 
 */
function smartparticipation_core_update_about_menu($node, $action) {

  $original = $node->original;
  
  // Updating an existing node
  if ($action == 'update') {
    
    // Publication status changed
    if ($node->status != $original->status) {
      
      // Changed from unpublished to published
      if ($node->status) {
        smartparticipation_core_show_main_menu_about_link();
        
      // Changed from published to unpublished
      } else {
        smartparticipation_core_hide_main_menu_about_link();
      }

    }
    smartparticipation_core_set_menu_param_from_nid('About', $node->nid, 'link_title', $node->title);

  }
  // Deleting a node
  else if ($action == 'delete') {
    smartparticipation_core_hide_main_menu_about_link();
  }
  
}


/**
 * Show the main menu About link if it is hidden.
 */
function smartparticipation_core_show_main_menu_about_link() {
  smartparticipation_core_toggle_main_menu_about_link(0);  
}

/**
 * Remove the About item from the main menu if there are no published
 * sp_about nodes.
 */
function smartparticipation_core_hide_main_menu_about_link() {

  $about_node_count = _smartparticipation_core_get_published_node_count_by_type('sp_about');
  
  if (! $about_node_count) {
    smartparticipation_core_toggle_main_menu_about_link(1);
  }
}

/**
 * Show or hide the main menu About link.
 * @param $hidden - the new value of the link's hidden property 
 */
function smartparticipation_core_toggle_main_menu_about_link($hidden) {
  
  // Could alternatively do a direct SQL query for the mlid, and avoid the loop.
  $main_menu_links = menu_tree_all_data('main-menu', null, 1);

  foreach ($main_menu_links as $link_item) {
    
    $link = $link_item['link'];
    if ($link['title'] == t('About')) {
      
      if ($link['hidden'] != $hidden) {
        $link['hidden'] = $hidden;
        menu_link_save($link);
      }
      
      break;
    }
  }
}

/**
 * Show or hide the main menu 'Open Proposals' link.
 * @param $hidden - the new value of the link's hidden property
 */
function smartparticipation_core_toggle_main_menu_open_proposals_link($hidden) {

  // Could alternatively do a direct SQL query for the mlid, and avoid the loop.
  $main_menu_links = menu_tree_all_data('main-menu', null, 1);

  foreach ($main_menu_links as $link_item) {

    $link = $link_item['link'];
    if ($link['title'] == t('Open Proposals')) {

      if ($link['hidden'] != $hidden) {
        $link['hidden'] = $hidden;
        menu_link_save($link);
      }

      break;
    }
  }
}

/**
 * Show or hide the main menu 'Closed Proposals' link.
 * @param $hidden - the new value of the link's hidden property
 */
function smartparticipation_core_toggle_main_menu_closed_proposals_link($hidden) {

  // Could alternatively do a direct SQL query for the mlid, and avoid the loop.
  $main_menu_links = menu_tree_all_data('main-menu', null, 1);

  foreach ($main_menu_links as $link_item) {

    $link = $link_item['link'];
    if ($link['title'] == t('Closed Proposals')) {

      if ($link['hidden'] != $hidden) {
        $link['hidden'] = $hidden;
        menu_link_save($link);
      }

      break;
    }
  }
}

/**
 *
 * Set menu parameter from menu title ('About', 'Learn', ...) and nid
 *
 * @param $menu
 * @param $nid
 * @param $param
 * @param $value
 */
function smartparticipation_core_set_menu_param_from_nid($menu, $nid, $param, $value) {

  $main_menu_links = menu_tree_all_data('main-menu');

  foreach ($main_menu_links as $link_item) {

    $link = $link_item['link'];

    if ($link['title'] == t($menu)) {
      foreach ($link_item['below'] as $about_item) {
        if ($about_item['link']['link_path'] == 'node/' . $nid) {
          $about_item['link'][$param] = $value;
          menu_link_save($about_item['link']);
          break;
        }
      }
    }

  }

}

/**
 * Show or hide the footer menu About link.
 * @param $hidden - the new value of the link's hidden property
 */
function smartparticipation_core_toggle_footer_menu_about_link($hidden) {

  // Could alternatively do a direct SQL query for the mlid, and avoid the loop.
  $footer_menu_links = menu_tree_all_data('sp-footer-menu', null, 1);

  //dvm($footer_menu_links);

  foreach ($footer_menu_links as $link_item) {

    $link = $link_item['link'];
    if ($link['title'] == t('About')) {

      if ($link['hidden'] != $hidden) {
        $link['hidden'] = $hidden;
        menu_link_save($link);
      }

      break;
    }
  }
}


/**
 * Implements template_preprocess_menu_link().
 * 
 * @param $variables
 */
function smartparticipation_core_preprocess_menu_link(&$variables) {

  $archive_proposals = &drupal_static('archive_proposals');
  $archive_dialog = &drupal_static('archive_dialog');

  if (!$archive_proposals) {

    $archive_proposals =
        db_select('field_data_field_sp_url', 'u')
            ->fields('u', array('entity_id', 'field_sp_url_value'))
          ->condition('u.bundle', 'sp_proposal')
            ->execute()
            ->fetchAllKeyed();

  }

  $element_href = $variables['element']['#href'];

  // replace the node href with the archive url if it exists
  if (substr($element_href, 0, 5) == 'node/') {
    $nid = explode('/', $element_href);
    $nid = $nid[1];
    if (array_key_exists($nid, $archive_proposals)) {
      $variables['element']['#href'] = $archive_proposals[$nid];
      $variables['element']['#attributes']['class'][] = 'archive';
      $archive_dialog = true;
    }
  }

  // $variables doesn't tell us which menu we're in; we can only refer to the
  // link element itself. 
  if ($variables['element']['#href'] == 'announcements') {
    $count = _smartparticipation_core_new_announcement_count();
    if ($count) {
      $variables['element']['#title'] = $variables['element']['#title'] . ' <span>' . $count . '</span>';
      $variables['element']['#localized_options']['attributes']['title'] = t($count . ' new announcement' . ($count > 1 ? 's' : ''));
      $variables['element']['#localized_options']['html'] = true;
    }
  }


  // Hide unpublished About menu items,  Hide About menu if no items are published
  $published_about_nids = array_keys(_smartparticipation_core_get_published_nodes_by_type('sp_about'));
  $all_about_nids = array_keys(_smartparticipation_core_get_nodes_by_type('sp_about'));
  if (empty($published_about_nids)) {
    smartparticipation_core_toggle_main_menu_about_link(1);
    smartparticipation_core_toggle_footer_menu_about_link(1);
  }
  else {
    smartparticipation_core_toggle_main_menu_about_link(0);
    smartparticipation_core_toggle_footer_menu_about_link(0);
    if (isset($nid)) {
      if (!in_array($nid,$published_about_nids) && in_array($nid,$all_about_nids)) {
        $variables['element']['#attributes']['style'][] = "display:none";
      }
    }
  }

}
