<?php

/**
 * @file
 * Functions to create fields that are not created in association with a 
 * specific entity.
 */
 
/**
 * Implements hook_field_info().
 * Use this to create new field TYPES, not just fields of pre-existing types
 * (I believe).
 */
/*
function smartparticipation_core_field_info() {
  $fields = array();
  $fields += smartparticipation_core_proposal_field_info();
  return $fields;
}
*/
 
/**
 * Module function to create custom fields that are not defined by a custom
 * smartparticipation_core entity.
 */
function smartparticipation_core_create_fields() {

  //smartparticipation_core_create_field_image();
  
  // Create fields that can be attached to any entity type.
  foreach (smartparticipation_core_installed_fields() as $field) {
    field_create_field($field);
  }  
  
  // Create custom fields on the user entity.
  smartparticipation_core_create_custom_user_fields();
  
  // Create custom fields on the user entity.
  smartparticipation_core_create_custom_comment_fields();
}

function smartparticipation_core_create_field_image() {
  $field = array(
    'field_name' => 'field_image',
    'type' => 'image',
    'cardinality' => 1,
    'locked' => FALSE,
    'indexes' => array('fid' => array('fid')),
    'settings' => array(
      'uri_scheme' => 'public',
      'default_image' => FALSE,
    ),
    'storage' => array(
      'type' => 'field_sql_storage',
      'settings' => array(),
    ),
  );
  field_create_field($field);


  // Many of the following values will be defaulted, they're included here as an illustrative examples.
  // See http://api.drupal.org/api/function/field_create_instance/7
  $instance = array(
    'field_name' => 'field_image',
    'entity_type' => 'node',
    'label' => 'Image',
    'bundle' => 'article',
    'description' => st('Upload an image to go with this article.'),
    'required' => FALSE,

    'settings' => array(
      'file_directory' => 'field/image',
      'file_extensions' => 'png gif jpg jpeg',
      'max_filesize' => '',
      'max_resolution' => '',
      'min_resolution' => '',
      'alt_field' => TRUE,
      'title_field' => '',
    ),

    'widget' => array(
      'type' => 'image_image',
      'settings' => array(
        'progress_indicator' => 'throbber',
        'preview_image_style' => 'thumbnail',
      ),
      'weight' => -1,
    ),

    'display' => array(
      'default' => array(
        'label' => 'hidden',
        'type' => 'image',
        'settings' => array('image_style' => 'large', 'image_link' => ''),
        'weight' => -1,
      ),
      'teaser' => array(
        'label' => 'hidden',
        'type' => 'image',
        'settings' => array('image_style' => 'medium', 'image_link' => 'content'),
        'weight' => -1,
      ),
    ),
  );
  field_create_instance($instance);
}

/**
 * Module function defining the fields created in smartparticipation_core_create_fields().
 */
function smartparticipation_core_installed_fields() {
  
  $fields = array();

  $fields['field_sp_menu_title'] = array(
    'active' => '1',
    'cardinality' => '1',
    'deleted' => '0',
    'entity_types' => array('node'),
    'field_name' => 'field_sp_menu_title',
    'foreign keys' => array(
      'format' => array(
        'columns' => array(
          'format' => 'format',
        ),
        'table' => 'filter_format',
      ),
    ),
    'indexes' => array(
      'format' => array(
        0 => 'format',
      ),
    ),
    'locked' => '0',
    'module' => 'text',
    'settings' => array(
      'max_length' => '100',
    ),
    'translatable' => '0',
    'type' => 'text',
  );
    
  $fields['field_sp_url'] = array(
    'active' => '1',
    'cardinality' => '1',
    'deleted' => '0',
    'entity_types' => array(),
    'field_name' => 'field_sp_url',
    'foreign keys' => array(
      'format' => array(
        'columns' => array(
          'format' => 'format',
        ),
        'table' => 'filter_format',
      ),
    ),
    'indexes' => array(
      'format' => array(
        0 => 'format',
      ),
    ),
    'locked' => '0',
    'module' => 'text',
    'settings' => array(
      'max_length' => '255',
    ),
    'translatable' => '0',
    'type' => 'text',
  );
  
  $fields['field_sp_secondary_image'] = smartparticipation_core_secondary_image_field();
  
  $fields['field_sp_phase_closed_message'] = smartparticipation_core_phase_closed_message_field();
  
  return $fields;

}

/**
 * Define the field field_sp_secondary_image.
 */
function smartparticipation_core_secondary_image_field() {
  
  return array(      
    'active' => '1',
    'cardinality' => '1',
    'deleted' => '0',
    'entity_types' => array(),
    'field_name' => 'field_sp_secondary_image',
    'foreign keys' => array(
      'fid' => array(
        'columns' => array(
          'fid' => 'fid',
        ),
        'table' => 'file_managed',
      ),
    ),
    'indexes' => array(
      'fid' => array(
        0 => 'fid',
      ),
    ),
    'locked' => '0',
    'module' => 'image',
    'settings' => array(
      'default_image' => 0,
      'uri_scheme' => 'public',
    ),
    'translatable' => '0',
    'type' => 'image',
  );
}

