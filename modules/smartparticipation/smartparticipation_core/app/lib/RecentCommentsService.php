<?php

namespace SmartParticipation;


class RecentCommentsService extends FeaturedCommentsService
{

  protected function getByProposalQuery($select = 'c.cid', $order = 'c.created DESC')
  {
    $query =
      "SELECT $select FROM {field_data_field_sp_proposal_topic} pt
       INNER JOIN {node} nt ON pt.field_sp_proposal_topic_nid = nt.nid
       INNER JOIN {field_data_field_sp_topic_subtopic} ts ON pt.field_sp_proposal_topic_nid = ts.entity_id
       INNER JOIN {node} nst ON ts.field_sp_topic_subtopic_nid = nst.nid
       INNER JOIN {comment} c ON ts.field_sp_topic_subtopic_nid = c.nid
       LEFT JOIN {field_data_field_sp_comment_type} ct ON ct.entity_id = c.cid
       LEFT JOIN {field_data_field_sp_comment_mod_action} cma ON cma.entity_id = c.cid
       WHERE pt.entity_id = :proposal_nid
       AND c.status = 1
       AND nt.status = 1
       AND nst.status = 1
       AND (ct.field_sp_comment_type_value IS NULL OR ct.field_sp_comment_type_value != 'moderator')
       AND (cma.field_sp_comment_mod_action_value IS NULL OR cma.field_sp_comment_mod_action_value != 'quarantine')";
    if (!empty($this->max_id)) {
      $query = $this->appendMaxIdCondition($query);
    }
    if (!empty($order)) {
      $query .= " ORDER BY " . $order;
    }
    return $query;
  }

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
       LEFT JOIN {field_data_field_sp_comment_type} ct ON ct.entity_id = c.cid
       LEFT JOIN {field_data_field_sp_comment_mod_action} cma ON cma.entity_id = c.cid
       WHERE c.status = 1
       AND np.status = 1
       AND ps.field_sp_proposal_status_value = 1
       AND nt.status = 1
       AND nst.status = 1
       AND (ct.field_sp_comment_type_value IS NULL OR ct.field_sp_comment_type_value != 'moderator')
       AND (cma.field_sp_comment_mod_action_value IS NULL OR cma.field_sp_comment_mod_action_value != 'quarantine')";
    if (!empty($this->max_id)) {
      $query = $this->appendMaxIdCondition($query);
    }
    if (!empty($order)) {
      $query .= " ORDER BY " . $order;
    }
    return $query;
  }

}