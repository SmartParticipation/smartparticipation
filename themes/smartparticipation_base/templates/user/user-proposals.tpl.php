<?php
/**
 * Template for RegRoom display of comments on user pages, grouped by proposal.
 * Includes user's comments, comments user endorsed, and replies to user's
 * comments.
 *
 * Available variables:
 *  - $user_comment_data: a structured array of user comments grouped by proposal.
 *
 * @see regroom_user_comments_page()
 * @see regroom_user_endorsements_page()
 * @see regroom_user_replies_page()
 */

?>

<section class="profile-comments">

  <?php if (! empty($user_comment_data['#items'])) : ?>
    <div>

      <?php foreach ($user_comment_data['#items'] as $proposal) : ?>

        <p>
        <a href="<?php echo $proposal['url']; ?>"><?php echo $proposal['title']; ?></a> -
        <?php echo $proposal['status']; ?> - <?php echo count($proposal['comments']); ?>
          comment<?php echo (count($proposal['comments']) != 1) ? 's' : ''; ?>
        </p>

      <?php endforeach; // proposal loop ?>
    </div>

  <?php else: ?>
    <p class="no-content"><?php echo $user_comment_data['no_comments_message']; ?></p>

  <?php endif; ?>
</section>
