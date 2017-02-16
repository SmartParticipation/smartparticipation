
SP.jQuery(document).ready(function () {
 SP.Profile.init();
});

(function(Self, $) {

  Self.init = function() {

    // Show the profile reminder dialog if present in markup
    var $dlgProfileReminder = $('#dlgProfileReminder');
    if ($dlgProfileReminder.length > 0) {
      // Log clicks on the profile form link
      $dlgProfileReminder.find('a').on('click', function () {
        SP.logEvent({
          'name': 'profile reminder clicked form link'
        });
      });
      // Show the dialog
      $dlgProfileReminder.rrDialog();
    }

  };

})(SP.Profile = {}, SP.jQuery);
