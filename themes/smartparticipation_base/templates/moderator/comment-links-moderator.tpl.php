<?php
/**
 * Template to display moderator comment links on the topic page.
 * 
 * Available variables:
 * - $comment_links
 *
 */
 
// print kpr($comment_links);
?>

<ul class="links inline">
  <?php if (isset($comment_links['comment']['comment-status'])) : ?>
    <li class="status-type">
      Status: 
        <!--  UNREAD status has class .status-unread (red color)         
              IN PROGRESS status has class .status-in-progress (orange color)
              DONE status has class  .status-done (green color) --> 
      <span class="status-type status-<?php echo $comment_links['comment']['comment-status']['class']; ?>"><?php echo $comment_links['comment']['comment-status']['value']; ?></span>
    </li>
  <?php endif; ?>
 
  <?php if (isset($comment_links['comment']['comment-action'])) : ?>
    <li class="action-type">
      Action: 
        <!--  NO REPLY action has class .action-no-reply (orange color)         
              REPLY action has class .action-reply (green color)
              QUARANTINE action has class  .action-quarantine (red color) --> 
      <span class="action-type action-<?php echo $comment_links['comment']['comment-action']['class']; ?>"><?php echo $comment_links['comment']['comment-action']['value']; ?></span>
    </li>
  <?php endif; ?>
   
  <?php print theme('links', $comment_links); ?>
  
</ul>