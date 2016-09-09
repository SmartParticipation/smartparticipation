<?php
/**
 * Template to display moderator interface.
 * 
 * Available variables:
 * - $data
 * - $page
 *
 */

extract($data['#items']);
?>

<div class="container" id="moderator_panel">

  <section>
    <div class="row">
      <div class="span4">
        <p id="com_stats_header" class="moderator_column_header"></p>
      </div>
      <div class="span8">
  
        <p class="moderator_column_header">
  
        <span id="filters_span">Show
          <select id="limit_dropdown">
            <option selected="selected" value="5">5</option>
            <option value="10">10</option>
            <option value="15">15</option>
            <option value="20">20</option>
            <option value="all">all</option>
          </select>
          comments per page
          <span class="divider">|</span>
          Order by
          <select id="filter_dropdown">
            <option selected="selected" value="newest" selected>newest to oldest</option>
            <option value="oldest">oldest to newest</option>
          </select>
          <span class="divider">|</span>
          <a id="topics_toggle" class="order_sup_deactivated">Topics</a>
          <span class="divider">|</span>
          <a id="users_toggle" class="order_sup_deactivated">Users</a>
        </span>
        <br/>
        <span id="comment_col_header">All Proposals - All </span> <span class="noncolored_header">comments</span>
        </p>
      </div>
    </div>
    <div class="row">
      <div  class="span4">
        <div id="proposal_accordion">
        <h3 nid="all" class="proposal_header">All Proposals</h3>
        <div class="proposal_content">
          <div nid="all" style="display:block;" class="filter_div">
            <?php   
              $all_node_stats = $node_stats['all'];
              $all_node_stats['topic_nid'] = 'all';
              print theme('moderator_comment_filter', array('data' => $all_node_stats)); 
            ?>
          </div>
        </div>
        
        <!-- Render dropdown for all -->
        <?php foreach(array_keys($node_structure) as $proposal_nid): ?>
          <br/>
          <h3 nid="<?php echo $proposal_nid; ?>" class="proposal_header"><?php echo $node_titles[$proposal_nid]; ?></h3>
          <div class="proposal_content">
            <div class="topic_accordion">
            <h3 nid="<?php echo $proposal_nid; ?>" class="filter_header">All Topics</h3>
            <div nid="<?php echo $proposal_nid; ?>" class="filter_div">
              <?php 

                $proposal_node_stats = $node_stats[$proposal_nid];
                $proposal_node_stats['topic_nid'] = $proposal_nid;
                $proposal_node_stats['title'] = $node_titles[$proposal_nid];
                print theme('moderator_comment_filter', array('data' => $proposal_node_stats)); 
              ?>
            </div>
  
            <?php foreach(array_keys($node_structure[$proposal_nid]) as $topic_nid): ?>
              <?php 
                $topic_node_stats = $node_stats[$topic_nid]; 
                $topic_node_stats['topic_nid'] = $topic_nid; 
              ?>
              <h3 nid="<?php echo $topic_nid; ?>" class="filter_header"><?php echo $node_titles[$topic_nid]; ?></h3>
              <div nid="<?php echo $topic_nid; ?>" class="filter_div">
                <?php print theme('moderator_comment_filter', array('data' => $topic_node_stats)); ?>
              </div>
  
            <?php endforeach; ?>
            </div>
          </div>
        <?php endforeach; ?>
        </div>
      </div>
  
      <div class="span8">
        <div class="page_control_div">
          <div id="comment_pager_top" class="comment_pager" default_items="<?php echo $comment_data['total_com']; ?>"></div>
          <span class="separator" style="float:left; margin-right:10px; margin-left:10px;"></span>
        </div>
        <div id="comment_col">
        <?php
          $counter = 1;
          foreach($comment_data as $comment){
            if(is_array($comment)){
            $comment = array_merge(array(
              'active_domain_nid' => 'all',
              'uid_filter' => '',
              'page_comment_index' => $counter
            ), (array)$comment);
            print theme('moderator_comment', array('comment' => $comment));
            $counter++;
            }
          }

          if (!$comment_data['total_com']) {
            print theme(
              'moderator_no_comments',
              array('no_comments_message' => smartparticipation_core_get_no_comments_message())
            );
          }
        ?>
        </div>
        <div class="page_control_div">
          <div id="comment_pager_bottom" class="comment_pager" default_items="<?php echo $comment_data['total_com']; ?>"></div>
           <span class="separator" style="float:left; margin-right:10px; margin-left:10px;"></span>
        </div>
      </div>
    </div>
  </section>

  <div>
    <?php echo $interest_survey; ?>
  </div>

</div>

