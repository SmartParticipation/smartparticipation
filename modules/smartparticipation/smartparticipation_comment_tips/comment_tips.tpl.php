<?php

/** @var \SmartParticipation\model\CommentTips $comment_tips */
if (!empty($comment_tips_data['#comment_tips'])) {

  $comment_tips = $comment_tips_data['#comment_tips'];
  $use_comment_tips_prompt = $comment_tips_data['use_comment_tips_prompt'];
  $comment_tips_get_state_base_url = $comment_tips_data['comment_tips_get_state_base_url'];

  $items = $comment_tips->tips_text;

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
