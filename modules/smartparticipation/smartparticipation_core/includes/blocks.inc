<?php
/**
 * @file
 * Define and configure blocks for RegRoom.
 */

/**
 * Implements hook_block_info().
 */
function smartparticipation_core_block_info() {

  return array(

      'about_menu' => array(
          'cache' => DRUPAL_CACHE_GLOBAL,
          'info' => t('About menu'),
          'pages' => 'about*',
          'region' => 'sidebar_second',
          'status' => TRUE,
          'visibility' => BLOCK_VISIBILITY_LISTED,
          'weight' => 0,
      ),

    'closed_proposals' => array(
      'cache' => DRUPAL_CACHE_GLOBAL,
      'info' => t('Closed Proposals'), 
      'pages' => '<front>',
      'region' => 'content',
      'status' => TRUE,
      'visibility' => BLOCK_VISIBILITY_LISTED,
      'weight' => 60,
    ),
      
    'coming_soon' => array(
      'cache' => DRUPAL_CACHE_GLOBAL,
      'info' => t('Coming Soon'),
      'pages' => '<front>', 
      'region' => 'content',            
      'status' => TRUE,
      'visibility' => BLOCK_VISIBILITY_LISTED,
      'weight' => 30, 
    ),

    'contact_form' => array(
      'cache' => DRUPAL_CACHE_GLOBAL,
      'info' => t('Contact Form'),
      'pages' => 'contact',
      'region' => 'sidebar_second',
      //'status' => FALSE,
      'visibility' => BLOCK_VISIBILITY_LISTED,
      'weight' => 10,
    ),

    'footer' => array(
      // Why not: 'cache' => DRUPAL_CACHE_GLOBAL,
      'info' => t('SmartParticipation Footer'),
      'region' => 'footer',
      'status' => TRUE,
      'visibility' => BLOCK_VISIBILITY_NOTLISTED,
    ),

    'header' => array(
      // Why not: 'cache' => DRUPAL_CACHE_PER_USER,
      'info' => t('SmartParticipation Header'),
      'region' => 'header',      
      'status' => TRUE,
      'visibility' => BLOCK_VISIBILITY_NOTLISTED,
    ),

    'learn_node_links' => array(
      'cache' => DRUPAL_CACHE_GLOBAL,    
      'info' => t('Learn Node Links'), 
      'pages' => 'faq',      
      'region' => 'sidebar_second',
      'status' => TRUE,
      'visibility' => BLOCK_VISIBILITY_LISTED,
      'weight' => 10,
    ),
    
    'learn_panel' => array(
      'cache' => DRUPAL_CACHE_PER_PAGE,    
      'info' => t('Learn Panel'),
      // This block is also manually loaded on proposal page. Can't get block
      // visibility by node type to work, especially in conjunction with page
      // visibility.      
      'pages' => '<front>',
      'region' => 'content',      
      'status' => TRUE,
      'visibility' => BLOCK_VISIBILITY_LISTED,
      'weight' => 10,
    ),

    'learn_panel_mobile' => array(
      'cache' => DRUPAL_CACHE_PER_PAGE,
      'info' => t('Learn Panel (mobile version)'),
      'pages' => '<front>',
      'region' => 'content',
      'status' => TRUE,
      'visibility' => BLOCK_VISIBILITY_LISTED,
      'weight' => 10,
    ),

    'login' => array(
      'cache' => DRUPAL_NO_CACHE,    
      'info' => t('SmartParticipation Login Link'),
      'visibility' => BLOCK_VISIBILITY_NOTLISTED,
    ),

    'open_proposals' => array(
      'cache' => DRUPAL_CACHE_GLOBAL,   
      'info' => t('Open Proposals'), 
      'pages' => '<front>',
      'region' => 'content',     
      'status' => TRUE,
      'visibility' => BLOCK_VISIBILITY_LISTED,
      'weight' => 20,      
    ),

    'other_questions' => array(
      'cache' => DRUPAL_CACHE_GLOBAL,    
      'info' => t('Other Questions'), 
      'pages' => 'faq',
      'region' => 'sidebar_second',
      'status' => TRUE,
      'visibility' => BLOCK_VISIBILITY_LISTED,
      'weight' => 5,      
    ),
 
    'profile_links' => array(
      'cache' => DRUPAL_NO_CACHE,
      'info' => t('Profile links block'), 
      'pages' => 'user*',
      'region' => 'sidebar_second',      
      'status' => TRUE,
      'visibility' => BLOCK_VISIBILITY_LISTED,
      'weight' => -10,
    ),

    'proposal_overview' => array(
      'cache' => DRUPAL_CACHE_GLOBAL,    
      'info' => t('Proposal Overview'),
      // This block is manually loaded on topic and document pages. Can't get 
      // block visibility by node type to work.
      'pages' => '',
      'region' => 'content',
      'status' => TRUE,
      'visibility' => BLOCK_VISIBILITY_LISTED,
      'weight' => 0,
    ),
      
    'recent_comments' => array(
      'cache' => DRUPAL_CACHE_PER_PAGE,
      'info' => t('Recent Comments'), 
      // This block will be manually loaded. No need to do the query if there
      // are no open proposals (front page) or the specific proposal is not
      // open (proposal page). 
      'pages' => '',
      'region' => 'content',
      'status' => TRUE,
      'visibility' => BLOCK_VISIBILITY_LISTED,
      'weight' => 40,      
    ),
    
    'recommended_comments' => array(
      'cache' => DRUPAL_CACHE_PER_PAGE,
      'info' => t('Recommended Comments'), 
      // This block will be manually loaded. No need to do the query if there
      // are no open proposals (front page) or the specific proposal is not
      // open (proposal page).    
      'pages' => '',
      'region' => 'content',
      'status' => TRUE,
      'visibility' => BLOCK_VISIBILITY_LISTED,
      'weight' => 50,      
    ),
      
    'submit_faq' => array(
      'cache' => DRUPAL_CACHE_GLOBAL,
      'info' => t('Submit FAQ'),
      'pages' => 'faq',
      'region' => 'sidebar_second',
      'status' => TRUE,
      'visibility' => BLOCK_VISIBILITY_LISTED,
      'weight' => 0,
    ),

  );
}

