<?php

/**
 * Add body classes if certain regions have content.
 */
function smartparticipation_base_preprocess_html(&$variables) {
  if (!empty($variables['page']['featured'])) {
    $variables['classes_array'][] = 'featured';
  }

  if (!empty($variables['page']['triptych_first'])
    || !empty($variables['page']['triptych_middle'])
    || !empty($variables['page']['triptych_last'])) {
    $variables['classes_array'][] = 'triptych';
  }

  if (!empty($variables['page']['footer_firstcolumn'])
    || !empty($variables['page']['footer_secondcolumn'])
    || !empty($variables['page']['footer_thirdcolumn'])
    || !empty($variables['page']['footer_fourthcolumn'])) {
    $variables['classes_array'][] = 'footer-columns';
  }
    
  if (smartparticipation_core_moderator_access()) {
    $variables['classes_array'][] = 'moderator';
  }

  if (smartparticipation_core_user_must_provide_username()) {
    $variables['classes_array'][] = 'no-username';
  }

  $variables['attributes_array']['data-instance-start'] = time();

  // Add conditional stylesheets for IE
  // MB: These will be defined in html.tpl.php
  //drupal_add_css(path_to_theme() . '/css/ie7.css', array('group' => CSS_THEME, 'browsers' => array('IE' => 'IE 7', '!IE' => FALSE), 'preprocess' => FALSE));
  //drupal_add_css(path_to_theme() . '/css/ie8.css', array('group' => CSS_THEME, 'browsers' => array('IE' => 'IE 8', '!IE' => FALSE), 'preprocess' => FALSE));
}

/**
 * Implements theme_preprocess_page().
 */
function smartparticipation_base_preprocess_page(&$variables) {

  if (!empty($variables['page']['#logintoboggan_denied']) && drupal_is_front_page() && drupal_anonymous_user()) {
    if ($key = array_search('page__front',$variables['theme_hook_suggestions'])) {
      unset($variables['theme_hook_suggestions'][$key]);
    }
    //unset($variables['page']['page_bottom']['regroom_login']);
    //unset($variables['page']['page_bottom']['regroom_archive']);
  }

  if (isset($variables['node']->type) ) {
    $node = $variables['node'];

    // These custom node types have a customized title display that is handled 
    // in  the node template, so the page template should not print the title.
    $node_type = $node->type;
    if ($node_type == 'sp_about' ||
        $node_type == 'sp_document' ||
        $node_type == 'sp_learn' ||
        $node_type == 'sp_proposal' ||
        $node_type == 'sp_topic') {
      $variables['title'] = '';
    }
  } 

  $path = request_path();  
  $path_parts = explode('/', $path);
  if ($path_parts[0] == 'moderator') {
    $variables['title'] = '';
  }

  // Establish the grid 
  $content_columns = 12;
  $sidebar_first_columns = 3;
  $sidebar_second_columns = 4;
  if (isset($variables['page']['sidebar_first']) 
    && ! empty($variables['page']['sidebar_first'])) {
      $content_columns -= $sidebar_first_columns;
  }
  if (isset($variables['page']['sidebar_second']) 
    && ! empty($variables['page']['sidebar_second'])) {
      $content_columns -= $sidebar_second_columns;
  }  
  $variables['content_columns'] = $content_columns;
  $variables['sidebar_first_columns'] = $sidebar_first_columns;
  $variables['sidebar_second_columns'] = $sidebar_second_columns;
  
}


/**
 * Override or insert variables into the page template for HTML output.
 */
/*function smartparticipation_base_process_html(&$variables) {
  // Hook into color.module.
  if (module_exists('color')) {
    _color_html_alter($variables);
  }
}*/

/**
 * Override or insert variables into the page template.
 */
