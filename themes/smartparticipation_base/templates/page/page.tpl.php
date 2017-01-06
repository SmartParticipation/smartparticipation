<?php
/**
 * @file
 * smartparticipation_base's theme implementation to display a single Drupal page.
 *
 * The doctype, html, head and body tags are not in this template. Instead they
 * can be found in the html.tpl.php template normally located in the
 * modules/system directory.
 *
 * Available variables:
 *
 * General utility variables:
 * - $base_path: The base URL path of the Drupal installation. At the very
 *   least, this will always default to /.
 * - $directory: The directory the template is located in, e.g. modules/system
 *   or themes/regroom_base.
 * - $is_front: TRUE if the current page is the front page.
 * - $logged_in: TRUE if the user is registered and signed in.
 * - $is_admin: TRUE if the user has permission to access administration pages.
 * - $theme_path: path to theme directory
 * - $image_path: path to theme image directory
 * - $page_columns: total number of columns in the page grid
 * - $sidebar_first_columns: columns allotted to sidebar_first region
 * - $sidebar_second_columns: columns allotted to sidebar_second_region
 * 
 *
 * Site identity:
 * - $front_page: The URL of the front page. Use this instead of $base_path,
 *   when linking to the front page. This includes the language domain or
 *   prefix.
 * - $logo: The path to the logo image, as defined in theme configuration.
 * - $site_name: The name of the site, empty when display has been disabled
 *   in theme settings.
 * - $site_slogan: The slogan of the site, empty when display has been disabled
 *   in theme settings.
 * - $hide_site_name: TRUE if the site name has been toggled off on the theme
 *   settings page. If hidden, the "element-invisible" class is added to make
 *   the site name visually hidden, but still accessible.
 * - $hide_site_slogan: TRUE if the site slogan has been toggled off on the
 *   theme settings page. If hidden, the "element-invisible" class is added to
 *   make the site slogan visually hidden, but still accessible.
 * - $site_proposal_type: rule, plan, etc.
 * - $site_proposal_type_plural: rules, plans, etc.
 * 
 * Navigation:
 * - $main_menu (array): An array containing the Main menu links for the
 *   site, if they have been configured.
 * - $secondary_menu (array): An array containing the Secondary menu links for
 *   the site, if they have been configured.
 * - $breadcrumb: The breadcrumb trail for the current page.
 *
 * Page content (in order of occurrence in the default page.tpl.php):
 * - $title_prefix (array): An array containing additional output populated by
 *   modules, intended to be displayed in front of the main title tag that
 *   appears in the template.
 * - $title: The page title, for use in the actual HTML content.
 * - $title_suffix (array): An array containing additional output populated by
 *   modules, intended to be displayed after the main title tag that appears in
 *   the template.
 * - $messages: HTML for status and error messages. Should be displayed
 *   prominently.
 * - $tabs (array): Tabs linking to any sub-pages beneath the current page
 *   (e.g., the view and edit tabs when displaying a node).
 * - $action_links (array): Actions local to the page, such as 'Add menu' on the
 *   menu administration interface.
 * - $feed_icons: A string of all feed icons for the current page.
 * - $node: The node object, if there is an automatically-loaded node
 *   associated with the page, and the node ID is the second argument
 *   in the page's path (e.g. node/12345 and node/12345/revisions, but not
 *   comment/reply/12345).
 *
 * Regions:
 * - $page['header']: Items for the header region.
 * - $page['featured']: Items for the featured region. DISABLED
 * - $page['highlighted']: Items for the highlighted content region. DISABLED
 * - $page['help']: Dynamic help text, mostly for admin pages.
 * - $page['content']: The main content of the current page.
 * - $page['sidebar_first']: Items for the first sidebar. DISABLED
 * - $page['sidebar_second']: Items for the first sidebar. DISABLED
 * - $page['triptych_first']: Items for the first triptych. DISABLED
 * - $page['triptych_middle']: Items for the middle triptych. DISABLED
 * - $page['triptych_last']: Items for the last triptych. DISABLED
 * - $page['footer_firstcolumn']: Items for the first footer column.
 * - $page['footer_secondcolumn']: Items for the second footer column.
 * - $page['footer_thirdcolumn']: Items for the third footer column.
 * - $page['footer_fourthcolumn']: Items for the fourth footer column.
 * - $page['footer']: Items for the footer region.
 *
 * @see template_preprocess()
 * @see template_preprocess_page()
 * @see template_process()
 * @see smartparticipation_base_process_page()
 * @see html.tpl.php
 */


?>
<div class="container" id="page-container">

  <?php print theme('header', array('page_header' => $page['header'])); ?>

  <?php print theme('messages', array('messages' => $messages)); ?>

  <?php
  if($tabs) {
    print render($tabs);  
  }  
  ?>
  
  <?php // NB render() handles gracefully if the element doesn't exist ?>
  <?php print render($page['content']['smartparticipation_core_learn_panel']); ?>
  
  <section>
      <div class="row">

        <?php if (@$page['sidebar_first']): ?>
        <div class="span<?php print $sidebar_first_columns; ?>">
            <?php print render($page['sidebar_first']); ?>
        </div>
        <?php endif; ?>
        
        <div class="span<?php print $content_columns; ?>">
          <?php print render($title_prefix); ?>
          <?php if ($title): ?>
            <h1 class="title" id="page-title">
              <?php print $title; ?>
            </h1>
          <?php endif; ?>
          <?php print render($title_suffix); ?>
          <?php print render($page['content']); ?>
        </div>

        <?php if (@$page['sidebar_second']): ?>
        <div class="span<?php print $sidebar_second_columns; ?>">
          <?php print render($page['sidebar_second']); ?>
        </div>
        <?php endif; ?>

      </div>
  </section>

</div>

<?php print render($page['footer']); ?>