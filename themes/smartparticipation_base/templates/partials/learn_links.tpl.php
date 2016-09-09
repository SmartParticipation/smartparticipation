<?php
/**
 * @file 
 * Template for list of learn nodes.
 * 
 * @ingroup regroom
 */

?>

<?php if ( ! empty($learn_links['#items']) ): ?>
  
  <h2><?php echo $learn_links['title']; ?></h2>

  <ul>
    <?php foreach ($learn_links['#items'] as $link): ?>
      <li><?php echo $link; ?></li>
    <?php endforeach; ?>
  </ul> 
  
<?php endif; ?>
