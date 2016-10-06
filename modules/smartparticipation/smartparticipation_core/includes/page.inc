<?php

/** 
 * Custom RegRoom page handling. 
 */

/**
 * Implements hook_page_alter().
 */
function smartparticipation_core_page_alter(&$page) {
  $page = smartparticipation_core_add_login_and_register_forms($page);
  $archive_dialog = &drupal_static('archive_dialog');
  $profile_reminder = !empty($_SESSION['smartparticipation_core_profile_reminder']);
  if ($archive_dialog) {
    $page = smartparticipation_core_add_archive_notice_dialog($page);
  }
  if ($profile_reminder) {
    // Only show the profile reminder once after login
    unset($_SESSION['smartparticipation_core_profile_reminder']);
    $page = smartparticipation_core_add_profile_reminder_dialog($page);
  }
  $endorsement_service = new \SmartParticipation\EndorsementService;
  if ($endorsement_service->canViewCommentEndorsementCounter() && $endorsement_service->canViewCommentEndorsementUsers()) {
    $page = smartparticipation_core_add_comment_endorsements_users_dialog($page);
  }
}

function smartparticipation_core_add_comment_endorsements_users_dialog($page)
{
  $markup =
    theme('dialog', array(
      'dlgId' => 'dlgCommentEndorsementsUsers',
      'title' => t('Comment endorsements'),
      'content' => null, // will be populated by JavaScript behavior
    ));

  $page['page_bottom']['smartparticipation_core_comment_endorsements_users'] = array(
    '#markup' => $markup,
  );

  return $page;
}

function smartparticipation_core_add_profile_reminder_dialog($page)
{
  $markup =
    theme('dialog', array(
      'dlgId' => 'dlgProfileReminder',
      'title' => t('Share some info about yourself!'),
      'content' => theme('profile_reminder'),
    ));

  $page['page_bottom']['smartparticipation_core_profile_reminder'] = array(
    '#markup' => $markup,
  );

  return $page;
}

/**
 * Add the archive notice dialog to the page if there are archived items in the closed proposals menu.
 */
function smartparticipation_core_add_archive_notice_dialog($page)
{
  $markup =
    theme('dialog', array(
      'dlgId' => 'dlgArchive',
      'title' => t('Archive Redirect Notice'),
      'content' => theme('archive_notice'),
    ));

  $page['page_bottom']['smartparticipation_core_archive'] = array(
    '#markup' => $markup,
  );

  return $page;
}

/**
 * Load the AddThis JavaScript on the topic page for comment sharing.
 */
function _smartparticipation_core_addthis_script() {
  
  $pubid = variable_get('smartparticipation_addthis_pubid');
    
  if ($pubid) {
        
    $addthis_script =    
      // Disable AddThis hash tag on urls
      'var addthis_config = {"data_track_addressbar": false, "data_track_clickback": false};
       var addthis_share = {
         url_transforms : {
           shorten: {
             twitter: \'bitly\'
           }
         },
         shorteners : {
           bitly : {}   
         }  
      }';
         
    drupal_add_js($addthis_script, array(
      'type' => 'inline',
      'scope' => 'footer',
    ));
      
    // NB This is correct even though it looks wrong!
    drupal_add_js('//s7.addthis.com/js/300/addthis_widget.js#pubid=ra-'. $pubid, 'external');
      
  }

}

function smartparticipation_core_get_profile_path_parts()
{
  return explode('/', drupal_get_normal_path(request_path()));
}

function smartparticipation_core_is_my_public_profile()
{
  global $user;
  $path_parts = smartparticipation_core_get_profile_path_parts();
  if ($path_parts[0] == 'user' && $path_parts[2] == 'profile') {
    if ($path_parts[1] == $user->uid && empty($path_parts[3])) {
      return true;
    }
  }
  return false;
}

