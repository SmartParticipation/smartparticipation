/**
 * Define RR namespace and store custom jQuery instance set by jQuery Multi
 */
 
var SP = {};

SP.jQuery = jq182;

/**
 * Wrapper function to SP.EventLogger.log, to prevent calls to an undefined
 * function when the JavaScript file is not loaded.
 */
SP.logEvent = function(data) {
  
  // console.log('logging ' + data.name);
  
  if (typeof SP.EventLogger !== 'undefined') {
    SP.EventLogger.log(data);
  }

};

SP.jQuery(document).ready(function($) {

  $(window).resize(function () {
    SP.Breakpoint.applyBodyClass();
  });

  SP.Breakpoint.init();

  if ($('#learn-accordion').length) {
    var learnAccordion = new SP.LearnAccordion('#learn-accordion', '#learn-accordion-mobile');
  }

  SP.Topic.init();
  SP.TopicHeader.init();
  SP.EventLogger.init();
  SP.Menu.init();
  SP.ProposalPhaseTabs.init();
  SP.InterestSurvey.init();
  SP.Carousel.init();

  // Truncate long text with a "more" link that shows full text.
  // To use: add truncate class to element to be truncated, and 
  // data-truncate-limit (a numeric value) to it or a parent element.
  $('.truncate').each(function() {
    var max = $(this).closest('[data-truncate-limit]').data('truncate-limit');
    if (max) {
      $(this).truncate({max_length: max});
    }
  });

  // Add glossary tooltip
  $('abbr.glossary').tooltip({container: 'body'});

  // Submenus on main nav should have the width of the parent element, if the parent link width 
  // is greater than the submenu width.
  $('ul.nice-menu-down li.menuparent a').mouseenter(function() {

    var $link = $(this),
        widthParentLink = $link.width(),
        $subMenu = $link.next(),
        widthSubmenu = $subMenu.width(),
        newWidthSubmenu = (widthParentLink <= widthSubmenu) ? 200 : widthParentLink;

    $subMenu.width(newWidthSubmenu + 'px');    
 
  });


  // Resize footer after window resizing or scrolling.

  /*function resizeFooterAfterWindowResize() {
    $('footer').css('width', $(window).width());
  }

  function resizeFooterAfterScroll() {
    var $window = $(window),
        windowsize = $window.width() + $window.scrollLeft();
    $('footer').css('width', windowsize);
   }*/

  /*resizeFooterAfterWindowResize();
  resizeFooterAfterScroll();

  $(window).resize(resizeFooterAfterWindowResize)
           .scroll(resizeFooterAfterScroll);*/
  
});
