<?php 
/**
 * @file 
 * Template for display of open proposals on site home page.
 */

?>


<?php if (!empty($proposals['#items'])) : ?>
  
  <section id="open-proposals">
    <div class="row">
      <div class="span12 section-heading">
        <h1><?php echo $proposals['title']; ?></h1>
        <?php 
          foreach ($proposals['#items'] as $nid=>$proposal) {
            print render($proposal);
          }
        ?>         
      </div>
    </div>

  </section>
  
<?php endif; ?> 

