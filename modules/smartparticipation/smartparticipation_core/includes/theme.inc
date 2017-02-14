<?php

/**
 * @file 
 * Functions for smartparticipation_core theming.
 * 
 * @ingroup smartparticipation_core
 * 
 * NB This file cannot be smartparticipation_core/includes/theme.inc: Drupal doesn't find it. 
 * There must be a name conflict with a system file, but why wouldn't Drupal 
 * check the full path??? Both OK: smartparticipation_core/includes/themes.inc, 
 * smartparticipation_core/include/theme.inc.
 */

 /** 
 * Set default smartparticipation_core themes.
 * 
 */
function smartparticipation_core_set_default_smartparticipation_core_themes()
{
  //$site_machine_name = variable_get('smartparticipation_site_machine_name');
  // acus@recommendationroom; regulationroom
  // Obviously in future there might be other types of names.
  //$names = explode('@', $site_machine_name);
  //$default_theme = $names[0];

  //  Overriding above code so that a proper theme gets installed
  $default_theme = 'smartparticipation_default';
  $admin_theme = 'smartparticipation_admin';

  smartparticipation_core_set_default_themes($default_theme, $admin_theme);

  // Apply admin theme settings
  variable_set('theme_' . $admin_theme . '_settings', array(
    // Admin theme should use default theme logo
      'logo_path' => theme_get_setting('logo_path', $default_theme),
  ));

  // Modify nice menus custom block values
  variable_set('nice_menus_name_1', 'Main Menu');
  variable_set('nice_menus_type_1', 'down');
  variable_set('nice_menus_menu_1', 'main-menu:0');

  // Move the nice menu block to the header
  db_merge('block')
        ->key(array(
            'module'=>'nice_menus',
            'delta'=> '1',
            'theme'=> $default_theme
        ))
        ->fields(array(
            'status' => 1,
            'region' => 'header',
            'pages' => '',
        ))
        ->execute();

}

/**
 * Set default themes.
 * 
 */
function smartparticipation_core_set_default_themes($default_theme, $admin_theme) {

  $themes_to_enable = array(
    $admin_theme,
    $default_theme,
  );
    
  try {
    
    theme_enable($themes_to_enable);

    // Change theme_default before calling theme_disable(), else bartik, the
    // default theme on install, won't be disabled.
    variable_set('admin_theme', $admin_theme);
    variable_set('theme_default', $default_theme);
            
    $available_themes = list_themes();
    $available_theme_names = array_keys($available_themes);
      
    $themes_to_disable = array_diff($available_theme_names, $themes_to_enable);

    theme_disable($themes_to_disable);

  } catch (Exception $e) {
    echo "Error configuring default smartparticipation_core themes. Please make sure necessary " .
         "themes are available.\n";
    throw new Exception("Cannot configure smartparticipation_core themes.", 0, $e);
  }
}
 

/**
* Implements hook_theme().
 * 
 * Module-defined theme functions must be registered here.
*/