/**
 * Implements hook_block_info_alter().
 * 
 * This method gets called once for each enabled theme.
 * 
 */
function smartparticipation_core_block_info_alter(&$blocks, $theme, $code_blocks) {
  // Make sure the profile links block is not cached.
  $blocks['smartparticipation_core']['profile_links']['cache'] = DRUPAL_NO_CACHE;

  // Disable blocks enabled by other modules that we don't want in either 
  // default (public) or admin theme.
  $blocks['system']['navigation']['status'] = FALSE;
  $blocks['user']['login']['status'] = FALSE;  
  $blocks['system']['powered-by']['status'] = FALSE;

  // Disable custom smartparticipation_core blocks in admin theme
  // Would rather delete these from the db or not create them in the first
  // place, but there's no block_delete() function, and no way to specify 
  // theme in hook_block_info(), so we settle for disabling them.
  if ($theme == variable_get('admin_theme')) {
    foreach ($blocks['smartparticipation_core'] as $name=>$block) {
      $block['status'] = 0;
      $block['visibility'] = 0;
      $block['region'] = '-1';
      $block['pages'] = '';
      $block['cache'] = -1;
      $block['weight'] = 0;
      $blocks['smartparticipation_core'][$name] = $block;
    }
  }
}

/**
 * Implements hook_block_view().
 *
 * This hook generates the contents of the blocks themselves.
 * 
 * We generally set the $block['subject'] to NULL, because the default block
 * template block.tpl.php encloses the subject in an h2. We want to control the
 * heading level on a block-specific basis without writing new templates for 
 * each block, so we set the subject to null and print the title with the block
 * content.
 * 
 * @param $delta - the requested block
 */
