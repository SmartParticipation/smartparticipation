/**
 * Control the mobile menu provided by the Mobile sliding menu (mmenu) module.
 */

(function (Self, $) {

  var menuSelector = '#mmenu_left';

  Self.isVisible = function () {
    return $(menuSelector).is(':visible');
  };

  Self.close = function () {
    $(menuSelector).data("mmenu").close();
  };

})(SP.MobileMenu = {}, jQuery);