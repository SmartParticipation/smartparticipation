<?php
/**
 * @file
 * Define corresponding node references for smartparticipation_core module
 */

/**
 * Module function to define required corresponding node reference fields.
 */
function smartparticipation_core_corresponding_node_references() {
  
  /*
   * NB In order for these items to be checked on the CNR configuration page,
   * each value should be sorted alphabetically by bundle/field name. E.g., 
   * this value:
   * smartparticipation_core_news*field_sp_news_proposal*smartparticipation_core_proposal*field_sp_proposal_news',
   * should NOT be reversed to:
   * 'smartparticipation_core_proposal*field_sp_proposal_news*smartparticipation_core_news*field_sp_news_proposal',
   * The CNR will still work, but it won't appear to be selected on the CNR 
   * configuration page.
   */
  return array(
    'sp_document*field_sp_document_proposal*sp_proposal*field_sp_proposal_document',
    'sp_announcement*field_sp_announcement_proposal*sp_proposal*field_sp_proposal_announcement',
    'sp_news*field_sp_news_proposal*sp_proposal*field_sp_proposal_news',
    'sp_proposal*field_sp_proposal_topic*sp_topic*field_sp_topic_proposal',
    'sp_subtopic*field_sp_subtopic_topic*sp_topic*field_sp_topic_subtopic',
  );
}


/**
 * Module function to insert corresponding node references into the db.
 */
function smartparticipation_core_add_corresponding_node_references() {

  foreach (smartparticipation_core_corresponding_node_references() as $cnr) {
    $cnr_obj = new stdClass();
    $cnr_obj->node_types_content_fields = $cnr;
    $cnr_obj->enabled = 1;
    drupal_write_record('corresponding_node_references', $cnr_obj);
  }  
}

/**
 * Module function to delete corresponding node references when module is 
 * uninstalled.
 */
function smartparticipation_core_delete_corresponding_node_references() {

  db_delete('corresponding_node_references')
    ->condition('node_types_content_fields', 
      smartparticipation_core_corresponding_node_references())
    ->execute();
}


