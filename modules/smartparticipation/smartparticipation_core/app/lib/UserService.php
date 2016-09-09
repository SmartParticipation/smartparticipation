<?php

namespace SmartParticipation;


use SmartParticipation\model\User;

/**
 * Class UserService
 * @package SmartParticipation
 */
class UserService
{

  /**
   * @param $uid
   * @return User
   */
  public function find($uid = null)
  {
    if (empty($uid)) {
      $uid = $GLOBALS['user']->uid;
    }
    if (empty($uid)) {
      return false;
    }
    $drupal_user = user_load($uid);
    $user = new User;
    $this->map($user, $drupal_user);
    return $user;
  }

  /**
   * @param User $user
   * @param $drupal_user
   * @return User
   */
  protected function map(User $user, $drupal_user)
  {
    $user_array = (array) $drupal_user;
    foreach ($user_array as $key => $value) {
      if (!is_array($value) || (is_array($value) && !isset($value[LANGUAGE_NONE]))) {
        $user->$key = $value;
      } else {
        $user->$key = $value[LANGUAGE_NONE][0]['value'];
      }
    }
    return $user;
  }

  /**
   * @param \SmartParticipation\model\User $user
   * @return bool
   */
  public static function isAdmin(User $user)
  {
    if (in_array('administrator', $user->roles)) {
      return TRUE;
    }
    return FALSE;
  }

}