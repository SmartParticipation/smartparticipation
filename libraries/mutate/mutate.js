/*
 * jQuery Mutate
 * http://www.jqui.net/jquery-projects/jquery-mutate-official/
 */

mutate_event_stack = [
  {
    name:'width',
    handler:function (elem) {
      n = {el:elem}
      if (!jQuery(n.el).data('mutate-width')) {
        jQuery(n.el).data('mutate-width', jQuery(n.el).width());
      }
      if (jQuery(n.el).data('mutate-width') && jQuery(n.el).width() != jQuery(n.el).data('mutate-width')) {
        jQuery(n.el).data('mutate-width', jQuery(n.el).width());
        return true;
      }
      return false;
    }
  },
  {
    name:'height',
    handler:function (n) {
      element = n;
      if (jQuery(element).is(':visible')) {
        if (!jQuery(element).data('mutate-height')) {
          jQuery(element).data('mutate-height', jQuery(element).height());
        }
        if (jQuery(element).data('mutate-height') && jQuery(element).height() != jQuery(element).data('mutate-height')) {
          jQuery(element).data('mutate-height', jQuery(element).height());
          return true;
        }
      }
    }
  },
  {
    name:'top',
    handler:function (n) {
      if (!jQuery(n).data('mutate-top')) {
        jQuery(n).data('mutate-top', jQuery(n).css('top'));
      }

      if (jQuery(n).data('mutate-top') && jQuery(n).css('top') != jQuery(n).data('mutate-top')) {
        jQuery(n).data('mutate-top', jQuery(n).css('top'));
        return true;
      }
    }
  },
  {
    name:'bottom',
    handler:function (n) {
      if (!jQuery(n).data('mutate-bottom')) {
        jQuery(n).data('mutate-bottom', jQuery(n).css('bottom'));
      }

      if (jQuery(n).data('mutate-bottom') && jQuery(n).css('bottom') != jQuery(n).data('mutate-bottom')) {
        jQuery(n).data('mutate-bottom', jQuery(n).css('bottom'));
        return true;
      }
    }
  },
  {
    name:'right',
    handler:function (n) {
      if (!jQuery(n).data('mutate-right')) {
        jQuery(n).data('mutate-right', jQuery(n).css('right'));
      }

      if (jQuery(n).data('mutate-right') && jQuery(n).css('right') != jQuery(n).data('mutate-right')) {
        jQuery(n).data('mutate-right', jQuery(n).css('right'));
        return true;
      }
    }
  },
  {
    name:'left',
    handler:function (n) {
      if (!jQuery(n).data('mutate-left')) {
        jQuery(n).data('mutate-left', jQuery(n).css('left'));
      }

      if (jQuery(n).data('mutate-left') && jQuery(n).css('left') != jQuery(n).data('mutate-left')) {
        jQuery(n).data('mutate-left', jQuery(n).css('left'));
        return true;
      }
    }
  },
  {
    name:'hide',
    handler:function (n) {
      if (jQuery(n).is(':hidden')) {
        return true;
      }
    }
  },
  {
    name:'show',
    handler:function (n) {
      if (jQuery(n).is(':visible')) {
        return true;
      }
    }
  },
  {
    name:'scrollHeight',
    handler:function (n) {
      if (!jQuery(n).data('prev-scrollHeight')) {
        jQuery(n).data('prev-scrollHeight', jQuery(n)[0].scrollHeight);
      }

      if (jQuery(n).data('prev-scrollHeight') && jQuery(n)[0].scrollHeight != jQuery(n).data('prev-scrollHeight')) {
        jQuery(n).data('prev-scrollHeight', jQuery(n)[0].scrollHeight);
        return true;
      }
    }
  },
  {
    name:'scrollWidth',
    handler:function (n) {
      if (!jQuery(n).data('prev-scrollWidth')) {
        jQuery(n).data('prev-scrollWidth', jQuery(n)[0].scrollWidth);
      }

      if (jQuery(n).data('prev-scrollWidth') && jQuery(n)[0].scrollWidth != jQuery(n).data('prev-scrollWidth')) {
        jQuery(n).data('prev-scrollWidth', jQuery(n)[0].scrollWidth);
        return true;
      }
    }
  },
  {
    name:'scrollTop',
    handler:function (n) {
      if (!jQuery(n).data('prev-scrollTop')) {
        jQuery(n).data('prev-scrollTop', jQuery(n)[0].scrollTop());
      }

      if (jQuery(n).data('prev-scrollTop') && jQuery(n)[0].scrollTop() != jQuery(n).data('prev-scrollTop')) {
        jQuery(n).data('prev-scrollTop', jQuery(n)[0].scrollTop());
        return true;
      }
    }
  },
  {
    name:'scrollLeft',
    handler:function (n) {
      if (!jQuery(n).data('prev-scrollLeft')) {
        jQuery(n).data('prev-scrollLeft', jQuery(n)[0].scrollLeft());
      }

      if (jQuery(n).data('prev-scrollLeft') && jQuery(n)[0].scrollLeft() != jQuery(n).data('prev-scrollLeft')) {
        jQuery(n).data('prev-scrollLeft', jQuery(n)[0].scrollLeft());
        return true;
      }
    }
  }
];

;
(function ($) {
  mutate = {speed:1, event_stack:mutate_event_stack, stack:[], events:{}, add_event:function (evt) {
    mutate.events[evt.name] = evt.handler;
  }, add:function (event_name, selector, callback, false_callback) {
    mutate.stack[mutate.stack.length] = {event_name:event_name, selector:selector, callback:callback, false_callback:false_callback}
  }};
  function reset() {
    var parent = mutate;
    if (parent.event_stack != 'undefined' && parent.event_stack.length) {
      $.each(parent.event_stack, function (j, k) {
        mutate.add_event(k);
      });
    }
    parent.event_stack = [];
    $.each(parent.stack, function (i, n) {
      $(n.selector).each(function (a, b) {
        if (parent.events[n.event_name](b) === true) {
          if (n['callback']) {
            n.callback(b, n);
          }
        }
        else {
          if (n['false_callback']) {
            n.false_callback(b, n)
          }
        }
      })
    })
    setTimeout(reset, mutate.speed);
  }

  reset();
  $.fn.extend({mutate:function () {
    var event_name = false, callback = arguments[1], selector = this, false_callback = arguments[2] ? arguments[2] : function () {
    };
    if (arguments[0].toLowerCase() == 'extend') {
      mutate.add_event(callback);
      return this;
    }
    $.each($.trim(arguments[0]).split(' '), function (i, n) {
      event_name = n;
      mutate.add(event_name, selector, callback, false_callback);
    });
    return this;
  }});
})(jQuery);