function smartparticipation_core_theme($existing, $type, $theme, $path) {

  $theme_path = path_to_theme();

  /*if ($theme_path == drupal_get_path('theme', 'smartparticipation_admin')) {
    return array();
  }*/

  $base_theme_path = drupal_get_path('theme', 'smartparticipation_base');
  $base_theme_template_path = $base_theme_path . '/templates';
  $base_theme_block_template_path = $base_theme_template_path . '/blocks';
  $base_theme_custom_page_template_path = $base_theme_template_path . '/custom_pages';
  $base_theme_partials_path = $base_theme_template_path . '/partials';
  $base_theme_partials_complete_reg_path = $base_theme_partials_path . '/complete_reg';
  $base_theme_user_template_path = $base_theme_template_path . '/user';
  $base_theme_moderator_template_path = $base_theme_template_path . '/moderator';
    
  return array(

      'about_links' => array(
          'path' => $base_theme_block_template_path,
          'render element' => 'about_links',
          'template' => 'about_links',
      ),

    'accept_terms_notice' => array(
      'path' => $base_theme_partials_complete_reg_path,
      'template' => 'accept_terms_notice',
    ),
    
    'add_link' => array(
      'path' => $base_theme_partials_path,
      'template' => 'add_link',
    ),

    'alert' => array(
      'path' => $base_theme_partials_path,
      'template' => 'alert',
    ),
      
    // TODO Reimplement using node views. Then these theme definitions won't be
    // needed. See About pages: media, research & publications, sponsors,
    // team.
    'announcement' => array(
      'path' => $base_theme_partials_path,
      'render element' => 'announcement',
      'template' => 'announcement',   
    ),
    'announcements' => array(
      'path' => $base_theme_custom_page_template_path,
      'render element' => 'announcements',
      'template' => 'announcements',   
    ),

    'archive_notice' => array(
      'path' => $base_theme_partials_path,
      'template' => 'archive_notice',
    ),

      'carousel' => array(
          'path' => $base_theme_partials_path,
          'render element' => 'carousel_data',
          'template' => 'carousel',
      ),

      'carousel_prompt' => array(
      'path' => $base_theme_partials_path,
      'render element' => 'prompt',
      'template' => 'carousel_prompt',        
    ),
      
    'closed_proposal' => array(
      'path' => $base_theme_partials_path,
      'render element' => 'proposal',
      'template' => 'closed_proposal',
    ),
      
    'closed_proposals' => array(
      'path' => $base_theme_block_template_path,
      'render element' => 'proposals',
      'template' => 'closed_proposals',
    ),
      
    'coming_soon' => array(
      'path' => $base_theme_block_template_path,
      'render element' => 'coming_soon',
      'template' => 'coming_soon', 
    ),
    
    'comment_links_moderator' => array(
      'path' => $base_theme_moderator_template_path,
      // Name of the structured array that will hold the renderable information.
      'render element' => 'comment_links',
      'template' => 'comment-links-moderator',             
    ),

      'contact' => array(
          'path' => $base_theme_custom_page_template_path,
          'render element' => 'contact',
          'template' => 'contact',
      ),

      'dialog' => array(
      'path' => $base_theme_partials_path,
      'template' => 'dialog',
    ),
    
    'document_carousel' => array(
      'path' => $base_theme_partials_path,
      'template' => 'node_carousel',
    ),

    'does_not_accept_terms' => array(
      'path' => $base_theme_partials_complete_reg_path,
      'template' => 'does_not_accept_terms',
    ),

    // TODO Reimplement using node views. Then these theme definitions won't be
    // needed. See About pages: media, research & publications, sponsors,
    // team.    
    'faqs' => array(
      'path' => $base_theme_custom_page_template_path,
      'render element' => 'faq_categories',
      'template' => 'faqs',   
    ),

    'featured_comments' => array(
      'path' => $base_theme_block_template_path,
      // Name of the structured array that will hold the renderable information.
      'render element' => 'featured_comments',
      // We need to supply the template name explicitly, because it is in the
      // module rather than the theme.
      'template' => 'featured_comments',
    ),

    'featured_comments_items' => array(
      'path' => $base_theme_partials_path,
      'render element' => 'featured_comments_data',
      'template' => 'featured_comments_items',
    ),
      
    'features' => array(
      'path' => $base_theme_partials_path,
      'render element' => 'features',
      'template' => 'features', 
    ),

    'features_1col' => array(
      'path' => $base_theme_partials_path,
      'render element' => 'features',
      'template' => 'features_1col', 
    ),

    'file_link_new_window' => array(
      'function' => 'smartparticipation_core_theme_file_link_new_window',
      'variables' => array(
        'file' => NULL,
        'icon_directory' => NULL,
        'settings' => NULL,
      ),
    ),
      
    'footer' => array(
        'path' => $base_theme_partials_path,
        'template' => 'footer',
        'render element' => 'footer',
    ),
    
    'footer_block_links' => array(
      'path' => $base_theme_partials_path,
      'template' => 'footer_block_links',     
    ),
    
    'header' => array(
      'path' => $base_theme_partials_path,
      'template' => 'header',
      'variables' => array(
        'logo' => '/'. $theme_path .'/logo.png',
        'site_name' => variable_get('site_name'),
        'page_header' => NULL, // the render array
      ),      
    ),

    'learn_menu_links' => array(
      'path' => $base_theme_partials_path,
      // Name of the structured array that will hold the renderable information.
      'render element' => 'learn_links',
      'template' => 'learn_links',     
    ),
                 
    'learn_panel' => array(
      'path' => $base_theme_partials_path,
      // Name of the structured array that will hold the renderable information.
      'render element' => 'panel',
      'template' => 'learn_panel',     
    ),

    'learn_panel_mobile' => array(
      'path' => $base_theme_partials_path,
      'render element' => 'panel',
      'template' => 'learn_panel_mobile',
    ),

    'messages' => array(
      'path' => $base_theme_partials_path,
      'template' => 'messages',
    ),
      
    'moderator' => array(
      'path' => $base_theme_moderator_template_path,
      'render element' => 'data',
      'template' => 'moderator',
    ),

    'moderator_comment' => array(
      'path' => $base_theme_moderator_template_path,
      'render element' => 'comment',
      'template' => 'moderator-comment',
    ),
      
    'moderator_comment_filter' => array(
      'path' => $base_theme_moderator_template_path,
      'render element' => 'data',        
      'template' => 'moderator-comment-filter',
    ),

    'moderator_no_comments' => array(
      'path' => $base_theme_moderator_template_path,
      'template' => 'moderator-no-comments',
    ),
      
    'moderator_user_view' =>  array(
      'path' => $base_theme_moderator_template_path,
      'render element' => 'user_data',
      'template' => 'moderator-user-view',
    ),
       
    'node_carousel' => array(
      'path' => $base_theme_partials_path,
      'render element' => 'carousel_data',        
      'template' => 'node_carousel',
    ),
      
    'profile_nav_comment_count' =>array(
      'path' => $base_theme_partials_path,
      'render element' => 'count',        
      'template' => 'profile_nav_comment_count',    
    ),

    'profile_reminder' => array(
      'path' => $base_theme_partials_path,
      'template' => 'profile_reminder',
    ),
    
    'open_proposals' => array(
      'path' => $base_theme_block_template_path,
      'render element' => 'proposals',
      'template' => 'open_proposals',
    ),
      
    'proposal_overview' => array(
      'path' => $base_theme_partials_path,
      'render element' => 'proposal',
      'template' => 'proposal_overview',
    ),
    
    'smartparticipation_core_node_order' => array(
      'render element' => 'element',
    ),
    
    'smartparticipation_core_taxonomy_overview_terms' => array(
      'render element' => 'form',
    ),
    
    'social_login' => array(
      'path' => $base_theme_partials_path,
      'template' => 'social_login',
    ),

    'tabbed_topic_carousel' => array(
      'path' => $base_theme_partials_path,
      'render element' => 'tabbed_topic_carousel_data',
      'template' => 'tabbed_topic_carousel',
    ),
      
    'topic_header' => array(
      'path' => $base_theme_partials_path,
      'template' => 'topic_header',    
    ),

      'user_comments_by_proposal' => array(
          'path' => $base_theme_user_template_path,
          'render element' => 'user_comment_data',
          'template' => 'user-comments-by-proposal',
      ),

      'user_profile_links' => array(
          'path' => $base_theme_block_template_path,
          'render element' => 'user_profile_links',
          'template' => 'user_profile_links',
      ),

      'user_proposals' => array(
          'path' => $base_theme_user_template_path,
          'render element' => 'user_comment_data',
          'template' => 'user-proposals',
      ),

      'user_picture' => array(
      'path' => $base_theme_user_template_path,
      'render element' => 'user_data',
      'template' => 'user-picture',
    ),

    'user_stats' =>  array(
      'path' => $base_theme_user_template_path,
      'render element' => 'user_data',
      'template' => 'user-stats',
    ),

    'user_thumbnail' => array(
      'path' => $base_theme_user_template_path,
      'render element' => 'user_data',
      'template' => 'user-thumbnail',
    ),

  );
  
}

