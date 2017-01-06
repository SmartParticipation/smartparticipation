<?php
/**
 * @file
 * smartparticipation_base's theme implementation to display an about node.
 * 
 * NB: The node must have title "Meet the Team" to invoke this template.
 *
 * Available variables:
 * - $title: the (sanitized) title of the node.
 * - $content: An array of node items. Use render($content) to print them all,
 *  or print a subset such as render($content['field_example']). Use
 *  hide($content['field_example']) to temporarily suppress the printing of a
 *  given element.
 * - $user_picture: The node author's picture from user-picture.tpl.php.
 * - $date: Formatted creation date. Preprocess functions can reformat it by
 *  calling format_date() with the desired parameters on the $created variable.
 * - $name: Themed username of node author output from theme_username().
 * - $node_url: Direct url of the current node.
 * - $display_submitted: Whether submission information should be displayed.
 * - $submitted: Submission information created from $name and $date during
 *  template_preprocess_node().
 * - $classes: String of classes that can be used to style contextually through
 *  CSS. It can be manipulated through the variable $classes_array from
 *  preprocess functions. The default values can be one or more of the
 *  following:
 *  - node: The current template type, i.e., "theming hook".
 *  - node-[type]: The current node type. For example, if the node is a
 *   "Blog entry" it would result in "node-blog". Note that the machine
 *   name will often be in a short form of the human readable label.
 *  - node-teaser: Nodes in teaser form.
 *  - node-preview: Nodes in preview mode.
 *  The following are controlled through the node publishing options.
 *  - node-promoted: Nodes promoted to the front page.
 *  - node-sticky: Nodes ordered above other non-sticky nodes in teaser
 *   listings.
 *  - node-unpublished: Unpublished nodes visible only to administrators.
 * - $title_prefix (array): An array containing additional output populated by
 *  modules, intended to be displayed in front of the main title tag that
 *  appears in the template.
 * - $title_suffix (array): An array containing additional output populated by
 *  modules, intended to be displayed after the main title tag that appears in
 *  the template.
 *
 * Other variables:
 * - $node: Full node object. Contains data that may not be safe.
 * - $type: Node type, i.e. story, page, blog, etc.
 * - $comment_count: Number of comments attached to the node.
 * - $uid: User ID of the node author.
 * - $created: Time the node was published formatted in Unix timestamp.
 * - $classes_array: Array of html class attribute values. It is flattened
 *  into a string within the variable $classes.
 * - $zebra: Outputs either "even" or "odd". Useful for zebra striping in
 *  teaser listings.
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
 *  main body content.
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
  
  <?php /* ?>
  <!-- Slider -->
  <!-- TODO The carousel needs to be dynamically generated using the markup shown here -->
  <!--<div id="meet-team" class="carousel-container"> 
   <div class="carousel-prev paging-link"></div>
   <div class="carousel-next paging-link"></div>
   <div class="carousel-items">
    <div class="rs-carousel module">
     <ul class="rs-carousel-runner">
      <li class="rs-carousel-item">

       <h2 class="team-member">Person 1</h2>
       <p class="bio-member">Lorem ipsum dolor sit amet, consectetur adipiscing elit. Proin sit <br />
        amet eros et justo facilisis fringilla sed ac sem. Donec sodales <br />commodo ultricies. Etiam sed massa eu ante iaculis convallis <br />sed eu nisl. Cras commodo ligula at mi viverra non lobortis <br />metus pharetra. <a class="url-member" href="#" title="">website url</a></p>
       <img class="photo-member" src ="/sites/all/themes/regroom_base/images/about/meet-the-team/meet-the-team.jpg" alt=""/>

      </li>
      <li class="rs-carousel-item">

       <h2 class="team-member">Person 2</h2>
       <p class="bio-member">Pellentesque pulvinar, dolor dignissim gravida hendrerit, mauris lectus mattis nibh, eget ultricies ligula ipsum quis justo. Sed arcu purus, accumsan vel molestie at, dictum ac nibh. Sed quis dolor quis lorem ultrices varius. Sed massa nisi, lobortis consectetur tempor ut, pellentesque ac erat. Aliquam aliquam varius mauris id rutrum. Nunc bibendum diam vitae justo porta placerat. Donec suscipit pharetra tortor quis consectetur. <a class="url-member" href="#" title="">website url</a></p>
       <img class="photo-member" src ="/sites/all/themes/regroom_base/images/about/meet-the-team/meet-the-team.jpg" alt=""/>

      </li>
      <li class="rs-carousel-item">

       <h2 class="team-member">Person 3</h2>
       <p class="bio-member">Lorem ipsum dolor sit amet, consectetur adipiscing elit. Proin sit amet eros et justo facilisis fringilla sed ac sem. Donec sodales commodo ultricies. Etiam sed massa eu ante iaculis convallis sed eu nisl. Cras commodo ligula at mi viverra non lobortis metus pharetra. <a class="url-member" href="#" title="">website url</a></p>
       <img class="photo-member" src ="/sites/all/themes/regroom_base/images/about/meet-the-team/meet-the-team.jpg" alt=""/>

      </li>

      <li class="rs-carousel-item">

       <h2 class="team-member">Person 4</h2>
       <p class="bio-member">Pellentesque pulvinar, dolor dignissim gravida hendrerit, mauris lectus mattis nibh, eget ultricies ligula ipsum quis justo. Sed arcu purus, accumsan vel molestie at, dictum ac nibh. Sed quis dolor quis lorem ultrices varius. Sed massa nisi, lobortis consectetur tempor ut, pellentesque ac erat. Aliquam aliquam varius mauris id rutrum. Nunc bibendum diam vitae justo porta placerat. Donec suscipit pharetra tortor quis consectetur. <a class="url-member" href="#" title="">website url</a></p>
       <img class="photo-member" src ="/sites/all/themes/regroom_base/images/about/meet-the-team/meet-the-team.jpg" alt=""/>

      </li>
      <li class="rs-carousel-item">

       <h2 class="team-member">Austin Eustice</h2>
       <p class="bio-member">is the lead designer for Cornell’s eRulemaking Initiative project, RegulationRoom. He has worked on projects for design agencies, educational organizations, and creative professionals across the country and brings a passion for clean design and user interface to all his creative executions. He specializes in identity work, print and web design, and illustration and is currently available for freelance projects. <a class="url-member" href="#" title="">website url</a></p>
       <img class="photo-member" src ="/sites/all/themes/regroom_base/images/about/meet-the-team/meet-the-team.jpg" alt=""/>

      </li>
      <li class="rs-carousel-item">

       <h2 class="team-member">Person 6</h2>
       <p class="bio-member">Lorem ipsum dolor sit amet, consectetur adipiscing elit. Proin sit amet eros et justo facilisis fringilla sed ac sem. Donec sodales commodo ultricies. Etiam sed massa eu ante iaculis convallis sed eu nisl. Cras commodo ligula at mi viverra non lobortis metus pharetra. <a class="url-member" href="#" title="">website url</a></p>
       <img class="photo-member" src ="/sites/all/themes/regroom_base/images/about/meet-the-team/meet-the-team.jpg" alt=""/>

      </li>
      <li class="rs-carousel-item">

       <h2 class="team-member">Person 7</h2>
       <p class="bio-member">Pellentesque pulvinar, dolor dignissim gravida hendrerit, mauris lectus mattis nibh, eget ultricies ligula ipsum quis justo. Sed arcu purus, accumsan vel molestie at, dictum ac nibh. Sed quis dolor quis lorem ultrices varius. Sed massa nisi, lobortis consectetur tempor ut, pellentesque ac erat. Aliquam aliquam varius mauris id rutrum. Nunc bibendum diam vitae justo porta placerat. Donec suscipit pharetra tortor quis consectetur. <a class="url-member" href="#" title="">website url</a></p>
       <img class="photo-member" src ="/sites/all/themes/regroom_base/images/about/meet-the-team/meet-the-team.jpg" alt=""/>

      </li>
      <li class="rs-carousel-item">

       <h2 class="team-member">Person 8</h2>
       <p class="bio-member">Lorem ipsum dolor sit amet, consectetur adipiscing elit. Proin sit amet eros et justo facilisis fringilla sed ac sem. Donec sodales commodo ultricies. Etiam sed massa eu ante iaculis convallis sed eu nisl. Cras commodo ligula at mi viverra non lobortis metus pharetra. <a class="url-member" href="#" title="">website url</a></p>
       <img class="photo-member" src ="/sites/all/themes/regroom_base/images/about/meet-the-team/meet-the-team.jpg" alt=""/>

      </li>

      <li class="rs-carousel-item">

       <h2 class="team-member">Person 9</h2>
       <p class="bio-member">Pellentesque pulvinar, dolor dignissim gravida hendrerit, mauris lectus mattis nibh, eget ultricies ligula ipsum quis justo. Sed arcu purus, accumsan vel molestie at, dictum ac nibh. Sed quis dolor quis lorem ultrices varius. Sed massa nisi, lobortis consectetur tempor ut, pellentesque ac erat. Aliquam aliquam varius mauris id rutrum. Nunc bibendum diam vitae justo porta placerat. Donec suscipit pharetra tortor quis consectetur. <a class="url-member" href="#" title="">website url</a></p>
       <img class="photo-member" src ="/sites/all/themes/regroom_base/images/about/meet-the-team/meet-the-team.jpg" alt=""/>

      </li>
      
      <li class="rs-carousel-item">

       <h2 class="team-member">Person 10</h2>
       <p class="bio-member">Pellentesque pulvinar, dolor dignissim gravida hendrerit, mauris lectus mattis nibh, eget ultricies ligula ipsum quis justo. Sed arcu purus, accumsan vel molestie at, dictum ac nibh. Sed quis dolor quis lorem ultrices varius. Sed massa nisi, lobortis consectetur tempor ut, pellentesque ac erat. Aliquam aliquam varius mauris id rutrum. Nunc bibendum diam vitae justo porta placerat. Donec suscipit pharetra tortor quis consectetur. <a class="url-member" href="#" title="">website url</a></p>
       <img class="photo-member" src ="/sites/all/themes/regroom_base/images/about/meet-the-team/meet-the-team.jpg" alt=""/>

      </li>
      <li class="rs-carousel-item">

       <h2 class="team-member">Person 11</h2>
       <p class="bio-member">Pellentesque pulvinar, dolor dignissim gravida hendrerit, mauris lectus mattis nibh, eget ultricies ligula ipsum quis justo. Sed arcu purus, accumsan vel molestie at, dictum ac nibh. Sed quis dolor quis lorem ultrices varius. Sed massa nisi, lobortis consectetur tempor ut, pellentesque ac erat. Aliquam aliquam varius mauris id rutrum. Nunc bibendum diam vitae justo porta placerat. Donec suscipit pharetra tortor quis consectetur. <a class="url-member" href="#" title="">website url</a></p>
       <img class="photo-member" src ="/sites/all/themes/regroom_base/images/about/meet-the-team/meet-the-team.jpg" alt=""/>

      </li>

      <li class="rs-carousel-item">

       <h2 class="team-member">Person 12</h2>
       <p class="bio-member">Pellentesque pulvinar, dolor dignissim gravida hendrerit, mauris lectus mattis nibh, eget ultricies ligula ipsum quis justo. Sed arcu purus, accumsan vel molestie at, dictum ac nibh. Sed quis dolor quis lorem ultrices varius. Sed massa nisi, lobortis consectetur tempor ut, pellentesque ac erat. Aliquam aliquam varius mauris id rutrum. Nunc bibendum diam vitae justo porta placerat. Donec suscipit pharetra tortor quis consectetur. <a class="url-member" href="#" title="">website url</a></p>
       <img class="photo-member" src ="/sites/all/themes/regroom_base/images/about/meet-the-team/meet-the-team.jpg" alt=""/>

      </li>
      <li class="rs-carousel-item">

       <h2 class="team-member">Person 13</h2>
       <p class="bio-member">Pellentesque pulvinar, dolor dignissim gravida hendrerit, mauris lectus mattis nibh, eget ultricies ligula ipsum quis justo. Sed arcu purus, accumsan vel molestie at, dictum ac nibh. Sed quis dolor quis lorem ultrices varius. Sed massa nisi, lobortis consectetur tempor ut, pellentesque ac erat. Aliquam aliquam varius mauris id rutrum. Nunc bibendum diam vitae justo porta placerat. Donec suscipit pharetra tortor quis consectetur. <a class="url-member" href="#" title="">website url</a></p>
       <img class="photo-member" src ="/sites/all/themes/regroom_base/images/about/meet-the-team/meet-the-team.jpg" alt=""/>

      </li>      
      <li class="rs-carousel-item">

       <h2 class="team-member">Person 14</h2>
       <p class="bio-member">Pellentesque pulvinar, dolor dignissim gravida hendrerit, mauris lectus mattis nibh, eget ultricies ligula ipsum quis justo. Sed arcu purus, accumsan vel molestie at, dictum ac nibh. Sed quis dolor quis lorem ultrices varius. Sed massa nisi, lobortis consectetur tempor ut, pellentesque ac erat. Aliquam aliquam varius mauris id rutrum. Nunc bibendum diam vitae justo porta placerat. Donec suscipit pharetra tortor quis consectetur. <a class="url-member" href="#" title="">website url</a></p>
       <img class="photo-member" src ="/sites/all/themes/regroom_base/images/about/meet-the-team/meet-the-team.jpg" alt=""/>

      </li>      
     </ul>
    </div>
    <ol id="meet-team	" class="rs-carousel-pagination">
     <li class="rs-carousel-pagination-link">
      <a href="#page-1"><img src="/sites/all/themes/regroom_base/images/about/meet-the-team/people-team_01.gif" /></a>
     </li>
     <li class="rs-carousel-pagination-link">
      <a href="#page-2"><img src="/sites/all/themes/regroom_base/images/about/meet-the-team/people-team_02.gif" /></a>
     </li>
     <li class="rs-carousel-pagination-link">
      <a href="#page-3"><img src="/sites/all/themes/regroom_base/images/about/meet-the-team/people-team_03.gif" /></a>
     </li>
     <li class="rs-carousel-pagination-link">
      <a href="#"><img src="/sites/all/themes/regroom_base/images/about/meet-the-team/people-team_04.gif" /></a>
     </li>
     <li class="rs-carousel-pagination-link">
      <a href="#"><img src="/sites/all/themes/regroom_base/images/about/meet-the-team/people-team_05.gif" /></a>
     </li>
     <li class="rs-carousel-pagination-link">
      <a href="#"><img src="/sites/all/themes/regroom_base/images/about/meet-the-team/people-team_06.gif" /></a>
     </li>
     <li class="rs-carousel-pagination-link">
      <a href="#"><img src="/sites/all/themes/regroom_base/images/about/meet-the-team/people-team_07.gif" /></a>
     </li>
     <li class="rs-carousel-pagination-link">
      <a href="#"><img src="/sites/all/themes/regroom_base/images/about/meet-the-team/people-team_08.gif" /></a>
     </li>
     <li class="rs-carousel-pagination-link">
      <a href="#"><img src="/sites/all/themes/regroom_base/images/about/meet-the-team/people-team_09.gif" /></a>
     </li>
     <li class="rs-carousel-pagination-link">
      <a href="#"><img src="/sites/all/themes/regroom_base/images/about/meet-the-team/people-team_10.gif" /></a>
     </li>
     <li class="rs-carousel-pagination-link">
      <a href="#"><img src="/sites/all/themes/regroom_base/images/about/meet-the-team/people-team_11.gif" /></a>
     </li>
     <li class="rs-carousel-pagination-link">
      <a href="#"><img src="/sites/all/themes/regroom_base/images/about/meet-the-team/people-team_12.gif" /></a>
     </li>
     <li class="rs-carousel-pagination-link">
      <a href="#"><img src="/sites/all/themes/regroom_base/images/about/meet-the-team/people-team_13.gif" /></a>
     </li>
     <li class="rs-carousel-pagination-link">
      <a href="#"><img src="/sites/all/themes/regroom_base/images/about/meet-the-team/people-team_14.gif" /></a>
     </li>
    </ol> 
   </div>
  </div>
  <?php */ ?>
  
