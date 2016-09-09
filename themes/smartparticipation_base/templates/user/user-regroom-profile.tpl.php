<?php
/**
 * Template for user profile page.
 */
extract($page_vars);

if (!$profile_empty) {
  print theme($profile_theme, array('profile' => $profile));
} else {
  print \SmartParticipation\ProfileHelper::emptyProfile($no_content_message);
}

return;

//use SmartParticipation\ProfileService;
//
//$profile_service = new ProfileService;
//$profile_service->get_profile_field_info($profile);

//$active_profile_type = variable_get('regroom_active_profile','main');

//function regroom_get_profile_field_value($name, $profile)
//{
//  //$profile_type = variable_get('regroom_active_profile','main');
//  return regroom_field_get_item_value('profile2', $profile, $name, 'value', 'LANGUAGE_NONE');
//}

/*if(!empty($profile['main']) && $active_profile_type == 'main') {
  // load profile fields
  $first_name = regroom_field_get_item_value('profile2', $profile['main'], 'field_first_name', 'value', $langcode = 'LANGUAGE_NONE');
  $last_name = regroom_field_get_item_value('profile2', $profile['main'], 'field_last_name', 'value', $langcode = 'LANGUAGE_NONE');
  include_once DRUPAL_ROOT . '/includes/locale.inc';
  $countries = country_get_list();
  $country = regroom_get_profile_field_value('field_country', $profile);
  if ($country == 'US') {
    $state = regroom_get_profile_field_value('field_state', $profile);
  }
  else {
    $state_province = regroom_get_profile_field_value('field_state_province', $profile);
  }
  if (!empty($country)) {
    $country = $countries[$country];
  }
  $city = regroom_get_profile_field_value('field_city', $profile);
  $about = regroom_field_get_item_value('profile2', $profile['main'], 'field_about_me', 'value', $langcode = 'LANGUAGE_NONE');
  $website = regroom_field_get_item_value('profile2', $profile['main'], 'field_website', 'value', $langcode = 'LANGUAGE_NONE');
  $twitter = regroom_field_get_item_value('profile2', $profile['main'], 'field_twitter_id', 'value', $langcode = 'LANGUAGE_NONE');
  $facebook = regroom_field_get_item_value('profile2', $profile['main'], 'field_facebook_username', 'value', $langcode = 'LANGUAGE_NONE');
  */?><!--

  <h3><?/*= $first_name . ' ' . $last_name */?></h3>

  <?php /*if (!empty($city) || !empty($state) || !empty($state_province)) { */?>
    <div class="location">
      <div class="city-state">
        <?php
/*        $location = $city;
        if ($location) {
          $location .= ', ';
        }
        if (!empty($state)) {
          $location .= $state;
        }
        elseif (!empty($state_province)) {
          $location .= $state_province;
        }
        print $location;
        */?>
      </div>
      <div class="country">
        <?/*= $country */?>
      </div>
    </div>
  <?php /*} */?>

  --><?php
/*
  print $about;
  print '<br><br>';

  print l($website, $website);

  print '<br>';

  if ($twitter != '') {
    print '<b>Twitter</b>: ' . $twitter;
  }

  print '<br>';

  if ($facebook != '') {
    print '<b>Facebook</b>: ' . $facebook;
  }
} else*/

$no_content = true;
use SmartParticipation\ProfileService;
/** @var ProfileService $profile_service */

$profile_service = new ProfileService;

if (!$profile_service->isProfileEmpty($profile)) {
  /*$years = regroom_get_profile_field_value('field_years_teaching',$profile);
  $years_cc = regroom_get_profile_field_value('field_years_clark_county',$profile);
  $level = regroom_get_profile_field_value('field_level',$profile);
  $level_other = regroom_get_profile_field_value('field_level_other',$profile);
  $performance_zone = regroom_get_profile_field_value('field_performance_zone',$profile);
  $my_areas_1 = regroom_get_profile_field_value('field_my_areas_1',$profile);
  $my_areas_2 = regroom_get_profile_field_value('field_my_areas_2',$profile);
  $my_areas_3 = regroom_get_profile_field_value('field_my_areas_3',$profile);
  $why_education = regroom_get_profile_field_value('field_why_education',$profile);
  $awards = regroom_get_profile_field_value('field_awards',$profile);
  $about = regroom_get_profile_field_value('field_about_me',$profile);
*/
  $fields = field_info_fields();
  $all_areas = list_allowed_values($fields['field_my_areas_1']);
  $all_levels = list_allowed_values($fields['field_level']);

  $years = $profile->field_years_teaching;
  $years_cc = $profile->field_years_clark_county;
  $level = $profile->field_level;
  $level_other = $profile->field_level_other;
  $performance_zone = $profile->field_performance_zone;
  $my_areas_1 = $profile->field_my_areas_1;
  $my_areas_2 = $profile->field_my_areas_2;
  $my_areas_3 = $profile->field_my_areas_3;
  $why_education = $profile->field_why_education;
  $awards = $profile->field_awards;
  $about = $profile->field_about_me;

  if ($years) {
    print '<p class="years">Years in education: ' . $years . '</p>';
    $no_content = false;
  }
  if ($years_cc) {
    print '<p>Years at CCSD: ' . $years_cc . '</p>';
    $no_content = false;
  }
  if ($level) {
    if (!empty($level_other)) {
      $level = $level_other;
    } else {
      $level = $all_levels[$level];
    }
    print '<p>My current assignment: ' . $level . '</p>';
    $no_content = false;
  }
  $my_areas =  array($my_areas_1, $my_areas_2, $my_areas_3);
  $my_area = '';
  $count = 0;
  foreach ($my_areas as $area) {
    if ($area) {
      if ($my_area) {
        $my_area .= ', ';
      }
      $count++;
      $my_area .= $all_areas[$area];
    }
  }
  if ($my_area) {
    $label = 'My area';
    if ($count > 1) {
      $label .= 's';
    }
    $label .= ': ';
    print '<p>' . $label . $my_area . '</p>';
    $no_content = false;
  }
  if ($why_education) {
    print '<p>Why I went into eduation:<br>' . nl2br($why_education) . '</p>';
    $no_content = false;
  }
  if ($awards) {
    print '<p>Awards or other recognition I\'ve received for my work in education:<br>' . nl2br($awards) . '</p>';
    $no_content = false;
  }
  if ($about) {
    print '<p>Other things people might want to know about me:<br>' . nl2br($about) . '</p>';
    $no_content = false;
  }

//
//Years in education: [number dropdown]                    Years at CCSD: [number dropdown]
//My current assignment: [dropdown list of building types; if other can have a fillin-great; if not, OK]        Performance Zone: [number dropdown 1-16
//My areas (up to 3): [dropdown] [dropdown] [dropdpwn]
//
//Why I went into education:
//[text box]
//
//Awards or other recognition I've received for my work in education:
//[text box]
//
//Other things people might want to know about me:
//[text box]
//

}
  //else {

/*if (!empty($no_content_message) && $no_content) {
  print '<p class="no-content">' . $no_content_message . '</p>';
}*/

//}
