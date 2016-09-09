<?php

/**
 * Template to render a single announcement on the announcements page.
 * 
 * Available variables: 
 *  - $announcement: associative array containing announcement data
 *    - title
 *    - start_date
 *    - body
 *  
 * This may or may not be the same template we want to use if we display the
 * announcement on a full page. For now, we are not displaying such a page to
 * the public.
 * 
 * TODO Figure out if we can make use of the node templating system instead. See
 * RR-1581.
 */

?>

<article>
  <h2><?php echo $announcement['title']; ?></h2>
  <time datetime="<?php echo $announcement['start_date']; ?>" pubdate><?php echo $announcement['start_date']; ?></time>
  <?php echo $announcement['body']; ?>
</article> 