function smartparticipation_core_is_my_profile()
{
  global $user;
  $path_parts = smartparticipation_core_get_profile_path_parts();
  if ($path_parts[0] == 'user' && $path_parts[1] == $user->uid) {
    return true;
  }
  return false;
}

function smartparticipation_core_is_profile()
{
  $path_parts = smartparticipation_core_get_profile_path_parts();
  if ($paths_parts[0] == 'user') {
    return true;
  }
  return false;
}

/**
 * Implements hook_preprocess_page().
 * 
 * TODO At least some of this should be in the theme rather than the module.
 */
function smartparticipation_core_preprocess_page(&$variables) {

  $drupal_page_service = new \SmartParticipation\DrupalPageService;
  if ($drupal_page_service->pageContainsInterestSurvey($variables)) {
    // Disable messages for the page template and instead show them in the
    // interest survey node template.  See smartparticipation_core_preprocess_node().
    $drupal_page_service->disablePageMessages($variables);
  }
  
  $module_path = drupal_get_path('module', 'smartparticipation_core');
  $js_path = $module_path . '/js/';
  // NB request_path() returns a path alias, current_path() Drupal's internal
  // path.
  $request_path = request_path();
  $path_parts = explode('/', $request_path);

  $is_my_public_profile = smartparticipation_core_is_my_public_profile();

  $theme_name = variable_get('theme_default');
  $theme_css_path = drupal_get_path('theme', $theme_name) . '/css/';
  //$base_theme_css_path = path_to_theme() . '/css/';
  $base_theme_css_path = drupal_get_path('theme', 'smartparticipation_base') . '/css/';
    
  // All pages CSS

  // Not on admin pages
  if (arg(0) != 'admin') {
    // Load CSS for a recent version of jQuery UI
    drupal_add_css(libraries_get_path('jquery_ui') . '/jquery-ui-1.11.4/jquery-ui.structure.min.css', array('group' => CSS_SYSTEM, 'every_page' => TRUE));
  }

  drupal_add_css($base_theme_css_path . 'h5bp/normalize.css');
  drupal_add_css($base_theme_css_path . 'h5bp/main.css');
  //drupal_add_css($base_theme_css_path . 'fonts.css');
  if ($GLOBALS['theme'] != variable_get('admin_theme')) {
    // Get the last modified timestamp of the theme css file.
    // Although the timestamp is not part of the actual filename, it is used
    //   in the source name below when adding the css to the markup.  This will
    //   be rewritten with an .htaccess rule to point to the actual file without
    //   the timestamp in the name.  This prevents browsers from keeping a
    //   cached version of the CSS after a change is made.
    //$theme_css_last_modified = filemtime(drupal_realpath(drupal_get_path('theme', $theme_name)).'/css/'.$theme_name.'.css');
    // preprocess is false to prevent the theme css from using Drupal compression.
    // When Drupal aggregation and comprssion is used on this file, some styles go missing.
    //drupal_add_css($theme_css_path . variable_get('theme_default') . '.' . $theme_css_last_modified . '.css',array('preprocess'=>false));

    drupal_add_css($theme_css_path . variable_get('theme_default') . '.css');
  }

  // All pages JavaScript
  //drupal_add_js(libraries_get_path('smartparticipation_core') . '/smartparticipation_core.libraries.js');
  drupal_add_js($module_path . '/build/js/smartparticipation.scripts.js');
  // preprocess is false to prevent jw player from using Drupal compression
  // jw player is unable to include dependent scripts when Drupal compression is turned on
  drupal_add_js($js_path . 'jwplayer/jwplayer.js',array('preprocess'=>false));
  smartparticipation_core_init_jwplayer();

  // TODO gulp.js is now used to compress and concatenate scripts - consider removing the unused "add js" code below

  /*drupal_add_js(libraries_get_path('modernizr') . '/modernizr-2.6.2.min.js');
  drupal_add_js(libraries_get_path('jquery_ui') . '/jquery-ui-1.9.1.custom.min.js');
  drupal_add_js(libraries_get_path('bootstrap-tooltip') . '/tooltip.js');
  drupal_add_js(libraries_get_path('jTruncate') . '/jquery_truncator.js');
  drupal_add_js($js_path . 'alert.js');*/

  // Negative conditions
  /*if ($path_parts[0] != 'reports') {
    drupal_add_js(libraries_get_path('infieldlabel') . '/jquery.infieldlabel.min.js');
  }*/
  
  /*if (smartparticipation_core_log_events()) {
    drupal_add_js($js_path . 'event_logger.js');
  }*/
  
  // Enable Drupal Ajax framework on all pages, not just on forms.
  // drupal_add_library('system', 'drupal.ajax');
              
  // Preprocess based on node type
  // TODO Can we move these to the node preprocessing functions, or should the
  // js be added at the page level? Does it matter?
  if ( !empty($variables['node']) ) {

    $node_type = $variables['node']->type;    
    switch ($variables['node']->type) {

      case 'sp_document':
        /*drupal_add_js($js_path . 'topic_header.js');
        drupal_add_js(libraries_get_path('jquery_ui_carousel') . '/jquery.rs.carousel.js');
        drupal_add_js($js_path . 'carousel.js');*/
        break;

      case 'sp_learn':
        /*drupal_add_js(libraries_get_path('liteaccordion') . '/liteaccordion.jquery.min.js');
        drupal_add_js($js_path . 'learn_accordion.js');*/
        break;

      case 'sp_proposal':
        // Not on editing tabs     
        /*if ($path_parts[0] != 'node') {
          drupal_add_js(libraries_get_path('jquery_ui_carousel') . '/jquery.rs.carousel.js');
          drupal_add_js(libraries_get_path('liteaccordion') . '/liteaccordion.jquery.min.js');
          drupal_add_js($js_path . 'learn_accordion.js');
          drupal_add_js($js_path . 'carousel.js');
          drupal_add_js($js_path . 'proposal_phase_tabs.js');
          drupal_add_js($js_path . 'proposal.js');
        }*/
        break;

      case 'sp_team_member':
        smartparticipation_core_preprocess_team_member_page($variables);
        break;

      case 'sp_topic':
        /*drupal_add_js(libraries_get_path('mutate') . '/mutate.js');
        drupal_add_js(libraries_get_path('jquery_ui_multi_open_accordion') . '/jquery.multi-accordion.js');
        drupal_add_js(libraries_get_path('jquery_ui_carousel') . '/jquery.rs.carousel.js');
        drupal_add_js(libraries_get_path('jquery_easing') . '/jquery.easing.1.3.js');
        drupal_add_js(libraries_get_path('ua_parser') . '/ua-parser-0.6.2.min.js');
        drupal_add_js($js_path . 'carousel.js');
        drupal_add_js($js_path . 'plugins/jquery.ba-throttle-debounce.js');
        drupal_add_js($js_path . 'topic.js');
        drupal_add_js($js_path . 'topic_header.js');*/
//        drupal_add_js(libraries_get_path('ckeditor') . '/ckeditor.js');
        
        _smartparticipation_core_addthis_script();
        
        if (smartparticipation_core_moderator_access()){
          drupal_add_js($js_path . 'moderator/moderator_controls.js');
        }
      
        break;
        
    }

  // Preprocess based on request path
  
  } else if (drupal_is_front_page()) {
    
    smartparticipation_core_preprocess_front_page($variables, $js_path);

   
//  } else if ($request_path == 'faq') {
    
    //drupal_add_js($js_path . 'faqs.js');
  
  } else if ($request_path == 'moderator') {

    //drupal_add_js(libraries_get_path('jquery_ui_multi_open_accordion') . '/jquery.multi-accordion.js');
    drupal_add_js($js_path . 'moderator/moderator_interface.js');
    // Moved moderator css into Sass file _smartparticipation_core.moderator.scss so it's now included in the main theme css file.
    // This was done to use the theme color in the moderator interface.
    //drupal_add_css(path_to_theme() . '/css/moderator.css', array('weight' => 999, 'group' => CSS_THEME));


  } else if ($is_my_public_profile) {

    global $user;

    $variables['title_prefix'] = array(
      '#markup' => l(
        t('Edit my profile'),
        'user/'.$user->uid.'/profile/edit',
        array('attributes' => array('class' => 'button edit-my-profile'))
      )
    );

//  } else if ($path_parts[0] == 'user' || $path_parts[0] == 'users') {

    //drupal_add_js(libraries_get_path('jquery_ui_multi_open_accordion') . '/jquery.multi-accordion.js');
    //drupal_add_js($js_path . 'profile.js', 'file');
    
  }
  
  smartparticipation_core_set_page_variables($variables);

}