function smartparticipation_base_process_page(&$variables) {
  // Hook into color.module.
  if (module_exists('color')) {
    _color_page_alter($variables);
  }
  // Always print the site name and slogan, but if they are toggled off, we'll
  // just hide them visually.
  $variables['hide_site_name']   = theme_get_setting('toggle_name') ? FALSE : TRUE;
  $variables['hide_site_slogan'] = theme_get_setting('toggle_slogan') ? FALSE : TRUE;
  if ($variables['hide_site_name']) {
    // If toggle_name is FALSE, the site_name will be empty, so we rebuild it.
    $variables['site_name'] = filter_xss_admin(variable_get('site_name', 'Drupal'));
  }
  if ($variables['hide_site_slogan']) {
    // If toggle_site_slogan is FALSE, the site_slogan will be empty, so we rebuild it.
    $variables['site_slogan'] = filter_xss_admin(variable_get('site_slogan', ''));
  }
  // Since the title and the shortcut link are both block level elements,
  // positioning them next to each other is much simpler with a wrapper div.
  if (!empty($variables['title_suffix']['add_or_remove_shortcut']) && $variables['title']) {
    // Add a wrapper div using the title_prefix and title_suffix render elements.
    $variables['title_prefix']['shortcut_wrapper'] = array(
      '#markup' => '<div class="shortcut-wrapper clearfix">',
      '#weight' => 100,
    );
    $variables['title_suffix']['shortcut_wrapper'] = array(
      '#markup' => '</div>',
      '#weight' => -99,
    );
    // Make sure the shortcut link is the first item in title_suffix.
    $variables['title_suffix']['add_or_remove_shortcut']['#weight'] = -100;
  }
  
  // These node types will print their own title
  // TODO This will probably apply to other node types as well, since they get
  // custom title treatment in the node template.
  //if (isset($variables['node']) && $variables['node']->type == 'regroom_about') {
  //  $variables['title'] = '';
  //}
  
}

/**
 * Implements hook_preprocess_maintenance_page().
 */
function smartparticipation_base_preprocess_maintenance_page(&$variables) {
  // By default, site_name is set to Drupal if no db connection is available
  // or during site installation. Setting site_name to an empty string makes
  // the site and update pages look cleaner.
  // @see template_preprocess_maintenance_page
  if (!$variables['db_is_active']) {
    $variables['site_name'] = '';
  }
  drupal_add_css(drupal_get_path('theme', 'smartparticipation_base') . '/css/maintenance-page.css');
}

/**
 * Override or insert variables into the maintenance page template.
 */
function smartparticipation_base_process_maintenance_page(&$variables) {
  
  // Drupal sends us here on a 500 server error, in addition to maintenance
  // mode.
  $status = drupal_get_http_header('status'); 
  if (strpos($status, '500') == 0) {
    // Template = maintenance-page--500.tpl.php  
    $variables['theme_hook_suggestions'][] = 'maintenance_page__500';
    
  // Maintenance mode
  } else {

    // Always print the site name and slogan, but if they are toggled off, we'll
    // just hide them visually.
    $variables['hide_site_name']   = theme_get_setting('toggle_name') ? FALSE : TRUE;
    $variables['hide_site_slogan'] = theme_get_setting('toggle_slogan') ? FALSE : TRUE;
    if ($variables['hide_site_name']) {
      // If toggle_name is FALSE, the site_name will be empty, so we rebuild it.
      $variables['site_name'] = filter_xss_admin(variable_get('site_name', 'Drupal'));
    }
    if ($variables['hide_site_slogan']) {
      // If toggle_site_slogan is FALSE, the site_slogan will be empty, so we rebuild it.
      $variables['site_slogan'] = filter_xss_admin(variable_get('site_slogan', ''));
    }
  }
}


/**
 * Override or insert variables into the node template.
 */