/**
 * Return an array specifying the field_sp_phase_closed_message field.
 */
function smartparticipation_core_phase_closed_message_field() {

  return array(
    'active' => 1,
    'cardinality' => 1,
    'deleted' => 0,
    // Keep this open rather than specifying array('node'); we might want to
    // add it to the phase field collection entity as a top-level default.
    'entity_types' => array(),
    'field_name' => 'field_sp_phase_closed_message',
    'foreign keys' => array(
      'format' => array(
        'columns' => array(
          'format' => 'format',
        ),
        'table' => 'filter_format',
      ),
    ),
    'indexes' => array(
      'format' => array(
          0 => 'format',
      ),
    ),
    'locked' => 0,
    'module' => 'text',
    'settings' => array(),
    'translatable' => 0,
    'type' => 'text_long',
  );
}

/**
 * Module function to delete fields not defined by a custom smartparticipation_core entity.
 */
function smartparticipation_core_delete_fields() {
  
  // Delete general smartparticipation_core fields.
  foreach (array_keys(smartparticipation_core_installed_fields()) as $field_name) {
    field_delete_field($field_name);
  }
  
  // Delete custom user fields.
  smartparticipation_core_delete_custom_user_fields();
  
  // Delete custom comments fields.
  smartparticipation_core_delete_custom_comment_fields();
  
  field_purge_batch(1000);
  
  
  // A hack to clean up records that for some reason aren't getting deleted in 
  // field_purge_batch, though they are marked for deletion.
  // TODO Fix the problem and remove this.
  /*
  db_delete('field_config')
  ->condition('field_name', 'field\_rr\_%', 'LIKE')
  ->execute();
  db_delete('field_config_instance')
  ->condition('field_name', 'field\_rr\_%', 'LIKE')
  ->execute();  
  */
}

/**
 * Wraps the Field module function field_get_items() to return the first item of 
 * the array of items returned by that function.
 * 
 * @param $entity_type
 *   The type of $entity; e.g., 'node' or 'user'.
 * @param $entity
 *   The entity containing the data to be displayed.
 * @param $field_name
 *   The field to be displayed.
 * @param $langcode
 *   (optional) The language code $entity->{$field_name} has to be displayed in.
 *   Defaults to the current language.
 *
 * @return
 *   The first field item keyed by delta if available, NULL otherwise.
 */
function smartparticipation_core_field_get_item($entity_type, $entity, $field_name, $langcode = NULL) {
  // NB field_get_items() returns false if no items; then list($item) is null
  $item = field_get_items($entity_type, $entity, $field_name, $langcode);
  return !empty($item) ? $item[0] : NULL;
}

/**
 * Wraps smartparticipation_core_field_get_item() to return a specific keyed value. 
 * 
 * @param $entity_type
 *   The type of $entity; e.g., 'node' or 'user'.
 * @param $entity
 *   The entity containing the data to be displayed.
 * @param $field_name
 *   The field to be displayed.
 * @param $key
 *   The key of the value to be returned. 
 * @param $langcode
 *   (optional) The language code $entity->{$field_name} has to be displayed in.
 *   Defaults to the current language.
 *
 * @return
 *   The value of $key in the first field item of $entity for field $field_name.
 *   If there are no items or $key is not set, return the  empty string.
 * 
 */
function smartparticipation_core_field_get_item_value($entity_type, $entity, $field_name, $key, $langcode = NULL) {
  $item = smartparticipation_core_field_get_item($entity_type, $entity, $field_name, $langcode);  
  return $item && isset($item[$key]) ? $item[$key] : '';
}

/**
 * Wraps field_get_items() to return an empty array if no items. field_get_items()
 * returns false instead, which is awkward to code against.
 */
function smartparticipation_core_field_get_items($entity_type, $entity, $field_name, $langcode = NULL) {
  $items = field_get_items($entity_type, $entity, $field_name, $langcode);
  return $items ? $items : array();
}

/**
 * Implements hook_field_formatter_info().
 */
function smartparticipation_core_field_formatter_info() {
  
  return array(
      
    'smartparticipation_core_associated_doc' => array(
      'label' => t('Associated document'),
      'description' => t('Based on the file_default formatter; creates a link to the document that opens in a new window.'),
      'field types' => array('file'),
    ),
      
  );
}

/**
 * Implements hook_field_formatter_view().
 */
function smartparticipation_core_field_formatter_view($entity_type, $entity, $field, $instance, $langcode, $items, $display) {
  
  $element = array();

  switch ($display['type']) {
    
    case 'smartparticipation_core_associated_doc':
      foreach ($items as $delta => $item) {
        $element[$delta] = array(
          '#theme' => 'file_link_new_window',
          '#file' => (object) $item,
          '#settings' => $display['settings'],
        );
      }
      break;

  }

  return $element;
}