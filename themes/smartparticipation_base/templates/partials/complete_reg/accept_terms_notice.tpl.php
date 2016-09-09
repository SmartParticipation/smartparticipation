<p class="accept-terms-notice">
  <?php
    if (!isset($email) && !isset($username)) {
      print t("In order to complete your registration, you must agree to the site terms and conditions.");
    } else {
      if (isset($email)) {
        print t("In order to complete your registration, you must provide your email address and agree to the site terms and conditions.");
      } elseif (isset($username)) {
        print t("In order to complete your registration, you must provide a username and agree to the site terms and conditions.");
      }
    }
  ?>
</p>