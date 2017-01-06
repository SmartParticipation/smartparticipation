<?php 
/**
 * @file 
 * Template for display of a closed proposal in the closed proposal block.
 *
 * 
 * Available variables:
 * $proposal
 *   - 
 * 
 * See also: smartparticipation_closed_proposal_data()
 */

extract($proposal['proposal']);


?>

<li>
  <p class="title">
    <?php echo $image; ?>
    <span>
      <?php echo $title; ?>
      <span class="date"><?php echo t('Opened'); ?> <time datetime="<?php echo $dates['open']['attribute']; ?>"><?php echo $dates['open']['display']; ?></time></span>
      <span class="date"><?php echo t('Closed'); ?> <time datetime="<?php echo $dates['close']['attribute']; ?>"><?php echo $dates['close']['display']; ?></time></span>
    </span>
  </p>
  <div class="description"><?php echo $summary; ?></div>
  <!-- 
                      The Consumer Financial Protection Bureau (CFPB) believes debt collection is an important issue for consumers. 
                      In this discussion, CFPB asked for input about the debt collection system, about consumer experiences, and about how new rules for debt 
                      collectors might better protect consumers without imposing unnecessary burdens on industry. During the time commenting was open, 8480 
                      people visited RegulationRoom and 224 people posted 956 comments. Many people joined the discussion to share stories about their own debt 
                      collection experience. Posts on debt collection litigation and use of phones and mobile phones in debt collections were the most popular. -->
  
</li>