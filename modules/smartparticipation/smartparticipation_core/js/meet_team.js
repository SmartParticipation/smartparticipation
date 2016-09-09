
 
SP.jQuery(document).ready(function($) {

  return; // This is not in use - return to prevent conflicts with other carousels

  // Carousel for meet the team page
  $('.rs-carousel').carousel({
    
      nextPrevActions: true,
      itemsPerTransition:1,
      insertPrevAction: function () {
          return $('<a href="#">prev</a>').appendTo($(this).closest('.carousel-container').find('.carousel-prev'));
      },
      insertNextAction: function () {
          return $('<a href="#" class="rs-carousel-action-next"></a>').appendTo($(this).closest('.carousel-container').find('.carousel-next'));
      }
  }).show();
  
  
  $('ol.rs-carousel-pagination li').each(function( index ){
    $this = $(this)
    $this.click(function(){
      $(':rs-carousel').carousel('goToItem', index + 1)
    });
  });

  
  // Makes carousel responsive  
  $(window).resize(function () {
      //$('.rs-carousel').carousel('refresh');
      
      $(':rs-carousel').carousel('refresh');
  });


});