/**
 * Preprocess the front page.
 */
function smartparticipation_core_preprocess_front_page(&$variables, $js_path) {
  
  // There are open proposals
  if (! empty($variables['page']['content']['smartparticipation_core_open_proposals']['#items'])) {
	$items = array();
	
    $recent_comments = module_invoke('smartparticipation_core', 'block_view', 'recent_comments');
    $items['smartparticipation_core_recent_comments'] = $recent_comments;

   	$recommended_comments = module_invoke('smartparticipation_core', 'block_view', 'recommended_comments');
	$items['smartparticipation_core_recommended_comments'] = $recommended_comments;
   
  } else {
    // Later there may be other features to display if there are no open 
    // proposals.
    $items = array();
    hide($variables['page']['content']['smartparticipation_core_open_proposals']);
  }
  
  $variables['page']['content']['features'] = smartparticipation_core_page_features($items);
  
  
  /*drupal_add_js(libraries_get_path('liteaccordion') . '/liteaccordion.jquery.min.js');
  drupal_add_js(libraries_get_path('jquery_ui_carousel') . '/jquery.rs.carousel.js');
  drupal_add_js($js_path . 'learn_accordion.js');
  drupal_add_js($js_path . 'carousel.js');
  drupal_add_js($js_path . 'proposal_phase_tabs.js');*/
  
}

