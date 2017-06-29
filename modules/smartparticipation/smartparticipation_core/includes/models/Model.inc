<?php


namespace SmartParticipation\model;


abstract class Model
{

  protected static function getAttributeLabels()
  {
    return array();
  }

  public static function getLabel($property)
  {
    $labels = static::getAttributeLabels();
    if (!empty($labels[$property])) {
      return $labels[$property];
    } else {
      return $property;
    }
  }

}