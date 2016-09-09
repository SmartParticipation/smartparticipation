
(function (Self, $) {

  Self.init = function () {

    $('.rs-carousel').carousel({
      itemsPerTransition: 'auto',
      touch: Modernizr && Modernizr.touch,
      //translate3d:Modernizr && Modernizr.csstransforms3d,
      pagination: false,
      insertPrevAction: function () {
        return $('<a href="#" class="rs-carousel-action-prev"></a>').appendTo($(this).closest('.carousel-container').find('.carousel-prev'));
      },
      insertNextAction: function () {
        return $('<a href="#" class="rs-carousel-action-next"></a>').appendTo($(this).closest('.carousel-container').find('.carousel-next'));
      }
    }).show(400, function () {
      var $rsCarousel = $(this);

      // Show the paging controls if they are needed
      //if ($rsCarousel.carousel('getNoOfItems') > 5) {
      $rsCarousel.closest('.carousel-container').find('.paging-link').show();
      //} else {
      //  $rsCarousel.closest('.carousel-container').addClass('no-paging');
      //}

      $rsCarousel.carousel('refresh');
      refreshArrowState($rsCarousel);
    });

    $('.carousel-prompt').click(function () {
      $(this).next('.proposal-topics').fadeOut('slow').fadeIn('slow');
    });


    // Make carousel responsive
    $(window).resize(function () {
      $('.rs-carousel').carousel('refresh');
      $('.rs-carousel').each(function () {
        var $carousel = $(this);
        refreshArrowState($carousel);
      });
    });

    // Show paging arrows only if there is an active arrow (there is more than one page).
    var refreshArrowState = function ($carousel) {
      var noOfPages = $carousel.carousel('getNoOfPages');
      if (noOfPages === 1) {
        $carousel.closest('.carousel-container').find('.paging-link').hide();
      } else {
        $carousel.closest('.carousel-container').find('.paging-link').show();
      }
    }

  };

})(SP.Carousel = {}, SP.jQuery);