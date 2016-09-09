<?php
/**
 * @file
 * Template for display of featured comments (recent, recommended).
 *
 * Currently used on both home page and proposal page to display recent comments
 * (per site, per proposal) and recommended comments (per site, per proposal).
 *
 * Available variables:
 *
 * - $featured_comments
 *   - block_title: the title of the block being rendered
 *   - #items. An array of comment data. For each comment:
 *     - body: the comment body
 *     - date: the date the comment was submitted
 *     - permalink: link element targeting the comment permalink
 *     - proposal_link: link to the associated proposal
 *     - user_profile_link: link to the comment author's profile
 *     - user_picture: themed user picture
 *
 */
// print kpr($featured_comments); 
?>

<h2 class="section-content-divider"><?php echo $featured_comments['block_title']; ?></h2>

<?php if ( ! empty($featured_comments['featured_comments_items']['#items']) ) : ?>

  <?php print render($featured_comments['featured_comments_items']); ?>

<?php else : ?>

  <p class="no-content"><?php echo $featured_comments['no_comments_message']; ?></p>

<?php endif; ?>
