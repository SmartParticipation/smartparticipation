<?php 
/**
 * @file 
 * Template for display of closed proposals on site home page.
 *
 * 
 * Available variables:
 * 
 * See also: regroom_closed_proposals_block_content()
 */
?>

<?php if (!empty($proposals['#items'])) : ?>
  
  <section id="closed-proposals">
    <div class="row">
      <div class="span12 section-heading">
        <h1><?php echo $proposals['title']; ?></h1>
        <div class="row">
          <div class="span12 closed-proposals">
            <div class="topic-closed carousel-container">
              <div class="carousel-prev paging-link"></div>
              <div class="carousel-items">
                <div class="rs-carousel module">
                  <ul>
                    <?php
                    foreach ($proposals['#items'] as $nid=>$proposal) {
                      print render($proposal);
                    }
                    ?>   
        
                  </ul>
                </div> <!-- .rs-carousel module -->
              </div>
              <div class="carousel-next paging-link"></div>
            </div> <!-- .topic-closed carousel-container -->
          </div> <!-- .span12 closed-proposals -->
        </div> <!-- .row -->
      </div> <!--  .span12 section-heading -->
    </div>  <!-- .row -->
  </section> <!-- .closed-proposals -->      
  
<?php endif; ?> 