</div>
 
<!-- Nav -->
<div class="span2">
  <?php // TODO This navigation needs to be dynamically generated ?>
  <nav id="about-pages">
    <ul>
      <li><a href="/about/overview" title="Overview">Overview</a></li>
      <li><a href="/about/history" title="History">History</a></li>
      <li><a class="active" href="/about/meet-team" title="Meet the Team">Meet the Team</a></li>
      <li><a href="/about/sponsors-and-partners" title="Sponsors &amp; Partners">Sponsors &amp; Partners</a></li>
      <li><a href="/about/media" title="Media">Media</a></li>
      <li><a href="/about/research-and-publications" title="Research &amp; Publications">Research &amp; Publications</a></li>   
    </ul>
  </nav>
 </div>
</div>


<section id="meet-the-team" class="row">
  <div class="span12">
  
    <?php echo render($content['field_sp_about_intro']); ?>
  
    <?php if (isset($content['field_sp_about_node_list_heading'])) : ?>
      <h2><?php echo render($content['field_sp_about_node_list_heading']); ?></h2>
    <?php endif; ?>
  
    <?php 
      if (count($content['team_members'])) {
        foreach ($content['team_members'] as $team_member) {
          echo render($team_member);
        }
      } else {
        ?><p class="no-content"><?php echo t('There are currently no team members to display.'); ?></p><?php 
      }
    ?>
    
  </div>
</section>

<?php echo render($content['body']); ?>