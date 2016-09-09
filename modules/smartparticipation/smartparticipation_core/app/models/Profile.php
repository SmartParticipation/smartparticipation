<?php

namespace SmartParticipation\model;

/**
 * Class Profile
 * @package SmartParticipation\model
 *
 * @property int $pid
 * @property string $type
 * @property string $label
 * @property int $uid
 * @property int $created
 * @property int $changed
 */
abstract class Profile
{
  /** @var ProfileFieldGroup[] */
  public $grouped_fields = array();

  /** @var ProfileFieldDependency[] */
  public $dependent_fields = array();
}