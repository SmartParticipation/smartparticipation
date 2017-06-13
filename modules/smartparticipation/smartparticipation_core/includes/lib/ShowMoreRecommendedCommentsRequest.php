<?php

namespace SmartParticipation;


use SmartParticipation\model\ShowMoreCommentsResult;

class ShowMoreRecommendedCommentsRequest extends ShowMoreFeaturedCommentsRequest
{

  /**
   * @param int|null $proposal_nid
   * @param int|null $max_id return cids greater than
   * @param int|null $count number of comments to return
   * @return ShowMoreCommentsResult
   */
  public function run($proposal_nid, $max_id, $count)
  {
    $recent_comments_service = new RecommendedCommentsService;

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

  /**
   * The ID attribute of the show more button.
   * @return string
   */
  public function getShowMoreButtonId()
  {
    return 'recommended-comments-show-more';
  }

  /**
   * The selector of the column that contains the featured comments.
   * @return string
   */
  public function getCommentsColumnSelector()
  {
    return '#recent-comments div.recommended-comments-data';
  }
}