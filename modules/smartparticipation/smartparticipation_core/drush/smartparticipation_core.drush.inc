<?php 

/**
 * Implements hook_drush_command().
 * 
 */
function smartparticipation_core_drush_command() {
  
   $items = array();
   
   $items['delete-node-revisions'] = array(
     'aliases' => array('dnr'),
     'options' => array(
       'nids' => 'Comma-delimited list of node nids',
     ),
     'description' => 'Delete all revisions of the specified node, using Drupal\'s node_delete_revisions() to ensure no database artifacts remain.',
   );
   
   $items['purge-all-comments'] = array(
     'aliases' => array('pac'),
     'description' => 'Purge all comments from the database, using Drupal\'s comment_delete_multiple() to ensure no database artifacts remain.',
   );
   
   return $items;
}


/**
 * Delete all revisions of the nodes specified by the array of nids.

 */
function drush_smartparticipation_core_delete_node_revisions() {
  
  if ($nids = drush_get_option('nids')) {
    $nids = explode(',', $nids);
    
    foreach ($nids as $nid) {
      $node = node_load($nid);
      $revisions = node_revision_list($node);
 
      $deleted_count = 0;

      foreach ($revisions as $revision) {
        // Will not delete the current revision
        if (node_revision_delete($revision->vid)) {
          $deleted_count++;
        }
      }
      
      $pl = $deleted_count == 1 ? '' : 's';
      print "Deleted $deleted_count revision{$pl} of node $nid.\n";
      
    }
  }
}


/**
 * Purges ALL comments from the database. Use only as a tool in development
 * environments.
 */
function drush_smartparticipation_core_purge_all_comments() {
  
  $cids = db_select('comment', 'c')->fields('c', array('cid'))
      ->execute()->fetchCol();
  
  comment_delete_multiple($cids);
  
  $comment_count = count($cids);
  $pl = $comment_count == 1 ? '' : 's';
  
  print "Deleted $comment_count comment{$pl}.\n";
  
}

