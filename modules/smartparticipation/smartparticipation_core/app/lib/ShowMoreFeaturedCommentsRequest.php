<?php

namespace SmartParticipation;


use SmartParticipation\model\ShowMoreCommentsResult;

abstract class ShowMoreFeaturedCommentsRequest
{
  /**
   * Start the next request where comments ids are greater than the max id.
   * @var int
   */
  private $next_max_id;

  /**
   * Are there more comments remaining after this request?
   * @var bool
   */
  private $more_comments;

  /**
   * @param int|null $proposal_nid
   * @param int|null $max_id return cids greater than
   * @param int|null $count number of comments to return
   * @return ShowMoreCommentsResult
   */
  abstract public function run($proposal_nid, $max_id, $count);

  /**
   * The ID attribute of the show more button.
   * @return string
   */
  abstract public function getShowMoreButtonId();

  /**
   * The selector of the column that contains the featured comments.
   * @return string
   */
  abstract public function getCommentsColumnSelector();

  /**
   * @param array $cids Drupal comment ids
   * @return mixed
   */
  protected function setNextMaxId(array $cids)
  {
    $this->next_max_id = end($cids);
  }

  protected function getNextMaxId()
  {
    return $this->next_max_id;
  }

  /**
   * @param int $remaining_comments_count
   * @return bool
   */
  protected function setMoreComments($remaining_comments_count)
  {
    $this->more_comments = ! empty($remaining_comments_count);
  }

  protected function getMoreComments()
  {
    return $this->more_comments;
  }
}