function smartparticipation_core_block_view($delta = '') {

  $block = array();
  
  switch ($delta) {
    case 'about_menu':
      $block['subject'] = NULL;
      $block['content'] = smartparticipation_core_about_links();
      break;

    case 'closed_proposals':
      $block['subject'] = NULL;
      $block['content'] = smartparticipation_core_closed_proposals_block_content();
      break;
      
    case 'coming_soon':
      $block['subject'] = NULL;
      $block['content'] = smartparticipation_core_coming_soon_block_content();
      break;

    case 'contact_form':
      $block['subject'] = t('Send Us a Message');
      $block['content'] = drupal_get_form('smartparticipation_core_contact_form');
      break;

    case 'footer':
      $block['subject'] = NULL;
      //$block['content'] = theme('footer');
      $block['content'] = smartparticipation_core_footer_content();
      break;
        
    case 'header':
      $block['subject'] = NULL;
      $block['content'] = theme('header');
      break;

    case 'learn_node_links':
      $block['subject'] = NULL;
      $block['content'] = smartparticipation_core_learn_node_links_block_content();
      break;

    case 'learn_panel':
      $block['subject'] = NULL;
      $block['content'] = smartparticipation_core_learn_panel_block_content();
      break;

    case 'learn_panel_mobile':
      $block['subject'] = NULL;
      $block['content'] = smartparticipation_core_learn_panel_mobile_block_content();
      break;

    case 'login':
      if (user_is_anonymous() && !(arg(0) == 'user' && !is_numeric(arg(1)))) {
        $block['subject'] = t('Login');
        $block['content'] = l(t('Login'), 'user/login');
      }
      break;

    case 'open_proposals':
      $block['subject'] = NULL;
      $block['content'] = smartparticipation_core_open_proposals_block_content();
      break;

    case 'other_questions':
      $block['subject'] = NULL;
      $block['content'] = l(t('Want to give feedback on our site?'), 'contact');
      break;
        
    case 'profile_links':
      $block = smartparticipation_core_user_profile_links();
      break;
        
    case 'recent_comments':
      $block['subject'] = NULL;
      $block['content'] = smartparticipation_core_recent_comments_block_content();
      break;  
    
    case 'recommended_comments':
      $block['subject'] = NULL;
      $block['content'] = smartparticipation_core_recommended_comments_block_content();
      break;

    case 'submit_faq':
      $block['subject'] = t('Ask a Question');
      $block['content'] = drupal_get_form('smartparticipation_core_submit_faq_form');
      break;
  }

  return $block;
}

/**
 * A module-defined block content function.
 * 
 * Can return either an HTML string or a render array.
 */
function smartparticipation_core_learn_panel_block_content() {
  
  $learn_nodes = _smartparticipation_core_learn_panel_nodes();

  $add_node_instructions = _smartparticipation_core_learn_panel_add_node_instructions();

  // Return a render array
  return array(
    '#items' => $learn_nodes, 
    '#theme' => 'learn_panel',
    'add_node_instructions' => $add_node_instructions,
    'single_node_view' => FALSE,
  );

}

/**
 * @return array Render array for learn panel mobile version
 */
function smartparticipation_core_learn_panel_mobile_block_content() {

  $learn_nodes = array_reverse(_smartparticipation_core_learn_panel_nodes());

  $add_node_instructions = _smartparticipation_core_learn_panel_add_node_instructions();

  return array(
    '#items' => $learn_nodes,
    '#theme' => 'learn_panel_mobile',
    'add_node_instructions' => $add_node_instructions,
  );
}

/**
 * @return null|string
 *
 * Get the instruction markup for adding learn panel nodes.
 *
 */
function _smartparticipation_core_learn_panel_add_node_instructions() {

  return (empty($learn_nodes) && user_access('create sp_learn content')) ?
    t('Please <a href="@href">create learn pages</a> on your site in order to display the accordion.', array(
      '@href' => '/node/add/sp-learn',
    )) :
    '';

}

/**
 * @return array Learn panel nodes
 */
function _smartparticipation_core_learn_panel_nodes() {

  // Fetch the data
  $query = 'SELECT n.nid FROM {node} n
            INNER JOIN {menu_node} mn ON n.nid = mn.nid
            INNER JOIN {menu_links} ml ON mn.mlid = ml.mlid
            WHERE n.type = :type
            AND n.status = :status
            ORDER BY ml.weight
            LIMIT 5'; // Maximum supported by the presentation layer based on space limitations

  $nids = db_query($query, array(
    ':type' => 'sp_learn',
    ':status' => 1,
  ))
    ->fetchCol();

  // Assemble the necessary data from each node
  $learn_nodes = array();
  foreach ($nids as $nid) {
    $data = smartparticipation_core_learn_panel_node_data(node_load($nid));
    if ($data['display_in_learn_panel']) {
      $learn_nodes[] = $data;
    }
  }

  return $learn_nodes;

}


/**
 * Retrieve the set of open proposals, sort, and return a render array.
 * 
 */
function smartparticipation_core_open_proposals_block_content() {

  $query = 'SELECT n.nid FROM {node} n
            INNER JOIN {field_data_field_sp_proposal_status} s ON n.nid = s.entity_id
            WHERE n.type = \'sp_proposal\'
            AND n.status = 1
            AND s.field_sp_proposal_status_value = 1;';
            
  $nids = db_query($query)->fetchCol();

  $items = array();
  foreach ($nids as $nid) {
    $items[$nid] = smartparticipation_core_proposal_overview_data($nid);
  }

  // NB Use uasort() rather than usort() to preserve the array keys as node
  // nids. usort() reindexes from 0.
  uasort($items, 'smartparticipation_core_sort_open_proposals_by_date_desc');

  // Return a render array
  return array(
    '#items' => $items, 
    '#theme' => 'open_proposals',
    'title' => t('Open @proposal_type', array(
      '@proposal_type' => ucfirst(variable_get('smartparticipation_site_proposal_type_plural', 'proposals')),
    )),
  );

}

