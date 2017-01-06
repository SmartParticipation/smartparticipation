<?php
/**
 * Template for SmartParticipation display of comments on user pages, grouped by proposal.
 * Includes user's comments, comments user endorsed, and replies to user's 
 * comments.
 * 
 * Available variables:
 *  - $user_comment_data: a structured array of user comments grouped by proposal.
 * 
 * @see smartparticipation_user_comments_page()
 * @see smartparticipation_user_endorsements_page()
 * @see smartparticipation_user_replies_page()
 */

?>

<section class="profile-comments">

  <?php if (! empty($user_comment_data['#items'])) : ?>
    <div id="multiAccordion">
    
      <?php foreach ($user_comment_data['#items'] as $proposal) : ?>
      
        <h3><a href="<?php echo $proposal['url']; ?>"><?php echo $proposal['title']; ?> | <span class="status"><?php echo $proposal['status']; ?></span></a></h3>
    
        <div class="comments">
          <?php foreach ($proposal['comments'] as $comment) : ?>
            <article>
              <header>
                <h3>
                  <?php print $comment['author_thumbnail']; ?>
                  <span class="plain"><?php echo $comment['author_name']; ?></span>
                  <?php 
                    /* 
                     * For proper format: http://www.whatwg.org/specs/web-apps/current-work/multipage/text-level-semantics.html#datetime-value
                     * <time class="comment-time" pubdate datetime="2010-11-13T02:10:00Z"> </time> 
                     */
                  ?>
                  <time class="comment-time" pubdate><?php echo $comment['display_date']; ?></time>
        
                  <div class="right">
                    <?php if ($comment['endorsement_count']) : ?>
                      <span class="endorsement-counter"><?php echo $comment['endorsement_count']; ?></span>
                    <?php endif; ?>
                    <?php if ($comment['recommended']) : ?>                    
                      <?php $title = t('Recommended by the moderator for demonstrating effective commenting skills.'); ?>
                      <p class="recommended" title="<?php echo $title; ?>">
                        <span class="recommended-icon"></span>
                      </p>
                    <?php endif; ?>
                    <span class="comment-number"><?php echo $comment['number']; ?></span>
                   </div>
                </h3>
              </header>
        
              <div class="comment">
                <?php echo $comment['body']; ?>
              </div>
              
              <p class="permalink">
                <?php echo $comment['permalink']; ?>
              </p>
            </article>  
          <?php endforeach; // comment loop ?>
        </div>
        
      <?php endforeach; // proposal loop ?>      
    </div>
  
  <?php else: ?>
    <p class="no-content"><?php echo $user_comment_data['no_comments_message']; ?></p>
    
  <?php endif; ?>
</section>
