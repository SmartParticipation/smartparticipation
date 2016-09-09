/**
 * Script for proposal phase tabs on topic carousel.
 */

(function (Self, $) {

  Self.init = function () {

    // Handle proposal phase tabs states

    $('.proposal-phases ul.tabs').each(function () {
      // For each set of tabs, we want to keep track of
      // which tab is active and its associated content
      var $active,
        $content,
        $links = $(this).find('a');

      // If the location.hash matches one of the links, use that as the active tab.
      $active = $links.filter('[href="' + location.hash + '"]').first();

      // If the location.hash doesn't match a tab, use the designated active tab.
      if (!$active.length) {
        $active = $links.filter('.active');
      }

      // Failsafe: make the first tab active (this shouldn't happen, since the
      // controller also assigns a default active tab).
      if (!$active.length) {
        $active = $links.first();
      }

      $active.addClass('active');
      $content = $($active.attr('href'));

      // Hide the remaining content
      $links.not($active).each(function () {
        $($(this).attr('href')).hide();
      });

      // Bind the click event handler
      $(this).on('click', 'a', function (e) {
        // Make the old tab inactive.
        $active.removeClass('active');
        $content.hide();

        // Update the variables with the new link and content
        $active = $(this);
        $content = $($(this).attr('href'));

        // Make the tab active.
        $active.addClass('active');
        $content.show();

        $('.rs-carousel').carousel('refresh');

        // Prevent the anchor's default click action
        e.preventDefault();
      });
    });

  };

})(SP.ProposalPhaseTabs = {}, SP.jQuery);