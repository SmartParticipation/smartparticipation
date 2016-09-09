<?php
/**
 * Template to display moderator comment links on the topic page.
 *
 * Available variables:
 * - $user_data
 *
 */
?>

<div id='user_view_header'>
  <div id='user_view_header_right_content'>
    <?php echo $user_data['name']; ?>'s comments
    <span class='divider'>|</span>  <button id='deactivate_user_view_button'>All users</button>
  </div>
</div>