function smartparticipation_base_preprocess_node(&$variables) {
  
  $node = $variables['node'];

  $view_mode = $variables['view_mode'];
  
  $variables['theme_hook_suggestions'][] = 
    'node__' . $node->type . '__' . $view_mode;
     
  if (smartparticipation_core_is_full_page_node_view($node, $view_mode)) {
    $variables['classes_array'][] = 'node-full';
    
    if ($node->type == 'sp_about' ||
        $node->type == 'page') {
      
      $alias = url('node/' . $node->nid);
      $alias = explode('/', $alias);
      // Hyphen creates problems in the template name, so change to underscore.
      $alias = str_replace('-', '_', end($alias));

      $variables['theme_hook_suggestions'][] = 
          'node__' . $node->type . '__' . $alias . '__' . $view_mode;
          
    } else if ($node->type == 'sp_topic') {
      $variables['classes_array'][] = 
          $variables['content']['phase_allows_comments'] ? 'with-comments' : 'without-comments';
    }
  }  
  
  $status = drupal_get_http_header('status');  
  switch ($status) {
    case '403 Forbidden':    
      // Template = node--page--403.tpl.php  
      $variables['theme_hook_suggestions'][] = 'node__page__403';
      break;      
    case '404 Not Found':
      // Template = node--page--404.tpl.php  
      $variables['theme_hook_suggestions'][] = 'node__page__404';
      break;
  }

}

/**
 * Override or insert variables into the block template.
 */
function smartparticipation_base_preprocess_block(&$variables) {
  // In the header region visually hide block titles.
  if ($variables['block']->region == 'header') {
    $variables['title_attributes_array']['class'][] = 'element-invisible';
  }
  
}

/**
 * Implements theme_menu_tree().
 */
function smartparticipation_base_menu_tree($variables) {
  return '<ul class="menu clearfix">' . $variables['tree'] . '</ul>';
}

/**
 * Implements theme_field__field_type().
 */
function smartparticipation_base_field__taxonomy_term_reference($variables) {
  $output = '';

  // Render the label, if it's not hidden.
  if (!$variables['label_hidden']) {
    $output .= '<h3 class="field-label">' . $variables['label'] . ': </h3>';
  }

  // Render the items.
  $output .= ($variables['element']['#label_display'] == 'inline') ? '<ul class="links inline">' : '<ul class="links">';
  foreach ($variables['items'] as $delta => $item) {
    $output .= '<li class="taxonomy-term-reference-' . $delta . '"' . $variables['item_attributes'][$delta] . '>' . drupal_render($item) . '</li>';
  }
  $output .= '</ul>';

  // Render the top-level DIV.
  $output = '<div class="' . $variables['classes'] . (!in_array('clearfix', $variables['classes_array']) ? ' clearfix' : '') . '"' . $variables['attributes'] .'>' . $output . '</div>';

  return $output;
}


/**
 * Returns HTML for a form element.
 *
 * Each form element is wrapped in a DIV container having the following CSS
 * classes:
 * - form-item: Generic for all form elements.
 * - form-type-#type: The internal element #type.
 * - form-item-#name: The internal form element #name (usually derived from the
 *   $form structure and set via form_builder()).
 * - form-disabled: Only set if the form element is #disabled.
 *
 * In addition to the element itself, the DIV contains a label for the element
 * based on the optional #title_display property, and an optional #description.
 *
 * The optional #title_display property can have these values:
 * - before: The label is output before the element. This is the default.
 *   The label includes the #title and the required marker, if #required.
 * - after: The label is output after the element. For example, this is used
 *   for radio and checkbox #type elements as set in system_element_info().
 *   If the #title is empty but the field is #required, the label will
 *   contain only the required marker.
 * - invisible: Labels are critical for screen readers to enable them to
 *   properly navigate through forms but can be visually distracting. This
 *   property hides the label for everyone except screen readers.
 * - attribute: Set the title attribute on the element to create a tooltip
 *   but output no label element. This is supported only for checkboxes
 *   and radios in form_pre_render_conditional_form_element(). It is used
 *   where a visual label is not needed, such as a table of checkboxes where
 *   the row and column provide the context. The tooltip will include the
 *   title and required marker.
 *
 * If the #title property is not set, then the label and any required marker
 * will not be output, regardless of the #title_display or #required values.
 * This can be useful in cases such as the password_confirm element, which
 * creates children elements that have their own labels and required markers,
 * but the parent element should have neither. Use this carefully because a
 * field without an associated label can cause accessibility challenges.
 *
 * @param $variables
 *   An associative array containing:
 *   - element: An associative array containing the properties of the element.
 *     Properties used: #title, #title_display, #description, #id, #required,
 *     #children, #type, #name.
 *
 * @ingroup themeable
 */
