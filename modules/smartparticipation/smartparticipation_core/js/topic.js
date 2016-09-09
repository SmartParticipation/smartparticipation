/**
 * Script for topic pages.
 */

/*SP.jQuery(document).ready(function () {
  SP.Topic.init();
});*/


(function (Self, $) {

  var subtopicHeader = 'div.subtopic_title',
    subtopicBody = 'div.subtopic_body',
    subtopicDetails = 'div.subtopic-details',
    subtopicText = 'div.subtopic_text',
    subtopicComments = 'div.subtopic_comments',
    commentsForm = 'form.comment-form',
    commentsFormContainer = 'div.comments-form-container',
    commentsContainer = 'div.comments-container',
    commentText = '.comment-text',
    commentButton = "[id^='edit-submit']",
    commentCancelButton = '.cancel-comment',
    commentAddLink = '.comment-add-link a',
    commentAddLinkSave = 'button.comment-add-link-save',
    commentAddLinkCancel = 'a.comment-add-link-cancel',
    commentAddLinkFields = 'div.comment-add-link-fields',
    commentTipsData = 'div.comment-tips-alert-data',
    commentTipsButton = 'a.comment-tips-button',
    endorsementUsers = '.endorsement-users',
    endorsementUsersDialog = '#dlgCommentEndorsementsUsers',
    endorseButton = '.comment-endorse a',
    showEndorsementUsersLink = 'a.show-users',
    replyButton = '.comment-reply a',
    recommendedFilter = 'a.recommended-filter',
    textareaWrapper = '.form-textarea-wrapper',
    expandAllLink = '.expand-all a',
    collapseAllLink = '.collapse-all a',
    parser = new UAParser(),
    os = parser.getOS(),
    browser = parser.getBrowser(),
    isWin81 = false,
    isIE11 = false,
    isWinSafari = false,
    isiOS = false,
    isWin10 = false,
    isEdge = false,
    dialogOpenOnLoad,
    isLoggedIn = $('body').hasClass('logged-in'),
    lastActiveBreakpoint = SP.Breakpoint.getActiveBreakpoint(),
    resizeLastActiveBreakpoint;

  if (os.name == 'Windows') {
    if (os.version == '8.1') {
      isWin81 = true;
    }
    if (os.version == '10') {
      isWin10 = true;
    }
    
    if (browser.name == 'IE') {
      if (browser.version == '11.0') {
        isIE11 = true;
      }
    } else if (browser.name == 'Edge') {
      isEdge = true;
    }
    
  } else if (os.name == 'iOS') {
    isiOS = true;
  }

  Self.init = function () {

    if (!Self.isTopicPage()) {
      return;
    }

    //$dialog = $('.ui-dialog');
    //dialogOpen = $dialog.length ? $dialog.is(':visible') : false;
    dialogOpenOnLoad = SP.Dialog.dialogOpen();

    Self.Subtopic.init();

    Self.Comment.init();

    Self.TopicHeaderMenu.init();

    window.onbeforeunload = function() {
      var commentTextFound = Self.Comment.commentTextNotSubmitted();
      if (commentTextFound) {
        Self.Comment.logUnsubmittedCommentText();
        return 'It looks like you\'ve entered text in a comment box but didn\'t submit it.';
      }
      //return null;
    };

    // Allow linking (opening, scrolling) to subtopics on the same topic page.
    $(window).on('hashchange', function(event) {

      var subtopic, active, $accordion = $(subtopicDetails);

      if (!Self.TopicState.getHashUpdated()) {
        if (subtopic = Self.Scroll.checkSubtopicHash()) {
          $accordion.multiAccordion('option','active','none');
          active = Self.Scroll.processActiveSubtopic(subtopic);
          // Scrolling is broken when opening multiple subtopics.
          $accordion.multiAccordion('option','active',active);
        }
      }

      Self.TopicState.setHashUpdated(false);

    });

  };

  Self.isTopicPage = function () {
    return $('body').hasClass('node-type-sp-topic');
  };

  Self.TopicState = {

    _storageVarName: 'topicState',

    _hashUpdated: false,

    setStoredState: function () {
      var cvalue,
        expires = new Date();

      if (cvalue = Self.TopicState.getCurrent(true)) {
        // Expire 1 hour from now.
        expires.setTime(expires.getTime()+(60*60*1000));
        document.cookie = Self.TopicState._storageVarName+'=' + JSON.stringify(cvalue) + "; expires=" + expires.toGMTString();
      }

      return false;
    },

    getStoredState: function () {
      var cdata,
        data;

      cdata = document.cookie.split(';');

      $.each(cdata,function (i, cvalue) {
        cvalue = cvalue.split('=');
        if (cvalue[0] == Self.TopicState._storageVarName) {
          data = cvalue[1];
          return;
        }
      });

      if (data) {
        if (!SP.Dialog.dialogOpen() || isLoggedIn) {
          Self.TopicState.deleteStoredState();
        }
        return $.parseJSON(data);
      }

      return false;
    },

    // Remove the stored data
    deleteStoredState: function () {
      var expires = new Date();
      // A time in the past
      expires.setTime(expires.getTime()-1);
      document.cookie = Self.TopicState._storageVarName+'=; expires='+expires.toGMTString();
    },

    getCurrent: function (setStoredState) {
      var $openSubtopicHeaders = $(subtopicHeader+'.ui-state-active'),
        subtopicNodeIds = new Array(),
        hash = window.location.hash;

      $openSubtopicHeaders.each(function () {
        subtopicNodeIds.push($(this).data('rr-event_entity-id'))
      });

      if (subtopicNodeIds.length) {
        if (subtopicNodeIds.length == $(subtopicHeader).length) {
          return 'all';
        }
        return subtopicNodeIds;

      } else if (setStoredState) {

        if (hash.substr(1,3) == 'all') {
          return 'all';

        } else if (hash.substr(1,4) == 'nid-') {
          return hash.split('-').slice(1);
        }

      }

      return false;
    },

    updateHash: function(value) {
      var subtopicNodeIds,
        hashValue = '';

      if (value === undefined) {
        if (subtopicNodeIds = Self.TopicState.getCurrent()) {
          if (subtopicNodeIds == 'all') {
            hashValue = 'all';
          } else if (subtopicNodeIds instanceof Array) {
            hashValue = 'nid';
            $.each(subtopicNodeIds, function (i,nid) {
              hashValue += '-' + nid;
            });
          }
        }
      } else {
        hashValue = value;
      }
      this.setHashUpdated(true);
      window.location.hash = hashValue;
    },

    getHashUpdated: function () {
      return this._hashUpdated;
    },

    setHashUpdated: function (value) {
      this._hashUpdated = value;
    }

  };

  Self.TopicHeaderMenu = {

    init: function () {

      var $menu = $('#topic-page');

      $menu.find('li').each(function () {

        var $link = $(this).find('a');

        if ($link.find('span.future-phase').length) {
          $link.click(function () {
            return Self.TopicHeaderMenu.alert($link, 'unavailable');
          });
        } else if ($link.attr('href').length <= 1) {
          $link.click(function () {
            // TODO See RR-2477. Need a different type for 'no-documents'
            return Self.TopicHeaderMenu.alert($link, 'no-topics');
          });
        }

      });

    },

    alert: function($link, type) {

      var id, 
          message, 
          idText = $link.text().toLowerCase().split(' ').join('-'), 
          linkText = $link.text();
      
      switch (type) {
        case 'no-documents':
          id = 'no-' + idText + '-alert';
          message = "There are no " + linkText + " to display.";
          break;          
        case 'no-topics':
          id = 'no-' + idText + '-alert';
          message = "There are no " + linkText + " topics to display.";
          break;
        case "unavailable":
          id = idText + '-alert';
          message = "The " + linkText + " phase is currently unavailable.";
      }

      SP.Alert.open(id, message);

      return false;

    }


  };

    
  Self.Subtopic = {

    // Expand all subtopics action:
    // Will be set to true until all subtopics are expanded.
    _expandAllClicked: false,
    // Keeps track of the subtopics opened so far after clicking expand all.
    _expandAllOpenedCount: 0,

    _subtopicScrollSpeed: 1500, // milliseconds
    // Keeps track of the subtopics opened so far when opening multiple from a hash value.
    _scrollToMultipleOpenedCount: 0,

    windowHeight: null,

    windowWidth: null,

    init: function () {
      var $subtopicDetails = $(subtopicDetails),
        $commentsFormContainer = $(commentsFormContainer),
        $commentsContainer = $(commentsContainer),
        $expandAllLink = $(expandAllLink),
        $collapseAllLink = $(collapseAllLink),
        subtopic = Self.Scroll.checkSubtopicHash(),
        comment = Self.Scroll.checkCommentHash(),
        totalSubtopics = $(subtopicHeader).length,
        scrollDelay = null;

      /*$subtopicDetails.find(textareaWrapper).each(function () {
        Self.Subtopic.applyResizableTextareaBehavior($(this));
      });*/

      $subtopicDetails.multiAccordion({

        header:subtopicHeader,

        body:subtopicBody,

        init:function (event, ui) {
          $subtopicDetails.css('visibility','visible');
        },

        tabShownComplete:function (event, ui) {
          var $subtopicBody = ui.tab.next(),
            scrolledFromHash = false,
            subtopicToScrollTo = false;

          Self.Subtopic.checkContentHeight($subtopicBody);

          // If a hash location is provided, scroll to it unless it's already happened.
          if (!Self.Scroll.hasScrolled() && !SP.Dialog.dialogOpen()) {
            if (subtopic && !dialogOpenOnLoad) {
              scrolledFromHash = true;
              if (subtopic == 'all') {
                Self.Subtopic._scrollToMultipleOpenedCount++;
                // Wait for all subtopics to open before scrolling.
                if (Self.Subtopic._scrollToMultipleOpenedCount == totalSubtopics) {
                  subtopicToScrollTo = 1;
                }
              } else if (subtopic instanceof Array) {
                Self.Subtopic._scrollToMultipleOpenedCount++;
                // Wait for all given subtopics to open before scrolling.
                if (Self.Subtopic._scrollToMultipleOpenedCount == subtopic.length) {
                  subtopicToScrollTo = subtopic.sort()[0];
                }
              } else {
                subtopicToScrollTo = subtopic;
              }

              if (subtopicToScrollTo) {
                Self.Scroll.scrollToSubtopic(subtopicToScrollTo);
              }

            }

            if (comment) {
              scrolledFromHash = true;
              Self.Scroll.scrollToComment(comment);
            }
          }

          // If the subtopic wasn't opened/scrolled to on page load with a hash.
          if (!scrolledFromHash && !SP.Dialog.dialogOpen()) {
            // Scroll to the subtopic unless it's being opened because 'expand all' was clicked.
            if (!Self.Subtopic._expandAllClicked) {
              Self.Scroll.scrollToSubtopicAnimate($subtopicBody.prev(), Self.Subtopic._subtopicScrollSpeed);
              
            } else {
              Self.Subtopic._expandAllOpenedCount++;
              // Expand all was clicked, so wait until the last subtopic is expanded (don't scroll for each expanded subtopic).
              if (Self.Subtopic._expandAllOpenedCount == totalSubtopics) {
                Self.Subtopic._expandAllClicked = false;
                Self.Subtopic._expandAllOpenedCount = 0;
                // Scroll to the first subtopic
                Self.Scroll.scrollToSubtopicAnimate($(subtopicHeader+':first'), Self.Subtopic._subtopicScrollSpeed);
              }
            }
          }

          Self.Subtopic.checkViewPort();
 
        },

        tabHiddenStart:function (event, ui) {
          var $subtopicHeader = ui.tab,
            $subtopicBody = $subtopicHeader.next();

          // When closing a subtopic with a header title fixed to the screen top, adjust
          // the window scroll position so the closed subtopic header will remain in view
          // and positioned at the top of the screen.
          if ($subtopicHeader.css('position') == 'fixed') {
            Self.Scroll.scrollToElement($subtopicBody);
          }

        },

        tabHiddenComplete:function (event, ui) {
          var $subtopicHeader = ui.tab,
            $subtopicBody = $subtopicHeader.next();

          if ($subtopicHeader.css('position') != 'relative') {
            $subtopicHeader.css('position','relative');
          }

          if ($subtopicBody.find(subtopicComments).css('position', 'fixed')) {
            $subtopicBody.find(subtopicComments).css('position', 'static');
          }

          if ($subtopicHeader.data('topSpacer') != undefined) {
            $subtopicBody.css('padding-top',parseFloat($subtopicBody.css('padding-top')) - $subtopicHeader.outerHeight());
          }

          $(subtopicHeader).show();
          
          Self.Subtopic.checkViewPort();
          
        },

        tabHidden: function (event, ui) {
          var $subtopicHeader = ui.tab,
            $subtopicBody = $subtopicHeader.next(),
            $forms = $subtopicBody.find('form');

          // Clear values and errors from comment and reply forms.
          $forms.each(function (i,form) {
            var $form = $(form);
            SP.Form.clearCommentErrorAndValue($form);
          });     

          // See comments in scroll() event listener below.
          // ui.content.find(commentsContainer).data('scrollLogged', false); 
        }

      });

      if (subtopic || comment) {
        Self.Scroll.init($subtopicDetails, subtopic, comment);
      }
      
      $commentsContainer.scroll(function() {
        
        var $this = $(this);
        
        // Don't log when scrolling is the automatic result of a comment 
        // permalink request or submitting a new comment.
        if (Self.Scroll.getAutoScrollToComment()) {
          return;
        }
        
        /* RR-2504 Tried to make scroll logging more accurate by logging once
         * per opening the subtopic rather than once per subtopic per page view.
         * Encountered odd technical difficulties: in this implementation,
         * sometimes opening the subtopic fires the scroll event. Try this 
         * sequence: open the subtopic, scroll, close, reopen - second opening
         * fires the scroll event and it is incorrectly logged. Not worth 
         * further effort at this time.
        if ($this.data('scrollLogged')) {
          return;
        }
        
        $this.data('scrollLogged', true);
        */
        
        // Log only once per subtopic per page view
        $this.off('scroll');

        SP.logEvent({
          'name': 'scroll comments',
          'entity_type': 'node',
          'entity_id': $this.closest('.comment-wrapper').data('rr-event_entity-id')
        });
      });

      // Expand all subtopic tabs
      $expandAllLink.click(function() {
        Self.Subtopic._expandAllClicked = true;
        $subtopicDetails.multiAccordion('option','active','all');
        Self.TopicState.updateHash('all');
        SP.logEvent({
          'name': 'expand all subtopics'
        });
        return false;
      });

      $collapseAllLink.click(function() {      
        $subtopicDetails.multiAccordion('option','active','none');
        Self.TopicState.updateHash('');
        SP.logEvent({
          'name': 'collapse all subtopics'
        });
        return false;
      });
      
      // Open/close subtopic tab
      $(subtopicHeader).click(function() {
        
        var $this = $(this),
            eventName = ($this.hasClass('ui-state-active') ? 'open' : 'close') + ' subtopic';

        Self.TopicState.updateHash();
        
        SP.logEvent({
          'name': eventName,
          'entity_type': 'node',
          'entity_id': $this.data('rr-event_entity-id')
        });
      });

      Self.Subtopic.windowHeight = $(window).height();
      Self.Subtopic.windowWidth = $(window).width();


      // Scrolling can appear choppy in Windows - this helps smooth things
      if ((isWin81 && isIE11) || (isWin10 && isEdge)) {

        /*$('body').on('touchstart.owl', function () {
         console.log('touchmove',event);
         });*/

        $('body').on("mousewheel", function () {
          preventSmoothScroll(event, event.wheelDelta);
        });

        $(document).keydown(function () {
          switch (event.keyCode) {
            case 38:
              preventSmoothScroll(event, 120);
              break;
            case 40:
              preventSmoothScroll(event, -120);
              break;
          }
        });

        function preventSmoothScroll(event, delta) {
          event.preventDefault();
          var wd = delta;
          var csp = window.pageYOffset;
          window.scrollTo(0, csp - wd);
        }

      }

      $(window).scroll(Self.Subtopic.checkViewPort);

      $(window).resize(function () {
        var newWindowHeight = $(window).height(),
          newWindowWidth = $(window).width();

        if(Self.Subtopic.windowHeight!=newWindowHeight || Self.Subtopic.windowWidth!=newWindowWidth){
          Self.Subtopic.checkViewPort();
        }

        Self.Subtopic.windowHeight = newWindowHeight;
        Self.Subtopic.windowWidth = newWindowWidth;

        if (SP.Breakpoint.getActiveBreakpoint() != resizeLastActiveBreakpoint) {
          resizeLastActiveBreakpoint = SP.Breakpoint.getActiveBreakpoint();
          Self.Subtopic.destroyResizablePanelsBehavior();
          Self.Subtopic.applyResizablePanelsBehavior();
        }

      });

      resizeLastActiveBreakpoint = SP.Breakpoint.getActiveBreakpoint();
      Self.Subtopic.applyResizablePanelsBehavior();

      // Watch for changes in height to comment form divs.
      // Changes in height may occur if validation errors are
      // present or the user has changed the height of the textarea.
      $commentsFormContainer.mutate('height', function(){
        Self.Subtopic.checkViewPort();
      });
    },

    // Determine if the advanced scrolling should be disabled for the device.
    isSimpleScrollDevice: function () {
      return SP.Breakpoint.isPhone()
        || SP.Breakpoint.isTablet()
        || isiOS;
    },

    destroyResizablePanelsBehavior: function() {

      var $subtopicText = $(subtopicText);
      $subtopicText.each(function () {
        if ($(this).resizable("instance") !== undefined) {
          $(this).resizable("destroy");
          $(this).removeAttr("style");
        }
      });

    },

    applyResizablePanelsBehavior: function() {

      if ( ! SP.Breakpoint.isDesktop()) {
        return;
      }

      var $subtopicText = $(subtopicText),
        activeBreakpoint = SP.Breakpoint.getActiveBreakpoint(),
        activeLimits;

      var breakpointLimits = {
          lgDevice: { maxWidth: 800, minWidth: 300 },
          mdDevice: { maxWidth: 650, minWidth: 300 }
      };

      activeLimits = breakpointLimits[activeBreakpoint];

      $subtopicText.resizable({
        handles: 'e',
        maxWidth: activeLimits.maxWidth,
        minWidth: activeLimits.minWidth,
        resize: function (event, ui) {

          var currentWidth = ui.size.width,
            $subtopicText = $(this),
            $subtopicComments = $subtopicText.closest(subtopicBody).find(subtopicComments),
            containerWidth = $subtopicText.closest(subtopicBody).width(),
            subtopicCommentsPaddingMargin = $subtopicComments.outerWidth(true) - $subtopicComments.innerWidth(),
            subtopicTextPaddingMargin = $subtopicText.outerWidth(true) - $subtopicText.innerWidth(),
            totalPaddingMargin = subtopicCommentsPaddingMargin + subtopicTextPaddingMargin;

          $subtopicText.width(currentWidth);

          $subtopicComments.width(containerWidth - currentWidth - totalPaddingMargin);

          Self.Subtopic.checkViewPort();

        }
      });
    },

    applyResizableTextareaBehavior: function ($textareaWrapper, isReply) {

      return;

      var staticOffset = null;
      var $textarea = $textareaWrapper.addClass('resizable-textarea').find('textarea');
      var grippie = $('<div class="grippie"></div>').mousedown(startDrag);
      grippie.insertAfter($textarea);

      function getMaxTextAreaHeight($ta) {
        var $subtopicBody = $ta.closest(subtopicBody),
          $subtopicText = $subtopicBody.find(subtopicText),
          $commentFormContainer = $subtopicBody.find(commentsFormContainer),
          $commentFormHeader = $commentFormContainer.find('h3.title'),
          $subtopicComments = $subtopicBody.find(subtopicComments),
          $wrapper = $commentFormContainer.find('.form-type-textarea'),
          containerTextAreaHeightDifference =
            $commentFormContainer.height() - $wrapper.height() + $commentFormHeader.height(),
          maxTextAreaHeight;

        maxTextAreaHeight = $subtopicText.height() - containerTextAreaHeightDifference;

        if (isReply) {
          maxTextAreaHeight = $subtopicComments.height() - $commentFormContainer.height() - containerTextAreaHeightDifference;
        }

        return maxTextAreaHeight;
      }

      function startDrag(e) {
          staticOffset = $textarea.height() - e.pageY;
          $textarea.css('opacity', 0.25);
          $(document).mousemove(performDrag).mouseup(endDrag);
          return false;
      }
      function performDrag(e) {
          $textarea.height(Math.max(32, Math.min(staticOffset + e.pageY, getMaxTextAreaHeight($textarea))) + 'px');
          return false;
      }
      function endDrag(e) {
          $(document).unbind('mousemove', performDrag).unbind('mouseup', endDrag);
          $textarea.css('opacity', 1);
      }
    },

    // Adjust view of open subtopics based on position of window viewport.
    checkViewPort: function () {

      if (Self.Subtopic.isSimpleScrollDevice()) {
          // Prepare subtopics accordion for extra small device.
          $(commentsContainer).removeAttr('style');
          $(subtopicComments).removeAttr('style');
          $(subtopicText).removeAttr('style');
          $(subtopicBody).removeAttr('style');
          $(subtopicHeader).each(function () {
            if ($(this).hasClass('ui-state-active')) {
              $(this).next().css('display','block');
            }
          });
          //$(subtopicHeader).css('width','');
          $(subtopicHeader).removeAttr('style');
          return;
      }

      if (SP.Breakpoint.getActiveBreakpoint() != lastActiveBreakpoint) {
        $(subtopicHeader).removeAttr('style');
        $(subtopicComments).removeAttr('style');
        $(subtopicBody).css('padding-top','');
        lastActiveBreakpoint = SP.Breakpoint.getActiveBreakpoint();
        Self.Subtopic.checkViewPort();
      }

      $(subtopicText).each(function() {
        if ($(this).parent().css('display') == 'block') {

          var $element = $(this),
            $body = $element.parent(),
            bodyOffset = $body.offset(),
            $header = $body.prev(),
            headerOffset = $header.offset(),
            headerHeight = $header.outerHeight(),
            headerWidth = $header.width(),
            $comments = $element.next(),
            $commentsContainer = $comments.find(commentsContainer),
            $commentsFormContainer = $comments.find(commentsFormContainer),
            commentsFormContainerHeight = $commentsFormContainer.height(),
            offset = $element.offset(),
            $window = $(window),
            windowTop = $window.scrollTop(),
            windowLeft = $window.scrollLeft(),
            windowBottom = windowTop + $window.height(),
            commentsHeight,
            commentsWidth,
            commentsLeft,
            bodyMarginTop = $body.css('margin-top') == 'auto' ? '0' : $body.css('margin-top'),
            bodyTopSpacer =
              parseFloat($header.css('margin-bottom'))
              + parseFloat($body.css('padding-top'))
              + parseFloat(bodyMarginTop),
            topSpacer = headerHeight + bodyTopSpacer,
            $nextHeader = $(subtopicHeader).eq($(subtopicHeader).index($header) + 1),
            $subtopicDetails = $element.parent(),
            textMarginBottom = parseFloat($element.css('margin-bottom')),
            commentsBottom;

            // Show only subtopic headers that are in the viewport.

            if ($nextHeader.length) {
              if (headerOffset.top >= $nextHeader.offset().top) {
                $header.hide();
              } else {
                $header.show();
              }
            } else {
              if (headerOffset.top + headerHeight > offset.top + $element.height()) {
                $header.hide();
              } else {
                $header.show();
              }
            }

            // Attach subtopic headers when scrolling into the content of that subtopic.

            if ($header.css('position') != 'relative' && $header.data('originalOffsetTop') >= windowTop) {
              $header.css('position', 'relative');
              $header.css('left', 'auto');
              $body.css('padding-top', parseFloat($body.css('padding-top')) - headerHeight);
            }

            if ($header.css('position') != 'fixed' && headerOffset.top < windowTop) {
              $header.data('originalOffsetTop', headerOffset.top);
              $header.data('topSpacer', topSpacer);
              $header.css('position', 'fixed');
              $header.css('top', '0px');
              $header.width(headerWidth);
              $body.css('padding-top', parseFloat($body.css('padding-top')) + headerHeight);
            }

            if ($header.css('position') == 'fixed') {
              commentsWidth = $comments.width();
              $comments.width(commentsWidth);
              $comments.css('position', 'fixed');
              commentsLeft =
                bodyOffset.left
                + parseFloat($body.css('margin-left'))
                + $body.outerWidth()
                - (($body.outerWidth() - $body.width()) / 2)
                - $comments.outerWidth()
                - windowLeft;
              $comments.css('top', $header.data('topSpacer'));
              $comments.css('left', commentsLeft);
              $comments.css('right', '0px');

              // When the width of the browser is reduced to less than the content width
              // and the page is scrolled horizontally, prevent the subtopic header from drifting.
              if (windowLeft > 0) {
                $header.css('left', -windowLeft);
              } else {
                $header.css('left', 'auto');
              }

            } else {

              if (offset.top > windowTop) {
                if ($comments.css('position') == 'fixed') {
                  $comments.css('position', 'static');
                }
              }

              if (offset.top + $element.height() < windowBottom) {
                if ($header.data('topSpacer') != undefined) {
                  topSpacer = $header.data('topSpacer');
                }
              }
            }

            // Determine the height of the subtopic comments based on the viewport position.

            if ($(subtopicComments).length) {
              if ($nextHeader.length) {
                commentsBottom = $nextHeader.offset().top - textMarginBottom;
              } else {
                commentsBottom = $subtopicDetails.offset().top + $subtopicDetails.outerHeight();
              }

              if (commentsBottom > windowBottom) {
                commentsBottom = windowBottom;
              }

              commentsHeight = Math.round(commentsBottom - $comments.offset().top);

              if (commentsHeight < 0) {
                commentsHeight = 0;
              }

              $comments.height(commentsHeight);
              $commentsContainer.height(commentsHeight - commentsFormContainerHeight);
            }
            $body.data('lastWindowTop', windowTop);
        }
      });
    },

    // Set the height of an open subtopic based on various conditions.
    checkContentHeight: function ($subtopicBody) {

      if (!$subtopicBody.hasClass('subtopic_body')) {
        $subtopicBody = $(this).closest(subtopicBody);
      }

      var $content = $subtopicBody.find(subtopicText),
        contentHeight = $content.height(),
        commentsHeight,
        $commentsContainer = $subtopicBody.find(commentsContainer),
        $subtopicComments = $subtopicBody.find(subtopicComments),
        $subtopicText = $subtopicBody.find(subtopicText),
        bodySpacing = ($subtopicBody.outerHeight() - $subtopicBody.height())
          + ($subtopicText.outerHeight(true) - $subtopicText.height()),
        maxCommentsHeight = $(window).height()
          - ($(subtopicHeader+':first').outerHeight(true) * 2) - bodySpacing;

      $subtopicComments.height('auto');
      $commentsContainer.height('auto');
      commentsHeight = $subtopicBody.find('.comment-wrapper').height();

      if (contentHeight < commentsHeight) {
        if (commentsHeight > maxCommentsHeight) {
          if (contentHeight < maxCommentsHeight) {
            $content.height(maxCommentsHeight);
            $subtopicComments.height(maxCommentsHeight);
            $subtopicBody.height(maxCommentsHeight + parseFloat($subtopicText.css('margin-bottom')));
          }
        } else {
          $content.height(commentsHeight);
          $subtopicBody.height(commentsHeight + parseFloat($subtopicText.css('margin-bottom')));
        }
      }

      this.checkViewPort();

    }
  };

  Self.Scroll = {

    _scrolled: false,
    
    _autoScrollToComment: false,

    init: function ($subtopic, subtopic, comment) {
      var $comment,
        active;

      if (subtopic) {

        active = this.processActiveSubtopic(subtopic);

        $subtopic.multiAccordion('option','active',active);

      }

      if (comment) {
        $comment = $('#comment-wrapper-' + comment);
        if ($comment.length) {
          subtopic = this._getSubtopic($comment);
          $subtopic.multiAccordion('option','active',[subtopic-1]);
        }
      }
    },

    processActiveSubtopic: function (subtopic) {
      if (subtopic == 'all') {
        active = 'all';
      } else if (subtopic instanceof Array) {
        active = new Array();
        $.each(subtopic, function (i,v) {
          active.push(v-1);
        });
      } else {
        active = [subtopic-1];
      }
      return active;
    },

    checkCommentHash: function () {
      var hash = window.location.hash,
        cid;

      if (hash.substr(1,4) == 'cid-') {
        cid = parseInt(hash.split('-').pop());
        if (!isNaN(cid)) {
          return cid; // comment id
        }
      }

      return false;
    },

    checkSubtopicHash: function () {
      var hash = window.location.hash,
          hashVal = parseInt(hash.substr(1)),
          nodeId,
          nodeIds = new Array(),
          subtopics = new Array(),
          subtopicIndex,
          newHashValue,
          $dialog = $('.ui-dialog');

      // Hash is the open all subtopics value - #all
      if (hash.substr(1,3) == 'all') {
        return 'all';

      // Hash contains one or more node IDs
      } else if (hash.substr(1,4) == 'nid-') {
        nodeIds = hash.split('-');
        // Hash contains multiple node IDs - ex: #nid-13-16-22
        if (nodeIds.length > 2) {
          $.each(nodeIds, function (i,nid) {
            if (!isNaN(nid)) {
              if (subtopicIndex = Self.Scroll.getSubtopicIndex(nid)) {
                subtopics.push(subtopicIndex);
              }
            }
          });
          if (subtopics.length) {
            return subtopics; // Array of subtopic
          }

        // Hash contains a single node ID - ex: #nid-16
        } else {

          nodeId = nodeIds.pop();
          if (!isNaN(nodeId)) {
            if (subtopicIndex = Self.Scroll.getSubtopicIndex(nodeId)) {
              return subtopicIndex;
            }
          }
        }

      // Hash contains a subtopic index
      } else if (!isNaN(hashVal)) {
        return hashVal; // subtopic number
      }

      // Use stored values if found to set the subtopic and update the hash
      if (nodeIds = Self.TopicState.getStoredState()) {

        if (nodeIds == 'all') {

          Self.TopicState.updateHash('all');
          return 'all';

        } else {

          $.each(nodeIds, function (i,nid) {
            if (subtopicIndex = Self.Scroll.getSubtopicIndex(nid)) {
              if (!newHashValue) {
                newHashValue = 'nid';
              }
              newHashValue += '-' + nid;
              subtopics.push(subtopicIndex);
            }
          });

          if (subtopics.length) {

            Self.TopicState.updateHash(newHashValue);

            if (subtopics.length == 1) {
              return subtopics[0];
            } else {
              return subtopics;
            }
          }

        }
      }

      return false;
    },

    // Convert the subtopic node ID to the subtopic index
    getSubtopicIndex: function(subtopicNodeId) {
      var $subtopicHeader = $(subtopicHeader+'[data-rr-event_entity-id="'+subtopicNodeId+'"]');
      if ($subtopicHeader.length) {
        return $subtopicHeader.attr('id').split('-').pop();
      }
      return false;
    },

    // Immediately scroll window to given element.
    scrollToElement: function ($element) {
      $(window).scrollTop($element.offset().top);
    },

    // Handles scrolling to subtopic given a url subtopic number hash - ex. #3
    scrollToSubtopic: function (subtopic) {

      var $subtopic = $('#subtopic-title-' + subtopic);

      if ($subtopic.length) {
        this.scrollToElement($subtopic);
      }
      this._scrolled = true;

    },

    // Scroll to a subtopic in the given time/speed (milliseconds).
    scrollToSubtopicAnimate: function ($subtopicHeader, speed) {

      $('html,body').animate(
        { scrollTop: $subtopicHeader.offset().top },
        speed,
        'easeOutQuint'
      );

    },

    // Handles scrolling to a comment given a url comment id hash - ex. #cid-4
    scrollToComment: function (comment) {
      var $comment,
        $commentsContainer;

      $comment = $('#comment-wrapper-' + comment);

      if (Self.Subtopic.isSimpleScrollDevice()) {
        this.scrollToElement($comment);
        return;
      }

      if ($comment.length) {
        $commentsContainer = $comment.closest(commentsContainer);

        subtopic = this._getSubtopic($comment);

        $(window).scrollTop($('#subtopic-title-' + subtopic).offset().top);

        // Add a half-second delay before scrolling to the comment.  Firefox has an issue
        // where it won't scroll to the correct location when the page with a hash url
        // is refreshed.  This seems to fix the problem.
        setTimeout(function() {
          Self.Scroll._scrollToCommentAnimated($comment, $commentsContainer);
        }, 500);

      }
      
      this._scrolled = true;
    },

    // Scroll to a newly added comment
    scrollToNewComment: function (commentNumber) {
      var $commentWrapper = $('#comment-wrapper-' + commentNumber),
        $commentsContainer = $commentWrapper.closest(commentsContainer);

      if (Self.Subtopic.isSimpleScrollDevice()) {
        this.scrollToElement($commentWrapper);
        return;
      }

      setTimeout(function() {
        Self.Scroll._scrollToCommentAnimated($commentWrapper, $commentsContainer);
      }, 200);

    },
    
    // Animate scrolling to the specified comment
    _scrollToCommentAnimated: function($commentWrapper, $commentsContainer) {
      $commentsContainer.animate(
        {
          scrollTop:
            ($commentWrapper.offset().top + $commentsContainer.scrollTop()) - $commentsContainer.offset().top
        },
        {
          duration: 1000,
          easing: 'easeOutQuint',
          start: function() {
            Self.Scroll.setAutoScrollToComment(true);
          },
          always: function() {
            Self.Scroll.setAutoScrollToComment(false);
          },
          complete: function() {
            Self.Scroll._highlightCommentText($commentWrapper.find('.comment-text:first'));
          }          
        }
      );      
    },

    _highlightCommentText: function ($commentText) {
      $commentText.effect("highlight", {color:"#f4f4f4"}, 3000);
    },

    scrollToReplyOrRedactForm: function ($formElement) {

      var $form,
        $commentsContainer = $formElement.closest(commentsContainer);

      if ($formElement.is('form')) {
        $form = $formElement;

      } else {
        $form = $formElement.closest('form');
      }

      var scrollAnimate = function () {
        setTimeout(function () {
          $commentsContainer.animate({
            scrollTop: (
              $form.offset().top - (
                $commentsContainer.height() -
                $form.outerHeight()
              ) +
              $commentsContainer.scrollTop()
            ) -
            $commentsContainer.offset().top
          }, 'slow');
        }, 150);
      };

      // If there is an open reply or redact form that is currently, wait until
      // it is fully removed from the page before scrolling to the newly opened
      // reply or redact form.
      if (Self.Comment.areClosingForms($form)) {
        var intervalId = setInterval(function () {
          if (!Self.Comment.areClosingForms($form)) {
            scrollAnimate();
            clearInterval(intervalId);
          }
        }, 50);
      } else {
        scrollAnimate();
      }

    },

    _getSubtopic: function($comment) {
      return parseInt($comment.closest('div.subtopic_body').attr('id').split('-').pop());
    },

    hasScrolled: function() {
      return this._scrolled;
    },
    
    getAutoScrollToComment: function() {
      return this._autoScrollToComment;
    },
    
    setAutoScrollToComment: function(value) {
      this._autoScrollToComment = value;
    }
  };

  Self.Comment = {

    // The amount of time to fade in or out a comment form.
    _formFadeTime: 1000,

    // The opacity of a faded-out comment form.
    _formFadeOpacity: .35,

    init: function () {

      this.numberComments($(subtopicBody));

      this.Filter.init();
      this.Endorse.init();
      this.CommentTips.init();

      if (!SP.Login.isLoggedIn()) {
        this.notLoggedIn();
        
      } else if ($('#dlgInterestSurvey').length) {
        this.interestSurveyNotSubmitted();
        
      } else {        

        this.cancelCommentInput();
        //this.AddLink.init();

      }

      // Add class to force scrollbars to visible for recent OS X versions.
      /*if (isLionOrGreater) {
        $(commentsContainer).addClass('show-scroll');
      }*/

      //Self.ModCommentFrontEnd.applyClickListeners($('.ajax-comment-wrapper'));
    },

    // Return TRUE if any comment box contains text.
    commentTextNotSubmitted: function() {
      for (instanceKey in CKEDITOR.instances) {
        if (CKEDITOR.instances[instanceKey].getData()) {
          return true;
        }
      }
      return false;
    },

    logUnsubmittedCommentText: function () {
      var $editor, subtopicNid, commentText;
      for (instanceKey in CKEDITOR.instances) {
        commentText = CKEDITOR.instances[instanceKey].getData();
        if (commentText) {
          $editor = $('#' + instanceKey);
          subtopicNid = $editor.closest('.comment-wrapper').data('rr-event_entity-id');
          SP.EventLogger.log({
            'name': 'comment text beforeunload',
            'entity_type': 'node',
            'entity_id': subtopicNid,
            'detail_long': commentText
          }, false);
        }
      }
    },

    // Disable buttons and show alerts when the interest survey has not yet 
    // been submitted.

    interestSurveyNotSubmitted: function() {
      
      var link = Drupal.settings.rrAlert.interestSurveyLink;

      function getMessage(fragment) {
        return 'Please complete the ' + link + ' in order to ' + fragment + '.';
      }

      // NB unbind() alone doesn't prevent the ajax comment event listener from
      // responding to a reply button click, so we have replaced the real reply 
      // button with a dummy, as for anonymous users.
      this.disableCommenting(getMessage, 'interest-survey');
    },

    // Disable buttons and show log in alerts when the user is anonymous.
    notLoggedIn: function () {

      function getMessage(fragment) {
        return Self.Comment.getLoginRequiredAlertMessage(fragment);
      }

      this.disableCommenting(getMessage, 'login');

    },

    getLoginRequiredAlertMessage: function (fragment) {
      var alertSettings = Drupal.settings.rrAlert,
        loginLink = alertSettings.loginLink,
        registerLink = alertSettings.registerLink;
      return "Login required. Please " + loginLink + " or " + registerLink + " to " + fragment + ".";
    },

    openLoginRequiredCommentAlert: function() {
      SP.Alert.open('comment-alert', this.getLoginRequiredAlertMessage('make a comment'), 'login');
    },
    
    disableCommenting: function(getMessage, type) {

      // Open the comment form login alert when clicking inside the comment textarea.
      // Set the comment textarea to readonly.
      $(commentsForm + ' textarea').prop('readonly', true);
      $(commentsForm + ' .form-textarea-wrapper').on('click', function () {
        Self.Comment.openLoginRequiredCommentAlert();
      });

      // Unbind the ajax comments submit.
      jQuery(commentsForm + ' input.form-submit').unbind();

      // Open the comment form log in alert when the buttons are clicked.
      $(commentsForm + ' input[type="submit"]').unbind().on('click', function () {
        Self.Comment.openLoginRequiredCommentAlert();
        return false;
      });

      // Open the endorse alert after the button is clicked.
      $(endorseButton).unbind().on('click', function () {
        SP.Alert.open('endorse-alert', getMessage('endorse a comment'), type);
        return false;
      });

      // Open the reply alert after the button is clicked.
      $(replyButton).unbind().on('click', function (e) {
        SP.Alert.open('reply-alert', getMessage('make a reply'), type);
        return false;
      });      
    },

    // Get text entered in comment box.
    getCommentText: function ($commentForm) {
      var instanceKey = $commentForm.find('textarea').attr('id'),
        editor = CKEDITOR.instances[instanceKey];
      return editor.getData();
    },

    // Move the cursor to the comment box.
    focusOnCommentBox: function ($commentForm) {
      var instanceKey = $commentForm.find('textarea').attr('id'),
        editor = CKEDITOR.instances[instanceKey],
        data = this.getCommentText($commentForm);

      editor.setData(
        data, function () {
          editor.focus();
          // Position the cursor at the end of the comment text.
          var range = editor.createRange();
          range.moveToElementEditEnd( range.root );
          editor.getSelection().selectRanges( [ range ] );
        }
      );
    },

    userIsModerator: function () {
      return $('body').hasClass('moderator');
    },

    // The Drupal Ajax throbber.
    getThrobber: function () {
      return '<div class="ajax-progress ajax-progress-throbber"><div class="throbber">&nbsp;</div></div>';
    },

    // Show a custom Ajax submit throbber for comment and reply forms.
    // Called from the Ajax Comments module.
    showCommentSubmitThrobber: function (commentForm) {
      var $commentForm = $('#'+commentForm);

      // A hack/trick to insert the throbber before the button.
      // Inspired by: https://drupal.org/node/1059674#comment-5557408
      setTimeout(function () {
        $commentForm.find('.ajax-progress-throbber').remove();
        $commentForm.find('.form-actions .form-submit:first').before(Self.Comment.getThrobber());
        $commentForm.find('.form-actions .ajax-progress').css('display','inline-block');
      }, 1);

    },

    // Remove the custom Ajax submit throbber.
    removeCommentSubmitThrobber: function ($container) {
      $container.find('.form-actions .ajax-progress').remove();
    },

    // Cancel comment - clear textarea.
    cancelCommentInput: function () {

      $(commentCancelButton).each(function ( ) {
        var $cancelButton = $(this);
        if (!$cancelButton.data('cancelBehaviorEnabled')) {
          $cancelButton.data('cancelBehaviorEnabled', true);
          $(this).on('click', function () {
            var $form = $(this).closest('form');
            SP.Form.clearCommentValue($form);
            return false;
          });
        }
      });

    },

    // Watch for changes to a comment box textarea and clear errors if found.
    // NOTE: This behavior is now handled in the smartparticipation CKEditor plugin.
    /*clearCommentErrors: function () {

      $(commentsForm).find('textarea').on('input propertychange paste keyup keypress blur change', function () {

        var $commentForm = $(this).closest('form');

        if ($(this).val().length) {
          if (!$commentForm.data('errors-cleared')) {
            SP.Form.clearFormErrors($commentForm);
            $commentForm.data('errors-cleared', true);
          }
        }

      });

    },*/

    // Add or update comment numbering for a subtopic.
    numberComments: function ($subtopicBody, filter) {
      $subtopicBody.each(function () {

        var $subtopicBody = $(this),
          $comments;

        if (!$subtopicBody.hasClass('subtopic_body')) {
          $subtopicBody = $(this).closest(subtopicBody);
        }

        $comments = $subtopicBody.find('.comment');

        if (filter) {
          $comments = $comments.filter(':visible');
        }

        $comments.each(function (index) {
            $(this).find('.comment-number').text(index + 1);
        });

      });
    },

    // Update recommended count values
    updateRecommendedCount: function (action) {
      this.updateCommentCount(null, 'recommended', action);
    },

    // Update comment count values
    updateCommentCount: function ($subtopicBody, dataType, action) {

      var subtopic,
        selectors,
        icon,
        value;

      if ($subtopicBody !== null) {

        if (!$subtopicBody.hasClass('subtopic_body')) {
          $subtopicBody = $subtopicBody.closest(subtopicBody);
        }

        subtopic = $subtopicBody.attr('id').split('-')[2];
      }

      if (dataType === undefined) {
        dataType = 'comment';
      }

      if (action === undefined) {
        action = 'add';
      }

      switch (dataType) {
        case 'recommended':
          selectors = [
            'div.topic.carousel-container a.active + span.count + span.count'
          ];
          break;
        case 'comment':
          selectors = [
            'div.topic.carousel-container a.active + span.count',
            '[data-type="comment-count-topic"]',
            '#subtopic-body-'+subtopic+' [data-type="comment-count-subtopic"]',
            '#subtopic-title-'+subtopic+' [data-type="comment-count-subtopic"]'
          ];
          break;
      }

      // Set the comment count value(s)

      if (selectors !== undefined) {

        $(selectors.join(",")).each(function() {

          icon = null;

          if ($(this).children().length) {
            icon = $(this).children().clone();
          }

          value = parseInt($(this).text());

          if (action == 'remove') {
            value--;
          } else {
            value++;
          }

          $(this).text(value);

          if (icon !== null) {
            $(this).append(icon);
          }

        });

      }

    },

    // After a parent comment is successfully added.
    afterComment: function ($comment) {

      var $subtopicBody = $comment.closest(subtopicBody),
        commentId = $comment.attr('id').split('-').pop();

      this.numberComments($subtopicBody);

      this.updateCommentCount($subtopicBody);

      Self.Subtopic.checkContentHeight($subtopicBody);

      SP.Form.clearFormErrors($subtopicBody.find('form.comment-form'));

      this.removeCommentSubmitThrobber($subtopicBody);

      // Comment Tips - check if we should log the new comment submit.
      if (Self.Comment.CommentTips.shouldLogCommentSubmit) {
        Self.Comment.CommentTips.logCommentSubmit(commentId);
      }

    },

    afterModeratorCommment: function ($commentWrapper) {

      var cid, $replyButton, $replies,
        $parent = $commentWrapper.parent();

      SP.ModeratorControls.ModeratorControls.applyClickListeners($commentWrapper);

      // If the new comment is a reply, refresh the parent comment to show the new status.
      if ($parent.hasClass('ajax-comment-wrapper')) {

        cid = $parent.attr('id').split('-').pop();

        $replyButton = $parent.find('.ajax-comment-reply:first');
        $replyButton.detach();

        $.getJSON(Drupal.settings.basePath + 'comment/' + cid + '/render', function (data) {

          $replies = $parent.find('.ajax-comment-wrapper');
          $replies.detach();

          $parent.replaceWith(function () {
            return $(data.html).append($replies);
          });

          $parent = $('#'+$parent.attr('id'));

          SP.ModeratorControls.ModeratorControls.applyClickListeners($parent);
          $parent.find('.ajax-comment-reply:first').replaceWith($replyButton);
          Self.Comment.numberComments($commentWrapper.closest(subtopicBody));

        });

      }
    },

    // After a reply comment is successfully added.
    afterReply: function ($commentWrapper) {

      var $subtopicBody = $commentWrapper.closest(subtopicBody),
        $comment = $commentWrapper.find('.comment:first');
        $parentCommentWrapper = $commentWrapper.parent(),
        lastActiveReplyBoxId = $parentCommentWrapper.data('lastActiveReplyBoxId');

      // Remove the last used reply CKEditor instance.  For some reason it remains,
      // which triggers the beforeunload prompt and log event.  Also, errors
      // occur if the same comment reply button is clicked addition times and the
      // editor fails to initialize.
      CKEDITOR.instances[lastActiveReplyBoxId].destroy();

      this.enableCommentForm($commentWrapper.closest('.comment-wrapper').find('.comments-form-container form.comment-form'));

      $commentWrapper.parent().find('a.ajax-comment-reply').show();

      this.numberComments($subtopicBody, $subtopicBody.find(subtopicComments).data('active-filter'));

      this.updateCommentCount($subtopicBody);

      Self.Subtopic.checkContentHeight($subtopicBody);

      SP.Form.clearFormErrors($subtopicBody.find('form.comment-form'));

      this.removeCommentSubmitThrobber($subtopicBody);

      this.toggleRedactButton($comment);

      // The parent comment seems to get replaced too.  The redact button seems
      // to lose the Ajax click behavior.  Wait for the replacement to happen,
      // then reapply it.
      var parentId = '#' + $parentCommentWrapper.attr('id');
      var intervalId = setInterval(function() {
        var $parentWrapper = jQuery(parentId),
          $redactButton = $parentWrapper.find('ul.links:first .comment-redact a');
        if (!$redactButton.length) {
          clearInterval(intervalId);
        }
        if (!$redactButton.hasClass('ajax-processed')) {
          Drupal.attachBehaviors($redactButton);
          clearInterval(intervalId);
        }
      },50);
      // The endorsement users link behavior also needs to be
      // reattached after replacement.
      setTimeout(function () {
        var $parentComment = $(parentId),
          $endorsementUsersLink = $parentComment.find(showEndorsementUsersLink);
        Self.Comment.Endorse.applyShowUsersClick($endorsementUsersLink);
      }, 1000);

    },

    // A comment form is replaced with a new one after a comment is submitted - reapply behaviors.
    afterCommentFormReplace: function ($commentForm) {

      $commentForm.find('label').inFieldLabels();

      //this.AddLink.init();

      this.cancelCommentInput();

      // If using comment tips, clone and store the submit button from the new form.
      if (Self.Comment.CommentTips.clonedButtons.length > 0) {

        var commentButtonSelector = commentsForm+" "+commentButton,
          $commentSubmitButton = $commentForm.find(commentButton),
          index = $commentSubmitButton.index(commentButtonSelector),
          $clonedButton = Self.Comment.CommentTips.getClonedOriginalSubmitButton($commentSubmitButton);

        Self.Comment.CommentTips.storeClonedOriginalSubmitButton($clonedButton, index);

      }

    },

    // After a validation error occurs on a comment form.
    afterError: function ($commentForm) {

      //this.clearCommentErrors();

      //Self.Comment.CommentTips.refreshState($commentForm.closest(subtopicBody));

      if ($(commentTipsData).data('promptEnabled')) {

        Self.Comment.CommentTips.applyPromptClick($commentForm.find("[id^='edit-submit']"));

      }

    },

    // After a validation error occurs on a comment reply form.
    afterReplyError: function ($commentForm) {

      $commentForm.find('label').inFieldLabels();

      Self.Scroll.scrollToReplyOrRedactForm($commentForm);

    },

    // Are there reply or redact forms in the process of closing?
    areClosingForms: function ($activeForm) {
      return Self.Comment.getClosingForms($activeForm).length ? true : false;
    },

    // Get reply and redact forms that are open but not active.
    // Only one reply or redact form should be open within a subtopic comment panel.
    getClosingForms: function($activeForm) {
      var $commentsContainer = $activeForm.closest(commentsContainer);
      return $commentsContainer.find('form.comment-form:visible:not([id="'+$activeForm.attr('id')+'"])');
    },

    afterOpenRedactForm: function ($formElement) {

      var $editForm = $formElement.closest('form'),
        $commentForm = $editForm.closest('.comment-wrapper').find('.comments-form-container form'),
        $commentsContainer = $editForm.closest(commentsContainer),
        $replyForms = $commentsContainer.find('form.comment-form:not([id="'+$editForm.attr('id')+'"])'),
        $commentTextarea = $editForm.find('textarea'),
        $commentWrapper = $commentTextarea.closest('.ajax-comment-wrapper');

      $commentWrapper.data('lastActiveEditorId', $commentTextarea.attr('id'));
      
      this.disableCommentForm($commentForm);

      this.closeVisibleReplyAndRedactForms($replyForms);

      Self.Scroll.scrollToReplyOrRedactForm($formElement);
    },

    // After a reply form is opened.
    afterOpenReplyForm: function ($formElement) {

      var $replyForm = $formElement.closest('form'),
        $comment = $replyForm.prev(),
        $commentForm = $replyForm.closest('.comment-wrapper').find('.comments-form-container form'),
        activeFilter = $replyForm.closest(subtopicComments).data('active-filter'),
        $commentTextarea = $replyForm.find('textarea'),
        $commentWrapper = $commentTextarea.closest('.ajax-comment-wrapper');

      $commentWrapper.data('lastActiveReplyBoxId', $commentTextarea.attr('id'));

      this.showReplyForm($replyForm);

      Self.Scroll.scrollToReplyOrRedactForm($formElement);

      //this.AddLink.init($replyForm);

      if (!activeFilter) {
        this.disableCommentForm($commentForm);
      }

      this.toggleRedactButton($comment);

    },

    // Disable a comment form by fading it out and disabling the inputs.
    disableCommentForm: function ($commentForm) {
      $commentForm.data('disabled', 1);
      $commentForm.find(':input').prop('disabled', true);
      $commentForm.find(commentAddLink).unbind().click(function () {
        return false;
      });
      $commentForm.fadeTo(this._formFadeTime, this._formFadeOpacity);
    },

    // Enable a comment form by fading it back in and activating the inputs.
    enableCommentForm: function ($commentForm) {
      var activeFilter = $commentForm.closest(subtopicComments).data('active-filter');
      if (!activeFilter) {
        $commentForm.data('disabled', 0);
        $commentForm.fadeTo(this._formFadeTime, 1);
        $commentForm.find(':input').prop('disabled', false);
      }
    },

    // A reply form has been opened.
    showReplyForm: function ($replyForm) {
      var $commentsContainer = $replyForm.closest(commentsContainer),
        $replyForms = $commentsContainer.find('form.comment-form:not([id="'+$replyForm.attr('id')+'"])');

      // Hide the clicked reply button.
      $replyForm.prev().find('.ajax-comment-reply').hide();

      $replyForm.hide().fadeIn(this._formFadeTime);

      $replyForm.find('label').inFieldLabels();

      // Check for another open reply form in the subtopic and close it.
      this.closeVisibleReplyAndRedactForms($replyForms);
    },

    // Close given reply and redact forms.
    closeVisibleReplyAndRedactForms: function ($replyForms) {
      var $form;
      $replyForms.each(function () {
        $form = $(this);
        if ($form.is(':visible')) {
          if (Self.Comment.isReplyForm($form)) {
            Self.Comment.cancelReplyForm($(this), false);
          } else if (Self.Comment.isRedactForm($form)) {
            Self.Comment.cancelRedactForm($(this));
          }
        }
      });
    },

    isRedactForm: function ($form) {
      return $form.hasClass('comment-form-edit');
    },

    isReplyForm: function ($form) {
      return $form.hasClass('comment-form-reply');
    },

    cancelRedactForm: function ($redactForm) {

      var $cancelButton = jQuery('#'+$redactForm.attr('id')).find('.form-actions a.use-ajax'),
        $commentWrapper = $redactForm.parent(),
        $replyForm = $commentWrapper.children('.comment-form-reply'),
        editorInstanceKey = $redactForm.find('textarea').attr('id'),
        replyEditorInstanceKey;

      delete CKEDITOR.instances[editorInstanceKey];

      if ($replyForm.length) {
        replyEditorInstanceKey = $replyForm.find('textarea').attr('id');
        $replyForm.remove();
        delete CKEDITOR.instances[replyEditorInstanceKey];
      }

      jQuery.ajax($cancelButton.attr('href')).done(function(data) {
        var selector = data[1].selector,
          comment = data[1].data;
        jQuery(selector).replaceWith(comment);
        Drupal.attachBehaviors(jQuery(selector));
        var commentWrapperSelector = selector.split(" ").shift(),
          $commentWrapper = $(commentWrapperSelector),
          $endorsementUsersLink = $commentWrapper.find(showEndorsementUsersLink).first();
        Self.Comment.Endorse.applyShowUsersClick($endorsementUsersLink);
      });

      this.setRedactFormAutoClosed($commentWrapper);

    },

    toggleRedactButton: function ($comment) {
      var $redactButton = $comment.find('.comment-redact a');
      if ($redactButton.length) {
        $redactButton.toggle();
      }
    },


    // A reply form should be hidden.
    cancelReplyForm: function ($replyForm, scroll) {

      var $comment = $replyForm.prev();

      scroll = scroll || scroll === undefined ? true : false;
      var formFadeTime = scroll ? this._formFadeTime : 0;

      this.toggleRedactButton($comment);

      $replyForm.fadeOut(formFadeTime , function () {

        var $replyButton = $replyForm.prev().find('.ajax-comment-reply'),
          commentNumber = $replyButton.attr("id").split('-').pop();

        SP.Form.clearCommentErrorAndValue($replyForm);

        // If reply is clicked again, re-open the canceled reply form
        // instead of requesting a new one.
        // If the user is a moderator, always request a new form to avoid
        // issues after reloading the comment after moderator actions.
        if (!Self.Comment.userIsModerator()) {

          // This needs to be unbound because the ajax_command callback is still
          // attached to it. We want to show the form that is already hidden
          // instead of calling for a new one.
          jQuery('#'+$replyForm.attr('id')).prev().find('.ajax-comment-reply').unbind();

          // Reshow the reply form.
          $replyButton.addClass('clicked').unbind().attr("href", "#").show().bind({
            click: function(e) {
              var commentNumber = $(this).attr("id").split('-'),
                $replyForm = $('#comment-wrapper-' + commentNumber[1] + ' .comment-form'),
                $commentForm = $('#comment-wrapper-' + commentNumber[1]).closest('.comment-wrapper').find('.comments-form-container form'),
                activeFilter = $replyForm.closest(subtopicComments).data('active-filter');

              // Don't let people reply over and over - hide the reply button.
              $(this).hide();

              if (!activeFilter) {
                Self.Comment.disableCommentForm($commentForm);
              }

              Self.Comment.showReplyForm($replyForm);

              Self.Scroll.scrollToReplyOrRedactForm($replyForm);

              e.preventDefault();
            }
          });

        } else {
          $replyButton.show();
        }

        if (scroll) {
          // Scroll to the comment that had been selected for the reply.
          Self.Scroll.scrollToComment(commentNumber);
        }

        //return true;

      });

    },

    setRedactFormAutoClosed: function ($commentWrapper) {
      $commentWrapper.data('redactFormAutoClosed', true);
    },

    getRedactFormAutoClosed: function ($commentWrapper) {
      if ($commentWrapper.data('redactFormAutoClosed') !== undefined && $commentWrapper.data('redactFormAutoClosed')) {
        $commentWrapper.data('redactFormAutoClosed', false);
        return true;
      }
      return false;
    },

    // Occurs after saving or canceling the redact form.
    afterRedact: function ($commentWrapper) {

      var commentNumber = $commentWrapper.attr('id').split('-').pop(),
        formAutoClosed = false,
        //$parentCommentWrapper = $commentWrapper.parent(),
        lastActiveEditorId = $commentWrapper.data('lastActiveEditorId');

      delete CKEDITOR.instances[lastActiveEditorId];

      // Find out if the form was automatically canceled/closed due to another
      // form being opened.
      if (this.getRedactFormAutoClosed($commentWrapper)) {
        formAutoClosed = true;
      }

      this.enableCommentForm($commentWrapper.closest('.comment-wrapper').find('.comments-form-container form.comment-form'));

      this.numberComments($commentWrapper.closest(subtopicBody));

      // Don't scroll to the comment if the redact form was automatically closed.
      // Instead, only allow the scrolling action to the next form that was opened.
      // Otherwise, there would be multiple scrolling actions at the same time.
      if (!formAutoClosed) {
        Self.Scroll.scrollToComment(commentNumber);
      }

      Drupal.attachBehaviors();

      Self.Comment.Endorse.applyShowUsersClick($commentWrapper.find(showEndorsementUsersLink));

    },

    AddLink: {

      // Attach the add link behavior to comment/reply forms.
      init: function ($commentForm) {

        var $container,
          self = this,
          $commentForms;

        // Use the given comment form or use all comment forms.
        if ($commentForm !== undefined) {
          $commentForms = $commentForm;
        } else {
          $commentForms = $(commentsForm);
        }

        $commentForms.each(function () {

          $(this).find(commentAddLink).unbind().click(function () {
            $container = $(this).closest(commentsForm);
            self._toggleForm($container);
            return false;
          });

          $(this).find(commentAddLinkSave).unbind().click(function () {
            $container = $(this).closest(commentsForm);
            self._insertLink($container);
            return false;
          });

          $(this).find(commentAddLinkCancel).unbind().click(function () {
            $container = $(this).closest(commentsForm);
            self._toggleForm($container);
            return false;
          });

        });

      },

      _toggleForm: function ($container) {
        $container.find(commentAddLinkFields + ' input').val('').blur();
        $container.find(commentAddLinkFields).slideToggle('slow', function () {
          $(this).find('input[id^="link-url"]').focus();
          // Move add link fields into view if this is a reply form
          if ($(this).closest('.ajax-comment-wrapper').length) {
            Self.Scroll.scrollToReplyOrRedactForm($(this));
          } else {
            Self.Subtopic.checkContentHeight($(this).closest(subtopicBody));
          }
        });
        $container.find('[id^="edit-actions"]').toggle();
      },

      _insertLink: function ($container) {
        var $fields = $container.find(commentAddLinkFields),
          $linkUrl = $fields.find('input[id^="link-url"]'),
          linkUrl = $linkUrl.val(),
          linkText = $fields.find('input[id^="link-text"]').val(),
          $textarea = $container.find('textarea');

        if (linkUrl.length) {

          // if user has not entered http:// or https:// assume they mean http://
          if(!/^(https?):\/\//i.test(linkUrl)) {
            linkUrl = 'http://' + linkUrl;
          }

          if (!linkText.length) {
            linkText = linkUrl;
          }

          $textarea.val($textarea.val() + '<a href="' + linkUrl + '">' + linkText + '</a>').blur().focus();

          this._toggleForm($container);

        } else {

          $linkUrl.focus();

        }

      }

    },

    Endorse: {

      init: function () {

        if (SP.Login.isLoggedIn()) {
          this.applyClick($(endorseButton));
        }
        this.applyShowUsersClick($(showEndorsementUsersLink));

      },

      applyClick: function ($button) {

        $button.on('click', function () {

          var $button = $(this);

          // Add the Ajax throbber after the button.
          $button.after(Self.Comment.getThrobber());

          $.ajax({
            url: $(this).prop('href'),
            type: 'POST',
            dataType: 'json',
            data: { js: true },
            success: function (data) {
              if (data.status) {
                if (data.flagName == 'comment_endorsement') {

                  cid = data.contentId,
                    $commentWrapper = $('#comment-wrapper-'+cid);

                  // Remove the endorsements display for the comment.
                  $commentWrapper.find('.endorsement-counter:first span').fadeOut(1500);

                  // Toggle the endorsement button state.
                  $commentWrapper.find('.comment-endorse:first').html($(data.newLink).find('a'));

                  // Apply the click handler to the updated button.
                  Self.Comment.Endorse.applyClick($commentWrapper.find('.comment-endorse:first a'));

                  // The comment is being endorsed.
                  if (data.flagStatus == 'flagged') {
                    // Remove counter text if it's there.
                    $commentWrapper.find('.endorsement-counter:first span').remove();
                    // Insert updated counter text.
                    $commentWrapper.find('.endorsement-counter:first').append($('<span />')).find('span').hide().html(
                      '<span>'+data.endorsements+'</span>'+' endorsement'+(data.endorsements > 1 ? 's' : '')
                    ).fadeIn(1500);
                  }
                }
              }
            },
            error: function () {
              // Remove the Ajax throbber if the request doesn't succeed.
              $button.parent().find('.ajax-progress').remove();
            }
          });

          return false;

        });

      },

      applyShowUsersClick: function ($link) {

        $link.on('click', function () {

          var $link = $(this),
            $usersList = $link.closest(commentText).find(endorsementUsers);

          $(endorsementUsersDialog).html($usersList.clone());
          $(endorsementUsersDialog).find(endorsementUsers).show();
          $(endorsementUsersDialog).rrDialog();

          return false;

        });

      }

    },

    CommentTips: {

      // The original comment submit buttons that include the behaviors applied by AJAX Comments.
      clonedButtons: new Array(),

      // A jQuery UI overlay used when the user must choose an action.
      overlay: null,

      shouldLogCommentSubmit: false,

      init: function () {

        var self = this,
          $commentSubmitButton = $(commentsForm).find(commentButton);

        $(commentTipsButton).click(function () {

          var subtopicNid = $(this).closest('.comment-wrapper').data('rr-event_entity-id'),
            $commentForm = $(this).closest(commentsFormContainer).find(commentsForm),
            commentText = Self.Comment.getCommentText($commentForm),
            eventData;

          self.openAlert('button', subtopicNid);

          eventData = {
            'name': 'open comment tips with button',
            'entity_type': 'node',
            'entity_id': subtopicNid
          };

          if (commentText && commentText.length) {
            eventData.detail_long = commentText;
          }

          SP.logEvent(eventData);

          return false;

        });

        if ($(commentTipsData).data('promptEnabled')) {

          this.cloneOriginalSubmitButtons($commentSubmitButton);

          this.applyPromptClick($commentSubmitButton);

        }

      },

      // Log a comment submit to determine length of time from selecting revise in comment tips.
      logCommentSubmit: function(commentId) {

        var eventData = {
          'name': 'comment submitted',
          'entity_type': 'comment',
          'entity_id': commentId
        };

        SP.logEvent(eventData);

        this.shouldLogCommentSubmit = false;

      },

      // Store original submit buttons that include the AJAX Comment submit handler.
      cloneOriginalSubmitButtons: function ($commentSubmitButtons) {

        $commentSubmitButtons.each(function (index) {

          var $originalSubmitButton = Self.Comment.CommentTips.getClonedOriginalSubmitButton($(this));

          Self.Comment.CommentTips.storeClonedOriginalSubmitButton($originalSubmitButton, index);

        });

      },

      // Get the submit button with the AJAX Comments submit handler attached.
      getClonedOriginalSubmitButton: function ($commentSubmitButton) {

        var buttonID = '#'+$commentSubmitButton.attr('id'),
          $button = jQuery(buttonID),
          $originalSubmitButton = $button.clone(true);

        return $originalSubmitButton;

      },

      // Store the cloned submit button in the clonedButtons array.
      storeClonedOriginalSubmitButton: function ($originalSubmitButton, index) {

        this.clonedButtons[index] = $originalSubmitButton;

      },

      // Replace all comment submit buttons with the original buttons that include the AJAX Comments submit handler.
      restoreCommentSubmitButtons: function () {

        var $commentSubmitButton = $(commentsFormContainer).find(commentButton);

        $commentSubmitButton.each(function (i) {

          var $clonedButton = Self.Comment.CommentTips.clonedButtons[i];

          Self.Comment.CommentTips.restoreCommentSubmitButton(
            $(this),
            Self.Comment.CommentTips.clonedButtons[i]
          );

        });

      },

      // Restore a single comment button to the original state.
      restoreCommentSubmitButton: function ($commentSubmitButton, $clonedButton) {

        $commentSubmitButton.replaceWith($clonedButton);

      },

      /*disableFeaturesUnderPrompt: function(getMessage, type) {
        // Open the comment form login alert when clicking inside the comment textarea.
        // Set the comment textarea to readonly.
        $(commentsForm + ' textarea').prop('readonly', true).on('click', function () {
          //SP.Alert.open('comment-alert', getMessage('make a comment'), type);
        });

        // Unbind the ajax comments submit.
        jQuery(commentsForm + ' input.form-submit').unbind();

        // Open the comment form log in alert when the buttons are clicked.
        $(commentsForm + ' input[type="submit"], ' + commentAddLink).unbind().on('click', function () {
          //SP.Alert.open('comment-alert', getMessage('make a comment'), type);
          return false;
        });

        // Open the endorse alert after the button is clicked.
        $(endorseButton).unbind().on('click', function () {
          //SP.Alert.open('endorse-alert', getMessage('endorse a comment'), type);
          return false;
        });

        // Open the reply alert after the button is clicked.
        $(replyButton).unbind().on('click', function (e) {
          //SP.Alert.open('reply-alert', getMessage('make a reply'), type);
          return false;
        });
      },*/


      // Attach the comment tips click handler to the comment form submit buttons.
      applyPromptClick: function ($commentSubmitButton) {

        // Unbind submit handler on comment form submit buttons
        $commentSubmitButton.each(function () {

          var buttonID = '#'+$(this).attr('id'),
            $button = jQuery(buttonID),
            $clonedButton = $button.clone(true);

          $button.unbind();

        });

        $commentSubmitButton.on('click', function () {
          var $commentSubmitButton = $(this),
            subtopicNid = $commentSubmitButton.closest('.comment-wrapper').data('rr-event_entity-id'),
            $commentForm = $commentSubmitButton.closest(commentsFormContainer).find(commentsForm),
            commentText = Self.Comment.getCommentText($commentForm),
            commentButtonSelector = commentsForm+" "+commentButton,
            index = $commentSubmitButton.index(commentButtonSelector),
            eventData;

          if (!commentText.length || !$(commentTipsData).data('promptEnabled')) {
            Self.Comment.CommentTips.restoreCommentSubmitButton(
              $commentSubmitButton,
              Self.Comment.CommentTips.clonedButtons[index]
            );
            jQuery(commentButtonSelector).eq(index).mousedown();
            return;
          }

          Self.Comment.CommentTips.openAlert('comment', subtopicNid);

          eventData = {
            'name': 'open comment tips prompt',
            'entity_type': 'node',
            'entity_id': subtopicNid
          };

          if (commentText && commentText.length) {
            eventData.detail_long = commentText;
          }

          SP.logEvent(eventData);

          return false;

        });

      },

      // Check the comment tips state based on user activity and update the interface.
      refreshState: function($subtopicBody) {

        var subtopicNid = $subtopicBody.find('.comment-wrapper').data('rr-event_entity-id');

        $.get(
          $(commentTipsData).data('getStateBaseUrl')+subtopicNid,
          function (data) {
            var $commentSubmitButton;
            if (data.use_comment_tips_button) {
              $('a.comment-tips-button').show();
            } else {
              $('a.comment-tips-button').hide();
            }
            if (data.use_comment_tips_prompt) {
              $(commentTipsData).data('promptEnabled', true);
              $commentSubmitButton = $subtopicBody.find(commentButton);
              Self.Comment.CommentTips.applyPromptClick($commentSubmitButton);
            } else {
              $(commentTipsData).data('promptEnabled', false);
            }
          }
        );

      },

      openAlert: function(method, subtopicNid) {

        var commentTipsMarkup = $(commentTipsData).clone().html(),
          $commentTipsMarkup = $(commentTipsMarkup),
          numberOfTips = $commentTipsMarkup.find('li').length;

        // Add visual tip dividers
        $commentTipsMarkup.find('li:lt('+(numberOfTips-1)+')').prepend($('<span />').addClass('tip-divider'));

        // How is the comment alert being opened?
        // Automatically at comment submit or by user click of the comment tips button.
        // Automatically: 1) action buttons are needed 2) don't show close button
        // Button click: 1) don't show action buttons 2) show close button
        switch(method) {

          // Open the comment tips alert with a close button but no action buttons.
          case 'button':
            SP.Alert.open(
              'comment-tips-button-alert',
              $('<div />').addClass('comment-tips-wrapper').append($commentTipsMarkup),
              'comment-tips-button'
            );
            break;

          // Open a prompt that contains the comment tips and requires the user to
          // choose to either revise the comment or submit the comment as is.
          case 'comment':
            SP.Alert.open(
              'comment-tips-prompt-alert',
              $('<div />').addClass('comment-tips-wrapper action-prompt').attr('data-subtopic-nid',subtopicNid).append($commentTipsMarkup),
              'comment-tips-prompt',
              false
            );
            // Place an overlay so to make this a modal alert.
            this.shouldLogCommentSubmit = true;
            this.createOverlay();
            break;
        }

      },

      // Create a jQuery UI overlay to force modal action (prevent user access to page actions)
      createOverlay: function () {
        this.overlay =  $( "<div>" )
          .addClass( "ui-widget-overlay ui-front" )
          .appendTo( $('body') );
      },

      // Remove the jQuery UI overlay
      destroyOverlay: function () {
        this.overlay.remove();
      },

      // Attach the handlers when the comment tips alert contains the revise and submit-as-is actions.
      attachAlertActionHandlers: function ($container) {

        var $reviseCommentButton = $container.find('button.revise-comment'),
          $submitAsIsCommentButton = $container.find('button.submit-comment-as-is'),
          subtopicNid = $container.find('.comment-tips-wrapper').data('subtopicNid'),
          subtopicBody = '.subtopic-body-node-'+subtopicNid;

        $reviseCommentButton.on('click', function () {
          SP.logEvent({
              'name': 'comment tips revise comment selected',
              'entity_type': 'node',
              'entity_id': subtopicNid
          }, false);
          Self.Comment.CommentTips.destroyOverlay();
          Self.Comment.CommentTips.refreshState($(subtopicBody));
          SP.Alert.close($reviseCommentButton);
          // Return user focus to comment box
          var $commentForm = $(subtopicBody).find(commentsForm);
          Self.Comment.focusOnCommentBox($commentForm);
          return false;
        });

        $submitAsIsCommentButton.on('click', function () {
          SP.logEvent({
            'name': 'comment tips submit comment selected',
            'entity_type': 'node',
            'entity_id': subtopicNid
          }, false);
          Self.Comment.CommentTips.destroyOverlay();
          Self.Comment.CommentTips.refreshState($(subtopicBody));
          SP.Alert.close($submitAsIsCommentButton);
          // Restore original comment button that includes the submit handler
          var $commentSubmitButton = $(subtopicBody).find(commentsFormContainer).find(commentButton),
            commentButtonSelector = commentsForm+" "+commentButton,
            index = $commentSubmitButton.index(commentButtonSelector);
          Self.Comment.CommentTips.restoreCommentSubmitButton(
            $commentSubmitButton,
            Self.Comment.CommentTips.clonedButtons[index]
          );
          // Submit comment using AJAX Comments behavior
          jQuery(subtopicBody).find(commentsFormContainer).find(commentButton).mousedown();
        });

      }

    },

    Filter: {

      init: function () {

        $(recommendedFilter).on('click', this.showRecommended);

      },

      showRecommended: function () {

        var $filterLink = $(this),
          $subtopicComments = $filterLink.closest(subtopicComments),
          $comments = $subtopicComments.find('.comment'),
          $panelTitle = $subtopicComments.find('h3.title'),
          $commentCountWrapper = $panelTitle.find('span.comment-count').clone(),
          $commentForm = $subtopicComments.find(commentsFormContainer + ' .comment-form'),
          $commentCountIcon = $commentCountWrapper.find('.comment-count-icon'),
          $replyForms;

        $filterLink.toggleClass('active');

        // Show recommended comments
        if ($filterLink.hasClass('active')) {

          $subtopicComments.data('active-filter', 1);

          $filterLink.find('span').removeClass('recommended-filter-icon').addClass('comments-filter-icon');

          $filterLink.prop('title', 'View all comments');

          $panelTitle.html('Recommended comments').append($commentCountWrapper);
          
          $commentCountIcon.addClass('recommended-icon');

          $commentForm.fadeOut(1000);

          // Get comments that don't have the recommended icon.
          $comments = $comments.map(function () {
            if (!$(this).find('.recommended').length) {
              return this;
            }
          });

          // Fade out the comments.
          $comments.fadeOut(1000);

          // When all comments are done fading, re-number the visible comments.
          $comments.promise().done(function () {
            Self.Comment.numberComments($subtopicComments, true);
          });

          // Update count value to number of recommended comments.
          $commentCountWrapper.find('span:first').text(
            $commentCountWrapper.find('span:first').text() - $comments.length
          );
          
          SP.logEvent({
            'name': 'view recommended comments',
            'entity_type': 'node',
            'entity_id': $filterLink.closest('.comment-wrapper').data('rr-event_entity-id')
          });
          

        // Show all comments
        } else {

          $subtopicComments.data('active-filter', 0);

          $filterLink.find('span').removeClass('comments-filter-icon').addClass('recommended-filter-icon');

          $filterLink.prop('title', 'View recommended comments');

          $panelTitle.html('Comments').append($commentCountWrapper);
          
          $commentCountIcon.removeClass('recommended-icon');
          
          if ($commentForm.data('disabled')) {
            Self.Comment.enableCommentForm($commentForm);
          } else {
            $commentForm.fadeIn(1000);
          }

          // Show all comments.
          $comments.fadeIn(1000, function () {
            $replyForms = $subtopicComments.find(commentsContainer + ' form.comment-form');
            if ($replyForms.length) {
              Self.Comment.closeVisibleReplyAndRedactForms($replyForms);
            }
          });
          
          Self.Comment.numberComments($subtopicComments);

          // Restore count value to total number of comments.
          $commentCountWrapper.find('span:first').text($comments.length);

        }

        return false;

      }
    }
  };

})(SP.Topic = {}, SP.jQuery);