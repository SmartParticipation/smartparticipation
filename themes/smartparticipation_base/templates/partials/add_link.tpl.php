<?php // This template is no longer in use - it has been replaced by the CKEditor add link tool. ?>

<div class="comment-add-link"><a href="#addLink"><span class="add-link-icon"></span>Link</a></div>

<?php $commentFormId = !empty($commentFormId) ? '-' . $commentFormId : ''; ?>

<div class="comment-add-link-fields">
  <div class="form-item form-type-textfield">
    <label for="link-url<?php echo $commentFormId; ?>">Link URL</label>
    <input placeholder="Enter link URL" class="form-text" type="text" name="link-url" id="link-url<?php echo $commentFormId; ?>">
  </div>
  <div class="form-item form-type-textfield">
    <label for="link-text<?php echo $commentFormId; ?>">Link Text</label>
    <input placeholder="Enter link text" class="form-text" type="text" name="link-text" id="link-text<?php echo $commentFormId; ?>">
  </div>
  <div class="form-actions">
    <button class="comment-add-link-save">Add Link</button>
    <a class="comment-add-link-cancel">Cancel</a>
  </div>
</div>