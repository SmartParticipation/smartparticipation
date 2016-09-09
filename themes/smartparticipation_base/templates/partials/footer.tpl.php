<footer>
  <div class="container">
    <div class="row">
      <div class="span3">
        <?php echo $footer['#items']['column_1']; ?>
      </div>

      <div class="span4">
        <?php echo $footer['#items']['column_2']; ?>
      </div>

      <div class="span3 footer-nav">
        <nav>
          <?php
          //print render($footer_menu);
          $footer_menu = menu_tree('sp-footer-menu');
          print render($footer_menu);
          ?>
        </nav>
      </div>
      <?php
      extract($footer['#items']);
      if ($twitter_url || $facebook_url) {
        ?>
        <div class="span2 social-links">
          <nav>
            <ul>
              <?php if ($twitter_url) { ?>
              <li><a target="_blank" href="<?php echo $twitter_url; ?>" title="<?php echo $footer['site_name']; ?> on Twitter">Twitter</a></li>
              <?php } ?>
              <?php if ($facebook_url) { ?>
              <li><a target="_blank" href="<?php echo $facebook_url; ?>" title="<?php echo $footer['site_name']; ?> on Facebook">Facebook</a></li>
              <?php } ?>
            </ul>
          </nav>

          <p class="copyright"
           id="mobile-copyright">&copy;<?php echo date('Y'); ?> Cornell
          University</p>
        </div>
      <?php } ?>
    </div>
  </div>
</footer>

<aside id="sp-banner" class="row-fluid">
  <div class="span12 powered-by">
    <a href="http://smartparticipation.com/" target="_blank">
      <img src="<?php echo $footer['image_path']; ?>powered-by-smart-participation.png"
           alt="powered by SmartParticipation"/>
    </a>
  </div>
</aside>