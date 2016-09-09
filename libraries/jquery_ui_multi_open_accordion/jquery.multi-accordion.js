/*
 * jQuery UI Multi Open Accordion Plugin
 * Derived from https://code.google.com/p/jquery-multi-open-accordion/
 *
 * Original Author : Anas Nakawa (http://anasnakawa.wordpress.com/)
 * Original Date   : 22-Jul-2011
 * Released Under MIT License
 */
(function ($) {

  $.widget('ui.multiAccordion', {
    options:{
      active:'none',
      showAll:null,
      hideAll:null,
      header: 'h3',
      body: 'div',
      _classes:{
        accordion:'ui-accordion ui-widget ui-helper-reset ui-accordion-icons',
        header:'ui-accordion-header ui-helper-reset ui-state-default ui-corner-all',
        body:'ui-accordion-content ui-helper-reset ui-widget-content ui-corner-bottom',
        bodyActive:'ui-accordion-content-active',
        span:'ui-icon ui-icon-triangle-1-e',
        stateDefault:'ui-state-default',
        stateHover:'ui-state-hover'
      }
    },

    _create:function () {
      var self = this,
        options = self.options,
        $this = self.element,
        $header = $this.children(options.header),
        $body = $this.children(options.body);

      $this.addClass(options._classes.accordion);

      $header.each(function (index) {
        var $this = $(this);
        $this.addClass(options._classes.header).prepend('<span class="{class}"></span>'.replace(/{class}/, options._classes.span));
        if (self._isActive(index)) {
          self._showTab($this)
        }
      }); // end header each

      $this.children(options.body).each(function (index) {
        var $this = $(this);
        $this.addClass(options._classes.body);
      }); // end each

      $header.bind('click', function (e) {
        // preventing on click to navigate to the top of document
        e.preventDefault();
        var $this = $(this);
        var ui = {
          tab:$this,
          content:$this.next(options.body)
        };
        self._trigger('click', null, ui);
        if ($this.hasClass(options._classes.stateDefault)) {
          self._showTab($this);
        }
        else {
          self._hideTab($this);
        }
      });


      $header.bind('mouseover', function () {
        $(this).addClass(options._classes.stateHover);
      });

      $header.bind('mouseout', function () {
        $(this).removeClass(options._classes.stateHover);
      });

      // triggering initialized
      self._trigger('init', null, $this);

    },

    // destroying the whole multi open widget
    destroy:function () {
      var self = this;
      var $this = self.element;
      var $header = $this.children(options.header);
      var $body = $this.children(options.body);
      var options = self.options;
      $this.children(options.header).unbind('click mouseover mouseout');
      $this.removeClass(options._classes.accordion);
      $header.removeClass(options._classes.header).removeClass('ui-state-default ui-corner-all ui-state-active ui-corner-top').children('span').remove();
      $body.removeClass(options._classes.body + ' ' + options._classes.bodyActive).show();
    },

    // private helper method that used to show tabs
    _showTab:function ($this) {
      var self = this;
      var $span = $this.children('span.ui-icon');
      var $body = $this.next();
      var options = this.options;
      var ui = {
        tab:$this,
        content:$this.next(options.body)
      }
      $this.removeClass('ui-state-default ui-corner-all').addClass('ui-state-active ui-corner-top');
      $span.removeClass('ui-icon-triangle-1-e').addClass('ui-icon-triangle-1-s');
      $body.slideDown('fast', function () {
        $body.addClass(options._classes.bodyActive);
        self._trigger('tabShownComplete', null, ui);
      });
      this._trigger('tabShown', null, ui);
    },

    // private helper method that used to show tabs
    _hideTab:function ($this) {
      var self = this;
      var $span = $this.children('span.ui-icon');
      var $body = $this.next();
      var options = this.options;
      var ui = {
        tab:$this,
        content:$this.next(options.body)
      }
      
      $this.removeClass('ui-state-active ui-corner-top').addClass('ui-state-default ui-corner-all');
      $span.removeClass('ui-icon-triangle-1-s').addClass('ui-icon-triangle-1-e');
      this._trigger('tabHiddenStart', null, ui);
      $body.slideUp('fast', function () {
        $body.removeClass(options._classes.bodyActive);
        self._trigger('tabHiddenComplete', null, ui);
      });
	  this._trigger('tabHidden', null, ui);
    },

    // helper method to determine wether passed parameter is an index of an active tab or not
    _isActive:function (num) {
      var options = this.options;
      // if array
      if (typeof options.active == "boolean" && !options.active) {
        return false;
      }
      else {
        if (options.active.length != undefined) {
          for (var i = 0; i < options.active.length; i++) {
            if (options.active[i] == num) {
              return true;
            }
          }
        }
        else {
          return options.active == num;
        }
      }
      return false;
    },

    // return object contain currently opened tabs
//    _getActiveTabs:function () {
//      var $this = this.element;
//      var ui = [];
//      $this.children(options.body).each(function (index) {
//        var $content = $(this);
//        if ($content.is(':visible')) {
//          //ui = ui ? ui : [];
//          ui.push({
//            index:index,
//            tab:$content.prev(options.header),
//            content:$content
//          });
//        }
//      });
//      return (ui.length == 0 ? undefined : ui);
//    },

//    getActiveTabs:function () {
//      var el = this.element;
//      var tabs = [];
//      el.children(options.body).each(function (index) {
//        if ($(this).is(':visible')) {
//          tabs.push(index);
//        }
//      });
//      return (tabs.length == 0 ? [-1] : tabs);
//    },

    // setting array of active tabs
    _setActiveTabs:function (tabs) {
      var self = this;
      var $this = this.element;
      if (typeof tabs != 'undefined') {
        $this.children(self.options.body).each(function (index) {
          var $tab = $(this).prev(self.options.header);
          if ($.inArray(index,tabs) != -1) {
            self._showTab($tab);
          }
          // Don't fire close event on a closed tab
          else if (!$tab.hasClass(self.options._classes.stateDefault)) {
            self._hideTab($tab);
          }
        });
      }
    },

    // active option passed by plugin, this method will read it and convert it into array of tab indexes
    _generateTabsArrayFromOptions:function (tabOption) {
      var tabs = [];
      var self = this;
      var $this = self.element;
//      var size = $this.children($(self.options.header)).size();
      if ($.type(tabOption) === 'array') {
        return tabOption;
      }
      else {
        if ($.type(tabOption) === 'number') {
          return [tabOption];
        }
        else {
          if ($.type(tabOption) === 'string') {
            switch (tabOption.toLowerCase()) {
              case 'all':
                var size = $this.children(self.options.header).size();
                for (var n = 0; n < size; n++) {
                  tabs.push(n);
                }
                return tabs;
                break;
              case 'none':
                tabs = [-1];
                return tabs;
                break;
              default:
                return undefined;
                break;
            }
          }
        }
      }
    },

    // required method by jquery ui widget framework, used to provide the ability to pass options
    // currently only active option is used here, may grow in the future
    _setOption:function (option, value) {
      $.Widget.prototype._setOption.apply(this, arguments);
      var el = this.element;
      switch (option) {
        case 'active':
          this._setActiveTabs(this._generateTabsArrayFromOptions(value));
          break;
//        case 'getActiveTabs':
//          var el = this.element;
//          var tabs;
//          el.children(options.body).each(function (index) {
//            if ($(this).is(':visible')) {
//              tabs = tabs ? tabs : [];
//              tabs.push(index);
//            }
//          });
//          return (tabs.length == 0 ? [-1] : tabs);
//          break;
      }
    }

  });

  // helper array has object function
  // thanks to @Vinko Vrsalovic
  // http://stackoverflow.com/questions/143847/best-way-to-find-an-item-in-a-javascript-array
//  Array.prototype.hasObject = (!Array.indexOf ? function (o) {
//    var l = this.length + 1;
//    while (l -= 1) {
//      if (this[l - 1] === o) {
//        return true;
//      }
//    }
//    return false;
//  } : function (o) {
//    return (this.indexOf(o) !== -1);
//  }
//    );

})(jQuery);
