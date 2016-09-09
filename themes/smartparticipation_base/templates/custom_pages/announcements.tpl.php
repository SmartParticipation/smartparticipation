<?php
/**
 * Template for RegRoom Announcements page.
 * 
 * Available variables:
 *  - $announcements: a structured array of active announcements.
 *    - #items: the announcements
 *    - image_path: path to theme image directory
 * 
 * @see regroom_announcements_page()
 */

$items = $announcements['#items'];
?>


<section id="new-announcements" class="row">
  
  <div class="span3">
      <img class="" src ="<?php echo $announcements['image_path']; ?>/announcements/announcements.jpg" alt="Announcements"/>
  </div>

  <div class="span9">
    <?php if (!empty($items)): ?>

      <?php $first_announcement = array_shift($items); ?>
      <?php print render($first_announcement); ?>
       
    <?php else: ?>
      <p class="no-content"><?php echo t('There are no current announcements to display.'); ?></p>
    <?php endif; ?>
  </div>
  
</section>
  
<?php if (!empty($items)): ?>
  
  <section class="all-announcements" class="row">
    
    <?php // if we add grouping by year: <h2>2012</h2> ?>
      
    <?php foreach ($items as $announcement): ?>
      <?php print render($announcement); ?>      
    <?php endforeach; ?>
    
  </section>
  
<?php endif; ?>

