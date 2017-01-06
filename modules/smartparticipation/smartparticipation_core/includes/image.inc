<?php
/**
 * @file
 * Define image settings for SmartParticipation.
 */

function smartparticipation_core_create_custom_image_styles()
{

  $style = image_style_save(array(
      'name' => 'small_square_thumbnail',
      'label' => 'Small Square Thumbnail')
  );
  $effect = array(
    'name' => 'image_scale_and_crop',
    'data' => array(
      'width' => 50,
      'height' => 50,
    ),
    'isid' => $style['isid'],
  );
  image_effect_save($effect);

}