function smartparticipation_base_form_element($variables) {
  if (!empty($variables['element']['#parents'])) {
    // Is this element being used in a webform?
    if ($variables['element']['#parents'][0] == 'submitted') {
      return theme_webform_element($variables);
    }
  }
  $element = &$variables['element'];
  // This is also used in the installer, pre-database setup.
  $t = get_t();

  // This function is invoked as theme wrapper, but the rendered form element
  // may not necessarily have been processed by form_builder().
  $element += array(
    '#title_display' => 'before',
  );

  // Add element #id for #type 'item'.
  if (isset($element['#markup']) && !empty($element['#id'])) {
    $attributes['id'] = $element['#id'];
  }
  // Add element's #type and #name as class to aid with JS/CSS selectors.
  $attributes['class'] = array('form-item');
  if (!empty($element['#type'])) {
    $attributes['class'][] = 'form-type-' . strtr($element['#type'], '_', '-');
  }
  if (!empty($element['#name'])) {
    $attributes['class'][] = 'form-item-' . strtr($element['#name'], array(' ' => '-', '_' => '-', '[' => '-', ']' => ''));
  }
  // Add a class for disabled elements to facilitate cross-browser styling.
  if (!empty($element['#attributes']['disabled'])) {
    $attributes['class'][] = 'form-disabled';
  }

  $output = '<div' . drupal_attributes($attributes) . '>' . "\n";

  // If #title is not set, we don't display any label or required marker.
  if (!isset($element['#title'])) {
    $element['#title_display'] = 'none';
  }
  $prefix = isset($element['#field_prefix']) ? '<span class="field-prefix">' . $element['#field_prefix'] . '</span> ' : '';
  $suffix = isset($element['#field_suffix']) ? ' <span class="field-suffix">' . $element['#field_suffix'] . '</span>' : '';
  
  // Add element-specific error message beneath the form field.
  // form_get_error() expects $element['#parents'] to be set.
  if (isset($element['#parents'])) {
    $element_error = form_get_error($element);
    if ($element_error) {
      $suffix .= '<span class="field-error">' . $element_error . '</span> ';    
    }
  }
  
  switch ($element['#title_display']) {
    case 'before':
    case 'invisible':
      $output .= ' ' . theme('form_element_label', $variables);
      $output .= ' ' . $prefix . $element['#children'] . $suffix . "\n";
      break;

    case 'after':
      $output .= ' ' . $prefix . $element['#children'] . ($element['#type'] != 'checkbox' && $element['#type'] != 'radio' ? $suffix : '');
      $output .= ' ' . theme('form_element_label', $variables) . ($element['#type'] == 'checkbox' ? $suffix: '') . "\n";
      break;

    case 'none':
    case 'attribute':
      // Output no label and no required marker, only the children.
      $output .= ' ' . $prefix . $element['#children'] . $suffix . "\n";
      break;
  }

  if (!empty($element['#description'])) {
    $output .= '<div class="description">' . $element['#description'] . "</div>\n";
  }

  $output .= "</div>\n";

  return $output;
}

/*
 *
 * Remove cachebuster query string if it is found from resources that are using
 * Drupal aggregation and compression.  This can be the case if preprocess is
 * set to false.
 *
 * See: https://www.drupal.org/node/242875#comment-8285049
 */
