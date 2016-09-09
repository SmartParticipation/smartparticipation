<?php

/* This template is not in use. It is here as an example of how we would theme
 * 'node_add_list' with a template instead of a theme function 
 * theme_node_add_list() (specifically, smartparticipation_admin_node_add_base()).
 * 
 * The template requires a specification in regroom_theme() (unless I just
 * haven't found the right default template name.)
 * 
 * function regroom_theme($existing, $type, $theme, $path) {
   return array(
     // This is how we would add a custom template for 'node_add_list'. If we 
     // define both a custom template and function theme_node_add_list(), the
     // latter will apply.
    'node_add_list' => array(
      'path' => drupal_get_path('theme', 'smartparticipation_admin') . '/templates',
      'render element' => 'element',
      'template' => 'node-add-list',
    ),
   );
 }
 * 
 * The theme function does not. 
 * 
 * If both theme function and template exist, the theme function is used.
 * 
 * In either case, data can be modified in hook_preprocess_node_add_list() 
 * before being sent to the template or theme function.
 * 
 * Question: How do render arrays and hook_alter functions fit in?
 */

//print kpr($variables);
