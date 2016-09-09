<?php
/**
 * @file 
 * Template for comments list.
 * 
 * @ingroup regroom
 */

?>

<?php if ( ! empty($endorsements['#items']) ) { ?>

  <?php
  foreach ($endorsements['#items'] as $item) { 
  ?>

    username: <?php print $item['username'];?>
    <br>
    <?php print $item['body'];?>
    <hr>

  <?php
  }
} 
?>