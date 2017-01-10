<?php

namespace SmartParticipation\model;


class ABTestUserVariant extends Model
{
  /** @var string */
  public $abid;

  /** @var int */
  public $uid;

  /** @var string */
  public $variant;

  /** @var int timestamp */
  public $assigned;

  const VARIANT_A = 'VARIANT_A';

  const VARIANT_B = 'VARIANT_B';

  /**
   * @param string $abid
   * @param int $uid
   * @param string $variant
   * @param int $assigned
   */
  public function __construct($abid, $uid, $variant, $assigned)
  {
    $this->abid = $abid;
    $this->uid = $uid;
    $this->variant = $variant;
    $this->assigned = $assigned;
  }

  protected static function getAttributeLabels()
  {
    return array(
      'abid' => t('A/B Test Key'),
      'uid' => t('User ID'),
      'variant' => t('Assigned Variant'),
      'assigned' => t('Assignment Datetime'),
    );
  }
}