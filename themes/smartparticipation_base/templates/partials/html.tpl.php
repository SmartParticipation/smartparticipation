<!DOCTYPE html>
<!--[if lt IE 7]>      <html class="no-js lt-ie9 lt-ie8 lt-ie7"> <![endif]-->
<!--[if IE 7]>         <html class="no-js lt-ie9 lt-ie8"> <![endif]-->
<!--[if IE 8]>         <html class="no-js lt-ie9"> <![endif]-->
<!--[if gt IE 8]><!--> <html class="no-js"> <!--<![endif]-->
<head>
  <?php print $head; ?>
  <meta charset="utf-8">
  <meta http-equiv="X-UA-Compatible" content="IE=edge,chrome=1">
  <title><?php print $head_title; ?></title>
  <meta name="description" content="">
  <meta name="viewport" content="width=device-width">
  
  <!-- Favicon-->
  <?php $theme_path = '/' . drupal_get_path('theme', 'smartparticipation_base'); ?>
  <link rel="apple-touch-icon" sizes="57x57" href="<?php print $theme_path ?>/apple-touch-icon-57x57.png">
  <link rel="apple-touch-icon" sizes="60x60" href="<?php print $theme_path ?>/apple-touch-icon-60x60.png">
  <link rel="apple-touch-icon" sizes="72x72" href="<?php print $theme_path ?>/apple-touch-icon-72x72.png">
  <link rel="apple-touch-icon" sizes="76x76" href="<?php print $theme_path ?>/apple-touch-icon-76x76.png">
  <link rel="apple-touch-icon" sizes="114x114" href="<?php print $theme_path ?>/apple-touch-icon-114x114.png">
  <link rel="apple-touch-icon" sizes="120x120" href="<?php print $theme_path ?>/apple-touch-icon-120x120.png">
  <link rel="apple-touch-icon" sizes="144x144" href="<?php print $theme_path ?>/apple-touch-icon-144x144.png">
  <link rel="apple-touch-icon" sizes="152x152" href="<?php print $theme_path ?>/apple-touch-icon-152x152.png">
  <link rel="apple-touch-icon" sizes="180x180" href="<?php print $theme_path ?>/apple-touch-icon-180x180.png">
  <link rel="icon" type="image/png" href="<?php print $theme_path ?>/favicon-32x32.png" sizes="32x32">
  <link rel="icon" type="image/png" href="<?php print $theme_path ?>/favicon-194x194.png" sizes="194x194">
  <link rel="icon" type="image/png" href="<?php print $theme_path ?>/favicon-96x96.png" sizes="96x96">
  <link rel="icon" type="image/png" href="<?php print $theme_path ?>/android-chrome-192x192.png" sizes="192x192">
  <link rel="icon" type="image/png" href="<?php print $theme_path ?>/favicon-16x16.png" sizes="16x16">
  <link rel="manifest" href="<?php print $theme_path ?>/manifest.json">
  <meta name="msapplication-TileColor" content="#2b5797">
  <meta name="msapplication-TileImage" content="<?php print $theme_path ?>/mstile-144x144.png">
  <meta name="theme-color" content="#ffffff">

  <?php print $styles; ?>

  <!-- Le HTML5 shim, for IE6-8 support of HTML5 elements -->
  <!--[if lt IE 9]>
    <script src="http://html5shim.googlecode.com/svn/trunk/html5.js"></script>
  <![endif]-->

  <!--[if (gte IE 6)&(lte IE 8)]>
    <script type="text/javascript" src="http://cdnjs.cloudflare.com/ajax/libs/selectivizr/1.0.2/selectivizr-min.js"></script>
  <![endif]-->

  <?php print $scripts; ?>
  
  <!-- Google font Open Sans-->
  <link href='http://fonts.googleapis.com/css?family=Open+Sans:300,300italic,400,400italic,600,700' rel='stylesheet' type='text/css'>

  

</head>

<body class="<?php print $classes; ?>"<?php print $attributes; ?>>

<!--[if lt IE 8]>
<div id="browser-alert" class="alert">
  You are using an older version of Internet Explorer that is not supported by this site. Please <a href="http://windows.microsoft.com/ie">upgrade your browser</a> or switch to a recent version of <a href="http://www.google.com/chrome">Chrome</a>, <a href="http://www.firefox.com/">Firefox</a>, or <a href="http://www.apple.com/safari/">Safari</a>.
</div>
<![endif]-->

<div id="skip-link">
    <a href="#main-content" class="element-invisible element-focusable"><?php print t('Skip to main content'); ?></a>
</div>

<?php print $page_top; ?>
<?php print $page; ?>
<?php print $page_bottom; ?>

</body>
</html>