<?php

namespace SmartParticipation;


use ReflectionClass;
use ReflectionProperty;
use RuntimeException;

class Mapper
{

  protected $reflection;

  protected $properties;

  public function __construct($class_name)
  {
    $this->reflection = new ReflectionClass($class_name);
  }

  /**
   * @return ReflectionProperty[] applicable properties
   */
  protected function getProperties()
  {
    if ($this->properties === null) {
      $properties = array();

      foreach ($this->reflection->getProperties(ReflectionProperty::IS_PUBLIC) as $property) {
        if ($property->isStatic()) {
          continue; // skip static properties
        }

        $properties[$property->name] = $property;
      }

      $this->properties = $properties;
    }

    return $this->properties;
  }

  public function create($values)
  {
    $object = $this->reflection->newInstance();

    if (is_object($values)) {
      $values = get_object_vars($values);
    }

    if (!is_array($values)) {
      throw new RuntimeException("cannot map values for object of type: {$this->reflection->name} - array or object expected");
    }

    $this->map($object, $values);

    return $object;
  }

  protected function map($object, array $values)
  {
    $properties = $this->getProperties();

    foreach ($values as $name => $value) {
      if (array_key_exists($name, $properties)) {
        $properties[$name]->setValue($object, $value);
      }
    }
  }

}