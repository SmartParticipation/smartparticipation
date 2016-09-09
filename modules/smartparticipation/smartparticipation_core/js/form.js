/**
 * Script for forms.
 */

(function (Self, $) {

  Self.init = function() {

    // add asterisk
    $('form:not(.webform-client-form) .form-required').each(function () {
      $(this).closest('.form-item').prepend($('<abbr/>').addClass('required').append('<span>*</span>'));
    });

    // initialize inFieldLabels
    $('.form-type-textfield label, .form-type-password label, .form-type-textarea label, .form-type-emailfield label')
      .not('#smartparticipation-core-profile-form .form-type-textarea label, #smartparticipation-core-profile-form .form-type-textfield label')
      .inFieldLabels();

  };

  Self.clearFormErrorsAndValues = function($form) {
    this.clearFormErrors($form);
    this.clearFormValues($form);
  };

  Self.clearFormErrors = function($form) {
    // Clear validation errors set by Drupal
    $form.find('.error').removeClass('error');
    $form.find('.field-error').remove();
  };

  Self.clearCommentErrorAndValue = function ($form) {
    this.clearFormErrors($form);
    this.clearCommentValue($form);
  };

  Self.clearCommentValue = function ($form) {
    var instanceKey = $form.find('textarea').attr('id');
    CKEDITOR.instances[instanceKey].setData('');
  };

  Self.clearFormValues = function($form) {
    // Clear text entry fields.  The blur() is necessary to make the infield labels visible.
    $form.find(':input:not([type="submit"],[type="hidden"],[type="checkbox"],[type="radio"])').val("").blur();
    // Uncheck checkboxes and radios.
    $form.find('input[type="checkbox"],input[type="radio"]').prop("checked",false);

    // Clearing an auto-fill password field with .val("").blur() does not restore the label.
    // This hack will ensure the password field label visibility is restored.
    $form.find(':password').each(function () {
      $('label[for="'+$(this).attr('id')+'"]').show();
    });

    this.hideDependentComponents($form);
  };
  
  // Webform_conditionals JavaScript does not hide dependent components when
  // the form is closed. If we don't do that, then they show when the form is 
  // reopened.
  Self.hideDependentComponents = function($form) {

    $.each(Drupal.settings, function(key, info) {
      if (key.substring(0, 20) == 'webform_conditional_') {
        $.each(info.fields, function(triggerField_key, triggerField_info) {
          $.each(triggerField_info.dependent_fields, function() {
            $('#' + this.css_id).hide();
          })
        });
      return; 
      }
    });

  };

})(SP.Form = {}, SP.jQuery);
