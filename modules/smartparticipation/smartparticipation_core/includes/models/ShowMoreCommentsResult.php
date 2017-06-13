<?php

namespace SmartParticipation\model;


class ShowMoreCommentsResult
{
  /** @var array */
  private $cids = array();

  /** @var int|null */
  private $max_id = null;

  /** @var bool */
  private $more_comments = false;

  /** @var string */
  private $show_more_button_id;

  /** @var string */
  private $comments_column_selector;

  /**
   * ShowMoreCommentsResult constructor.
   * @param array $cids
   * @param int|null $max_id
   * @param bool $more_comments
   * @param string $show_more_button_id
   * @param string $comments_column_selector
   */
  public function __construct(array $cids, $max_id = null, $more_comments = false, $show_more_button_id, $comments_column_selector)
  {
    $this->cids = $cids;
    $this->max_id = $max_id;
    $this->more_comments = $more_comments;
    $this->show_more_button_id = $show_more_button_id;
    $this->comments_column_selector = $comments_column_selector;
  }

  /**
   * @return array
   */
  public function getCids()
  {
    return $this->cids;
  }

  /**
   * @return int|null
   */
  public function getMaxId()
  {
    return $this->max_id;
  }

  /**
   * @return bool
   */
  public function isMoreComments()
  {
    return $this->more_comments;
  }

  /**
   * @return string
   */
  public function getShowMoreButtonId()
  {
    return $this->show_more_button_id;
  }

  /**
   * @return string
   */
  public function getCommentsColumnSelector()
  {
    return $this->comments_column_selector;
  }

}