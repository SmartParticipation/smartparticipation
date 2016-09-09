<?php

/**
 * Template for topic and document page header.
 */

?>
<section id="topic-header" class="row">


  <div class="span3 pull-right">
    <nav id="topic-page">
      <ul>
        <?php
        // TODO Handle this like the proposal_documents_link. Put in a function
        // in the back end and pass the array of links to the template. Handle
        // display from a separate template.
        $proposal_nid = $proposal['nid_proposal'];

        foreach ($proposal_phases as $proposal_phase) :
          $proposal_phase_name = $proposal_phase['name'];
          $proposal_phase_class = $proposal_phase['phase_class'];

          $phase_tid = $proposal_phase['tid'];

          $phase_topic_link = '';
          $phase_topic_nid = $proposal_phase['phase_topic_nid'];

          if (!empty($phase_topic_nid)) {
            $phase_topic_link = url('node/' . $phase_topic_nid[0]);
          }
          else {
            $first_topic_nid = $proposal_phase['first_topic_nid'];
            $phase_topic_link = url('node/' . $first_topic_nid[0]);
          }

          ?>
          <li><a href="<?php print $phase_topic_link; ?>"
                 title="<?php print $proposal_phase_name; ?>"
                 class="<?php print ($node_type == 'topic' && $topic_phase_id == $phase_tid) ? 'active' : ''; ?>"><span
                class="<?php echo $proposal_phase_class; ?>"></span><?php print $proposal_phase_name; ?>
            </a>
          </li>
        <?php endforeach; ?>


        <li><?php echo $proposal_documents_link; ?></li>
    </nav>
  </div>

  <?php
  $has_proposal_image = false;
  if (!empty($proposal['image'])) {
    $has_proposal_image = true;
  }
  ?>


  <?php if ($has_proposal_image): ?>
  <div class="span3">
  
    <?php print $proposal['image']; ?>

  </div>
  <?php endif; ?>


  <div class="<?php echo $has_proposal_image ? 'span6' : 'span9'; ?>">

    <div id="topic-header-details" <?php //print $proposal['image'] ? "class='topic-with-image'" : "class='topic-with-no-image'"; ?>>
      <?php if (isset($proposal['status_label'])) : ?>
        <p class="status-label"><?php echo ucwords($proposal['status_label']); ?></p>
      <?php endif; ?>
      
      <div id="proposal-title">
        <h1>
          <?php print $proposal['link']; ?>
        </h1>
        
        <?php if (isset($proposal['description']) && $proposal['description']) : ?>
          <div id="proposal-summary">
            <a href="#proposal-summary-content" id="proposal-summary-link"><?php print t('Summary'); ?><span class="expand-summary"></span></a>          
            <div id="proposal-summary-content"><?php print $proposal['description'];?></div>
          </div>
        <?php endif; ?>
      </div>
      
      <?php if ($node_type == 'topic') : ?>
        <p class="topic-header-type">
          <span class="topic-header-topic-phase" title="<?php echo $topic_phase_title; ?>"><span class="<?php echo $topic_phase_class; ?>"></span><?php echo $topic_phase; ?></span>
          <span class="topic-header-type-title-wrapper">
            <span class="topic-header-type-title"><?php echo $node_title; ?></span>
          
            <?php if ($topic_allows_comments): ?>
            <span class="topic-header-message-count"> 
    					- <span data-type="comment-count-topic"><?php echo $topic_number_comments;?></span>
            	<span class="topic-header-message-count-icon"></span>
    				</span>
            <?php endif; ?>
          </span>

        </p>
    
        <?php if (! empty($same_phase_topics)) : ?>
          <?php $menu_title = t('Select other topics'); ?>
          <p class="select-other-topics"><a class="default" href="#" title="<?php echo $menu_title; ?>"><?php echo $menu_title; ?> <span class="expand-topics"></span></a></p>
          <ul id="topic-header-menu">
            <?php 
            // TODO The controller should send an array of links rather 
            // than node ids; see "other document" links below      
            foreach ($same_phase_topics as $topic) :
              $topic_link = smartparticipation_core_node_menu_title_link($topic);
            ?>
              <li><?php echo $topic_link; ?></li>
            <?php endforeach; ?>
          </ul>     
        <?php endif; ?>     
          
      <?php else : // document node ?>
        <p class="topic-header-type">
          <span class="topic-header-type-title"><?php echo $node_title; ?></span> 
        </p>     
        
        <?php if (! empty($document_links)) : ?>
          <?php $menu_title = t('Select other documents'); ?>
          <p class="select-other-topics"><a class="default" href="#" title="<?php echo $menu_title; ?>"><?php echo $menu_title; ?> <span class="expand-topics"></span></a></p>
          <ul id="topic-header-menu">
            <?php foreach ($document_links as $link) : ?>        
              <li><?php echo $link; ?></li>
            <?php endforeach; ?>
          </ul>     
        <?php endif; ?>      
        
      <?php endif; ?>
    </div>
  </div>
  
</section> 