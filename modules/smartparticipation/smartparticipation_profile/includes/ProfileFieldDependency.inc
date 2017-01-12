<?php

namespace SmartParticipation\model;

/**
 * Class ProfileFieldDependency
 * @package SmartParticipation\model
 */
class ProfileFieldDependency
{
  /** @var string */
  public $parent_field;

  /** @var string */
  public $trigger_value;

  /** @var string */
  public $child_field;

  /**
   * @param string $parent_field
   * @param string $trigger_value
   * @param string $child_field
   */
  public function __construct($parent_field, $trigger_value, $child_field)
  {
    $this->parent_field = $parent_field;
    $this->trigger_value = $trigger_value;
    $this->child_field = $child_field;
  }
}