/**
 * Implements template_preprocess_maintenance_page().
 */
function smartparticipation_core_preprocess_maintenance_page(&$variables) {
  smartparticipation_core_set_page_variables($variables);
}

/**
 * Set variables to add to all pages, including maintenance page.
 * NB hook_preprocess_page() doesn't get called for the maintenance page.
 */
function smartparticipation_core_set_page_variables(&$variables) {

  // TODO This turns out to be a bad strategy. When called from a template, 
  // path_to_theme() will return the parent or child value depending on which 
  // theme the template belongs to. This should be correct, so get rid of these
  // variables and make the calls from the templates.
  $variables['theme_path'] = $variables['base_path'] . path_to_theme();
  $variables['image_path'] = $variables['theme_path'] . '/images/';
  
  $variables['child_theme_path'] = $variables['base_path'] . variable_get('theme_default') ;
  $variables['child_theme_image_path'] = $variables['child_theme_path'] . '/images/';  

  $variables['site_proposal_type'] = variable_get('smartparticipation_site_proposal_type');
  $variables['site_proposal_type_plural'] = variable_get('smartparticipation_site_proposal_type_plural');
  
}

/**
 * Store settings values for use with custom slide-down alerts.
 */
function smartparticipation_core_init_alert() {
  $alert_settings['loginLink'] = l(t('log in'), 'user/login');
  $alert_settings['registerLink'] = l(t('register'), 'user/register');
  $alert_settings['interestSurveyLink'] = 
      l(t('interest survey'), current_path(), array(
          'fragment' => 'interestSurvey',
          'attributes' => array(
            'id' => 'interestSurveyLink',
           )
      ));
  $alert_settings['template'] = theme('alert');
  drupal_add_js(array('rrAlert' => $alert_settings), 'setting');
}

/**
 * Store settings values for use with dialogs/lightboxes.
 */
