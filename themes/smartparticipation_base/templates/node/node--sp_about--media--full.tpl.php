<?php
/**
 * @file
 * regroom_base's theme implementation to display the Media
 * about node.
 * 
 * NB: The node must have title "Media" to invoke this template.
 *
 * Available variables:
 * - $title: the (sanitized) title of the node.
 * - $content: An array of node items. Use render($content) to print them all,
 *   or print a subset such as render($content['field_example']). Use
 *   hide($content['field_example']) to temporarily suppress the printing of a
 *   given element.
 * - $user_picture: The node author's picture from user-picture.tpl.php.
 * - $date: Formatted creation date. Preprocess functions can reformat it by
 *   calling format_date() with the desired parameters on the $created variable.
 * - $name: Themed username of node author output from theme_username().
 * - $node_url: Direct url of the current node.
 * - $display_submitted: Whether submission information should be displayed.
 * - $submitted: Submission information created from $name and $date during
 *   template_preprocess_node().
 * - $classes: String of classes that can be used to style contextually through
 *   CSS. It can be manipulated through the variable $classes_array from
 *   preprocess functions. The default values can be one or more of the
 *   following:
 *   - node: The current template type, i.e., "theming hook".
 *   - node-[type]: The current node type. For example, if the node is a
 *     "Blog entry" it would result in "node-blog". Note that the machine
 *     name will often be in a short form of the human readable label.
 *   - node-teaser: Nodes in teaser form.
 *   - node-preview: Nodes in preview mode.
 *   The following are controlled through the node publishing options.
 *   - node-promoted: Nodes promoted to the front page.
 *   - node-sticky: Nodes ordered above other non-sticky nodes in teaser
 *     listings.
 *   - node-unpublished: Unpublished nodes visible only to administrators.
 * - $title_prefix (array): An array containing additional output populated by
 *   modules, intended to be displayed in front of the main title tag that
 *   appears in the template.
 * - $title_suffix (array): An array containing additional output populated by
 *   modules, intended to be displayed after the main title tag that appears in
 *   the template.
 *
 * Other variables:
 * - $node: Full node object. Contains data that may not be safe.
 * - $type: Node type, i.e. story, page, blog, etc.
 * - $comment_count: Number of comments attached to the node.
 * - $uid: User ID of the node author.
 * - $created: Time the node was published formatted in Unix timestamp.
 * - $classes_array: Array of html class attribute values. It is flattened
 *   into a string within the variable $classes.
 * - $zebra: Outputs either "even" or "odd". Useful for zebra striping in
 *   teaser listings.
 * - $id: Position of the node. Increments each time it's output.
 *
 * Node status variables:
 * - $view_mode: View mode, e.g. 'full', 'teaser'...
 * - $teaser: Flag for the teaser state (shortcut for $view_mode == 'teaser').
 * - $page: Flag for the full page state.
 * - $promote: Flag for front page promotion state.
 * - $sticky: Flags for sticky post setting.
 * - $status: Flag for published status.
 * - $comment: State of comment settings for the node.
 * - $readmore: Flags true if the teaser content of the node cannot hold the
 *   main body content.
 * - $is_front: Flags true when presented in the front page.
 * - $logged_in: Flags true when the current user is a logged-in member.
 * - $is_admin: Flags true when the current user is an administrator.
 *
 * Field variables: for each field instance attached to the node a corresponding
 * variable is defined, e.g. $node->body becomes $body. When needing to access
 * a field's raw values, developers/themers are strongly encouraged to use these
 * variables. Otherwise they will have to explicitly specify the desired field
 * language, e.g. $node->body['en'], thus overriding any language negotiation
 * rule that was previously applied.
 *
 * @see template_preprocess()
 * @see template_preprocess_node()
 * @see template_process()
 */

?>

<div id="about-header" class="row">
  <div class="span10">

    <?php if (isset($content['field_image'])): ?>
      <h1 class="about-image-header"><?php print $title; ?></h1>
    
      <?php print render($content['field_image']); ?>
              
    <?php else: ?>
      <h1><?php print $title; ?></h1>
    <?php endif; ?>
  </div>
  
  <div class="span2">
    <?php // TODO This navigation needs to be dynamically generated ?>
    <nav id="about-pages">
      <ul>
        <li><a href="/about/overview" title="Overview">Overview</a></li>
        <li><a href="/about/history" title="History">History</a></li>
        <li><a href="/about/meet-team" title="Meet the Team">Meet the Team</a></li>
        <li><a href="/about/sponsors-and-partners" title="Sponsors &amp; Partners">Sponsors &amp; Partners</a></li>
        <li><a class="active" href="/about/media" title="Media">Media</a></li>
        <li><a href="/about/research-and-publications" title="Research &amp; Publications">Research &amp; Publications</a></li>   
      </ul>
    </nav>
  </div>
</div>


<?php 

print render($content['body']);

$items = $content['news_items'];

if (empty($items)) {
  ?><p class="no-content"><?php echo t('There are no news items to display.'); ?></p><?php
  
} else {
  $first_item = array_shift($items); 
  
?>

  <h2><?php echo t('Latest News'); ?></h2>
  
  <section id="new-media" class="row">
    <div class="span3">
        <?php print render($content['field_sp_secondary_image']); ?>
    </div>
    <div class="span9">
      <?php print render($first_item); ?>
    </div>
  </section>
  
<?php 

  if (!empty($items)) {
  
    ?><section id="all-media" class="row"><?php 
    
    foreach ($items as $item) {
      print render($item);
    }
  
    ?></section><?php 
  }
} 



