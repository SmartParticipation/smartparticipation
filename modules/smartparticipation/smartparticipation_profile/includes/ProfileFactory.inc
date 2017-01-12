<?php

namespace SmartParticipation;

/**
 * Class ProfileFactory
 * @package SmartParticipation
 */
class ProfileFactory
{
  /**
   * @param null $profile_type
   * @return mixed
   * @throws \Exception
   */
  public static function build($profile_type)
  {
    $profile_class = static::classPrefix($profile_type) . 'Profile';
    $profile_class = 'SmartParticipation\\model\\'.$profile_class;
    if (class_exists($profile_class)) {
      return new $profile_class();
    } else {
      throw new \Exception("Invalid profile type given.");
    }
  }

  /**
   * @param $profile_type
   * @return string
   */
  private static function classPrefix ($profile_type)
  {
    switch ($profile_type) {
      default:
        return 'Main';
        break;
    }
  }

}