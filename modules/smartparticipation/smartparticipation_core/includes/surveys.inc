<?php




/** 
 * Return true iff the specified user has submitted the specified survey.
 * 
 * @param $uid - the user uid
 * @param $survey_id - the survey id
 * 
 * @return boolean - true if the user has submitted the survey, false 
 * otherwise.
 */
function _smartparticipation_core_user_submitted_survey($uid, $survey_nid) {
  
  $query = 'SELECT * 
            FROM {webform_submissions} 
            WHERE nid = :survey_id 
            AND uid = :uid';
  
  $result = db_query($query, array(
    ':survey_id' => $survey_nid, 
    ':uid' => $uid
  ));
  
  $count = $result->rowCount();
   
  return $count > 0;

}


function smartparticipation_core_surveys_form($form, $node) {
  $nid = $node['build_info']['args'][0]->nid;
  $proposal = node_load($nid);

  if(isset($proposal->field_sp_interest_survey[LANGUAGE_NONE])) {

    $survey_nid = smartparticipation_core_field_get_item_value('node', $proposal, 'field_sp_interest_survey', 'nid', $langcode = 'LANGUAGE_NONE');
    $message = l('Edit Interest Survey','node/' . $survey_nid . '/edit');
    $message .= '<br><br>';
    $message .= l('Download Interest Survey Data', 'node/' . $survey_nid . '/webform-results/download');
  } else {
    // TODO This link should work like the Add Topic link: open a form to add a new topic node, with proposal pre-selected.
    $message = l('Add Interest Survey','node/' . $nid . '/edit');
  }
  
  $form['message'] = array(
    '#type' => 'markup',
    '#markup' => $message,
  );

  return $form;
}


/*
 * Getting the list of questions for a specific survey
 */
function _smartparticipation_core_get_survey_questions($survey_nid) {
  $query = 'SELECT cid, name FROM {webform_component} wc  
            WHERE wc.nid = :survey_nid';
            
  $result = db_query($query, array(':survey_nid' => $survey_nid))->fetchAll();
  
  return $result;
}


/*
 * Getting the list of answers for a specific question
 */
function _smartparticipation_core_get_survey_answers($cid, $sid) {
  $query = 'SELECT data FROM {webform_submitted_data} d  
            WHERE d.sid = :sid 
            AND d.cid = :cid';
            
  $result = db_query($query, array(':sid' => $sid, ':cid' => $cid))->fetchAll();
  
  return $result;
}

/*
 * Getting survey responses from proposal nid and uid
 */
function _smartparticipation_core_get_survey_results ($proposal_nid, $uid) {

  $return = array();

  $query = '
  SELECT c.cid, s.sid, c.name, c.extra
  FROM field_data_field_sp_interest_survey i
  JOIN webform_submissions s on s.nid=i.field_sp_interest_survey_nid
  JOIN webform_component c on c.nid=s.nid
  WHERE i.entity_id = :pnid
  and s.uid = :uid
  ';

  $result = db_query($query, array(':pnid' => $proposal_nid, ':uid' => $uid))->fetchAll();

  foreach ($result as $r) {
    $query = '
    select data from webform_submitted_data d
    where d.sid = :sid
    and   d.cid = :cid
    ';

    $result1 = db_query($query, array(':sid' => $r->sid, ':cid' => $r->cid))->fetchAll();

    $response_vals = array();
    foreach($result1 as $r1) {
      if (!empty($r1->data)) {
        $response_vals[] = $r1->data;
      }
    }

    $responses = array();
    $data = unserialize($r->extra);
    if (isset($data['items'])) {
      $pairs = explode("\n", $data['items']);
      foreach($pairs as $pair) {
        $options = explode("|", $pair);
        $vals[$options[0]] = $options[1];
      }
      //print_r($vals);
      foreach ($response_vals as $r_val) {
        $responses[] = $vals[$r_val];
      }
      $return[] = array('question' => $r->name, 'responses' => $responses);
    }
    else {
      if (!empty($r1->data)) {
        $return[] = array(
          'question' => $r->name,
          'responses' => array($r1->data)
        );
      }
    }

  }

  return $return;

}