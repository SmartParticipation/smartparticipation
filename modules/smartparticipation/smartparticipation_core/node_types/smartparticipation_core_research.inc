<?php

/**
 * @file 
 * Define the research node type.
 * 
 * @ingroup smartparticipation_core
 */
 
/**
 * Create the research node type.
 */
function smartparticipation_core_create_research_node_type() {
  
  $t = get_t();
  
  $research = array(
    'type' => 'sp_research',
    'name' => $t('Research'),
    // Drupal will handle this node as if we had designed it in the UI.
    'base' => 'node_content', 
    'has_title' => TRUE,  
    'description' => $t('Content type for description of a research project.'),
    'custom' => TRUE,
    'help' => '',
  );  
  
  // Set defaults not explicitly declared above.
  $node_type = node_type_set_defaults($research);
  
  // Add body field and set label.
  node_add_body_field($node_type, $t('Description'));

  node_type_save($node_type);

  // Disable commenting for this node type  
  variable_set('comment_sp_research', COMMENT_NODE_HIDDEN);
  
  // Create a revision by default when a new version is saved
  variable_set('node_options_sp_research', array('revision', 'status'));

}


/**
 * Delete the research node type.
 * 
 */
function smartparticipation_core_delete_research_node_type() {
  smartparticipation_core_delete_node_type('sp_research');
}

function smartparticipation_core_research_node_view(&$node, $view_mode, $langcode) {

  $content = $node->content;
  
  if (! node_is_page($node)) {
    $content['title'] = $node->title;
  }
  
  $node->content = $content;

}
