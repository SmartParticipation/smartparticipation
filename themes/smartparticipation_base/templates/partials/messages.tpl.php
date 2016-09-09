<?php

/**
 * Print Drupal messages.
 */
?>

<?php if (isset($messages) && $messages): ?>
  <div id="messages">
    <div class="section clearfix">
      <?php print $messages; ?>
    </div>
  </div> 
<?php endif; ?>

