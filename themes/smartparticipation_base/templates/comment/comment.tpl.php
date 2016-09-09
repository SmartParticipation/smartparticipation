<?php
/**
 * @file
 * regroom_base's theme implementation for comments.
 *
 * Available variables:
 * - $author: Comment author. Can be link or plain text.
 * - $author_link: The link to the comment author, or plain text in the case of 
 *   a moderator comment.
 * - $comment_depth: The depth of the comment.
 * - $comment_permalink: A link element providing the comment's permalink.
 * - $content: An array of comment items. Use render($content) to print them all, or
 *   print a subset such as render($content['field_example']). Use
 *   hide($content['field_example']) to temporarily suppress the printing of a
 *   given element.
 * - $created: Formatted date and time for when the comment was created.
 *   Preprocess functions can reformat it by calling format_date() with the
 *   desired parameters on the $comment->created variable.
 * - $changed: Formatted date and time for when the comment was last changed.
 *   Preprocess functions can reformat it by calling format_date() with the
 *   desired parameters on the $comment->changed variable.
 * - $is_moderator: True if current user has moderator role, false otherwise.
 * - $new: New comment marker.
 * - $submitted: Submission information created from $author and $created during
 *   template_preprocess_comment().
 * - $picture: Authors picture.
 * - $signature: Authors signature.
 * - $status: Comment status. Possible values are:
 *   comment-unpublished, comment-published or comment-preview.
 * - $title: Linked title.
 * - $classes: String of classes that can be used to style contextually through
 *   CSS. It can be manipulated through the variable $classes_array from
 *   preprocess functions. The default values can be one or more of the following:
 *   - comment: The current template type, i.e., "theming hook".
 *   - comment-by-anonymous: Comment by an unregistered user.
 *   - comment-by-node-author: Comment by the author of the parent node.
 *   - comment-preview: When previewing a new or edited comment.
 *   The following applies only to viewers who are registered users:
 *   - comment-unpublished: An unpublished comment visible only to administrators.
 *   - comment-by-viewer: Comment by the user currently viewing the page.
 *   - comment-new: New comment since last the visit.
 * - $title_prefix (array): An array containing additional output populated by
 *   modules, intended to be displayed in front of the main title tag that
 *   appears in the template.
 * - $title_suffix (array): An array containing additional output populated by
 *   modules, intended to be displayed after the main title tag that appears in
 *   the template.
 * - $comment_recommended: True iff the comment has been recommended. 
 *
 * These two variables are provided for context:
 * - $comment: Full comment object.
 * - $node: Node object the comments are attached to.
 *
 * Other variables:
 * - $classes_array: Array of html class attribute values. It is flattened
 *   into a string within the variable $classes.
 *
 * @see hook_preprocess()
 * @see hook_preprocess_comment()
 * @see template_preprocess()
 * @see template_preprocess_comment()
 * @see template_process()
 * @see theme_comment()
 */


?>
<div class="<?php print $classes; ?> clearfix"<?php print $attributes; ?> data-recommended="<?php echo $comment_recommended ? '1' : '0'; ?>">

  <div class="attribution">

    <div class="submitted">
      <?php if ($comment_depth): ?>
      <div class="reply"><span class="reply-<?php print $comment_depth; ?>"></span></div>
      <?php endif; ?>

      <?php if (! empty($author_thumbnail)): ?>
        <?php print $author_thumbnail; ?>
      <?php endif; ?>

      <p class="commenter-name">
        <?php echo $author; ?>
      </p>
      <p class="comment-time">
        <?php print $created; ?>
      </p>
    </div>

    <div class="comment-extra">
      <?php if ($comment_recommended): ?>
        <?php $title = t('Recommended by the moderator for demonstrating effective commenting skills.'); ?>
        <p class="recommended">
          <span class="recommended-icon" title="<?php echo $title; ?>"></span>
        </p>
      <?php endif; ?>
      <p class="comment-number"></p>
    </div>

  </div>

  <div class="comment-text">


      <div class="endorsement-counter">
        <span>
        <?php if (!empty($endorsement_count)): ?>
          <?php print $endorsement_count; ?>
        <?php endif; ?>
        </span>
      </div>

    
    <div class="comment-arrow"></div>

    <div class="content truncate"<?php print $content_attributes; ?>>
      <?php
        // We hide the links now so that we can render them later.
        hide($content['links']);
        print render($content);
      ?>
    </div> <!-- /.content -->



    <?php if (!empty($endorsements)) { ?>
      <div class="endorsement-users" style="display:none">
        <ol>
        <?php
        $user_helper = new \SmartParticipation\UserHelper;
        /** @var \SmartParticipation\model\Endorsement $endorsement */
        foreach ($endorsements as $endorsement) {
          echo '<li>';
          echo $user_helper->getUserProfileLink($endorsement->user, array('target'=>'_blank'))
            . ' '
            . '<span class="endorsement-time">' . format_date($endorsement->timestamp) . '</span>';
          echo '</li>';
        }
        ?>
        </ol>
      </div>
    <?php } ?>

  </div> <!-- /.comment-text -->

  <?php print render($content['links']); ?>
  
  <?php if ($is_moderator): 
   /* TODO This needs to be moderator-specific. Try this approach: create a new
    * template comment_moderator.tpl.php that will include this, and also 
    * invoke the current template using theme('comment', $comment).
    * 
    * I believe for now we are not supporting the email note function, so that
    * can be commented out anyway.
    */
  ?>
    <div class='comment_mod_input_div'>
      <div class='input_header'>
          <span class='input_header_content'></span>
      </div>
  
      <!-- Details on emailing the note 
        Currently unsupported.  -->
      <!-- <div class='send_note_p'>
          <br/><span class='align_right'><input class='note_to' type='text' name='note_to'> *</span>To
          <br/><span class='align_right'><input class='note_subject' type='text' name='note_subject'> *</span>Subject
      </div> -->

      <textarea class="input_area"></textarea> 
      <div class='input_lower_span'>
        <span class='input_mod_error_span'></span>
        <button class='input_submit'>Submit</button>  or  <a class='input_cancel_link'>Cancel</a>
      </div><br/><br/>
    </div>
  <?php endif; ?>
</div>
