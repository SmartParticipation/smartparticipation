/**
 * Script for slide-down alerts.
 */

(function (Self, $) {

  function appendAlert (id, message, type, allowClose) {
    var settings = Drupal.settings.rrAlert,
      $template = $(settings.template),
      $closeButton = $template.find('.close').clone();

    allowClose = typeof allowClose !== 'undefined' ? allowClose : true;

    $template.attr('id', id);
    $template.find('.message').html(message);
    if (allowClose) {
      $template.find('.message').prepend($closeButton);
    }
    $template.appendTo('body');
    $('#'+id).find('button.close').click(function () {
      Self.close($(this));

      if (type !== undefined && type) {
        switch(type) {
          case 'comment-tips-button':
            SP.logEvent({
              'name': 'close comment tips'
            });
            break;
          case 'comment-tips-alert':
            SP.logEvent({
              'name': 'close comment tips prompt'
            });
            break;
        }
      }

    });
  }

  Self.open = function (id, message, type, allowClose) {
    var $openAlert = $('.alert:not(:hidden)'),
      $alert = $('#'+id);

    // Append the alert if it's not already there.
    if (!$alert.length) {
      appendAlert(id, message, type, allowClose);
      $alert = $('#'+id);
    }

    if ($openAlert.length) {
      // An alert is already open so slide it closed fast then replace it with the new one.
      $openAlert.slideUp('fast', function () {
        // If the same alert isn't being reopened, remove the already open alert.
        if (!$(this).is('#'+id)) {
          $(this).remove();
        }
        $('#'+id).slideDown('slow');
      });
    } else {
      $('#'+id).slideDown('slow');
    }

    // Handle specific alert types if provided.
    if (type !== undefined && type) {
      switch (type) {
      
        case 'interest-survey':
          SP.InterestSurvey.attachDialogClickHandler($('#'+id));
          break;
        
        // A login alert contains login/registration links.
        case 'login':
          if (!$openAlert.length) {
            SP.Login.attachDialogClickHandlers($('#' + id));
          }
          break;

        case 'comment-tips-prompt':
          if (!$openAlert.length) {
            SP.Topic.Comment.CommentTips.attachAlertActionHandlers($alert);
          }
          break;
      }
    }

  };

  Self.close = function ($button) {
    $button.closest('.alert').remove();
  };

})(SP.Alert = {}, SP.jQuery);
