<?php
/**
 * Template for proposal overview used on home page and proposal page.
 */

extract($proposal['proposal']);

/* On front page this falls under h1 Open Proposals, so must be h2. On proposal
 * page it is at top level, thus h1.
 */
$level = $is_front ? 2 : 1;
?>

<section class="row proposal-overview">
  <div class="span3">
  	<?php if ($image) : ?>
  	 	<div class="topic-header-image-wrapper">
        <?php echo $image;?>
      </div>
    <?php endif; ?>
  </div>
  <div class="span9">

    <?php if (! $is_front) : ?>
      <p class="status-label"><?php echo ucwords($status_label); ?></p>
    <?php endif; ?>
    
    <h<?php echo $level; ?>>
      <?php echo $is_front ? $link : $title; ?>
      <span class="total-comments" title="<?php echo $comment_count_title; ?>">
        <span class="count"><?php echo $comment_count; ?></span>
        <span class="icon"></span>
      </span>
    </h<?php echo $level; ?>>
    
    <?php // TODO The truncate limit has been customized to the Debt Collection summary.
          // We need a custom truncate function that looks for a close </p> tag. 
    ?>
    
		<div class="proposal-header-details <?php echo ((!$image) ? "proposal-with-no-image" : "proposal-with-image"); ?>"
             data-truncate-limit="<?php echo $data_truncate_limit; ?>">
		  <div class="truncate">
    	  <?php echo $description; ?>
    	</div>
		</div>
		
  </div>
</section>

<?php print render($tabbed_topic_carousel_data); ?>