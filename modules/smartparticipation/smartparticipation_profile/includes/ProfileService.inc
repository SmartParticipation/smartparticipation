<?php

namespace SmartParticipation;

use SmartParticipation\model\Profile;
use SmartParticipation\model\ProfileFieldDependency;
use SmartParticipation\model\ProfileFieldGroup;

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
    $profile = new Profile;

    $profile2_profile = profile2_load_by_user($uid, $this->active_profile);
    $profile->setProfile2Profile($profile2_profile);

    $profile_fields = $this->getProfileFieldInfo($profile);

    $profile_field_groups = $this->getProfileFieldGroups($profile_fields);
    $profile->setGroupedFields($profile_field_groups);

    $profile_field_dependencies = $this->getProfileFieldDependencies($profile_fields);
    $profile->setDependentFields($profile_field_dependencies);

    return $profile;
  }

  protected function getProfileFieldDependencies(array $profile_fields)
  {
    $profile_field_dependencies = array();
    $conditional_fields = conditional_fields_load_dependencies('profile2', $this->active_profile);
    if (!empty($conditional_fields['dependees'])) {
      foreach ($conditional_fields['dependees'] as $dependee_field_name => $dependee_conditional_fields) {
        foreach ($dependee_conditional_fields as $conditional_field_info) {
          if (array_key_exists($dependee_field_name, $profile_fields)) {
            $trigger_value = $conditional_field_info['options']['value_form'];
            $dependent_field_name = $conditional_field_info['dependent'];
            $profile_field_dependency = new ProfileFieldDependency(
              $dependee_field_name,
              $trigger_value,
              $dependent_field_name
            );
            array_push($profile_field_dependencies, $profile_field_dependency);
          }
        }
      }
    }
    return $profile_field_dependencies;
  }

  protected function getProfileFieldGroups(array $profile_fields)
  {
    $profile_field_groups = array();
    $field_group_info_groups = field_group_info_groups('profile2', $this->active_profile);
    if (!empty($field_group_info_groups['form'])) {
      foreach ($field_group_info_groups['form'] as $group_info) {
        $profile_field_group = new ProfileFieldGroup(
          $group_info->group_name,
          $group_info->label,
          $group_info->children
        );
        array_push($profile_field_groups, $profile_field_group);
      }
    }
    return $profile_field_groups;
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
    $profile2_profile = $profile->getProfile2Profile();
    $profile_fields = $this->getProfileFieldInfo($profile);
    // Ignore dependent fields
    $dependent_fields = array();
    foreach ($profile->getDependentFields() as $field_dependency) {
      $parent_field = $field_dependency->parent_field;
      $child_field = $field_dependency->child_field;
      if (empty($profile2_profile->$parent_field) && !empty($profile2_profile->$child_field)) {
        $dependent_fields[] = $child_field;
      }
      // The parent was set to the trigger value but the child field is empty - so ignore the parent field
      if (
        empty($profile2_profile->$child_field)
        && !empty($profile2_profile->$parent_field)
        && $profile2_profile->$parent_field == $field_dependency->trigger_value
      ) {
        $dependent_fields[] = $parent_field;
      }
    }
    $profile2_profile = $profile->getProfile2Profile();
    foreach ($profile_fields as $field_name => $field) {
      if (!empty($profile2_profile->$field_name) && !in_array($field_name, $dependent_fields)) {
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
  protected function orderFieldsByWeight(array $profile_fields)
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