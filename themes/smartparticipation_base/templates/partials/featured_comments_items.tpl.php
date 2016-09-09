<?php foreach ($featured_comments_data['#items'] as $comment) : ?>
  <article>

    <?php echo $comment['user_thumbnail']; ?>

    <div class="comment-data">

      <header>
        <h4>
          <?php // If no picture, we want to print the default picture.
          // Does user_picture theme do this automatically?
          // This image has a size of 50 by 50 pixels
          // please don't desine size in markup, css is
          // taking care of this'?>
          <?php // echo $comment['user_picture']; ?>

          <div class="username"><?php echo $comment['user_profile_link']; ?>
            <?php if ($comment['recommended']) : ?>
              <?php // TODO Move to view controller ?>
              <?php $title = t('Recommended by the moderator for demonstrating effective commenting skills.'); ?>
              <span class="recommended-icon" title="<?php echo $title; ?>"></span>
            <?php endif; ?>
          </div>

          <?php if ($comment['proposal_link']) : ?>
            <?php echo $comment['proposal_link'];?>
          <?php endif; ?>
          <time pubdate><?php echo $comment['date'];?></time>
        </h4>
      </header>

      <div class="comment-body">
        <?php echo smartparticipation_core_comment_excerpt($comment['body'], $comment['featured_comment_truncate_limit']) ?>
      </div>

      <div class="see-full-comment">
        <?php echo $comment['permalink']; ?>
      </div>

    </div>

  </article>
<?php endforeach; ?>

<?php if ( ! empty($featured_comments_data['more_comments']) ): ?>


  <div class="show-more-comments-button">
  <?php
  $attributes['attributes'] = array('class' => array('use-ajax','button'), 'id' => $featured_comments_data['show_more_button_id']);
  if (!empty($featured_comments_data['max_id'])) {
    $attributes['query'] = array('max_id' => $featured_comments_data['max_id']);
  }
  print l(t('Show more comments'), 'ajax/featured_comments/'.$featured_comments_data['comment_type'], $attributes);
  ?>
  </div>

<?php endif; ?>
