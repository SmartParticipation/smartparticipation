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

  /**
   * Read-accessors
   *
   * @param $name
   * @return mixed
   * @throws RuntimeException
   */
  public function __get($name)
  {
    $fn = "get_$name";

    if (false === method_exists($this, $fn)) {
      $class = get_class($this);

      throw new RuntimeException("undefined property {$class}::\${$name} or read-accessor {$class}::{$fn}()");
    }

    return $this->$fn();
  }

  /**
   * Write-accessors
   *
   * @param $name
   * @param $value
   * @throws RuntimeException
   */
  public function __set($name, $value)
  {
    $fn = "set_$name";

    if (false === method_exists($this, $fn)) {
      $class = get_class($this);

      throw new RuntimeException("undefined property {$class}::\${$name} or write-accessor {$class}::{$fn}()");
    }

    $this->$fn($value);
  }
}