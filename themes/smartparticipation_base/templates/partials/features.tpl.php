<?php 

/**
 * Template for Features section on home page and proposal page. Includes 
 * recent and recommended comment blocks. In future may also include a graphic
 * features block.
 */

?>

<section id="recent-comments">
  <div class="row">
    <div class="span12 section-heading">
      <h1><?php echo $features['title']; ?></h1>
    </div>
  </div>
    
  <div class="row">
    <?php
//     START What's new column #1
//     for 3 columns, use three-columns class
//    <div class="recent-comments-column two-columns">
    ?>

    <div class="recent-comments-column recent-comments-data span6">
    
      <?php echo render($features['#items']['smartparticipation_core_recent_comments']); ?>
    </div>
    <?php
//     START What's new column #2
//    <div class="recent-comments-column two-columns">
    ?>
    <div class="recent-comments-column recommended-comments-data span6">
      <?php echo render($features['#items']['smartparticipation_core_recommended_comments']); ?>
    </div>
        
   <?php /*
     <!-- START What's new column #3 -->  
     <div class="whats-happening-column three-columns">
       <h4>Feature Section</h4>
       <img class="feature-section" src="/sites/all/themes/regroom_base/images/featured-pie.png">
       <a href="#" title="" class="feature-section-more" href="#">See more visuals ></a>
      </div>
    */ ?>
        
  </div>
</section> <!--  #whats-happening-now -->
    
    

