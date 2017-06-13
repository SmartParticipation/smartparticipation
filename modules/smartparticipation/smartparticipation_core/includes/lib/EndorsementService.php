<?php

namespace SmartParticipation;


use SmartParticipation\model\Endorsement;
use SmartParticipation\model\User;

/**
 * Class EndorsementService
 * @package SmartParticipation
 */
class EndorsementService
{

  /**
   * Return endorsements for a comment.
   * @param int $cid comment id
   * @param bool $include_user attach the user object
   * @param string $order
   * @return Endorsement[]
   */
  public function findByCommentId($cid, $include_user = true, $order = 'u.name')
  {
    $query = 'SELECT fc.flagging_id AS fcid, fc.uid, fc.timestamp FROM {comment} c
          INNER JOIN {flagging} fc ON c.cid = fc.entity_id
          INNER JOIN {flag} f ON fc.fid = f.fid
          LEFT JOIN {users} u ON u.uid = fc.uid
          WHERE c.cid = :cid
          AND f.name = :endorsements
          ORDER BY UPPER(' . $order . ')';

    // TODO couldn't get the order to work as an argument
    $result = db_query($query, array(':cid' => $cid, ':endorsements' => 'comment_endorsement'));

    $endorsements = array();
    $user_service = new UserService;
    $user = null;

    foreach ($result->fetchAllAssoc('fcid') as $fcid => $row) {
      if ($include_user) {
        $user = $user_service->find($row->uid);
      }
      $endorsements[] = new Endorsement($row->fcid, $cid, $row->uid, $row->timestamp, $user);
    }

    return $endorsements;
  }

  /**
   * Return users that endorsed a comment.
   * @param $cid comment id
   * @return User[]
   */
  public function findCommentEndorsers($cid)
  {
    $endorsers = array();
    if ($endorsements = $this->findByCommentId($cid)) {
      foreach ($endorsements as $endorsement) {
        array_push($endorsers, $endorsement->user);
      }
    }
    return $endorsers;
  }

  /**
   * This markup is used by the JavaScript behavior for updating the counter.
   * @param array | int $endorsements
   * @return string
   */
  public function getEndorsementCounterMarkup($endorsements)
  {
    if (is_array($endorsements)) {
      $count = count($endorsements);
    } else {
      $count = $endorsements;
    }
    $show_users = $this->canViewCommentEndorsementUsers();
    $counter_markup = /*'<span>' .*/ $count /*. '</span> '*/ . t(' endorsement' . ($count > 1 ? 's' : ''));
    if ($show_users) {
      $counter_markup = l($counter_markup,'#',array('attributes'=> array('class'=>'show-users','data-toggle-off-text'=>t('hide users')),'html'=>true));
    }
    return $counter_markup;
  }

  /**
   * @param int $cid comment id
   * @return int
   */
  public function getCommentEndorsementsCount($cid)
  {
    return count($this->findByCommentId($cid, false));
  }

  /**
   * Does the current user have permission to view the endorsement counter on comments?
   * @return bool
   */
  /**
   * @param null|object $comment Drupal comment object
   * @return bool
   */
  public function canViewCommentEndorsementCounter($comment = null)
  {
    $can_view_counter = false;
    if (variable_get('smartparticipation_endorsements_counter_show_all_users')) {
      $can_view_counter = true;
    } elseif (!smartparticipation_core_is_anonymous_or_basic_authenticated_user()) {
      $can_view_counter = true;
    } elseif (!empty($comment)) {
      $user_service = new UserService;
      $user = $user_service->find();
      if (!empty($user)) {
        if ($comment->uid == $user->uid || _smartparticipation_core_comment_endorsed_by_user($comment->cid)) {
          $can_view_counter = TRUE;
        }
      }
    }
    return $can_view_counter;
  }

  /**
   * Does the current user have permission to view the endorsement users on comments?
   * @return bool
   */
  public function canViewCommentEndorsementUsers()
  {
    $can_view_users = false;
    if (variable_get('smartparticipation_endorsements_users_show_all_users')) {
      $can_view_users = true;
    } elseif (!smartparticipation_core_is_anonymous_or_basic_authenticated_user()) {
      $can_view_users = true;
    }
    return $can_view_users;
  }

}