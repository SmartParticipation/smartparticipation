<?php

namespace SmartParticipation;

use SmartParticipation\model\Profile;
use SmartParticipation\model\ProfileFieldDependency;

/**
 * Class ProfileService
 * @package SmartParticipation
 */
class ProfileService
{
  /** @var string */
  private $active_profile;

  public function __construct()
  {
    $this->active_profile = static::getActiveProfile();
  }

  /**
   * @param int $uid
   * @return Profile
   * @throws \Exception
   */
  public function find($uid)
  {
    $profile = ProfileFactory::build($this->active_profile);
    $profile2_profile = profile2_load_by_user($uid, $this->active_profile);
    if ($profile2_profile) {
      $this->map($profile, $profile2_profile);
    }
    return $profile;
  }

  /**
   * @param Profile $profile
   * @param $profile2_profile
   * @return Profile
   */
  protected function map(Profile $profile, $profile2_profile)
  {
    $profile_array = get_object_vars($profile2_profile);
    foreach ($profile_array as $key => $value) {
      if (!is_array($value) || (is_array($value) && !isset($value[LANGUAGE_NONE]))) {
        if (empty($value)) {
          $value = null;
        }
        $profile->$key = $value;
      } else {
        $profile->$key = $value[LANGUAGE_NONE][0]['value'];
      }
    }
    return $profile;
  }

  /**
   * @param Profile $profile
   * @return array
   */
  public function getProfileFieldInfo(Profile $profile)
  {
    $profile_fields = field_info_instances('profile2', $this->active_profile);
    $profile_fields = $this->orderFieldsByWeight($profile_fields);
    return $profile_fields;
  }

  /**
   * @param Profile $profile
   * @return bool
   */
  public function isProfileEmpty(Profile $profile)
  {
    $profile_empty = true;
    $profile_fields = $this->getProfileFieldInfo($profile);
    // Ignore dependent fields
    $dependent_fields = array();
    /** @var ProfileFieldDependency $field_dependency */
    foreach ($profile->dependent_fields as $field_dependency) {
      $parent_field = $field_dependency->parent_field;
      $child_field = $field_dependency->child_field;
      if (empty($profile->$parent_field) && !empty($profile->$child_field)) {
        $dependent_fields[] = $child_field;
      }
      // The parent was set to the trigger value but the child field is empty - so ignore the parent field
      if (empty($profile->$child_field) && !empty($profile->$parent_field) && $profile->$parent_field == $field_dependency->trigger_value) {
        $dependent_fields[] = $parent_field;
      }
    }
    foreach ($profile_fields as $field_name => $field) {
      if (!empty($profile->$field_name) && !in_array($field_name, $dependent_fields)) {
        $profile_empty = false;
        break;
      }
    }
    return $profile_empty;
  }

  /**
   * @param $profile_fields
   * @return array
   */
  protected function orderFieldsByWeight($profile_fields)
  {
    $field_weights = array();
    foreach ($profile_fields as $field_name => $field) {
      $field_weights[$field_name] = $field['display']['default']['weight'];
    }
    asort($field_weights);
    $ordered_fields = array();
    foreach ($field_weights as $field_name => $weight) {
      $ordered_fields[$field_name] = $profile_fields[$field_name];
    }
    return $ordered_fields;
  }

  /**
   * @return string
   */
  public static function getActiveProfile()
  {
    return variable_get('smartparticipation_active_profile','main');
  }

  /**
   * Is the user profile functionality enabled?
   * @return bool
   */
  public static function userProfilesAreActive()
  {
    return variable_get('smartparticipation_profile_status', 0);
  }

}