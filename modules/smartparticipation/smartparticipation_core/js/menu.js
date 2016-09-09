/**
 * Script for extra behaviors for drop-down menus.
 */

/*SP.jQuery(document).ready(function () {
  SP.Menu.init();
});*/

(function (Self, $) {

  Self.init = function () {

    // Apply the notice behavior to the archive links in the closed proposals menu.
    $('li.archive a', '#nice-menu-main-menu').on('click', function () {

      var $link = $(this);

      openArchiveNotice($link);

      return false;

    });

    // Apply the notice behavior to the archive links in the closed proposals block.
    $('p.title span a.archive','#block-smartparticipation-core-closed-proposals').on('click', function () {

      var $link = $(this);

      openArchiveNotice($link);

      return false;

    });

    function openArchiveNotice($link) {

        var archive_url = $link.prop('href');

        $('#dlgArchive').rrDialog();

        $('button.continue', '#dlgArchive').off();

        $('button.continue', '#dlgArchive').on('click', function () {

            $('#dlgArchive').dialog('close');

            //window.location = archive_url;
            var win = window.open(archive_url, '_blank');
            win.focus();

        });

        $('a.cancel', '#dlgArchive').off();

        $('a.cancel', '#dlgArchive').on('click', function () {

            $('#dlgArchive').dialog('close');

            return false;

        });

    }

  };

})(SP.Menu = {}, SP.jQuery);