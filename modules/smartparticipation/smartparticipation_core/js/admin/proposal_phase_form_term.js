/**
 * Script for proposal phase term form.
 */

SP.jQuery(document).ready(function($) {

  var $nameField = $('#edit-name'),
      $headingField = $('#edit-field-rr-phase-text-heading-und-0-value');
  
  $nameField.blur(function() {
    if ($nameField.val() && !$headingField.val()) {
      $headingField.val($nameField.val());
    }
  });
  
  // Hack to remove format selector, so users are forced to enter plain text
  // only. See notes in smartparticipation_core_proposal_phase_vocabulary_form_term_alter().
  $('#edit-description-format').hide();

});
