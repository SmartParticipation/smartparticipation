<header>
    <div class="row site-header">


        <div class="span6 user-nav pull-right">
          <?php



            print theme('user_thumbnail');

            $menu = theme('nice_menus', array(
              'id' => 'sp-user-menu',
              'direction' => 'down',
              'depth' => -1,
              'menu_name' => (
                user_is_logged_in() ?
                  'sp-user-menu-logged-in' :
                  'sp-user-menu-logged-out'
              ),
              'menu' => NULL,
            ));
            print $menu['content'];


          ?>
        </div>

      <div class="span6 pull-left">
        <!--<a id="nav-icon" href="#menu">
          <span></span>
          <span></span>
          <span></span>
        </a>-->

        <a id="nav-icon" href="#menu" title="Open the navigation">
          <span class="open-nav">Menu</span>
          <span class="close-nav">X</span>
        </a>

        <?php if ($logo): ?>
          <a href="/" id="logo">
            <img src="<?php print $logo; ?>" title="<?php print $site_name; ?>" alt="<?php print $site_name; ?>" class="logo" id="<?php print str_replace(" ", "-", strtolower($site_name)); ?>-logo" />
          </a>
        <?php endif; ?>
      </div>
    </div>

    <div class="header-nav">

      <?php
        // the desktop menu
        print render($page_header['nice_menus_1']);

        // the responsive menu
        //print render($page_header['system_main-menu']);

        /*$menu = theme('nice_menus', array(
          'id' => 'main-menu',
          'direction' => 'down',
          'depth' => -1,
          'menu_name' => 'main-menu',
          'menu' => NULL
        ));
        print $menu['content'];*/
      ?>

    </div>

</header>