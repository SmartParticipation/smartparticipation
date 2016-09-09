<div id="<?php if (@$dlgId) print $dlgId; ?>" title="<?php if (@$title) print $title; ?>" style="display:none;">
  <?php
    if (@$content) {
      if (is_array($content)) {
        foreach ($content as $content_segment) {
          print $content_segment;
        }
      } else {
        print $content;
      }
    }
  ?>
</div>