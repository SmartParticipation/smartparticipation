<?php

namespace SmartParticipation;


use SmartParticipation\model\Profile;
use SmartParticipation\model\ProfileField;
use SmartParticipation\model\ProfileFieldDependency;
use SmartParticipation\model\ProfileFieldGroup;

class ProfileHelper
{
  /** @var Profile */
  private $profile;

  /** @var array */
  private $profile_field_info;

  /** @var ProfileField[] */
  private $display_values;

  const TEXTFIELD = 'text_textfield';

  const SELECT = 'options_select';

  const TEXTAREA = 'text_textarea';

  public function __construct(Profile $profile)
  {
    $this->profile = $profile;
    $profile_service = new ProfileService;
    $this->profile_field_info = $profile_service->getProfileFieldInfo($profile);
    $this->display_values = $this->setDisplayValues();
  }

  public function build()
  {
    $display_values = $this->getDisplayValues();
    foreach ($display_values as $field_name => $profile_field) {
      $this->displayField($profile_field);
    }
  }

  /**
   * @return model\ProfileField[]
   */
  public function getDisplayValues()
  {
    return $this->display_values;
  }

  /**
   * @return ProfileField[]
   */
  private function setDisplayValues()
  {
    $display_values = array();
    foreach ($this->profile_field_info as $field_name => $info) {
      $value_type = $info['display']['default']['type'];
      $value = $this->fieldValue($field_name, $value_type);
      if ($value) {
        $label = $this->profile_field_info[$field_name]['label'];
        $label = rtrim($label,': ');
        $display_type = $info['widget']['type'];
        $display_values[$field_name] = new ProfileField($field_name, $label, $value, $display_type);
      }
    }
    $display_values = $this->applyFieldDependencies($display_values);
    $display_values = $this->applyFieldGroups($display_values);
    return $display_values;
  }

  /**
   * @param ProfileField[] $display_values
   * @return ProfileField[]
   */
  private function applyFieldDependencies($display_values)
  {
    /** @var ProfileFieldDependency $field_dependency */
    foreach ($this->profile->dependent_fields as $field_dependency) {
      $parent_field = $field_dependency->parent_field;
      $parent_field_value = $this->profile->$parent_field;
      $child_field = $field_dependency->child_field;
      if ($parent_field_value == $field_dependency->trigger_value) {
        $display_values[$parent_field]->value = $this->profile->$child_field;
      }
      if (!empty($display_values[$child_field])) {
        unset($display_values[$child_field]);
      }
      if (empty($display_values[$parent_field]->value)) {
        unset($display_values[$parent_field]);
      }
    }
    return $display_values;
  }

  /**
   * @param ProfileField[] $display_values
   * @return ProfileField[]
   */
  private function applyFieldGroups($display_values)
  {
    /** @var ProfileFieldGroup $grouped_field */
    foreach ($this->profile->grouped_fields as $grouped_field) {
      // Build the list value from the value of each field in the group
      $group_value = '';
      foreach ($grouped_field->fields as $field_name) {
        if (!empty($display_values[$field_name])) {
          if ($group_value) {
            $group_value .= ', ';
          } else if (!empty($display_values[$field_name]->value)) {
            $index = array_search($field_name, array_keys($display_values));
          }
          $group_value .= $display_values[$field_name]->value;
        }
      }
      if ($group_value) {
        // Insert the new group profile field (the merged list value) into the correct position (the position of the first field of the group)
        $current_index = 0;
        $tmp_display_values = array();
        foreach ($display_values as $field_name => $profile_field) {
          if ($index == $current_index) {
            $tmp_display_values[$grouped_field->field_name] = new ProfileField($grouped_field->field_name, $grouped_field->label, $group_value, self::TEXTFIELD);
          }
          $tmp_display_values[$field_name] = $profile_field;
          $current_index++;
        }
        $display_values = $tmp_display_values;
        // Remove the group fields that have now been merged into a single list
        foreach ($grouped_field->fields as $field_name) {
          unset($display_values[$field_name]);
        }
      }
    }
    return $display_values;
  }

  /**
   * @param ProfileField $profile_field
   */
  public function displayField(ProfileField $profile_field)
  {
    $class = str_replace('_', '-', $profile_field->field_name);
    switch ($profile_field->display_type) {
      case self::TEXTAREA:
        echo $this->longText($class, $profile_field->label, $profile_field->value);
        break;
      case self::TEXTFIELD:
      case self::SELECT:
        echo $this->text($class, $profile_field->label, $profile_field->value);
        break;
    }
  }

  /**
   * @param string $field_name
   * @param string $type
   * @return string
   */
  private function fieldValue($field_name, $type)
  {
    if (empty($this->profile->$field_name)) {
      return null;
    }
    switch ($type) {
      case 'text_default':
        $value = $this->profile->$field_name;
        break;
      case 'list_default':
        $list_values = list_allowed_values(field_info_field($field_name));
        $value = $list_values[$this->profile->$field_name];
        break;
    }
    return $value;
  }

  /**
   * @param string $class
   * @param string $label
   * @param string $value
   * @return string
   */
  public function text($class, $label, $value)
  {
    return
      '<div class="field text ' . $class . '">' .
        '<div class="label">' . $label . '</div>' .
        '<div class="value">' . htmlspecialchars($value) . '</div>' .
      '</div>';
  }

  /**
   * @param string $class
   * @param string $label
   * @param string $value
   * @return string
   */
  public function longText($class, $label, $value)
  {
    return
      '<div class="field long-text ' . $class . '">' .
        '<div class="label">' . $label . '</div>' .
        '<div class="value">' . nl2br(htmlspecialchars($value)) . '</div>' .
      '</div>';
  }

  /**
   * @param string $no_content_message
   * @return string
   */
  public static function emptyProfile($no_content_message)
  {
    return '<p class="no-content">' . $no_content_message . '</p>';
  }

}