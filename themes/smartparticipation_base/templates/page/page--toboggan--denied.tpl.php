<?php

$image_path = '/' . drupal_get_path('theme', 'smartparticipation_base') . '/images/splash/';
$register_link = l(t('Register'),'user/register');
?>

  <div class="container splash">

<div class="row">
    <?php if (!empty($messages)): ?>
    <?php print theme('messages', array('messages' => $messages)); ?>
    <?php endif; ?>

    <?php if ($logo): ?>
      <img src="<?php print $logo; ?>" title="<?php print $site_name; ?>" alt="<?php print $site_name; ?>" />
    <?php endif; ?>
  </div>


    <div class="splash-open-discussion">

      <div class="row">


        <div class="span7">

          <div class="about-text discussion-left">
            <?php echo t('This is a private discussion.  Please log in to join the discussion.'); ?>
          </div>
        </div>
        <div class="span5">
          <div class="login-form discussion-right">
            <h3>Log in</h3>
            <div class="register">
              Didn't receive an invitation?  <?= l('Register here', $register_link) ?>.
            </div>
            <?= render($page['content']) ?>
          </div>
        </div>
      </div>

    </div>

  </div>