function smartparticipation_core_init_dialog() {
  $dialog_settings['template'] = theme('dialog');
  drupal_add_js(array('rrDialog' => $dialog_settings), 'setting');
}

/**
 * Add JW Player key if it has been added in the configuration.
 */
function smartparticipation_core_init_jwplayer() {
  $key = trim(variable_get('smartparticipation_video_player_license_key'));
  if (!empty($key)) {
    drupal_add_js('jwplayer.key="'.$key.'";', 'inline');
  }
}

/**
 * Generate a render array for the features section of a page (currently front 
 * page and proposal page). 
 * NB This is NOT a Drupal block because the block content is passed in as an 
 * argument. It would not be usable from the user interface.
 * 
 * @param $items (optional) - array of items to display, defaults to empty array
 * @param $title (optional) - the title of the section
 * 
 * @return a render array for the features section
 * 
 */
function smartparticipation_core_page_features($items = array(), $title = '') {

  if (empty($title)) {
    $title = t('What\'s Happening Now');
  }
  
  $features = array(
    '#items' => $items,
    'title' => $title,
  );
  
  $recommended_comments_service = new \SmartParticipation\RecommendedCommentsService;
  $recommended_comments_enabled = $recommended_comments_service->getStatus();

  if ($recommended_comments_enabled) {
	$features['#theme'] = 'features';
  }
  else {
	$features['#theme'] = 'features_1col';  
  }
  
  // Suppress title and div markup if there are no features to display.
  if (empty($items)) {
    hide($features);
  }
  
  return $features;
}

/**
 * Implements hook_js_alter().
 */
function smartparticipation_core_js_alter(&$javascript) {

  if (module_exists('google_analytics')) {
    // Move Google Analytics script to the footer region
    $js = $javascript;
    $ga_account = variable_get('googleanalytics_account');
    foreach ($js as $key => $script) {
      if (is_numeric($key)) {
        if (!empty($ga_account)) {
          $data = $javascript[$key]['data'];
          if (strpos($data, $ga_account) !== FALSE) {
            $javascript[$key]['scope'] = 'footer';
          }
        }
      }
      else {
        $ga_module_path = drupal_get_path('module', 'google_analytics');
        $ga_js_path = $ga_module_path . '/googleanalytics.js';
        switch ($key) {
          case $ga_js_path:
            $javascript[$key]['scope'] = 'footer';
            break;
        }
      }
    }
  }

  // Use a newer jQuery version for libraries used by this module.
  // Based on the jQuery Multi module.
  // Add the following files:
  //  1) Newer jQuery
  //  2) Libraries script (concatenated library scripts used by this module)
  //  3) Switch script (stores newer jQuery instance and switches back to default instance/version)
  $jquery_version = '1.8.2';
  $jquery_path = libraries_get_path('jquery') . '/jquery-' . $jquery_version . '.min.js';
  $switch_path = drupal_get_path('module', 'smartparticipation_core') . '/js/jqmulti/switch.js';
  $libraries_path = drupal_get_path('module', 'smartparticipation_core') . '/build/js/smartparticipation.libraries.js';

  $jquery = array(
    $jquery_path => array(
      'weight' => -100,
      'group' => -51,
    )
  );
  $libraries = array(
    $libraries_path => array(
      'weight' => 100,
      'group' => -50,
    ),
  );
  $switch = array(
    $switch_path => array(
      'weight' => 100,
      'group' => -49,
    ),
  );

  $files = array_merge($jquery, $libraries, $switch);

  foreach ($files as $file => $options) {
    if (!array_key_exists($file, $javascript)) {
      $javascript[$file] = drupal_js_defaults();
      $javascript[$file]['data'] = $file;
    }
    if (isset($options['group'])) {
      $javascript[$file]['group'] = $options['group'];
    }
    if (isset($options['weight'])) {
      $javascript[$file]['weight'] = $options['weight'];
    }
  }
}