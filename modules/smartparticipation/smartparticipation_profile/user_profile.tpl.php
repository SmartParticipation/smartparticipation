<?php

/** @var array $profile_data */
/** @var \SmartParticipation\ProfileHelper $profile_helper */


$profile_helper = $profile_data['#helper'];
if (!$profile_data['profile_empty']) {
  $profile_helper->build();
} else {
  print $profile_helper::emptyProfile($profile_data['no_content_message']);
}