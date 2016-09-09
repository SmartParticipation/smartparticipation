<?php


namespace SmartParticipation\model;

/**
 * Class ABTest
 * @package SmartParticipation\model
 */
class ABTest extends Model
{

  /** @var string */
  public $abid;

  /** @var string */
  public $title;

  /** @var string */
  public $description;

  /** @var string */
  public $variant_a_title;

  /** @var string */
  public $variant_b_title;

  /** @var int */
  public $created;

  /** @var int */
  public $modified;

  /**
   * @param string $abid
   * @param string $title
   * @param string $description
   * @param string $variant_a_title
   * @param string $variant_b_title
   * @param int $created timestamp
   * @param int $modified timestamp
   */
  public function __construct($abid, $title, $description, $variant_a_title, $variant_b_title, $created = null, $modified = null)
  {
    $this->abid = $abid;
    $this->title = $title;
    $this->description = $description;
    $this->variant_a_title = $variant_a_title;
    $this->variant_b_title = $variant_b_title;
    $this->created = $created;
    $this->modified = $modified;
  }

  /**
   * @return array
   */
  public static function getAttributeLabels() {
    return array(
      'abid' => t('A/B Test Key'),
      'title' => t('Title'),
      'description' => t('Description'),
      'variant_a_title' => t('Variant A Title'),
      'variant_b_title' => t('Variant B Title'),
      'created' => t('Created'),
      'modified' => t('Last Modified'),
    );
  }

}