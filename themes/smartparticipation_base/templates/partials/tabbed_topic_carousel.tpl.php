<?php

/**
 * Template for topic carousel with phase tabs.
 * 
 * TODO Combine with tabless node carousel markup in node_carousel.tpl.php.
 * That template repeats the interior portion of this template. The data 
 * should be gathered in the controller rather than in the template, as the
 * node carousel does.
 */

extract($tabbed_topic_carousel_data);

$proposal_id = 'proposal' . $proposal_nid;


?>

<!-- Proposal topics, by phase -->

<section class="proposal-phases" id="<?php echo $proposal_id; ?>">

  <?php 
    /* Don't include tabs if only one phase. 
     * TODO Should be handled from controller. If only one tab displayed, use
     * tabless carousel. Otherwise tabbed carousel.
     */
  ?>
  <?php if (count($phases) > 1) : ?>
    <ul class="tabs">
      <?php
      $k = 1;
      foreach ($phases as $phase) : 

        $tab_id = $proposal_id . '-tab' . $k;
      ?>    
  
        <li>
          <a class="<?php echo $phase['active_class']; ?>" href="<?php echo '#' . $tab_id; ?>">
            <span class="<?php echo $phase['phase_class']; ?>"></span><?php print $phase['name'];?> 
            
            <?php if ($phase['show_comment_count']): ?>
              <span title="<?php echo $phase['comment_count_title']; ?>">
                <span class="count-divider"> - </span><span class="phase-comment-count" ><?php echo $phase['comment_count']; ?></span><span class="icon-comments-phase"></span>
              </span>
            <?php endif; ?>
          </a>
        </li>   
  
        <?php $k++; ?>
        
      <?php endforeach; ?>
    </ul>
  <?php endif; ?>
  
  <?php
  $k = 1;
  foreach ($phases as $phase) :
    
    $tab_id = $proposal_id . '-tab' . $k;

    ?>
    <div class="phase-tab" id="<?php echo $tab_id; ?>">

    <?php echo render($phase); ?>

    </div> <!--  .phase-tabs -->
    
    <?php $k++; ?>
    
  <?php endforeach; ?>
  
</section>


