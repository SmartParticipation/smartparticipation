<div id="<?php if (@$alert_id) {
  print $alert_id;
} ?>" class="alert row-fluid">
  <div class="span12 message">
    <button data-dismiss="alert" class="close" type="button">×</button>
    <?php if (@$message) {
      print $message;
    } ?>
  </div>
</div>