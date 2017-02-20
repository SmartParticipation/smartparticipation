<?php

namespace SmartParticipation;


use DatabaseStatementInterface;
use PDO;
use SmartParticipation\model\CommentTips;
use SmartParticipation\model\CommentTipsState;
use SmartParticipation\model\User;

class CommentTipsService
{
  /**
   * Return the oldest comment tips for a proposal phase.  There could be
   * multiple sets of comment tips for a proposal phase to support an A/B test.
   *
   * @param int $proposal_nid
   * @param int $phase_tid
   * @return null|CommentTips
   */
  public function find($proposal_nid, $phase_tid)
  {
    $query = $this->_find($proposal_nid, $phase_tid);
    if ($result = $query->fetchAssoc()) {
      return new CommentTips(
        $result['nid'],
        $result['title'],
        $proposal_nid,
        $phase_tid,
        $result['field_sp_comment_tips_sentence_value'],
        $result['body_value']
      );
    }
    return null;
  }

  /**
   * Return all sets of comments tips for a proposal phase.
   *
   * @param $proposal_nid
   * @param $phase_tid
   * @return CommentTips[]|null
   */
  public function findAll($proposal_nid, $phase_tid)
  {
    $query = $this->_find($proposal_nid, $phase_tid);
    if ($result = $query->fetchAllAssoc('nid', PDO::FETCH_BOTH)) {
      $comment_tips = array();
      foreach($result as $row) {
        array_push($comment_tips, new CommentTips(
          $row['nid'],
          $row['title'],
          $proposal_nid,
          $phase_tid,
          $row['field_sp_comment_tips_sentence_value'],
          $row['body_value']
        ));
      }
      return $comment_tips;
    }
    return null;
  }

  /**
   * @param $proposal_nid
   * @param $phase_tid
   * @return DatabaseStatementInterface
   */
  private function _find($proposal_nid, $phase_tid)
  {
    return db_query(
      "SELECT n.nid, n.title, sen.field_sp_comment_tips_sentence_value, bod.body_value
        FROM {node} n
        JOIN {field_data_field_sp_comment_tips_phase} pha ON n.nid = pha.entity_id
        JOIN {field_data_field_sp_comment_tips_proposal} pro ON pha.entity_id = pro.entity_id
        JOIN {field_data_body} bod ON pro.entity_id = bod.entity_id
        LEFT JOIN {field_data_field_sp_comment_tips_sentence} sen ON bod.entity_id = sen.entity_id
        WHERE pha.field_sp_comment_tips_phase_tid = :phase_tid
        AND pro.field_sp_comment_tips_proposal_nid = :proposal_nid
        ORDER BY n.created ASC",
      array(':phase_tid'=>$phase_tid,':proposal_nid'=>$proposal_nid)
    );
  }

  /**
   * Only basic authenticated users should receive comment tips.
   * @param CommentTips $comment_tips
   * @param User|array $user User object or Drupal user array
   */
  /*public function applyUserTypeFilter(CommentTips &$comment_tips, $user)
  {
    if ($user instanceof User) {
      $drupal_user = user_load($user->uid);
    } else {
      $drupal_user = $user;
    }
    if (!smartparticipation_core_is_basic_authenticated_user($drupal_user)) {
      $comment_tips = null;
    }
  }*/

  /**
   * A user should receive the prompt only if they have not before for this proposal-phase.
   * @param User $user
   * @param array $subtopic_nids
   * @return bool
   */
  public function userShouldReceiveProposalPhasePrompt(User $user, CommentTips $comment_tips)
  {
    $subtopic_nids = smartparticipation_core_get_proposal_phase_subtopics($comment_tips->proposal_nid, $comment_tips->phase_tid);
    if ($this->userHasReceivedProposalPhasePrompt($user, $subtopic_nids) || UserService::isAdmin($user)) {
      return false;
    }
    return true;
  }

  /**
   * A user should receive the comment tips button only after they have received the prompt for this proposal-phase.
   * @param \SmartParticipation\model\User $user
   * @param array $subtopic_nids
   * @return bool
   */
  public function userShouldReceiveProposalPhaseButton(User $user, CommentTips $comment_tips)
  {
    return ! $this->userShouldReceiveProposalPhasePrompt($user, $comment_tips);
  }

  /**
   * Check the event log to see if the user has previously received the prompt for a subtopic in this proposal-phase.
   * @param User $user
   * @param array $subtopic_nids
   * @return bool
   */
  protected function userHasReceivedProposalPhasePrompt(User $user, $subtopic_nids)
  {
    $result =
      db_select('sp_event_log','el')
        ->fields('el',array('eid'))
        ->condition('uid', $user->uid)
        ->condition('name', 'open comment tips prompt')
        ->condition('entity_id', $subtopic_nids, 'IN')
        ->execute();
    $num_records = $result->rowCount();
    return $num_records ? true : false;
  }

  public function getState(User $user, CommentTips $comment_tips)
  {
    $use_prompt = $this->userShouldReceiveProposalPhasePrompt($user, $comment_tips);
    $use_button = $this->userShouldReceiveProposalPhaseButton($user, $comment_tips);

    return new CommentTipsState(
      $use_prompt,
      $use_button
    );
  }

  public function getStateRequest(User $user, $subtopic_nid)
  {
    // Get all subtopics nids shared with subtopic_nid in this proposal-phase
    if ($proposal = smartparticipation_core_get_subtopic_proposal($subtopic_nid)) {
      $proposal_nid = $proposal->nid;
    }
    if ($phase = smartparticipation_core_get_subtopic_phase($subtopic_nid)) {
      $phase_tid = $phase->tid;
    }
    if ($comment_tips = $this->find($proposal_nid, $phase_tid)) {
      $comment_tips_state = $this->getState($user, $comment_tips);
      $timestamp = time();
      // return JSON - use_comment_tips_prompt (bool); use_comment_tips_button (bool)
      drupal_json_output(array(
        'use_comment_tips_prompt' => $comment_tips_state->useCommentTipsPrompt,
        'use_comment_tips_button' => $comment_tips_state->useCommentTipsButton,
        'timestamp' => $timestamp,
      ));
    } else {
      return drupal_not_found();
    }
  }
}