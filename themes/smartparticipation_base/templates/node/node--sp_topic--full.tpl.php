<?php
/**
 * @file
 * regroom_base's theme implementation to display a topic node.
 *
 * Available variables:
 * - $title: the (sanitized) title of the node.
 * - $content: An array of node items. Use render($content) to print them all,
 *   or print a subset such as render($content['field_example']). Use
 *   hide($content['field_example']) to temporarily suppress the printing of a
 *   given element.
 * - $user_picture: The node author's picture from user-picture.tpl.php.
 * - $date: Formatted creation date. Preprocess functions can reformat it by
 *   calling format_date() with the desired parameters on the $created variable.
 * - $name: Themed username of node author output from theme_username().
 * - $node_url: Direct url of the current node.
 * - $display_submitted: Whether submission information should be displayed.
 * - $submitted: Submission information created from $name and $date during
 *   template_preprocess_node().
 * - $classes: String of classes that can be used to style contextually through
 *   CSS. It can be manipulated through the variable $classes_array from
 *   preprocess functions. The default values can be one or more of the
 *   following:
 *   - node: The current template type, i.e., "theming hook".
 *   - node-[type]: The current node type. For example, if the node is a
 *     "Blog entry" it would result in "node-blog". Note that the machine
 *     name will often be in a short form of the human readable label.
 *   - node-teaser: Nodes in teaser form.
 *   - node-preview: Nodes in preview mode.
 *   The following are controlled through the node publishing options.
 *   - node-promoted: Nodes promoted to the front page.
 *   - node-sticky: Nodes ordered above other non-sticky nodes in teaser
 *     listings.
 *   - node-unpublished: Unpublished nodes visible only to administrators.
 * - $title_prefix (array): An array containing additional output populated by
 *   modules, intended to be displayed in front of the main title tag that
 *   appears in the template.
 * - $title_suffix (array): An array containing additional output populated by
 *   modules, intended to be displayed after the main title tag that appears in
 *   the template.
 *
 * Other variables:
 * - $node: Full node object. Contains data that may not be safe.
 * - $type: Node type, i.e. story, page, blog, etc.
 * - $comment_count: Number of comments attached to the node.
 * - $uid: User ID of the node author.
 * - $created: Time the node was published formatted in Unix timestamp.
 * - $classes_array: Array of html class attribute values. It is flattened
 *   into a string within the variable $classes.
 * - $zebra: Outputs either "even" or "odd". Useful for zebra striping in
 *   teaser listings.
 * - $id: Position of the node. Increments each time it's output.
 *
 * Node status variables:
 * - $view_mode: View mode, e.g. 'full', 'teaser'...
 * - $teaser: Flag for the teaser state (shortcut for $view_mode == 'teaser').
 * - $page: Flag for the full page state.
 * - $promote: Flag for front page promotion state.
 * - $sticky: Flags for sticky post setting.
 * - $status: Flag for published status.
 * - $comment: State of comment settings for the node.
 * - $readmore: Flags true if the teaser content of the node cannot hold the
 *   main body content.
 * - $is_front: Flags true when presented in the front page.
 * - $logged_in: Flags true when the current user is a logged-in member.
 * - $is_admin: Flags true when the current user is an administrator.
 *
 * Field variables: for each field instance attached to the node a corresponding
 * variable is defined, e.g. $node->body becomes $body. When needing to access
 * a field's raw values, developers/themers are strongly encouraged to use these
 * variables. Otherwise they will have to explicitly specify the desired field
 * language, e.g. $node->body['en'], thus overriding any language negotiation
 * rule that was previously applied.
 *
 * @see template_preprocess()
 * @see template_preprocess_node()
 * @see template_process()
 */
extract($content);

$topic_nid = $node->nid;

?>

