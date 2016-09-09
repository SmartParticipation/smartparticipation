/**
 * This custom plugin creates a jQuery UI Dialog with SmartParticipation-specific options.
 */

(function(Self, $) {

  $.fn.rrDialog = function (options) {

    var $dlg = this;

    options = $.extend({
      autoOpen:true,
      width:500,
      height:"auto",
      draggable:false,
      modal:true,
      resizable:false,
      autoResize:true,
      //position:"center center",
      close: function () {
        var $dlg = $(this),
            $form = $dlg.find('form');
        if ($form.length) {
          SP.Form.clearFormErrorsAndValues($form);
        }
        SP.logEvent({
          'name': 'close dialog',
          'detail': 'dialog: ' + $dlg.attr('id')
        });
        SP.Topic.TopicState.deleteStoredState();
      },
      open: function () {
        if (SP.Topic !== undefined) {
          SP.Topic.TopicState.setStoredState();
        }
        // Close the mobile menu if it's open.
        if (SP.MobileMenu.isVisible()) {
          SP.MobileMenu.close();
        }
      }
    }, options);

    $(window).resize(function () {
      //$dlg.dialog("option", "position", options.position);
      $dlg.dialog("option", "width", updateWidth($dlg.data('originalWidth')));
    });

    $dlg.data('originalWidth', options.width);
    options.width = updateWidth(options.width);

    function updateWidth(width) {
      // Use an 'auto' dialog width if the dialog width is less than the active breakpoint (for small devices).
      if (SP.Breakpoint.breakpoints[SP.Breakpoint.getActiveBreakpoint()].minWidth <= width) {
        width = 'auto';
      } else {
        if (width == 'auto') {
          width = $dlg.data('originalWidth');
        }
      }
      return width;
    }

    return $dlg.dialog(options);
  };

  // Is there an open dialog on the page?
  Self.dialogOpen = function () {
    var $dialog = $('.ui-dialog');
    return $dialog.length ? $dialog.is(':visible') : false;
  };

})(SP.Dialog = {}, SP.jQuery);