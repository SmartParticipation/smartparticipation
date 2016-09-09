(function ($) {

Drupal.behaviors.ajaxComments = {
  attach: function(context, settings) {

    // Responds to submission of new comment by the user.
    if ($(context).hasClass('ajax-comment-wrapper')) {
      // A comment has been successfully submitted.
      commentNumber = $(context).attr("id").split('-');

      if (context.parent().hasClass('comments-container')) {
        // Comment is a parent comment
        RR.Topic.Comment.afterComment(RR.jQuery('#'+context.attr('id')));
      } else {
        // Comment is a reply
        RR.Topic.Comment.afterReply(RR.jQuery('#'+context.attr('id')));
      }

      // The comment was added by a moderator.
      if (RR.Topic.Comment.userIsModerator()) {
        RR.Topic.Comment.afterModeratorCommment(RR.jQuery('#'+context.attr('id')));
      }

      RR.Topic.Scroll.scrollToNewComment(commentNumber[2]);

    } else if ($(context).hasClass('comment-form')) {
      // A form has been requested.
      if (context.parent().hasClass('comments-form-container')) {
        // A comment form is replaced after having been submitted.
        RR.Topic.Comment.afterCommentFormReplace(RR.jQuery('#'+context.attr('id')));
        if ($(context).find('textarea').hasClass('error')) {
          // Form is a new-thread comment box with an error.
          RR.Topic.Comment.afterError(RR.jQuery('#'+context.attr('id')));
        }
      } else if (context.parent().hasClass('ajax-comment-wrapper')) {
        if (context.hasClass('comment-form-reply')) {
          if ($(context).find('textarea').hasClass('error')) {
            // Form is a reply form with an error.
            RR.Topic.Comment.afterReplyError(RR.jQuery('#' + context.attr('id')));
          }
          else {
            // Form is a reply form without an error.
            RR.Topic.Comment.afterOpenReplyForm(RR.jQuery('#' + context.attr('id')));
          }
        } else if (context.hasClass('comment-form-edit')) {
          RR.Topic.Comment.afterOpenRedactForm(RR.jQuery('#' + context.attr('id')));
        }
      }
    } else if ($(context).hasClass('comment')) {
      // Submit or Cancel a redaction.
      RR.Topic.Comment.afterRedact(RR.jQuery('#'+context.closest('.ajax-comment-wrapper').attr('id')));
    }

    // Hide reply form if cancel is clicked.
    $("form.comment-form-reply a.ajax-comments-reply-cancel").click(function(e) {
      
      var commentForm = $(this).attr("href");

      RR.Topic.Comment.cancelReplyForm(RR.jQuery(commentForm));

      RR.Topic.Comment.enableCommentForm(RR.jQuery(commentForm).closest('.comment-wrapper').find('.comments-form-container form'));

      e.preventDefault();

    });

  }
};

}(jQuery));