<div id="node-<?php echo $topic_nid; ?>" class="<?php echo $classes; ?> clearfix"<?php echo $attributes; ?>>

  <div class="content clearfix"<?php echo $content_attributes; ?>>

    <?php echo theme('topic_header', smartparticipation_core_get_topic_header_data($proposal, $topic_nid)); ?>
        
    <?php if (!empty($node->field_sp_topic_subtopic)) : ?>
        
      <div class="subtopic-details">
        <header>
          <h2><?php echo t('Subtopics'); ?></h2>
        
          <div class="expand-all">
          <a href="#expandAll"><?php echo t('expand all'); ?><span class="expand-all-icon"></span></a>
          </div>
        
          <div class="collapse-all">
          <a href="#collapseAll"><?php echo t('collapse all'); ?><span class="collapse-all-icon"></span></a>
        </div>
      </header>

      <?php foreach ($subtopics as $index=>$subtopic) : ?>

        <div id="subtopic-title-<?php echo $index; ?>" class="subtopic_title subtopic-title-node-<?php echo $subtopic['nid']; ?>" data-rr-event_entity-id="<?php echo $subtopic['nid']; ?>">
          <?php echo $index ?><span class="separator">|</span><?php echo $subtopic['title']; ?>
    	    <?php if ($phase_allows_comments) : ?>
            - <span data-type="comment-count-subtopic"><?php echo $subtopic['comment_count']; ?></span><span class="subtopic_comments_dialog"></span>
    	    <?php endif; ?>
        </div>
 
        <?php if ($subtopic['body']) : ?>
  
          <div id="subtopic-body-<?php echo $index; ?>" class="row subtopic_body subtopic-body-node-<?php echo $subtopic['nid']; ?>">
            <div class="subtopic_text span7">
              <?php if (!empty($phase_text_heading)) { ?>
              <h3><?php echo $phase_text_heading; ?></h3>
              <?php } ?>
              
              <?php    
                // Closed non-commenting phases display the phase closed message here.
                if (isset($subtopic['phase_closed_message']) && $subtopic['phase_closed_message']) : 
              ?>
                  <div class="phase-closed">
                    <div><?php echo $subtopic['phase_closed_message']; ?></div>
                  </div>
              <?php endif; ?>
              
              <div class="subtopic-body-text">
                <?php                        
                  echo render($subtopic['doc']);

                  echo $subtopic['body'];
                ?>
              </div>

            </div>

            <?php if (isset($subtopic['comment_node_page_additions'])) : ?>
              <div class="subtopic_comments span5">
                <?php 
                  // Render the comments, and the comment form, where appropriate
                  echo render($subtopic['comment_node_page_additions']);               
                ?>  
              </div> 
            <?php endif; ?>
          </div>
          
        <?php endif; ?>
		      
      <?php endforeach; ?>

    </div> <!-- .subtopic-details -->    
     
  <?php endif; ?>
      
  </div> <!-- .content -->

  <?php 
    // TODO Again we are replicating the data retrieved for the topic header. We
    // need to get all the data ONCE, and prepackage it in various ways 
    // according to the needs of each template.
    $carousel_data = smartparticipation_core_topic_carousel_data($node);
    echo render($carousel_data); 
  ?>
  
</div> <!-- #node-<?php echo $topic_nid; ?> -->

<?php

// Insert the survey dialog markup if it's found.
if (!empty($interest_survey)) {
  echo $interest_survey;
}
/** @var \SmartParticipation\model\CommentTips $comment_tips */
if (!empty($comment_tips)) {

  $domdoc = new DOMDocument();
  $domdoc->loadHTML('<?xml encoding="UTF-8">' . $comment_tips->body);
  $tips = $domdoc->getElementsByTagName('li');
  $items = array();
  foreach ($tips as $tip) {
    $li = $tip->ownerDocument->saveXML($tip);
    if (preg_match('#<li>(.*?)</li>#',$li,$matches)) {
      array_push($items,$matches[1]);
    }
  }

  $number_of_tips = count($items);
  $column_span = floor(12 / $number_of_tips);
  $span_class = 'span' . $column_span;

  echo '<div class="comment-tips-alert-data" data-prompt-enabled="'.(!empty($use_comment_tips_prompt) ? '1' : '0').'" data-get-state-base-url="'.$comment_tips_get_state_base_url.'" style="display:none">';
  if (!empty($comment_tips->lead_sentence)) {
    echo '<h3>' . t($comment_tips->lead_sentence) . '</h3>';
  }
  echo '<div class="comment-tips-body row-fluid">';
  $tip_count = 0;
  $tip_divider = '<span class="tip-divider"></span>';
  foreach ($items as $item) {
    $tip_count++;
    $show_tip_divider = TRUE;
    if ($tip_count == $number_of_tips) {
      $show_tip_divider = FALSE;
    }
    echo '<div class="comment-tip-wrapper ' . $span_class . '">'
      . ($show_tip_divider ? $tip_divider : '')
      . '<span class="comment-tip">' . $item . '</span></div>';
  }
  echo '</div>';
  echo '<div class="comment-tips-actions">';
  echo '<button class="revise-comment">Revise comment</button>';
  echo '<span>or</span>';
  echo '<button class="submit-comment-as-is">Submit comment as is</button>';
  echo '</div>';
  echo '<div class="comment-tips-bg">';
  echo '<img src="/' . drupal_get_path('theme', 'smartparticipation_base') . '/images/comment-tips/reader.png">';
  echo '</div>';
  echo '</div>';
}

?>
