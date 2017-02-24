<?php

namespace SmartParticipation\model;

/**
 * Class CommentTips
 * @package SmartParticipation\model
 */
class CommentTips
{
  /** @var int */
  public $nid;

  /** @var string */
  public $title;

  /** @var int */
  public $proposal_nid;

  /** @var int */
  public $phase_tid;

  /** @var string */
  public $lead_sentence;

  /** @var array */
  public $tips_text;

  /**
   * @param int $nid
   * @param string $title
   * @param int $proposal_nid
   * @param int $phase_tid
   */
  public function __construct($nid, $title, $proposal_nid, $phase_tid, $lead_sentence, $tips_text)
  {
    $this->nid = $nid;
    $this->title = $title;
    $this->proposal_nid = $proposal_nid;
    $this->phase_tid = $phase_tid;
    $this->lead_sentence = $lead_sentence;
    $this->tips_text = $tips_text;
  }
}