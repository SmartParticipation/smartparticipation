/**
 * Front end moderator controls
 */

SP.jQuery(document).ready(function () {
  SP.ModeratorControls.init();
});

(function (Self, $) {

  Self.init = function() { 
    Self.ModeratorControls.applyClickListeners($('.ajax-comment-wrapper'));
  };

  // TODO Do we need this object wrapping the functions? Why not just declare
  // Self.applyClickListeners, etc.?
  Self.ModeratorControls = {
  
    /** 
     * Apply the click listeners to all links in comments inside the domain 
     * element.
     *
     * @param domainElement
     * The element containing the comments
     */
    applyClickListeners: function(domainElement) {
      
      var commentLinks = [
        '.comment-in-progress a',
        '.comment-note a',
        //'.comment-redact a',
        '.comment-quarantine a',
        '.comment-no-reply a',
        '.comment-recommend a'];
      
      // TODO Eliminate this for loop
      for (var i = 0; i < commentLinks.length; i++) {
        
        domainElement.find(commentLinks[i]).click(function(e) {
          
          e.preventDefault();
  
          // TODO Hoist all variable declarations to top of each function
          var currentElement = $(e.target),
            $commentEl = '',
            $inputSubmit;
          
          // TODO Can we replace the while loop with .closest()?
          while (currentElement.parent()) {
            
            if (currentElement.attr('class') == 'ajax-comment-wrapper') {
              $commentEl = currentElement;
              break;
            } else {
              currentElement = currentElement.parent();
            }
          }

          $inputSubmit = $commentEl.find('.input_submit');
        
          var liClasses = $(e.target).parent().attr('class').split(' '),
              $loading = $('<img>', 
                         {
                           'class': 'loading_gif',
                           // TODO We cannot hard-code the theme!
                           src: '/profiles/smartparticipation/themes/smartparticipation_base/images/ajax_loader.gif',
                           style: 'display:block;'
                         });
  
          var $replies = $commentEl.find('.ajax-comment-wrapper');
          $replies.detach();
  
          // Add note action
          if ($.inArray('comment-note', liClasses) == 0) {
            var cid = $commentEl.attr('id').split('-').pop();
 
            $inputSubmit.html('Add note')                
                        .click(function() {
                          var $inputArea = $commentEl.find('textarea:first');
                          var commentDiv = '#comment-wrapper-' + cid;
                          var $errorDiv = $(commentDiv + ' .input_mod_error_span');
                          if (!$inputArea.val()) {
                            $errorDiv.html('Note is required.');
                            return;
                          }
                          $commentEl.find('.comment_mod_input_div:first').after($loading);
                          $.post($(e.target).attr('href'), { content: $commentEl.find('textarea').val() },function(result) {
                            Self.ModeratorControls.reconstructComment($commentEl, result, $replies);
                          },'json');
                        });
            
            $commentEl.find('textarea').val('');
            $commentEl.find('.input_header_content').html('Add note');
            // TODO Here and elsewhere: replace css('display', 'block') with show() (does that automatically apply block display?)
            $commentEl.find('.send_note_button').css('display', 'block');
  
            Self.ModeratorControls.showInputArea($commentEl, commentLinks);
            $commentEl.append($replies);
          }
        
          // Redact action
          else if ($.inArray('comment-redact', liClasses) == 0) {
            var cid = $commentEl.attr('id').split('-').pop(),
              redactVerb =
                $commentEl.find('div.comment').hasClass('moderator-comment') ?
                    'Edit' : 'Redact';
            
            $inputSubmit.html(redactVerb)
              .click(function() {
                var $inputArea = $commentEl.find('textarea:first');
                var commentDiv = '#comment-wrapper-' + cid;
                var $errorDiv = $(commentDiv + ' .input_mod_error_span');
                if (!$inputArea.val()) {
                  $errorDiv.html('Comment is required.');
                  return;
                }
                $commentEl.find('.comment_mod_input_div:first').after($loading);
                  $.post($(e.target).attr('href'), {content: $inputArea.val()}, function (result) {
                    Self.ModeratorControls.reconstructComment($commentEl, result, $replies);
                  }, 'json');
              });


            $commentEl.append($loading);
            // Request the comment data and use the comment body value in the redact input area.
            // The comment body value does not contain extra markup - it is the raw value from the database.
            $.getJSON(Drupal.settings.basePath + 'comment/' + cid + '/get', function (result) {

              $commentEl.find('textarea:first').val(result.comment_body);
              $commentEl.find('.input_header_content').html(redactVerb + ' comment:');
              $commentEl.find('.send_note_p').hide();

              Self.ModeratorControls.showInputArea($commentEl, commentLinks);

              $commentEl.find('.loading_gif').remove();

              $commentEl.append($replies);
            });

          } 
        
          // Single click Ajax behaviors and responses
          else {
            $commentEl.append($loading);
            $commentEl.append($replies);
            $.getJSON($(e.target).attr('href'),function(result) {

              if ($.inArray('comment-recommend', liClasses) == 0) {

                // Comment was recommended
                if ($commentEl.find('.comment').attr('data-recommended')==0) {
                  SP.Topic.Comment.updateRecommendedCount();

                // Comment was unrecommended
                } else {
                  SP.Topic.Comment.updateRecommendedCount('remove');
                }
              }

              Self.ModeratorControls.reconstructComment($commentEl, result, $replies);

            });
          }
          
        });
      }
  
      $('.input_cancel_link').click(function(e) {
        var $inputDiv = $('.comment_mod_input_div'),
          moderatorActionInput = $inputDiv.has(e.target),
          $inputSubmit = $inputDiv.find('.input_submit');

        // Unbind submit button click handler
        $inputSubmit.unbind();

        // Hide
        moderatorActionInput.hide();
        moderatorActionInput.find('.send_note_button').hide();
        moderatorActionInput.find('.input_mod_error_span').html('');
  
        // TODO Remove for loop
        /*for (var i = 0; i < commentLinks.length; i++) {
          $('.ajax-comment-wrapper').has($(e.target)).find(commentLinks[i]).css('display', 'block');
        }*/
        $('.ajax-comment-wrapper').find('ul.links').show();
  
      });
      
    },
  
    /**
     * Reconstructs comment after an ajax process has completed and produced a result.
     *
     * @param comment_el
     * The comment element that is being reconstructed.
     *
     * @param result
     * The result from the ajax call.
     *
     * @param replies
     * Any replies to a comment coming afterwards.
     */
     reconstructComment: function($comment_el, result, $replies){
       // Store the original reply button with event handlers.
       var replyButton = 'a#reply-'+$comment_el.attr('id').split('-').pop(),
         $replyButton = $(replyButton).detach();

       $comment_el.html($(result['html']).html());

       $comment_el.append($replies);
       Self.ModeratorControls.applyClickListeners($comment_el);
       Self.ModeratorControls.renumberComments($('.subtopic_body').has($comment_el).attr('id'));
       // Replace the new reply button with the original reply button.
       $(replyButton).replaceWith($replyButton);
       $(replyButton).show();
     },
  
    /**
     * Show the input area for adding notes and redacting
     * 
     * @param commentEl
     * The comment element where the input area is to be shown.
     *
     * @param commentLinks
     * The links to be hidden when showing the input area.
     */
    showInputArea: function(commentEl,commentLinks) {
      
      commentEl.find('.comment_mod_input_div').css('display', 'block');
      commentEl.find('textarea').show();
      commentEl.find('.input_header').css('display', 'block');
  
      commentLinks.push('.comment-reply a');
      // TODO Remove the for loop: can't we just do commentLinks.hide()?
      /*for (var i = 0; i < commentLinks.length; i++) {
        commentEl.find(commentLinks[i]).hide();
      }*/
      commentEl.find('ul.links').hide();
    },
  
    /**
     * Renumber all comments within the specified subtopic body
     */
    renumberComments: function (subtopicBodyId) {
      
      $('#' + subtopicBodyId + ' .comment-number').each(function(index) {
      
        if ($(this).text() == '') {
          $(this).html(index + 1);
        }
        
      });
    }
    
  };


})(SP.ModeratorControls = {}, SP.jQuery);


  