<?php
global $user;
$profile_edit_path = 'user/' . $user->uid . '/profile/edit';
?>
<p><?=t(variable_get('smartparticipation_profile_reminder_text','You haven\'t filled out any of your profile. You can do so on your !profile_form_link.'),
    array('!profile_form_link'=>l(t(variable_get('smartparticipation_profile_form_link_text','profile form')),$profile_edit_path)))?></p>