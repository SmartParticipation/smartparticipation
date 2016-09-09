<?php

namespace SmartParticipation;


use SmartParticipation\model\User;

class UserHelper
{
  public function getUserProfileLink(User $user, $attributes = array())
  {
    return l($user->name, 'user/' . $user->uid . '/profile', array('attributes' => $attributes));
  }
}