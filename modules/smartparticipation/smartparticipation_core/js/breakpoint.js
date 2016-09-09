/**
 * For responsive functionality - add class to the body that indicates the current device screen width.
 */

(function (Self, $) {

  /**
   * min-widths based on bootstrap 2.3 default breakpoints:
   *
   * iPhone Retina - 320px
   *
   * extra small device - 480px
   *
   * small device - 767px
   *
   * medium device - 979px
   *
   * large devices - 1200px
   */

  Self.breakpoints = {
    lgDevice: {className: 'lg-device', minWidth: 1200},
    mdDevice: {className: 'md-device', minWidth: 979},
    smDevice: {className: 'sm-device', minWidth: 767},
    xsDevice: {className: 'xs-device', minWidth: 480},
    xxsDevice: {className: 'xxs-device', minWidth: 320}
  };

  Self._activeBreakpoint = '';

  Self.init = function () {
    Self.applyBodyClass();
  };

  Self.applyBodyClass = function () {

    var classToAdd,
      breakpoints = Self.breakpoints,
      mq = function (minWidth) {
        return Modernizr.mq('only screen and (min-width: ' + minWidth + 'px)');
      },
      $body = $('body');

    $.each(breakpoints, function (i, v) {
      if (mq(v.minWidth)) {
        classToAdd = v.className;
        Self.setActiveBreakpoint(i);
        return false;
      }
    });

    if (classToAdd !== undefined && !$body.hasClass(classToAdd)) {
      $.each(breakpoints, function (i, v) {
        $body.removeClass(v.className);
      });
      $body.addClass(classToAdd);
    }

  };

  Self.isDesktop = function () {
    switch (this.getActiveBreakpoint()) {
      case 'lgDevice':
      case 'mdDevice':
        return true;
        break;
      default:
        return false;
    }
  };

  Self.isTablet = function () {
    return this.getActiveBreakpoint() == 'smDevice' ? true : false;
  };

  Self.isPhone = function () {
    switch (this.getActiveBreakpoint()) {
      case 'xsDevice':
      case 'xxsDevice':
        return true;
        break;
      default:
        return false;
    }
  };

  Self.getActiveBreakpoint = function () {
    return this._activeBreakpoint;
  };

  Self.setActiveBreakpoint = function (breakpointKey) {
    this._activeBreakpoint = breakpointKey;
  };

})(SP.Breakpoint = {}, SP.jQuery);