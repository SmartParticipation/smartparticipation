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
class Profile
{
  /** @var ProfileFieldGroup[] */
  public $grouped_fields = array();

  /** @var ProfileFieldDependency[] */
  public $dependent_fields = array();

  private $profile2_profile;

  /**
   * @return mixed
   */
  public function getProfile2Profile() {
    return $this->profile2_profile;
  }

  /**
   * @param mixed $profile2_profile
   */
  public function setProfile2Profile($profile2_profile) {
    $this->profile2_profile = $profile2_profile;
  }

  /**
   * @return \SmartParticipation\model\ProfileFieldGroup[]
   */
  public function getGroupedFields() {
    return $this->grouped_fields;
  }

  /**
   * @param \SmartParticipation\model\ProfileFieldGroup[] $grouped_fields
   */
  public function setGroupedFields($grouped_fields) {
    $this->grouped_fields = $grouped_fields;
  }

  /**
   * @return \SmartParticipation\model\ProfileFieldDependency[]
   */
  public function getDependentFields() {
    return $this->dependent_fields;
  }

  /**
   * @param \SmartParticipation\model\ProfileFieldDependency[] $dependent_fields
   */
  public function setDependentFields($dependent_fields) {
    $this->dependent_fields = $dependent_fields;
  }
}