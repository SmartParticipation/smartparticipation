<?php

namespace SmartParticipation;


class RecommendedCommentsService extends FeaturedCommentsService
{
  const STATUS_VAR = 'smartparticipation_recommended_comments_status';

  function getStatus()
  {
    return variable_get(self::STATUS_VAR, 1);
  }

  /**
   * @param string $select
   * @param string $order
   * @return string
   */
  protected function getByProposalQuery($select = 'c.cid', $order = 'c.created DESC')
  {
    $query =
      "SELECT $select FROM {field_data_field_sp_proposal_topic} pt
      INNER JOIN {node} nt ON pt.field_sp_proposal_topic_nid = nt.nid
      INNER JOIN {field_data_field_sp_topic_subtopic} ts ON pt.field_sp_proposal_topic_nid = ts.entity_id
      INNER JOIN {node} nst ON ts.field_sp_topic_subtopic_nid = nst.nid
      INNER JOIN {comment} c ON ts.field_sp_topic_subtopic_nid = c.nid
      INNER JOIN {flag_content} fc ON c.cid = fc.content_id
      INNER JOIN {flags} f ON fc.fid = f.fid
      WHERE pt.entity_id = :proposal_nid
      AND f.name = 'comment_recommendation'
      AND c.status = 1
      AND nt.status = 1
      AND nst.status = 1";
    if (!empty($this->max_id)) {
      $query = $this->appendMaxIdCondition($query);
    }
    if (!empty($order)) {
      $query .= " ORDER BY " . $order;
    }
    return $query;
  }

  /**
   * @param string $select
   * @param string $order
   * @return string
   */
  protected function getQuery($select = 'c.cid', $order = 'c.created DESC')
  {
    $query =
      "SELECT $select FROM {field_data_field_sp_proposal_topic} pt
      INNER JOIN {node} nt ON pt.field_sp_proposal_topic_nid = nt.nid
      INNER JOIN {node} np ON pt.entity_id = np.nid
      INNER JOIN {field_data_field_sp_proposal_status} ps ON pt.entity_id = ps.entity_id
      INNER JOIN {field_data_field_sp_topic_subtopic} ts ON pt.field_sp_proposal_topic_nid = ts.entity_id
      INNER JOIN {node} nst ON ts.field_sp_topic_subtopic_nid = nst.nid
      INNER JOIN {comment} c ON ts.field_sp_topic_subtopic_nid = c.nid
      INNER JOIN {flag_content} fc ON c.cid = fc.content_id
      INNER JOIN {flags} f ON fc.fid = f.fid
      WHERE c.status = 1
      AND f.name = 'comment_recommendation'
      AND np.status = 1
      AND ps.field_sp_proposal_status_value = 1
      AND nt.status = 1
      AND nst.status = 1";
    if (!empty($this->max_id)) {
      $query = $this->appendMaxIdCondition($query);
    }
    if (!empty($order)) {
      $query .= " ORDER BY " . $order;
    }
    return $query;
  }
}