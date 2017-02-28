<?php
/**
 * @file
 * smartparticipation_base's theme implementation to provide an HTML container for comments.
 *
 * Available variables:
 * - $content: The array of content-related elements for the node. Use
 *   render($content) to print them all, or
 *   print a subset such as render($content['comment_form']).
 * - $classes: String of classes that can be used to style contextually through
 *   CSS. It can be manipulated through the variable $classes_array from
 *   preprocess functions. The default value has the following:
 *   - comment-wrapper: The current template type, i.e., "theming hook".
 * - $title_prefix (array): An array containing additional output populated by
 *   modules, intended to be displayed in front of the main title tag that
 *   appears in the template.
 * - $title_suffix (array): An array containing additional output populated by
 *   modules, intended to be displayed after the main title tag that appears in
 *   the template.
 *
 * The following variables are provided for contextual information.
 * - $node: Node object the comments are attached to.
 * The constants below the variables show the possible values and should be
 * used for comparison.
 * - $display_mode
 *   - COMMENT_MODE_FLAT
 *   - COMMENT_MODE_THREADED
 *
 * Other variables:
 * - $classes_array: Array of html class attribute values. It is flattened
 *   into a string within the variable $classes.
 *
 * @see template_preprocess_comment_wrapper()
 * @see theme_comment_wrapper()
 */

$comment_count = $content['#node']->comment_count;

?>

<div class="<?php print $classes; ?>"<?php print $attributes; ?> data-rr-event_entity-id="<?php echo $content['#node']->nid; ?>">

  <div class="comments-form-container">
    <?php print render($title_prefix); ?>
    <h3 class="title"><?php print t('Comments'); ?><span class="comment-count"><span data-type="comment-count-subtopic"><?php print $comment_count; ?></span><span class="comment-count-icon"></span></span></h3>

    <?php if (!empty($content['recommended_comments_enabled']) && ($comment_count || ! empty($content['comment_form']))) : ?>
      <a class="recommended-filter" href="#" title="View recommended comments">
        <span class="recommended-filter-icon"></span>
      </a>
    <?php endif; ?>

    <?php
      if (!empty($content['button_links'])) {
        foreach ($content['button_links'] as $button_link) {
          print $button_link;
        }
      }
    ?>

    <?php print render($title_suffix); ?>

    <?php if (isset($content['commenting_closed_message'])) : ?>
      <div class="phase-closed">
        <div><?php echo $content['commenting_closed_message']; ?></div>
      </div>
    <?php endif;  ?>
        
    <?php 
      // Show the comment form, if it exists. If commenting is closed, the form
      // has been removed from the content. 
      print render($content['comment_form']); 
    ?>

  </div> <!--  .comments-form-containter -->

  <div class="comments-container" data-truncate-limit="<?php echo $content['comment_truncate_limit']; ?>">
    <?php print render($content['comments']); ?>
  </div>
  
</div> <!-- #comments -->
