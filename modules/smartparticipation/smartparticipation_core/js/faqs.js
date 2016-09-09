/**
 * Script for faqs page.
 */

SP.jQuery(document).ready(function($) {

  if (!$('body').hasClass('page-faq')) {
    return;
  }

  var $block = $('#block-smartparticipation-core-submit-faq'),
    $button = $block.find('h2'),
    $form = $block.find('#smartparticipation-core-submit-faq-form'),
    $email = $block.find('#edit-email'),
    $question = $block.find('#edit-question');
  
  // Toggle question state and answer visibility
  $('.faq-question a').click(function() {
    $(this).find('.faq-question-icon').toggleClass('active');
    $(this).parent().nextAll('.faq-answer').slideToggle('fast');
    return false;
  });

  // Toggle the button state and form visibility
  $button.click(function () {
    $(this).toggleClass('open');
    $form.slideToggle('fast');
    // Restore form to its original state when closing
    if (! $(this).hasClass('open')) {
      SP.Form.clearFormErrorsAndValues($form);
    }
  });

  // If the form failed validation, show the form
  if ($email.hasClass('error') || $question.hasClass('error')) {
    $button.toggleClass('open');
    $form.show();
  }

});
