<?php 

/**
 * Functions relating to the SmartParticipation WYSIWYG editor
 */

/**
 * Configure the WYSIWYG editor
 */
function smartparticipation_core_configure_wysiwyg() {

  $full_html_wysiwyg = array(
    'default' => 1,
    'user_choose' => 0,
    'show_toggle' => 1,
    'theme' => 'advanced',
    'language' => 'en',

    // NB Order determines display order!
    'buttons' => array(

      'default' => array(
        'formatselect' => 1,

        'image' => 1,

        'bold' => 1,
        'italic' => 1,
        'underline' => 1,
        'strikethrough' => 1,

        'bullist' => 1,
        'numlist' => 1,
        'indent' => 1,
        'outdent' => 1,

        'link' => 1,
        'unlink' => 1,
        'anchor' => 1,

        'sub' => 1,
        'sup' => 1,
        'charmap' => 1,

        'blockquote' => 1,
        'justifyleft' => 1,
        'justifycenter' => 1,
        'justifyright' => 1,
        'justifyfull' => 1,

        'removeformat' => 1,

        'undo' => 1,
        'redo' => 1,

        'copy' => 1,
        'cut' => 1,
        'paste' => 1,

      ),

      'drupal' => array(),

      'paste' => array(
        'pasteword' => 1,
        'pastetext' => 1,
      ),

      'fullscreen' => array(
        'fullscreen' => 1,
      ),

      'spellchecker' => array(
        'spellchecker' => 1,
      ),

      'imce' => array(
        'imce' => 1,
      ),

    ),

    'toolbar_loc' => 'top',
    'toolbar_align' => 'left',
    'path_loc' => 'bottom',
    'resizing' => 1,
    'verify_html' => 1,
    'preformatted' => 0,
    'convert_fonts_to_spans' => 1,
    'remove_linebreaks' => 1,
    'apply_source_formatting' => 0,
    'paste_auto_cleanup_on_paste' => 0,
    'block_formats' => 'p,address,pre,h2,h3,h4,h5,h6,div',
    'css_setting' => 'theme',
    'css_path' => '',
    'css_classes' => '',
  );

  $full_html_wysiwyg_ckeditor = array (
      'default' => 1,
      'user_choose' => 0,
      'show_toggle' => 1,
      'add_to_summaries' => 1,
      'theme' => '',
      'language' => 'en',
      'buttons' =>
          array (
              'default' =>
                  array (
                      'Bold' => 1,
                      'Italic' => 1,
                      'Underline' => 1,
                      'Strike' => 1,
                      'JustifyLeft' => 1,
                      'JustifyCenter' => 1,
                      'JustifyRight' => 1,
                      'JustifyBlock' => 1,
                      'BulletedList' => 1,
                      'NumberedList' => 1,
                      'Outdent' => 1,
                      'Indent' => 1,
                      'Undo' => 1,
                      'Redo' => 1,
                      'Link' => 1,
                      'Unlink' => 1,
                      'Anchor' => 1,
                      'Image' => 1,
                      'Superscript' => 1,
                      'Subscript' => 1,
                      'Blockquote' => 1,
                      'Source' => 1,
                      'Cut' => 1,
                      'Copy' => 1,
                      'Paste' => 1,
                      'PasteText' => 1,
                      'PasteFromWord' => 1,
                      'RemoveFormat' => 1,
                      'SpecialChar' => 1,
                      'Format' => 1,
                      'Scayt' => 1,
                  ),
              'imce' =>
                  array (
                      'imce' => 1,
                  ),
          ),
      'toolbarLocation' => 'top',
      'resize_enabled' => 1,
      'default_toolbar_grouping' => 0,
      'simple_source_formatting' => 0,
      'acf_mode' => '0',
      'acf_allowed_content' => '',
      'css_setting' => 'theme',
      'css_path' => '',
      'stylesSet' => '',
      'block_formats' => 'p,address,pre,h2,h3,h4,h5,h6,div',
      'advanced__active_tab' => 'edit-basic',
      'forcePasteAsPlainText' => 0,
  );


  // TODO Should this be done with a call to a wysiwyg module api function
  // instead of a direct database insert?
  //$wysiwyg = db_insert('wysiwyg')->fields(array(
  //   'format' => 'full_html',
  //  'editor' => 'tinymce',
  //    'settings' => serialize($full_html_wysiwyg)
  //  ))
  //  ->execute();

  db_merge('wysiwyg')
      ->key(array('format' => 'full_html'))
      ->fields(array(
          'editor' => 'ckeditor',
          'settings' => serialize($full_html_wysiwyg_ckeditor),
      ))
      ->execute();

}


/**
 * Remove the settings for the WYSIWYG editor. 
 */
function smartparticipation_core_unconfigure_wysiwyg() {
  
  db_delete('wysiwyg')
    ->condition('format', 'full_html')
    ->execute();

}


function smartparticipation_core_wysiwyg_editor_settings_alter(&$settings, $context) {

  $profile = $context['profile'];
  $editor = $profile->editor;
  $format = $profile->format;

  if ($editor == 'ckeditor') {

    if ($format == 'comments') {

      // Plugins tabletools, liststyle, contextmenu are disabled to allow use
      // of the browser native context menu.  This is done so that the spell
      // check word replacements will be available for selection, whereas these
      // would not be listed in the CKEditor context menu.
      $settings['removePlugins'] = 'tabletools,liststyle,contextmenu,elementspath';
      // The confighelper plugin is used to set a placeholder value.
      $settings['extraPlugins'] = 'confighelper,smartparticipation';

      $settings['height'] = 100;

      // Disable extra tabs in link dialog.
      $settings['linkShowTargetTab'] = FALSE;
      $settings['linkShowAdvancedTab'] = FALSE;
      $settings['dialog_noConfirmCancel'] = TRUE;

      // Enable the native browser spell check
      $settings['disableNativeSpellChecker'] = FALSE;

    } else if ($format == 'full_html') {

      // Enable SCAYT by default
      $settings['scayt_autoStartup'] = TRUE;

    }

  }

}