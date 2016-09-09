/**
 * Script for topic header on topic and document pages.
 */

/*SP.jQuery(document).ready(function () {
  SP.TopicHeader.init();
});*/

(function (Self, $) {

  Self.init = function () {

    var $arrow = $('.expand-topics');
    var $topicHeaderMenu = $('#topic-header-menu'),
        //on drop down list for documents, there is an extra 1px padding right 
        $topicHeaderMenuDoc = $('.node-type-sp-document #topic-header-menu'),
        $selectOtherTopics = $('.select-other-topics a');
    
    // Expand/Collapse proposal summary
    var summaryContent = $('#proposal-summary-content');
    $('#proposal-summary-link').click(function(e){
        if ( !summaryContent.is(":visible") ) {
          $('#proposal-summary-link span').removeClass('expand-summary').addClass('collapse-summary');       
         } else { 
          $('#proposal-summary-link span').removeClass('collapse-summary').addClass('expand-summary');
         }
        summaryContent.slideToggle();
        e.preventDefault();        
    });   

    // Expand/Collapse dropdown topics menu  
    // MB: The width of the drop-down menu should be calculated depending on the width of the longest  
    // Submenu item.  
    $selectOtherTopics.click(function(e) {
      e.preventDefault();
      $topicHeaderMenu.toggle();

      //$dropdownWidth = parseInt($selectOtherTopics.width()) + parseInt($selectOtherTopics.css("padding-left")) + parseInt($selectOtherTopics.css("padding-right")) - 1;
      $dropdownWidth = 500;

      $topicHeaderMenu.css('width', ($dropdownWidth) +'px');

      $topicHeaderMenuDoc.css('width', ($dropdownWidth + 1) +'px');

      if ($topicHeaderMenu.is(':hidden')) {
          $(this).removeClass('active');
          $arrow.css('background-position','-29px -66px'); //exposes down arrow
      } else {
          $arrow.css('background-position','-50px -66px'); //exposes up arrow
          $(this).addClass('active');
      }     
    });

      $selectOtherTopics.blur(function() {
        if(!$topicHeaderMenu.is(":hover")) {
            if (!$topicHeaderMenu.is(':hidden')) {
                $(this).click();
            }
        }
    });

  };
  
})(SP.TopicHeader = {}, SP.jQuery);