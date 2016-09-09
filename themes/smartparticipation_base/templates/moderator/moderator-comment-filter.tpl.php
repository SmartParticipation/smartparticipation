<?php
/**
 * Template for filter dropdowns on the moderator page.
 *
 * Available variables:
 * - $data
 *
 */

// List of non-comment-specific variables definied prior to generating this theme
// TODO Move this into the controller and pass to template. Should be defined as
// a constant.
$filters = array(
  'unread' => array(
    'formal' => 'Unread',
    'label_class' => '',
  ),
  'in_progress' => array(
    'formal' => 'In progress',
    'label_class' => '',
  ),
  'done' => array(
    'formal' => 'Done',
    'label_class' => '',
  ),
  'reply' => array(
    'formal' => 'Replied',
    'label_class' => 'in_done_label',
  ),
  'no_reply' => array(
    'formal' => 'No reply',
    'label_class' => 'in_done_label',
  ),
  'quarantine' => array(
    'formal' => 'Quarantined',
    'label_class' => 'in_done_label_last',
  ),
  'redacted' => array(
    'formal' => 'Redacted',
    'label_class' => '',
  ),
  'recommended' => array(
    'formal' => 'Recommended',
    'label_class' => '',
  ),
  'all_comments' => array(
    'formal' => 'ALL comments',
    'label_class' => '',
  ),
);

?>

<!-- Activate default selection for page -->
<div class="radio_filters" nid="<?php echo $data['topic_nid']; ?>" title="">
  <?php foreach(array_keys($filters) as $filter): ?>
    <input type="radio" class="radio_<?php echo $filter; ?>" value="<?php echo $filter; ?>" />
      <label class="<?php echo $filters[$filter]['label_class']; ?>" for="radio_<?php echo $filter; ?>">
        <?php echo $filters[$filter]['formal']; ?>
        <span class='comment_count_inactive'>
          <?php echo $data[$filter]; ?>
        </span>
      </label>
  <?php endforeach; ?>
</div>