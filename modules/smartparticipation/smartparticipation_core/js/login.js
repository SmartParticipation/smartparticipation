/**
 * Script for login and register.
 */
(function (Self, $) {

  var dlgLogin = '#dlgLogin',
    dlgRegister = '#dlgRegister',
    dlgPassword = '#dlgPassword';

  function showLogin() {
    var $dlgLogin = $(dlgLogin);

    $dlgLogin.rrDialog({width:450});

    // Autocomplete values overlap labels on some browsers.
    // This clears the labels if they are visible.
    setInterval(function () {
      $dlgLogin.find('input[type=text],input[type=password]').each(function () {
        if (!($('label[for="'+$(this).attr('id')+'"]').is(':hidden')) && $(this).val() !== "") {
          $('label[for="'+$(this).attr('id')+'"]').hide();
        }
      });
    }, 200);
  }

  function showRegister() {
    var dlgWidth = 600;
    // Adjust the register form/dialog if it is for a private discussion.
    if ($('.splash .login-form').length > 0) {
      $(dlgRegister).find('.error').removeClass('error').addClass('register-error');
      $(dlgRegister).find('.field-error').removeClass('field-error').addClass('register-field-error');
      dlgWidth = 450;
    }
    if (!($('.social-login').length > 0)) {
      $('#dlgRegister form').css("width",'527px').css("border-right",'0px');
    }
    $(dlgRegister).rrDialog({width:dlgWidth});
  }

  function showPassword() {
    $(dlgPassword).rrDialog({width:450});
  }

  function attachDialogClickHandlers ($container) {

    var settings = Drupal.settings.rrLogin,
      $dlgLogin = $(dlgLogin),
      $dlgRegister = $(dlgRegister),
      $dlgPassword = $(dlgPassword);

    if (settings) {

      if ($container === undefined) {
        $container = $('body');
      }

      $container.find("a[href*='" + settings.loginPath + "']").each(function () {
        $(this).click(function () {
          showLogin();
          return false;
        });
      });

      $container.find("a[href*='" + settings.registerPath + "']").each(function () {
        $(this).click(function () {
          // Close Lost Password? dialog if it's open.
          // This can happen when the register link is clicked from failed validation on the Lost Password? dialog.
          if ($dlgPassword.dialog("instance") && $dlgPassword.dialog('isOpen')) {
            $dlgPassword.dialog('close');
          }
          // Restore the register form to its original state when re-opening the register form
          // after a email not found error has occurred.
          var $privateRegister = $('.private-discussion', dlgRegister);
          if ($privateRegister.length > 0) {
            $privateRegister
              .removeClass('email-not-found')
              .find('.register-field-error')
              .remove();
          }
          showRegister();
          return false;
        });
      });

      $container.find("a[href*='" + settings.passwordPath + "']").each(function () {
        $(this).click(function () {
          // Close Register dialog if it's open.
          if ($dlgRegister.dialog("instance") && $dlgRegister.dialog('isOpen')) {
            $dlgRegister.dialog('close');
          }
          // Close Login dialog if it's open.
          if ($dlgLogin.dialog("instance") && $dlgLogin.dialog('isOpen')) {
            $dlgLogin.dialog('close');
          }
          showPassword();
          return false;
        });
      });

    }

  }

  Self.attachDialogClickHandlers = function ($container) {

    attachDialogClickHandlers($container);

  };

  Self.isLoggedIn = function () {

    return ! $('body').hasClass('not-logged-in');

  };

  Drupal.behaviors.rrLogin =
  {
    attach:function () {
      var settings = Drupal.settings.rrLogin,
        $formLogin = $('#user-login'),
        $formRegister = $('#user-register-form'),
        $formPassword = $('#user-pass'),
        hash = window.location.hash;

      SP.Breakpoint.applyBodyClass();

      /*
       Since the Log in, Registration, and Lost password forms
       contain fields with matching names and all forms are being
       rendered on the page to support the dialogs, an error on
       one form could trigger an error on the others.  This
       function is used to remove any errors that may be contained
       on the other form dialogs.

       Update -- now using SP.Form.clearFormErrors
       */
//      function removeFormErrors($form) {
//        $form.find('.error').removeClass('error');
//        $form.find('.field-error').remove();
//      }

      // Set the form action to remain on current page after login or register.
      if (settings && settings.destination) {
        $formLogin.attr("action", settings.destination);
        $formRegister.attr("action", settings.destination);
      }

      // Automatically open the dialog if an error occurred.
      if (settings && settings.submitted == 'user_login') {
        if ($formLogin.find('.error').length) {
          SP.Form.clearFormErrors($formRegister);
          SP.Form.clearFormErrors($formPassword);
          showLogin();
        }
      } else if (settings && settings.submitted == 'user_register_form') {
        if ($formRegister.find('.error').length) {
          SP.Form.clearFormErrors($formLogin);
          SP.Form.clearFormErrors($formPassword);
          showRegister();
        }
      } else if (settings && settings.submitted == 'user_pass') {
        if ($formPassword.find('.error').length) {
          SP.Form.clearFormErrors($formLogin);
          SP.Form.clearFormErrors($formRegister);
          showPassword();
        }
      // Show login or register if URL hash is provided.
      } else if (hash) {
        if (hash == '#login') {
          showLogin();
        } else if (hash == '#register') {
          showRegister();
        } else if (hash == '#password') {
          showPassword();
        }
      }

      attachDialogClickHandlers();

      // If this is a private discussion splash page with a login form:
      // 1) Clear labels for autocomplete and set focus for splash page login form
      // 2) Attach the event logger to the registration link
      var $splashLogin = $('.splash .login-form'),
        $registerLink = $splashLogin.find('.register a');
      if ($splashLogin.length > 0) {
        var usernameInput = '#edit-name',
          $usernameInput = $(usernameInput),
          usernameLength = $usernameInput.val().length * 2,
          passwordInput = '#edit-pass';
        // Autocomplete values overlap labels on some browsers.
        // This clears the labels if they are visible.
        setInterval(function () {
          if ($usernameInput.val().length > 0) {
            $('label[for="'+$usernameInput.attr('id')+'"]').hide();
          }
          if ($(passwordInput).val().length > 0 || $(passwordInput+':-webkit-autofill').length > 0) {
            $('label[for="'+$(passwordInput).attr('id')+'"]').hide();
          }
        }, 200);
        // Set focus on the username input
        $usernameInput.focus();
        $usernameInput[0].setSelectionRange(usernameLength, usernameLength);
        // Log click events to the private discussion registration
        $registerLink.on('click', function () {
          SP.logEvent({
            'name': 'private register open dialog'
          });
        });
      }

    }
  };
})(SP.Login = {}, SP.jQuery);