/*function smartparticipation_base_process_html(&$vars)
{
  if (variable_get('preprocess_css') && variable_get('preprocess_js')) {

    $search = array('scripts' => 'src=', 'styles' => 'href=');//, 'styles' => '@import\surl\(', 'link' => 'href=');
    foreach ( $search as $var => $word ) {
      if ( !empty($vars[$var]) ) {
        $lines = explode("\n", $vars[$var]);
        $result = array();
        foreach($lines as $line) {
          $matches = array();
          if ( preg_match('/' . $word . '"(.*)"/', $line, $matches) ) {
            global $language;
            $match = $matches[1];
            $replacement = $matches[1];
            // remove the ? and everything behind it
            $pos = strpos($replacement, '?');
            $replaced = $line;
            if ( $pos !== FALSE ) {
              $replacement = substr($replacement, 0, $pos);
              $replaced = str_ireplace($match, $replacement, $line);
            }
            $result[] = $replaced;
          }
          else {
            $result[] = $line;
          }
        }
        if ( !empty($result) ) {
          $vars[$var] = implode("\n", $result);
        }
      }
    }

  }
}*/

/*
 * Implements hook_preprocess_user_thumbnail().
 */
function smartparticpation_core_preprocess_user_thumbnail(&$variables)
{
  $uid = null;
  if (! empty($variables['uid'])) {
    $uid = $variables['uid'];
  } elseif (! empty($variables['user'])) {
    $uid = $variables['user']->uid;
  }
  if ($uid) {
    $account = user_load($uid);
    if (! empty($variables['username'])) {
      $username = $variables['username'];
    } else {
      $username = format_username($account);
    }
    $alt = t("@user's picture", array('@user' => $username));
    if (! empty($account->picture)) {
      $url = image_style_url('small_square_thumbnail', $account->picture->uri);
    }
    else {
      if (smartparticipation_core_user_is_moderator($account)) {
        $image = 'no_pic_moderator.png';
        $url = path_to_theme() . '/images/user/' . $image;
        $moderator_placeholder_image = variable_get('smartparticipation_moderator_placeholder_image');
        if ($moderator_placeholder_image) {
          $img_file = file_load($moderator_placeholder_image);
          $url = image_style_url('small_square_thumbnail', $img_file->uri);
        }
      }
      else {
        $image = 'no_pic_user.png';
        $url = path_to_theme() . '/images/user/' . $image;
        $user_placeholder_image = variable_get('smartparticipation_user_placeholder_image');
        if ($user_placeholder_image) {
          $img_file = file_load($user_placeholder_image);
          $url = image_style_url('small_square_thumbnail', $img_file->uri);
        }
      }
    }
    $variables['user_thumbnail'] = theme('image', array('path' => $url, 'alt' => $alt, 'title' => $alt));
  }
}

/*
 * Implements hook_preprocess_user_picture().
 */
function smartparticipation_core_preprocess_user_picture(&$variables)
{
  $uid = null;
  if (! empty($variables['uid'])) {
    $uid = $variables['uid'];
  } elseif (! empty($variables['user'])) {
    $uid = $variables['user']->uid;
  }
  if ($uid) {
    $account = user_load($uid);
    if (! empty($variables['username'])) {
      $username = $variables['username'];
    } else {
      $username = format_username($account);
    }
    $alt = t("@user's picture", array('@user' => $username));
    if (! empty($account->picture)) {
      $url = image_style_url('medium', $account->picture->uri);
      $variables['user_picture'] = theme('image', array('path' => $url, 'alt' => $alt, 'title' => $alt));
    }
    /*else {
      // TODO allow the dummy images to be uploaded
      $image = 'no_pic_user.png';
      if (regroom_user_is_moderator($account)) {
        $image = 'no_pic_moderator.png';
      }
      $url = path_to_theme() . '/images/user/' . $image;
    }*/
  }
}

function smartparticipation_base_html_head_alter(&$head_elements)
{
  // http://drupal.stackexchange.com/questions/31270/override-favicon-output
  // Search the head elements for the Favicon
  foreach ($head_elements as $key => $element) {
    if (!empty($element['#attributes'])) {
      if (array_key_exists('href', $element['#attributes'])) {
        if (strpos($element['#attributes']['href'], 'misc/favicon.ico') > 0)
        {
          // Delete the favicon link entirely
          unset($head_elements[$key]);
        }
      }
    }
  }
}
