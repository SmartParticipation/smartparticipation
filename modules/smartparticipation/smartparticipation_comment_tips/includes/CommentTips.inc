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

  /** @var string */
  public $body;

  /**
   * @param int $nid
   * @param string $title
   * @param int $proposal_nid
   * @param int $phase_tid
   * @param int $body
   */
  public function __construct($nid, $title, $proposal_nid, $phase_tid, $lead_sentence, $body)
  {
    $this->nid = $nid;
    $this->title = $title;
    $this->proposal_nid = $proposal_nid;
    $this->phase_tid = $phase_tid;
    $this->lead_sentence = $lead_sentence;
    $this->body = $body;
  }
}