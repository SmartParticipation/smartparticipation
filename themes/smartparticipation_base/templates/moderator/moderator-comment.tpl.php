<?php
/**
 * Template for comments on the moderator page.
 *
 * Available variables:
 * - $comment
 *
 */

// List of non comment specific variables definied prior to generating this theme
$active_domain_nid = $comment['active_domain_nid'];
$uid_filter = $comment['uid_filter'];
$page_comment_index = $comment['page_comment_index'];

$button_text_dict = array(
  'redact' => array(
    true => 'Redact again',
    false => 'Redact'
  ),
  'quarantine' => array(
    true => 'Unquarantine',
    false => 'Quarantine'
  ),
  'recommend' => array(
    true => 'Unrecommend',
    false => 'Recommend'
  ),
  'reply' => array(
    true =>'Edit reply',
    false => 'Reply'
  ),
);

$formal_filter_dictionary = array(
  'unread' => 'Unread',
  'in_progress' => 'In progress',
  'done' => 'Done',
  'reply' => 'Reply',
  'no_reply' => 'No reply',
  'quarantine' => 'Quarantine',
  'redacted' => 'Redacted',
  'recommended' => 'Recommended',
  'all_comments'=> 'All'
);



?>

<div class='mod_panel_comment' cid='<?php echo $comment['cid']; ?>' uid='<?php echo $comment['uid']; ?>' token='<?php echo $comment['token']; ?>'>

  <!-- Print proposal, topic, and date -->
  <div class='comment_header'>
    <div class='comment_left_header'>
      <?php if($active_domain_nid == 'all'): ?>
        <span class='proposal_span'> <?php echo $comment['comment_proposal']; ?></span><br/>
      <?php endif; ?>
  
      <span class='topic_span'><?php echo $comment['comment_topic_title']; ?></span><br/>
      <span class='subtopic_span'><?php echo $comment['comment_subtopic_title']; ?></span><br/>
      <span class='created_span'><?php echo $comment['created_display_date']; ?></span>
      <span class='permalink'><?php echo $comment['permalink']; ?></span>
    </div>
  
    <!-- Print user profile, name, and profile -->
    <?php if($uid_filter == ''): ?>
      <div class='comment_right_header'>
        <a class='comment_user_comment_history'>All <?php echo $comment['username']; ?>'s comments</a><br/>
        <a class='comment_user_interest_survey'><?php echo $comment['username']; ?>'s Interest survey</a><br/>
        <a class='comment_username' href='<?php echo smartparticipation_core_path_to_user_profile($comment['uid']); ?>'><?php echo $comment['username']; ?></a><br/>

        <div class='comment_user_pic'><?php //echo $comment['author_picture']?></div>
      </div>
    <?php endif; ?>

  </div> <!--  .comment_header -->

  <div class='clear_float'></div>

  <!--Insert tags -->
  <div class='tag_container'>
    <?php if($comment['moderation_status'] == 'unread'): ?>
      <div class='comment_unread_tag comment_tag' num='<?php echo $comment['cid']; ?>' >Unread</div>
    <?php elseif($comment['moderation_status'] == 'done'): ?>
      <div class='comment_done_tag comment_tag' num='<?php echo $comment['cid']; ?>' >Done</div>
      <div class='done_tooltip comment_tag_tooltip' ><?php echo $formal_filter_dictionary[$comment['moderation_action']]; ?></div>
    <?php elseif($comment['moderation_status'] == 'in_progress'): ?>
      <div class='comment_in_progress_tag comment_tag' num='<?php echo $comment['cid']; ?>' >In Progress</div>
        <?php if ($comment['original'] != null || count($comment['notes']) > 0) : ?>
          <div class='in_progress_tooltip comment_tag_tooltip' >
            <?php if ($comment['original'] != null): ?>Redacted<?php endif; ?>
            <?php if ($comment['original'] != null && count($comment['notes']) > 0): ?><div class='note_content_spacer'></div><?php endif; ?>
            <?php if (count($comment['notes']) > 0): ?>Added Note<?php endif; ?>
          </div>
        <?php endif; ?>
    <?php endif; ?>
    
    <div class='clear_float'></div>
    
    <?php if($comment['recommended'] == 'yes'): ?>
      <div class='comment_rec_tag' ><img class='heart_img' src='/sites/all/themes/regroom_base/images/heart.png' alt='heart'></img></div>
    <?php endif; ?>
  </div>

  <!-- Print page index number -->
  <div class='mod_panel_comment_content'>

    <!-- Hidden original comment if it exists -->
    <?php if($comment['original'] != ''): ?>
      <div class='original_comment_div' >
        <img class='x_img' src='/sites/all/themes/regroom_base/images/x.png'></img><h3 class='redact_comment_header'>Original Comment</h3>
        <?php // TODO We should be sending the comment object and calling render on the field, rather than brute-force printing the safe value. ?>
        <p><?php echo $comment['original']['safe_value']; ?></p>
        <div class='loading_restore_original'><img class='loading_gif' src='/sites/all/themes/regroom_base/images/ajax_loader.gif'></img></div>
        <button class='submit_restore_original'>Restore original</button>
      </div>
    <?php endif; ?>

    <!-- Comment index -->
    <span class='page_comment_index'><?php echo $page_comment_index ?></span>

    <!-- Print comment content -->
    <div class='content_text'>
      <?php // TODO We should be sending the comment object and calling render on the body, rather than brute-force printing the safe value. ?>
      <?php echo $comment['body']['safe_value']; ?>
    </div>

    <!-- Moderator buttons -->
    <ul class='links inline comment_links'>
      <!-- Spacer -->
      <div class='ul_spacer'></div>

      <!-- In progress -->
      <li><a action='in_progress' >In progress</a></li>

      <!-- Divider -->
      <span class='action_divider'>|</span>

      <!-- Add note -->
      <li><a action='add_note_input'>Add note</a></li>

      <!-- Divider -->
      <span class='action_divider'>|</span>

      <!-- Redact -->
      <?php $is_active = $comment['original'] != ''; ?>
      <li>
        <?php
        if ($is_active) {
          echo $comment['redact_again_permalink'];
        } else {
          echo $comment['redact_permalink'];
        }
        ?>
      </li>

      <?php if($is_active): ?>
        <li><a action='show_original_comment'>See original comment</a></li>
      <?php endif; ?>

      <!-- Divider -->
      <span class='action_divider'>|</span>

      <!-- Reply -->
      <?php $is_active = $comment['moderation_action'] == 'reply'; ?>
      <?php if($is_active): ?>
        <li><?php echo $comment['edit_reply_permalink']; ?></li>
      <?php else: ?>
        <li><?php echo $comment['reply_permalink']; ?></li>
      <?php endif; ?>

      <!-- Quarantine -->
      <?php $is_active = $comment['moderation_action'] == 'quarantine' ?>
      <li><a action='quarantine'><?php echo $button_text_dict['quarantine'][$is_active]; ?></a></li>

      <!-- No reply action -->
      <li><a action='no_reply'>No reply</a></li>

      <!-- Divider -->
      <span class='action_divider'>|</span>

      <!-- Recommend -->
      <?php $is_active = $comment['recommended'] == 'yes'; ?>
      <li><a action='recommend'> <?php echo $button_text_dict['recommend'][$is_active]; ?></a></li>
    </ul>

    <!-- Input area for adding notes, redacting, replying -->
    <div class='comment_mod_input_div'>
      <div class='input_header'>
        <span class='input_header_content'></span>
        <!--  <span class='send_note_button'>Send Note</span> -->
      </div>

      <!-- Details on emailing the note -->
      <!--  
      <div class='send_note_p'>
        <br/><span class='align_right'><input class='note_to' type='text' name='note_to'> *</span>To
        <br/><span class='align_right'><input class='note_subject' type='text' name='note_subject'> *</span>Subject
      </div>
      -->

      <textarea class='input_area'></textarea>
      <div class='error_div'>
        <span class='input_mod_error_span'></span>
      </div>

      <div class='input_lower_span'>

        <button class='input_submit'>Submit</button>  or  <a class='input_cancel_link'>Cancel</a>
        <span class='input_submit_email'><input type="checkbox" class="checkbox_email"/> Email this note</span>
      </div>

      <div class="email_fields">
      To: <input class='note_to' type='text' name='note_to' value=''>  &nbsp; &nbsp; Subject: <input class='note_subject' type='text' name='note_subject' value=''>
      </div>

      <br/><br/>
    </div> <!-- .comment_mod_input_div -->

    <!-- Hidden reply contents -->
    <div class="hidden_data">
      <div class="comment_body"><?php echo $comment['body']['value']; ?></div>
      <div class='reply_body'><?php echo $comment['reply_body'] ?></div>
      <div class='reply_cid'><?php echo $comment['reply_cid']?></div>
      <div class='interest_survey_result' style="display: none">
        <?php
        foreach($comment['survey_data'] as $survey_data) {
          echo '<div>' .$survey_data['question'] . '<ul>';
          foreach ($survey_data['responses'] as $response) {
            echo '<li>' . $response . '</li>';
          }
          echo '</ul></div>';
        }
        ?>
      </div>
    </div>

    <!-- Loading gif -->
    <div class='loading_p'><img class='loading_gif' src='/sites/all/themes/regroom_base/images/ajax_loader.gif'></img></div>

    <!-- Notes if any exist -->
    <?php if (count($comment['notes']) > 0): ?>
      <div class='comment_notes_dropdown'>
        <h3 class='notes_dropdown_header'>&nbsp;&nbsp;&nbsp;&nbsp;Notes <span class='notes_length_span'><?php echo count($comment['notes']); ?></span></h3>
        <div class='note_expansion' style='display: none;'>
          <?php for ($i = 0; $i < count($comment['notes']); $i++): ?>
            <div class='note_content'><?php echo $comment['notes'][$i]->created; ?><br/>
                &nbsp;&nbsp;&nbsp;&nbsp;<?php echo $comment['notes'][$i]->body_value; ?>
            </div>
            <?php if ($i < count($comment['notes']) - 1): ?>
              <div class='note_content_spacer'></div>
            <?php endif; ?>
          <?php endfor; ?>
        </div>
      </div>
    <?php endif; ?>

  </div>
</div>