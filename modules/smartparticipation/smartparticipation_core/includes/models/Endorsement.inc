<?php

namespace SmartParticipation\model;

/**
 * Class Endorsement
 * @package SmartParticipation\model
 */
class Endorsement
{
  /** @var int flag content id */
  public $fcid;

  /** @var int comment id */
  public $cid;

  /** @var int user id */
  public $uid;

  /** @var int timestamp */
  public $timestamp;

  /** @var User $user */
  public $user;

  /**
   * @param int $fcid
   * @param int $cid
   * @param int $uid
   * @param int $timestamp
   */
  public function __construct($fcid, $cid, $uid, $timestamp, $user)
  {
    $this->fcid = $fcid;
    $this->cid = $cid;
    $this->uid = $uid;
    $this->timestamp = $timestamp;
    $this->user = $user;
  }
}