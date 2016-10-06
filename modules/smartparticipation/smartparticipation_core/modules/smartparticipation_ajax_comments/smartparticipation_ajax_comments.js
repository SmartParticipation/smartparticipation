(function ($) {

Drupal.behaviors.ajaxComments = {
  attach: function(context, settings) {

    // Responds to submission of new comment by the user.
    if ($(context).hasClass('ajax-comment-wrapper')) {
      // A comment has been successfully submitted.
      commentNumber = $(context).attr("id").split('-');

      if (context.parent().hasClass('comments-container')) {
        // Comment is a parent comment
        SP.Topic.Comment.afterComment(SP.jQuery('#'+context.attr('id')));
      } else {
        // Comment is a reply
        SP.Topic.Comment.afterReply(SP.jQuery('#'+context.attr('id')));
      }

      // The comment was added by a moderator.
      if (SP.Topic.Comment.userIsModerator()) {
        SP.Topic.Comment.afterModeratorCommment(SP.jQuery('#'+context.attr('id')));
      }

      SP.Topic.Scroll.scrollToNewComment(commentNumber[2]);

    } else if ($(context).hasClass('comment-form')) {
      // A form has been requested.
      if (context.parent().hasClass('comments-form-container')) {
        // A comment form is replaced after having been submitted.
        SP.Topic.Comment.afterCommentFormReplace(SP.jQuery('#'+context.attr('id')));
        if ($(context).find('textarea').hasClass('error')) {
          // Form is a new-thread comment box with an error.
          SP.Topic.Comment.afterError(SP.jQuery('#'+context.attr('id')));
        }
      } else if (context.parent().hasClass('ajax-comment-wrapper')) {
        if (context.hasClass('comment-form-reply')) {
          if ($(context).find('textarea').hasClass('error')) {
            // Form is a reply form with an error.
            SP.Topic.Comment.afterReplyError(SP.jQuery('#' + context.attr('id')));
          }
          else {
            // Form is a reply form without an error.
            SP.Topic.Comment.afterOpenReplyForm(SP.jQuery('#' + context.attr('id')));
          }
        } else if (context.hasClass('comment-form-edit')) {
          SP.Topic.Comment.afterOpenRedactForm(SP.jQuery('#' + context.attr('id')));
        }
      }
    } else if ($(context).hasClass('comment')) {
      // Submit or Cancel a redaction.
      SP.Topic.Comment.afterRedact(SP.jQuery('#'+context.closest('.ajax-comment-wrapper').attr('id')));
    }

    // Hide reply form if cancel is clicked.
    $("form.comment-form-reply a.ajax-comments-reply-cancel").click(function(e) {
      
      var commentForm = $(this).attr("href");

      SP.Topic.Comment.cancelReplyForm(SP.jQuery(commentForm));

      SP.Topic.Comment.enableCommentForm(SP.jQuery(commentForm).closest('.comment-wrapper').find('.comments-form-container form'));

      e.preventDefault();

    });

  }
};

}(jQuery));
