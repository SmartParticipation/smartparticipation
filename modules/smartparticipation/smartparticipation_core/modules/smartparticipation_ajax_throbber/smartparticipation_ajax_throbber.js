
(function ($) {
  Drupal.behaviors.ajaxThrobber = {
    attach: function (context, settings) {

      setInterval(function () {
        $('.ajax-progress .throbber').each(function() {
          if (!$(this).hasClass('animate')) {
            $(this).addClass('animate');
            startAnimation($(this));
          }
        });
      }, 100);

      function startAnimation($throbber) {
        var counter = 0;
        setInterval(function() {
          var frames=12; var frameWidth = 15;
          var offset=counter * -frameWidth;
          $throbber.css(
            'background-position',
            offset + "px" + " " + 0 + "px"
          );
          counter++; if (counter>=frames) counter =0;
        }, 120);
      }

    }
  };
}(jQuery));
