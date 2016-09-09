/*
 * Custom behaviors for the SmartParticipation comment box editor.
 */
CKEDITOR.plugins.add( 'smartparticipation', {
  init: function( editor ) {

    // Simplify the add link dialog (show only the URL field)
    CKEDITOR.on( 'dialogDefinition', function( ev )
    {
      var dialogName = ev.data.name;
      var dialogDefinition = ev.data.definition;

      if ( dialogName == 'link' )
      {
        var infoTab = dialogDefinition.getContents( 'info' );
        var urlOptions = infoTab.get('urlOptions');
        urlOptions.children[0].widths = ['0%','100%'];
        var linkType = infoTab.get('linkType');
        linkType.style = 'display:none';
        var protocol = infoTab.get('protocol');
        protocol.style = 'display:none';
        //infoTab.remove( 'linkType' );
        //infoTab.remove( 'protocol' );
      }

    });

    // Clear an error message when change happens in the comment box.
    var $editor = jQuery('#'+editor.name),
      $textareaWrapper = $editor.closest('.form-type-textarea'),
      hasError = $textareaWrapper.find('.field-error').length > 0 ? true : false;

    if (hasError) {
      editor.once('change', function () {
        var $commentForm = $editor.closest('form');
        SP.Form.clearFormErrors($commentForm);
      });
    }

    // Open the login required alert when clicking a comment editor.
    if (!SP.Login.isLoggedIn()) {
      editor.on('contentDom', function () {
        var editable = editor.editable();
        editable.attachListener(editable, 'mousedown', function () {
          SP.Topic.Comment.openLoginRequiredCommentAlert();
        });
      });
    }

  }
});
