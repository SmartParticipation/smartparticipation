/**
 * Script for surveys.
 */

(function (Self, $) {
  
  var $messages = $('#messages'),
      messages;
  
  Self.init = function() {
    //Interest survey should display when the page loads
    Self.showInterestSurvey();
  };
  
  Self.showInterestSurvey = function() {
    // Open the survey in a dialog.
    $('#dlgInterestSurvey').rrDialog({
      width: 650//,
      // Position at top to avoid a bug in Chrome in which the dialog doesn't
      // close the first time the close button is clicked, when dependent form
      // fields are displayed. See http://bugs.jqueryui.com/ticket/8789. The
      // solution may be to upgrade to version 10 of jQuery UI.
      //position: "top",
      //dialogClass: "no-close"
    });    
  };

  Self.attachDialogClickHandler = function($container) {
    // Attach click handler to link in slider alert.
    $container.find('a').click(function() {
      Self.showInterestSurvey();
      return false;    
    });
  };

  if ($messages.length) {
    // Hide Drupal messages from body of page.
    $messages.hide();

    messages = $messages.html();

    // Place the Drupal messages from the page into the dialog.
    $('#dlgInterestSurvey .content').prepend(messages);
  }

 
})(SP.InterestSurvey = {}, SP.jQuery);

  
/*
SP.jQuery(document).ready(function() {
  SP.InterestSurvey.init();
});*/
