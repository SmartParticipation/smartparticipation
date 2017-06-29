<?php

namespace SmartParticipation;


use PDO;

abstract class FeaturedCommentsService
{

  /** @var int|null cid to start query */
  protected $max_id = null;

  /** @var int|null number of comments to return */
  protected $count = null;

  /** @var int|null nid of proposal */
  protected $proposal_nid = null;

  /** @var array values to bind to query */
  private $query_params = array();

  /**
   * Assemble the comment data into a structured array.
   * @param array $cids
   * @return array
   */
  public function getData(array $cids)
  {
    $comment_items = array();
    foreach ($cids as $cid) {
      $comment_items[] = smartparticipation_core_featured_comment_data($cid);
    }
    return $comment_items;
  }

  public function getItemsRenderArray($comment_type, $comment_items, $more_comments, $max_id, $show_more_button_id)
  {
    return array(
      '#items' => $comment_items,
      '#theme' => 'featured_comments_items',
      'more_comments' => $more_comments,
      'max_id' => $max_id,
      'show_more_button_id' => $show_more_button_id,
      'comment_type' => $comment_type,
    );
  }

  /**
   * @param int|null $max_id
   * @param int|null $count
   */
  private function setPagingProperties($max_id, $count)
  {
    $this->max_id = $max_id;
    if (!empty($max_id)) {
      $this->query_params[':max_id'] = (int) $this->max_id;
    }
    $this->count = $count;
  }

  /**
   * @param int $proposal_nid
   */
  private function setProposalNid($proposal_nid)
  {
    $this->proposal_nid = $proposal_nid;
    $this->query_params[':proposal_nid'] = (int) $this->proposal_nid;
  }

  /**
   * @param int $proposal_nid
   * @param int|null $max_id
   * @param int|null $count
   * @return mixed
   */
  public function findByProposal($proposal_nid, $max_id = null, $count = null)
  {
    $this->setPagingProperties($max_id, $count);

    $this->setProposalNid($proposal_nid);

    $query = $this->getByProposalQuery();

    $cids = $this->executeQuery($query)->fetchCol();

    // TODO return RecentComment objects
    return $cids;
  }

  /**
   * @param int|null $max_id
   * @param int|null $count
   * @return mixed
   */
  public function findAll($max_id = null, $count = null)
  {
    $this->setPagingProperties($max_id, $count);

    $query = $this->getQuery();

    $cids = $this->executeQuery($query)->fetchCol();

    return $cids;
  }

  /**
   * Wraps and calls the appropriate Drupal database query method.  Where a
   * limit clause is needed, Drupal requires a different query method to be called.
   *
   * @param string $query
   * @return \DatabaseStatementInterface
   */
  private function executeQuery($query)
  {
    if (empty($this->count)) {
      return db_query($query, $this->query_params);
    } else {
      return db_query_range($query, 0, $this->count, $this->query_params);
    }
  }

  public function countAll($max_id = null)
  {
    $this->setPagingProperties($max_id, null);
    $query = $this->getQuery('count(c.cid)');
    return (int) $this->executeQuery($query)->fetchField();
  }

  public function countByProposal($proposal_nid, $max_id = null)
  {
    $this->setPagingProperties($max_id, null);
    $this->setProposalNid($proposal_nid);
    $query = $this->getByProposalQuery('count(c.cid)');
    return (int) $this->executeQuery($query)->fetchField();
  }

  /**
   * @param string $query
   * @return string
   */
  protected function appendMaxIdCondition($query)
  {
    return $query . " AND c.cid < :max_id";
  }

  /**
   * @param string $select
   * @param string $order
   * @return string
   */
  abstract protected function getByProposalQuery($select, $order);

  /**
   * @param string $select
   * @param string $order
   * @return string
   */
  abstract protected function getQuery($select, $order);

}