/**
 * Retrieve the set of closed proposals, sort, and return a render array.
 *
 */
function smartparticipation_core_closed_proposals_block_content() {
  
  $query = 'SELECT n.nid FROM {node} n
            INNER JOIN {field_data_field_sp_proposal_status} s ON n.nid = s.entity_id      
            WHERE n.type = \'sp_proposal\'
            AND n.status = 1
            AND s.field_sp_proposal_status_value = 0;';
            
  $nids = db_query($query)->fetchCol();
  $items = array();
  foreach ($nids as $nid) {
    $items[$nid] = smartparticipation_core_closed_proposal_data($nid);
  }
  
  // NB Use uasort() rather than usort() to preserve the array keys as node
  // nids. usort() reindexes from 0.
  uasort($items, 'smartparticipation_core_sort_closed_proposals_by_date_desc');

  // Return a render array
  return array(
    '#items' => $items,
    '#theme' => 'closed_proposals',
    'title' => t('Closed @proposal_type', array(
      '@proposal_type' => ucfirst(variable_get('smartparticipation_site_proposal_type_plural', 'proposals')),
    )),
  );
  
}


/**
 * Return a renderable array of Learn links.
 */
function smartparticipation_core_learn_node_links_block_content() {

  // Assemble the main menu learn node links into a rendereable array.
  // TODO At least we're now making use of the Drupal menu. However, somehow
  // there should be a way to just grab a subtree of the menu and render it.
  // If not, is there a way to put the individual links into an array and call
  // render() from the template? It seems like we shouldn't have to manually
  // repackage the menu data.
  $learn_menu_links = array();
  $main_menu = menu_tree_all_data('main-menu', null, 2);
  foreach ($main_menu as $menu_link) {
    if ($menu_link['link']['link_title'] == t('Learn')) {
      $links = $menu_link['below'];
      foreach ($links as $learn_link) {
        $link = $learn_link['link'];
        $learn_menu_links[] = l($link['link_title'], $link['link_path']);  
      }
    }
  }

  return array(
    '#items' => $learn_menu_links, 
    '#theme' => 'learn_menu_links',
    'title' => t('Learn More'),
  );  
}

/**
 * Assemble block content for recent comments block.
 */
function smartparticipation_core_recent_comments_block_content() {

  $cids = array();

  $node = menu_get_object();

  $show_more_request = new \SmartParticipation\ShowMoreRecentCommentsRequest;

  // Proposal page
  if ($node && $node->type == 'sp_proposal') {
    
    $proposal_nid = $node->nid;

    $result = $show_more_request->run(
      $proposal_nid,
      null,
      \SmartParticipation\ShowMoreFeaturedComments::DEFAULT_COMMENT_COUNT
    );

  // Other pages
  } else {

    $result = $show_more_request->run(
      null,
      null,
      \SmartParticipation\ShowMoreFeaturedComments::DEFAULT_COMMENT_COUNT
    );

  }
  
  return smartparticipation_core_featured_comments_render_array(
    array(),
    'recent',
    $result
  );

}

/**
 * Assemble block content for recommended comments block.
 */
function smartparticipation_core_recommended_comments_block_content() {

  $cids = array();
  
  $node = menu_get_object();

  $show_more_request = new \SmartParticipation\ShowMoreRecommendedCommentsRequest;

  // Proposal page
  if ($node && $node->type == 'sp_proposal') {

    $proposal_nid = $node->nid;

    $result = $show_more_request->run(
      $proposal_nid,
      null,
      \SmartParticipation\ShowMoreFeaturedComments::DEFAULT_COMMENT_COUNT
    );

    // Other pages
  } else {

    $result = $show_more_request->run(
      null,
      null,
      \SmartParticipation\ShowMoreFeaturedComments::DEFAULT_COMMENT_COUNT
    );

  }

  return smartparticipation_core_featured_comments_render_array(
    array(),
    'recommended',
    $result
  );
  
}

/**
 * Assemble block content for footer block.
 */
function smartparticipation_core_footer_content() {

  $footer=array();
  $vals = variable_get('smartparticipation_footer_column_1');
  $footer['column_1'] = $vals['value'];

  $vals = variable_get('smartparticipation_footer_column_2');
  $footer['column_2'] = $vals['value'];

  $footer['facebook_url'] = variable_get('smartparticipation_footer_facebook_url');

  $footer['twitter_url'] = variable_get('smartparticipation_footer_twitter_url');

  $base_theme_path = drupal_get_path('theme', 'smartparticipation_base');

  // Return a render array
  return array(
      '#items' => $footer,
      '#theme' => 'footer',
      'image_path' => '/' . $base_theme_path . '/images/',
      'site_name' => variable_get('site_name'),
  );

}