/**
 * Returns HTML for a link to a file that opens in a new window.
 *
 * @param $variables
 *   An associative array containing:
 *   - file: A file object to which the link will be created.
 *   - icon_directory: (optional) A path to a directory of icons to be used for
 *     files. Defaults to the value of the "file_icon_directory" variable.
 *
 * Copied from file.module theme_file_link(), added link text setting and target 
 * attribute to open in new window.
 */
function smartparticipation_core_theme_file_link_new_window($variables) {
  
  $file = $variables['file'];
  $icon_directory = $variables['icon_directory'];

  $url = file_create_url($file->uri);
  $icon = theme('file_icon', array('file' => $file, 'icon_directory' => $icon_directory));

  // Set options as per anchor format described at
  // http://microformats.org/wiki/file-format-examples
  $options = array(
    'attributes' => array(
      'type'
      => $file->filemime . '; length=' . $file->filesize,
      'target' => '_blank',
    ),
  );

  if (isset($variables['settings']['link_text'])) {
    $link_text = $variables['settings']['link_text'];
  } else if (empty($file->description)) {
    $link_text = $file->filename;
  } else {
    $link_text = $file->description;
    $options['attributes']['title'] = check_plain($file->filename);
  }

  return '<p class="file">' . $icon . ' ' . l($link_text, $url, $options) . '</p>';
}


/**
 * Implements hook_themes_enabled().
 *
 * @param $theme_list
 */
function smartparticipation_core_themes_enabled($theme_list)
{
  $theme_name = $theme_list[0];
  switch($theme_name) {
    default:
      $profile = 'main';
  }
  variable_set('smartparticipation_active_profile',$profile);
}