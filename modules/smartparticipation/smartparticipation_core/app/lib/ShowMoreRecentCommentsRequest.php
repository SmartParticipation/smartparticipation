<?php

namespace SmartParticipation;


use SmartParticipation\model\ShowMoreCommentsResult;

/**
 * Handles a show more request for recent comments and returns the result data.
 *
 * Class ShowMoreRecentCommentsRequest
 * @package SmartParticipation
 */
class ShowMoreRecentCommentsRequest extends ShowMoreFeaturedCommentsRequest
{
  /**
   * @param int|null $proposal_nid
   * @param int|null $max_id
   * @param int|null $count
   * @return ShowMoreCommentsResult
   */
  public function run($proposal_nid, $max_id, $count)
  {
    $recent_comments_service = new RecentCommentsService;

    if (empty($proposal_nid)) {
      $recent_comment_ids = $recent_comments_service->findAll($max_id, $count);
    } else {
      $recent_comment_ids = $recent_comments_service->findByProposal($proposal_nid, $max_id, $count);
    }
    $this->setNextMaxId($recent_comment_ids);
    if (empty($proposal_nid)) {
      $comments_remaining = $recent_comments_service->countAll($this->getNextMaxId());
    } else {
      $comments_remaining = $recent_comments_service->countByProposal($proposal_nid, $this->getNextMaxId());
    }
    $this->setMoreComments($comments_remaining);

    return new ShowMoreCommentsResult(
      $recent_comment_ids,
      $this->getNextMaxId(),
      $this->getMoreComments(),
      $this->getShowMoreButtonId(),
      $this->getCommentsColumnSelector()
    );
  }

  public function getShowMoreButtonId()
  {
    return 'recent-comments-show-more';
  }


  public function getCommentsColumnSelector()
  {
    return '#recent-comments div.recent-comments-data';
  }
}