/**
 * Assemble comment data for featured comment blocks (recent and recommended
 * comments). This function is called after a query has been executed to fetch
 * the comment cids.
 *
 * @param array $cids - the cids of the comments to be displayed
 * @return a render array of structured comment data for display
 */
function smartparticipation_core_featured_comments_render_array(array $cids, $comment_type, \SmartParticipation\model\ShowMoreCommentsResult $show_more_result = null) {

  if (empty($show_more_result)) {
    $cids = array_keys($cids);
    $more_comments = false;
    $max_id = null;
    $show_more_button_id = null;
  } else {
    $cids = $show_more_result->getCids();
    $more_comments = $show_more_result->isMoreComments();
    $max_id = $show_more_result->getMaxId();
    $show_more_button_id = $show_more_result->getShowMoreButtonId();
  }

  if ($more_comments) {
    drupal_add_library('system', 'drupal.ajax');
  }

  $recent_comments_service = new \SmartParticipation\RecentCommentsService;
  $comment_items = $recent_comments_service->getData($cids);

  $featured_comments_items_render_array =
    $recent_comments_service->getItemsRenderArray(
      $comment_type,
      $comment_items,
      $more_comments,
      $max_id,
      $show_more_button_id
    );

  // Return a render array
  return array(
    '#theme' => 'featured_comments',
    'featured_comments_items' => $featured_comments_items_render_array,
    'block_title' => ucwords(t($comment_type . ' comments')),
    'no_comments_message' => 
        t('There are no ' . $comment_type . ' comments to display at this time.'),
  );
}

/**
 * Specify content for Coming Soon block. This block displays any published
 * proposals with status = 'coming'. If there are no coming proposals, and no
 * open proposals, the block displays the site's generic coming soon image and
 * text.
 * 
 * TODO For now we're just displaying the generic image and text if there are
 * no open proposals. Add the additional logic for 'coming' proposals. Need to
 * make proposal status field multi-valued rather than binary.
 */
function smartparticipation_core_coming_soon_block_content() {

  $show = false;
  $title = variable_get('smartparticipation_coming_soon_heading');
  $text = '';
  $image = '';
  $signup_link = '';
  
  // TODO Fetch the upcoming proposals, once that value has been added to the
  // status field.
  $upcoming_proposals = array();
  
  if (! empty($upcoming_proposals)) {
    $show = true;
    
  } else {
    // TODO Change 1 to 'open'
    $open_proposal_count = _smartparticipation_core_proposal_count(1);
    
    if ($open_proposal_count == 0) {
      
      $text = variable_get('smartparticipation_coming_soon_text');
      
      $signup_text = variable_get('smartparticipation_new_proposal_notification_signup_text');
      
      // Text or signup text is required for the block to display
      if ($text || $signup_text) {
        $show = true;
        
        if ($signup_text) {
          $to = variable_get('site_mail');
          $proposal_type = variable_get('smartparticipation_site_proposal_type');
          $site_name = variable_get('site_name');
          $subject = t('@site_name: New @proposal_type notification', array(
            '@proposal_type' => $proposal_type,
            '@site_name' => $site_name, 
          ));
          $body = t('Please send me an email notification when a new @proposal_type opens on @site_name.', array(
            '@proposal_type' => $proposal_type,
            '@site_name' => $site_name, 
          ));
          $href = 'mailto:' . $to . '?subject=' . $subject . '&body=' . $body;
          $signup_link = l($signup_text, $href);
        } 
        
        $fid = variable_get('smartparticipation_coming_soon_image');
        
        if ($fid) {
          
          $file = file_load($fid);
          
          if ($file) {
            $image = theme('image', array(
              'path' => $file->uri,
              // Note that the managed_file form field type doesn't allow
              // alt text to be entered with the image.
              'alt' => $title,
              'title' => $title,
              'attributes' => array(
                'id' => 'coming-soon-image',
              ),
            ));           
          }
        }
      }

    }
  
  } 
  
  if ($show) {       
    $block = array(
      '#items' => $upcoming_proposals,
      'image' => $image,
      'signup_link' => $signup_link,
      'text' => $text,
      'title' => $title,
    );
  } else {
    $block = array(
      '#printed' => true,  
    );
  }
  
  $block['#theme'] = 'coming_soon';

  return $block;
  
}

