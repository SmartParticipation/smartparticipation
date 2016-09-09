/**
 * Moderator interface page
 */

SP.jQuery(document).ready(function () {
  SP.ModeratorInterface.init();
});

(function (Self, $) {

  var active_action_cid = "",
    active_domain_filter = "all_comments",
    active_domain_nid = "all",
    active_node_accordions = new Array(),
    cc = 0,
    formal_filter_dictionary = {
      "unread":"Unread",
      "in_progress":"In progress",
      "done":"Done",
      "reply":"Replied to",
      "no_reply":"No Reply",
      "quarantine":"Quarantined",
      "redacted":"Redacted",
      "recommended":"Recommended",
      "all_comments":"All"
    },
    hold_pager_refresh = false,
    limit = 5,
    offset = 0,
    order = "newest",
    order_sup = "",
    total_com = 0,
    uid_filter = "";
  
  Self.init = function () { // test
  
    var icons = {
    header: "ui-icon-circle-arrow-e",
    activeHeader: "ui-icon-circle-arrow-s"
    };
  
    //Apply click events to buttons on left-hand topic/proposal filters.
    $('.radio_filters').each(function() {
    var nid = $(this).attr('nid');
  
    $('.filter_div[nid=' + nid + ']').buttonset();
  
    //Apply hover effects to tags
    $('.filter_div[nid=' + nid + '] label').mouseenter(function(e) {
      $(this).addClass('ui-state-hover');
    });
    $('.filter_div[nid=' + nid + '] label').mouseleave(function(e) {
      $(this).removeClass('ui-state-hover');
    });
  
    //Click, not change, chosen to allow for preservation of cross node selections
    $('.filter_div[nid=' + nid + '] label').click(function() {
  
      var active_nid_labels = $(".radio_filters[nid="+ active_domain_nid+"]").find("label");
  
      active_nid_labels.removeClass("ui-state-active");
      active_nid_labels.find("span.comment_count_active").addClass("comment_count_inactive");
      active_nid_labels.find("span.comment_count_inactive").removeClass("comment_count_active");
  
      active_domain_nid = $(this).parent().attr('nid');
      active_domain_filter = $(this).parent().find('.' + $(this).attr('for')).attr('value');
  
      var new_domain_label = $(".radio_filters[nid="+ active_domain_nid+"]").find("label[for=radio_" + active_domain_filter + "]");
      new_domain_label.addClass("ui-state-active");
      new_domain_label.find("span.comment_count_inactive").addClass("comment_count_active");
      new_domain_label.find("span.comment_count_active").removeClass("comment_count_inactive");
  
      //Do not refresh comments in the all users view.
      if (uid_filter == '') {
        Self.ModOperations.refresh_comments();
      }
    });
    });
  
    //This generates the default comments
    total_com = $("#comment_pager_top").attr('default_items');
    if (!hold_pager_refresh) {
      Self.ModUI.pager_refresh(limit);
    }
    hold_pager_refresh = false;
  
  
    $("#filter_dropdown").val("newest");
    $("#limit_dropdown").val("5");
  
    $( "#proposal_accordion" ).multiAccordion({active: 0});
  
    $(".comment_notes_dropdown").multiAccordion({active: 'none'});
  
    //Change offset to reflect current page
    jQuery('#comment_pager_top').click(function() {
        $("#comment_pager_bottom").pagination('selectPage',$("#comment_pager_top").pagination('getCurrentPage'));
        Self.ModOperations.change_offset(($("#comment_pager_top").pagination('getCurrentPage') * limit) - limit);
    });

    jQuery('#comment_pager_bottom').click(function() {
        $("#comment_pager_top").pagination('selectPage',$("#comment_pager_bottom").pagination('getCurrentPage'));
        Self.ModOperations.change_offset(($("#comment_pager_bottom").pagination('getCurrentPage') * limit) - limit);
    });

    $('.topic_accordion').multiAccordion({
          active: 'all'
    });
  
    //Add classes for default filter of nid: all, filter: ALL comments
    $('.radio_filters[nid=all]').find('label[for=radio_all_comments]').addClass('ui-state-active');
    $('.radio_filters[nid=all]').find('label[for=radio_all_comments]').find('span.comment_count_inactive').addClass('comment_count_active');
    $('.radio_filters[nid=all]').find('label[for=radio_all_comments]').find('span.comment_count_inactive').removeClass('comment_count_inactive');
  
    Self.ModUI.apply_onclick_events();
    Self.ModUI.apply_filter_handlers();
  
    Self.ModUI.apply_hover_effects();
  
  };

  /*
   * Functions related to the moderator interface UI.
   */
  Self.ModUI = {
    apply_onclick_events: function () {
      $('#deactivate_user_view_button').click(function() {
        Self.ModUI.deactivate_user_view();
      });
  
      $('.mod_panel_comment').each(function() {
        var cid = $(this).attr('cid'),
          uid = $(this).attr('uid'),
          comment_el = Self.ModUI.get_comment_el(cid);
  
        //Activate user centric view
        comment_el.find('.comment_user_comment_history').click(function() {
          Self.ModUI.activate_user_view(uid);
        });

        //User Interest survey dialog
        comment_el.find('.comment_user_interest_survey').click(function() {
          Self.ModUI.show_interest_survey(cid);
        });

        //Show original comment when
        comment_el.find('.x_img').click(function() {
          Self.ModUI.show_original_comment(cid, 'hide');
        });
  
        comment_el.find('.submit_restore_original').click(function() {
          Self.ModOperations.mod_action("restore_original", cid);
        });
  
        //Moderator action links
        comment_el.find('ul.comment_links').find('li').find('a').each(function() {
          jQuery(this).click(function() {
            Self.ModOperations.mod_action($(this).attr('action'), cid);
          });
        });
  
        comment_el.find('.input_cancel_link').click(function() {
          Self.ModUI.hide_mod_action_input(cid);
        });
  
        comment_el.find('.send_note_button').click(function() {
          Self.ModUI.show_send_note(cid);
        });
      });
    },
  
    apply_filter_handlers: function() {
      jQuery('#filter_dropdown').change(function() {
        Self.ModOperations.change_order(jQuery(this).attr('value'));
      });
  
      jQuery('#limit_dropdown').change(function() {
        Self.ModOperations.change_limit(jQuery(this).attr('value'));
      });
  
      jQuery('#topics_toggle').click(function() {
        Self.ModOperations.change_order_sup('topics');
      });
  
      jQuery('#users_toggle').click(function() {
        Self.ModOperations.change_order_sup('users');
      });
    },
  
    show_original_comment: function (cid, showOrHide) {
      var comment_el = Self.ModUI.get_comment_el(cid);
      if (showOrHide == "show") {
        comment_el.find('.original_comment_div').css("display", "block");
        comment_el.find('.redact_comment_header').css("display","block");
      } else{
        comment_el.find('.original_comment_div').css("display","none");
        comment_el.find('.redact_comment_header').css("display","none");
      }
    },
  
    mod_action_input: function (action, cid) {
      var comment_el = Self.ModUI.get_comment_el(cid);
      if (comment_el.find(".comment_mod_input_div").css("display") == "block" && active_action_cid == action + "_" + cid) {
        Self.ModUI.hide_mod_action_input(cid);
      } else{
        if (action == "add_note_input") {
          comment_el.find('.input_submit').html("Add note");
          comment_el.find('.input_submit').click(function() {
            Self.ModOperations.mod_action('add_note', cid );
          });
          comment_el.find('.input_submit_email').css('display', 'inline');
          comment_el.find('.checkbox_email').click(function() {
            if (comment_el.find('.checkbox_email').prop('checked')) {
              //console.log('checked');
              comment_el.find('.email_fields').css('display', 'block');
            }
            else {
              comment_el.find('.email_fields').css('display', 'none');
            }
          });
          comment_el.find('textarea').val('');
          comment_el.find('.send_note_button').css('display', 'block');
  
          //Send note link removed, see previous versions if needed to be restored
          comment_el.find('.input_header_content').html('Add note');
        } else if (action == "redact_input") {
          comment_el.find('.input_submit').html("Redact");
          comment_el.find('.input_submit').click(function() {
            Self.ModOperations.mod_action('redact', cid );
          });
          comment_el.find('textarea').val(comment_el.find('.hidden_data .comment_body').html());
          comment_el.find('.input_header_content').html("Redact comment");
          comment_el.find('.send_note_p').css("display", "none");
          comment_el.find('.content_text').css("display", "none");
        } else if (action == "edit_reply_input") {
          comment_el.find('.input_submit').html("Edit reply");
          comment_el.find('.input_submit').click(function() {
            Self.ModOperations.mod_action('edit_reply', cid );
          });
          // This is used for the 'Edit Reply' function
          comment_el.find('textarea').val(comment_el.find('.hidden_data .reply_body').html());
          comment_el.find('.input_header_content').html("Edit reply");
          comment_el.find('.send_note_p').css("display", "none");
        }
        active_action_cid = action + "_" + cid;
        comment_el.find('.mod_panel_comment_content').removeClass("content_hover");
        comment_el.find('.comment_links').css("display", "none");
        comment_el.find('.comment_notes_dropdown').css('display', 'none');
  
        comment_el.find(".comment_mod_input_div").css("display", "block");
        comment_el.find('textarea').show();
        comment_el.find('.input_header').css("display", "block");
      }
    },
  
    show_send_note: function ($cid) {
      var comment_el = Self.ModUI.get_comment_el($cid);
      if (comment_el.find('.send_note_p').css('display') == 'none') {
        comment_el.find('.send_note_p').css('display', 'block');
        comment_el.find('.send_note_button').html('Don\'t send note');
      } else{
        comment_el.find('.send_note_p').css('display', 'none');
        comment_el.find('.send_note_button').html('Send note');
        comment_el.find('.note_to').val('');
      }
    },
  
    hide_mod_action_input: function (cid) {
      var comment_el = Self.ModUI.get_comment_el(cid);

      // Unbind submit button click handler
      comment_el.find('.comment_mod_input_div .input_submit').unbind();
      comment_el.find('.checkbox_email').unbind();

      //Hide
      comment_el.find(".comment_mod_input_div").css("display", "none");
      comment_el.find('.send_note_button').css('display', 'none');
      comment_el.find('.input_mod_error_span').html("");
      comment_el.find('.input_submit_email').css('display', 'none');
      comment_el.find('.email_fields').css('display', 'none');
      comment_el.find('.checkbox_email').prop('checked', false);
  
      //Show
      comment_el.find('.comment_links').css("display", "inline");
      comment_el.find('.content_text').css("display", "block");
      comment_el.find('.comment_notes_dropdown').css('display', 'block');
  
      active_action_cid = "";
    },
  
    get_visible_comments_with_position: function() {
      var arr = new Array();
  
      $(".mod_panel_comment").each(function() {
        var cid = $(this).attr('cid');
        var index = $(this).find('.page_comment_index').html();
  
        arr[cid] = index;
      });
  
      return arr;
    },
  
    show_comment_removed: function(pos) {
      var totalHTML = "";
      var newHTML = "<div id='comment_removed_by_filter'>Comment hidden due to filter parameters</div>";
  
      var com_pos_reached = true;
      $('.mod_panel_comment').each(function() {
        if ($(this).find('.page_comment_index').html() == pos) {
          com_pos_reached = false;
          totalHTML += newHTML;
        }
        totalHTML += $(this).clone().wrap('<p>').parent().html();
      });
      if (com_pos_reached) {
        totalHTML += newHTML;
      }
      $("#comment_col").html(totalHTML);
    },
  
    get_comment_el: function(cid) {
      var toReturn;
      $(".mod_panel_comment").each(function() {
        if (jQuery(this).attr('cid') == cid) {
          toReturn = $(this);
          //Breaks loop
          return false;
        }
      });
      return toReturn;
    },
  
    apply_hover_effects: function() {
      //Refresh tooltips
      $(".comment_tag").mouseenter(function(e) {
        var cid = this.getAttribute("num");
        var o = {
          left: e.pageX + 10,
          top: e.pageY - 15
        };
        Self.ModUI.get_comment_el(cid).find('.comment_tag_tooltip').css("display","block").offset(o);
      });
      $(".comment_tag").mouseout(function(e) {
        var cid = this.getAttribute("num");
        Self.ModUI.get_comment_el(cid).find('.comment_tag_tooltip').css("display","none");
      });
  
      //Refresh background color effects for comment content area
      $('.mod_panel_comment_content').mouseenter(function() {
        if ($(this).find(".comment_mod_input_div").css("display") == "none") {
          $(this).addClass("content_hover");
        }
      });
      $('.mod_panel_comment_content').mouseleave(function() {
        $(this).removeClass("content_hover");
      });
    },
  
    activate_user_view: function (uid) {
      var radio_filter = $(".radio_filters[nid="+ active_domain_nid+"]");
  
      radio_filter.find("label").removeClass("ui-state-active");
      radio_filter.find("label").find("span.comment_count_inactive").addClass("comment_count_inactive");
      radio_filter.find("label").find("span.comment_count_inactive").removeClass("comment_count_active");
  
      uid_filter = uid;
  
      Self.ModUI.refresh_col_header();
      Self.ModOperations.refresh_comments();
    },

    show_interest_survey: function (cid) {
      var $dlgSurvey = $('#dlgSurvey');

      $(".mod_panel_comment").each(function() {
        if ($(this).attr('cid') == cid){
          username = $(this).find('.comment_username').text();
          $dlgSurvey.rrDialog({width:500, title:username+'\'s Interest Survey'});
          $dlgSurvey.html($(this).find('.hidden_data .interest_survey_result').html());
        }
      });

    },

    deactivate_user_view: function() {
      uid_filter = "";
  
      Self.ModOperations.refresh_comments();
    },
  
    refresh_col_header: function() {
      var headerHTML = "";
  
      if (uid_filter != '') {
        $("#comment_col_header").html('User - All');
      } else{
        if (active_domain_nid == 'all') {
          headerHTML = "All Proposals"
        } else{
          //If this is a proposal nid, use the proposal's name
          if ($('.proposal_header[nid=' + active_domain_nid + ']').html() != null) {
            headerHTML = $('.proposal_header[nid=' + active_domain_nid + ']').html();
          }
          //Otherwise this should be a topic nid, use its title from the filters
          else{
            headerHTML = $('.filter_header[nid='+active_domain_nid+"]").html();
          }
        }
        $("#comment_col_header").html(headerHTML + " - " + formal_filter_dictionary[active_domain_filter]);
      }
    },
  
    pager_refresh: function (items_per_page) {

      $("#comment_pager_top, #comment_pager_bottom").pagination({
          items: total_com,
          itemsOnPage: items_per_page,
          cssStyle: 'compact-theme'
      });

      $("#comment_pager_top, #comment_pager_bottom").pagination("redraw");

    }
  
  };

  Self.ModOperations = {
    change_limit: function (new_limit) {
      limit = new_limit;
      Self.ModOperations.refresh_comments();
    },
  
    change_order: function (new_order) {
      order = new_order;
      Self.ModOperations.refresh_comments();
    },
  
    change_order_sup: function (new_order_sup) {
      if (new_order_sup == order_sup) {
        $('#' + new_order_sup + '_toggle').attr('class', 'order_sup_deactivated');
        order_sup = "";
      } else{
        if (order_sup != "") {
          $('#' + order_sup + '_toggle').attr('class', 'order_sup_dectivated');
        }
        $('#' + new_order_sup + '_toggle').attr('class', 'order_sup_activated');
        order_sup = new_order_sup;
  
      }
      Self.ModOperations.refresh_comments();
    },
  
    mod_action: function(action, cid) {
      var token,
        comment_el = Self.ModUI.get_comment_el(cid),
        com_position = comment_el.find('.page_comment_index').html(),
        data = {};
  
      //Array of currently visible comments
      var comment_to_position = Self.ModUI.get_visible_comments_with_position();
  
      if (action == "edit_reply_input" || action == "add_note_input" || action == "redact_input") {
        Self.ModUI.mod_action_input(action, cid);
        return;
      } else if (action == "show_original_comment") {
        Self.ModUI.show_original_comment(cid, 'show');
        return;
      }
  
      //Get tokenstr for this comment
      $('.mod_panel_comment').each(function() {
        if ($(this).attr('cid') == cid) {
          token = $(this).attr('token');
        }
      });
  
      var url = "/moderator/comment/" + cid + "/action?token=" + token + "&action=" + action + "&active_domain_nid=" + active_domain_nid + "&uid_filter=" + uid_filter + "&order=" + order +"&order_sup=" + order_sup + "&domain_filter=" + active_domain_filter
        + "&offset=" + offset + "&limit=" + limit;
  
      for(var comment in comment_to_position) {
        url += "&" + comment + "=" + comment_to_position[comment];
      }
  
      //Error checking for input fields
      var errorText = "";
  
      if (action == "add_note" || action == "redact" || action == "reply" || action == "edit_reply") {
        var $inputArea = comment_el.find('textarea:first');
        var val = $inputArea.val();
  
        if (action == "add_note") {
          
          // Email note feature
          if (comment_el.find('.email_fields').css("display") == "block") {
            var re = new RegExp("[A-Za-z0-9._%+-]+@[A-Za-z0-9.-]+\.[A-Za-z]{2,4}");
            var note_to = comment_el.find('.note_to').val();
            var note_subject = comment_el.find('.note_subject').val();
            if (!re.test(note_to)) {
              errorText = "Please provide a valid email address to send the note to.";
            } else if (note_subject.length > 100) {
              errorText = "Please limit the email subject to 100 characters.";
            }
            else {
              url += "&note_to=" + note_to + "&note_subject=" + note_subject;
            }
          }
  
          if (val == null || val.length <= 0) {
            errorText = "Notes cannot be empty.";
          }
  
        } else if (action == "redact") {
          if (val == null ||  val.length <= 0) {
            errorText = "Comments cannot be empty.";
          }
        } else if (action == "edit_reply" || action == "reply") {
          if (val == null ||  val.length <= 0) {
            errorText = "Replies cannot be empty.";
          }
        }
  
        if (action == "edit_reply") {
          url += "&reply_cid=" + comment_el.find('.reply_cid').html();
        }
  
        //url += "&input=" + val;
        data.input = val;
      }
  
      //alert(url);
  
      if (errorText == "") {
        progressIndicatorSelector = action === 'restore_original' ? 
            '.loading_restore_original .loading_gif' :   
            '.loading_p .loading_gif';
        comment_el.find(progressIndicatorSelector).show();

        //console.log(url);
        $.post(url,data,function(result) {
          //Check the status for access
          if (result["status"] == "access denied") {
            alert("Access denied.  Please try logging in again.");
          }
  
          //Refresh statistics
          for (var nodeKey in result["node_stats"]) {
            if (result["node_stats"].hasOwnProperty(nodeKey)) {
              for (var statKey in result["node_stats"][nodeKey]) {
                if (result["node_stats"][nodeKey].hasOwnProperty(statKey)) {
                  $(".radio_filters[nid=" + nodeKey +"]").find("label[for=radio_" + statKey + "]").find('span.comment_count_inactive, span.comment_count_active').html(result["node_stats"][nodeKey][statKey])
                }
              }
            }
          }
  
          total_com = result['total_com'];
		  
          if (!hold_pager_refresh) {
            Self.ModUI.pager_refresh(limit);
          }
          hold_pager_refresh = false;

		  var delay=0;
		  if (total_com < $('.mod_panel_comment').length) {
				delay = 2000;
		  }
	  
          Self.ModUI.get_comment_el(cid).fadeOut( delay, function() {
			  $("#comment_col").html(result['comment_HTML']);
	  
			  //Show message if comment was removed due to filter
			  var com_pos_reached = true;
			  $('.mod_panel_comment').each(function() {
				var cur_pos = $(this).find('.page_comment_index').html();
				if (cur_pos == com_position ) {
				  com_pos_reached = false;
                  if ($(this).attr('cid') != cid) {
                    Self.ModUI.show_comment_removed(com_position);
                  }
                }
			  });
			  if (com_pos_reached) {
				Self.ModUI.show_comment_removed(com_position);
			  }
	  
			  //Reset some animations
			  comment_el.find('.loading_gif').css("display","none");
			  SP.jQuery(".comment_notes_dropdown").multiAccordion({active: 'none'});
			  Self.ModUI.apply_hover_effects();
			  Self.ModUI.apply_onclick_events()	
		
        });		    

    	},'json');
      } else{
        Self.ModUI.get_comment_el(cid).find('.input_mod_error_span').html(errorText);
      }
    },
  
    refresh_comments: function () {
      jQuery("#comment_col").css("opacity", "0.4");
      if (!hold_pager_refresh) {
        offset = 0;
      }
      
		url = "/moderator/ajax?cmd=comment_refresh&order=" + order +"&order_sup=" + order_sup + "&active_domain_nid=" + active_domain_nid + "&domain_filter=" + active_domain_filter
        + "&offset=" + offset + "&limit=" + limit + "&uid_filter=" + uid_filter;
		//console.log(url);
      $.getJSON(url,function(result) {
  
        total_com = result['total_com'];
        if (!hold_pager_refresh) {
          Self.ModUI.pager_refresh(limit);
        }
        hold_pager_refresh = false;
  
        $("#comment_col").html(result['comment_HTML']);
        $("#comment_col").css("opacity", "1.0");
        SP.jQuery(".comment_notes_dropdown").multiAccordion({active: 'none'});
        Self.ModUI.apply_hover_effects();
        Self.ModUI.apply_onclick_events();
        Self.ModUI.refresh_col_header();
        $(".region-content").goTo;
      });
    },
  
    change_offset: function (new_offset) {
      //$(".region-content").goTo();
      hold_pager_refresh = true;
      offset = new_offset;
  
      Self.ModOperations.refresh_comments();
    }
  
  };


  /**
   * simplePagination.js v1.4
   * A simple jQuery pagination plugin.
   * http://flaviusmatis.github.com/simplePagination.js/
   *
   * Copyright 2012, Flavius Matis
   * Released under the MIT license.
   * http://flaviusmatis.github.com/license.html
   */
  var methods = {
    init: function(options) {
      var o = $.extend({
        items: 1,
        itemsOnPage: 1,
        pages: 0,
        displayedPages: 5,
        edges: 2,
        currentPage: 1,
        hrefTextPrefix: '#page-',
        hrefTextSuffix: '',
        prevText: 'Prev',
        nextText: 'Next',
        ellipseText: '&hellip;',
        cssStyle: 'light-theme',
        selectOnClick: true,
        onPageClick: function(pageNumber, event) {
          // Callback triggered when a page is clicked
          // Page number is given as an optional parameter
        },
        onInit: function() {
          // Callback triggered immediately after initialization
        }
      }, options || {});

      var self = this;

      o.pages = o.pages ? o.pages : Math.ceil(o.items / o.itemsOnPage) ? Math.ceil(o.items / o.itemsOnPage) : 1;
      o.currentPage = o.currentPage - 1;
      o.halfDisplayed = o.displayedPages / 2;

      this.each(function() {
        self.addClass(o.cssStyle).data('pagination', o);
        methods._draw.call(self);
      });

      o.onInit();

      return this;
    },

    selectPage: function(page) {
      methods._selectPage.call(this, page - 1);
      return this;
    },

    prevPage: function() {
      var o = this.data('pagination');
      if (o.currentPage > 0) {
        methods._selectPage.call(this, o.currentPage - 1);
      }
      return this;
    },

    nextPage: function() {
      var o = this.data('pagination');
      if (o.currentPage < o.pages - 1) {
        methods._selectPage.call(this, o.currentPage + 1);
      }
      return this;
    },

    getPagesCount: function() {
      return this.data('pagination').pages;
    },

    getCurrentPage: function () {
      return this.data('pagination').currentPage + 1;
    },

    destroy: function() {
      this.empty();
      return this;
    },

    redraw: function() {
      methods._draw.call(this);
      return this;
    },

    disable: function() {
      var o = this.data('pagination');
      o.disabled = true;
      this.data('pagination', o);
      methods._draw.call(this);
      return this;
    },

    enable: function() {
      var o = this.data('pagination');
      o.disabled = false;
      this.data('pagination', o);
      methods._draw.call(this);
      return this;
    },

    _draw: function() {
      var $panel = this,
        o = $panel.data('pagination'),
        interval = methods._getInterval(o),
        i;

      methods.destroy.call(this);

      // Generate Prev link
      if (o.prevText) {
        methods._appendItem.call(this, o.currentPage - 1, {text: o.prevText, classes: 'prev'});
      }

      // Generate start edges
      if (interval.start > 0 && o.edges > 0) {
        var end = Math.min(o.edges, interval.start);
        for (i = 0; i < end; i++) {
          methods._appendItem.call(this, i);
        }
        if (o.edges < interval.start && (interval.start - o.edges != 1)) {
          $panel.append('<span class="ellipse">' + o.ellipseText + '</span>');
        } else if (interval.start - o.edges == 1) {
          methods._appendItem.call(this, o.edges);
        }
      }

      // Generate interval links
      for (i = interval.start; i < interval.end; i++) {
        methods._appendItem.call(this, i);
      }

      // Generate end edges
      if (interval.end < o.pages && o.edges > 0) {
        if (o.pages - o.edges > interval.end && (o.pages - o.edges - interval.end != 1)) {
          $panel.append('<span class="ellipse">' + o.ellipseText + '</span>');
        } else if (o.pages - o.edges - interval.end == 1) {
          methods._appendItem.call(this, interval.end++);
        }
        var begin = Math.max(o.pages - o.edges, interval.end);
        for (i = begin; i < o.pages; i++) {
          methods._appendItem.call(this, i);
        }
      }

      // Generate Next link
      if (o.nextText) {
        methods._appendItem.call(this, o.currentPage + 1, {text: o.nextText, classes: 'next'});
      }
    },

    _getInterval: function(o) {
      return {
        start: Math.ceil(o.currentPage > o.halfDisplayed ? Math.max(Math.min(o.currentPage - o.halfDisplayed, (o.pages - o.displayedPages)), 0) : 0),
        end: Math.ceil(o.currentPage > o.halfDisplayed ? Math.min(o.currentPage + o.halfDisplayed, o.pages) : Math.min(o.displayedPages, o.pages))
      };
    },

    _appendItem: function(pageIndex, opts) {
      var self = this, options, $link, o = self.data('pagination');

      pageIndex = pageIndex < 0 ? 0 : (pageIndex < o.pages ? pageIndex : o.pages - 1);

      options = $.extend({
        text: pageIndex + 1,
        classes: ''
      }, opts || {});

      if (pageIndex == o.currentPage || o.disabled) {
        $link = $('<span class="current">' + (options.text) + '</span>');
      } else {
        $link = $('<a href="' + o.hrefTextPrefix + (pageIndex + 1) + o.hrefTextSuffix + '" class="page-link">' + (options.text) + '</a>');
        $link.click(function(event) {
          return methods._selectPage.call(self, pageIndex, event);
        });
      }

      if (options.classes) {
        $link.addClass(options.classes);
      }

      self.append($link);
    },

    _selectPage: function(pageIndex, event) {
      var o = this.data('pagination');
      o.currentPage = pageIndex;
      if (o.selectOnClick) {
        methods._draw.call(this);
      }
      return o.onPageClick(pageIndex + 1, event);
    }

  };
  
  $.fn.pagination = function(method) {

    // Method calling logic
    if (methods[method] && method.charAt(0) != '_') {
      return methods[method].apply(this, Array.prototype.slice.call(arguments, 1));
    } else if (typeof method === 'object' || !method) {
      return methods.init.apply(this, arguments);
    } else {
      $.error('Method ' +  method + ' does not exist on jQuery.pagination');
    }

  };

  $.fn.goTo = function() {
    $('html, body').animate({
      scrollTop: $(this).offset().top + 'px'
    }, 'fast');
    return this; // for chaining...
  };

})(SP.ModeratorInterface = {}, SP.jQuery);
