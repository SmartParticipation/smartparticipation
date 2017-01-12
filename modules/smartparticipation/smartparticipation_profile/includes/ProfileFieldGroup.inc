<?php

namespace SmartParticipation\model;

/**
 * Class ProfileFieldGroup
 * @package SmartParticipation\model
 */
class ProfileFieldGroup
{
  /** @var string */
  public $field_name;

  /** @var string */
  public $label;

  /** @var array */
  public $fields;

  /**
   * @param string $field_name
   * @param string $label
   * @param array $fields
   */
  public function __construct($field_name, $label, $fields)
  {
    $this->field_name = $field_name;
    $this->label = $label;
    $this->fields = $fields;
  }
}