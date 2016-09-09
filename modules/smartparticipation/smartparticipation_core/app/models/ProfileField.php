<?php

namespace SmartParticipation\model;

/**
 * Class ProfileField
 * @package SmartParticipation\model
 */
class ProfileField {
  /** @var string */
  public $field_name;

  /** @var string */
  public $label;

  /** @var string */
  public $value;

  /** @var string */
  public $display_type;

  /**
   * @param string $field_name
   * @param string $label
   * @param string $value
   * @param string $type
   */
  public function __construct($field_name, $label, $value, $display_type)
  {
    $this->field_name = $field_name;
    $this->label = $label;
    $this->value = $value;
    $this->display_type = $display_type;
  }
}