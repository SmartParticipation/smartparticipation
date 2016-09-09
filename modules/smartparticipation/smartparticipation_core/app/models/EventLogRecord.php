<?php
/**
 * Created by PhpStorm.
 * User: Brian
 * Date: 5/15/15
 * Time: 1:38 PM
 */

namespace SmartParticipation\model;


class EventLogRecord extends Model
{
  /** @var int */
  public $eid;

  /** @var string */
  public $name;

  /** @var string */
  public $detail;

  /** @var string */
  public $detail_long;

  /** @var string */
  public $entity_type;

  /** @var int */
  public $entity_id;

  /** @var string */
  public $path;

  /** @var int */
  public $uid;

  /** @var string */
  public $username;

  /** @var string */
  public $hostname;

  /** @var int */
  public $timestamp;

  /** @var int */
  public $instance_start;

  /**
   * @param int $eid
   * @param string $name
   * @param string $detail
   * @param string $detail_long
   * @param string $entity_type
   * @param int $entity_id
   * @param string $path
   * @param int $uid
   * @param string $hostname
   * @param int $timestamp
   * @param int $instance_start
   */
  public function __construct(/*$eid, $name, $detail, $detail_long, $entity_type, $entity_id, $path, $uid, $hostname, $timestamp, $instance_start*/)
  {
    /*$this->eid = $eid;
    $this->name = $name;
    $this->detail = $detail;
    $this->detail_long = $detail_long;
    $this->entity_type = $entity_type;
    $this->entity_id = $entity_id;
    $this->path = $path;
    $this->uid = $uid;
    $this->hostname = $hostname;
    $this->timestamp = $timestamp;
    $this->instance_start = $instance_start;*/
  }

  /**
   * @return array
   */
  protected static function getAttributeLabels()
  {
    return array(
      'eid' => t('Event ID'),
      'name' => t('Event Name'),
      'detail' => t('Event Detail'),
      'detail_long' => t('Event Data'),
      'entity_type' => t('Entity Type'),
      'entity_id' => t('Entity ID'),
      'path' => t('Path'),
      'uid' => t('User ID'),
      'username' => t('Username'),
      'hostname' => t('IP Address'),
      'timestamp' => t('Event Datetime'),
      'instance_start' => t('Instance Start Datetime'),
    );
  }
}