<?php
/**
 * @file
 * Functions related to smartparticipation_core vocabularies.
 */

 /**
 * Create custom smartparticipation_core taxonomy vocabularies. Each vocabulary will also add
 * terms where appropriate.
 * 
 */
function smartparticipation_core_create_vocabularies() {
  
  smartparticipation_core_create_faq_category_vocabulary(1);
   
  smartparticipation_core_create_proposal_phase_vocabulary(2);
  
  smartparticipation_core_create_glossary_vocabulary();
  
  smartparticipation_core_add_default_proposal_phases();
}

 /**
  * Add a vocabulary to the taxonomy tree. Does nothing if the vocabulary 
  * already exists.
  * 
  *  @param name - the human readable name for the vocabulary
  *  @param machine_name - the machine name for the vocabulary
  *  @param description - the human readable description for the vocabulary
  *  @param weight - the weight for the vocabulary
  * 
  * @return status. If the vocabulary was added, return the vid of the new
  * vocabulary. Otherwise, return -1.
  * 
  */
function smartparticipation_core_add_vocabulary($name, $machine_name, $description, $weight)  {
  
  $t = get_t();
  $vid = -1;
  
  $vocabulary = taxonomy_vocabulary_machine_name_load($machine_name);
  
  if (!$vocabulary)  {
    
    $vocabulary = array(
      'name' => $t($name),
      'machine_name' => $machine_name,
      'description' => $t($description),
      'hierarchy' => 1,
      'module' => 'smartparticipation_core',
      'weight' => $weight,
    );
    
    $vocabulary = (object) $vocabulary;        
    taxonomy_vocabulary_save($vocabulary);
    
    $vocabulary = taxonomy_vocabulary_machine_name_load($machine_name);
    $vid = $vocabulary->vid;
    
  }
  
  return $vid;
  
}

/**
 * Create a single taxonomy term and add to a specific vocabulary if not 
 * already present
 *  @param vocab - the machine name of the vocabulary
 *  @param term - the human readable name of the term to be added
 *  @param desc - the human readable description of the term to be added (Optional)
 *  @param weight - the weight of the term in the vocabulary list (Optional)
 *  @param parent - single parent id or array of parent ids (Optional)
 *  @param fields - an associative array of field keys and values to add to the term (Optional)
 * 
 * @return the id of the newly created term
 */
function smartparticipation_core_create_term($vocab, $term, $weight = 0,  $desc = '', $parent = 0, $fields = array())  {  
  $t = get_t();
  $added = NULL;
  $added = taxonomy_get_term_by_name($term);
  if ($added == '' || $added == NULL)  {
    $vocabulary = taxonomy_vocabulary_machine_name_load($vocab);
    $vid = $vocabulary->vid;

    $new_term = array(
      'name' => $t($term), 
      'description' => $t($desc), 
      'parent' => $parent, 
      'weight' => $weight,
      'vid' => $vid,
    );
    
    foreach ($fields as $key => $value) {
      $new_term[$key] = $value;
    }
    
    $term_obj = (object) $new_term;
    
    $status = taxonomy_term_save($term_obj);
    
    return $term_obj->tid; // return the new term id 
  }
}

/**
 * Create an array of simple field values from key-value pairs. This works only
 * with fields with simple values such as strings, booleans, etc. 
 * 
 * TODO Should pass in $langcode rather than hard-coding LANGUAGE_NONE?
 *  
 * @param values - key-pairs
 
 * @return an array of field value data to be saved 
 */
function smartparticipation_core_set_term_field_values($values = array()) {
  
  $field_values = array();
  
  foreach ($values as $key => $value) {
    $field_values[$key] = array(
      LANGUAGE_NONE => array(
        0 => array(
          'value' => $value),
       ),
    );
  }
  
  return $field_values;
}

/**
 * Implements hook_taxonomy_term_presave().
 */
function smartparticipation_core_taxonomy_term_presave($term) {

  switch ($term->vocabulary_machine_name) {

    case 'sp_proposal_phase':
      smartparticipation_core_proposal_phase_term_presave($term);
      break;
  }
}

/**
 * Form modifications for taxonomy term forms.
 * 
 */
function smartparticipation_core_form_taxonomy_form_term_alter(&$form, &$form_state, $form_id) {

  // Modifications applicable to all vocabularies (though could override in 
  // vocabulary-specific functions called below)
  $form['path']['#access'] = FALSE; 

  // Vocabulary-specific modifications
  $vocabulary = $form['#vocabulary'];
  if ($vocabulary->module == 'smartparticipation_core') {
    
    $vocabulary_machine_name = $vocabulary->machine_name;

    if ($vocabulary_machine_name == 'sp_proposal_phase') {
      smartparticipation_core_proposal_phase_vocabulary_form_term_alter($form, $form_state, $form_id);
    
    } else if (smartparticipation_core_is_glossary_vocabulary($vocabulary_machine_name)) {
      smartparticipation_core_glossary_vocabulary_form_term_alter($form, $form_state, $form_id);           
    }
  }
}
 
/**
 * Delete custom smartparticipation_core vocabularies. 
 * 
 */
function smartparticipation_core_delete_vocabularies() {
  smartparticipation_core_delete_faq_category_vocabulary();
  smartparticipation_core_delete_proposal_phase_vocabulary();
  smartparticipation_core_delete_proposal_glossary_vocabulary_all();
  smartparticipation_core_delete_glossary_vocabulary();
}

/**
 * Delete the specified vocabulary and any taxonomy terms created
 *  @machine_name - the machine name for the vocabulary to delete
 * 
 */
function smartparticipation_core_delete_vocabulary($machine_name)  {
  // delete the vocabularies and any taxonomy terms created
  $vid = db_select('taxonomy_vocabulary', 'v')
    ->fields('v', array('vid'))
    ->condition("v.machine_name", $machine_name)
    ->execute()
    ->fetchField();
  taxonomy_vocabulary_delete($vid);
}


/**
 * @return the maximum weight value for the vocabularies
 */
function smartparticipation_core_get_vocabulary_maximum_weight() {
  $query = 'SELECT weight FROM {taxonomy_vocabulary} ORDER BY weight DESC LIMIT 1';
  $result = db_query($query);
  $record = $result->fetchObject();
  $count = $result->rowCount();
  
  return $count > 0 ? $record->weight : 0;

}

// Get the themable table form with drop down functionality
function theme_smartparticipation_core_taxonomy_overview_terms($variables) {
  $form = $variables['form'];

  $page_increment = $form['#page_increment'];
  $page_entries = $form['#page_entries'];
  $back_step = $form['#back_step'];
  $forward_step = $form['#forward_step'];

  // Add drag and drop if parent fields are present in the form.
  if ($form['#parent_fields']) {
    drupal_add_tabledrag('taxonomy', 'match', 'parent', 'term-parent', 'term-parent', 'term-id', FALSE);
    drupal_add_tabledrag('taxonomy', 'depth', 'group', 'term-depth', NULL, NULL, FALSE);
    drupal_add_js(drupal_get_path('module', 'taxonomy') . '/taxonomy.js');
    drupal_add_js(array('taxonomy' => array('backStep' => $back_step, 'forwardStep' => $forward_step)), 'setting');
    drupal_add_css(drupal_get_path('module', 'taxonomy') . '/taxonomy.css');
  }
  drupal_add_tabledrag('taxonomy', 'order', 'sibling', 'term-weight');

  $errors = form_get_errors() != FALSE ? form_get_errors() : array();
  $rows = array();
  foreach (element_children($form) as $key) {
    if (isset($form[$key]['#term'])) {
      $term = &$form[$key];

      $row = array();
      $row[] = (isset($term['#term']['depth']) && $term['#term']['depth'] > 0 ? theme('indentation', array('size' => $term['#term']['depth'])) : '') . drupal_render($term['view']);
      if ($form['#parent_fields']) {
        $term['tid']['#attributes']['class'] = array('term-id');
        $term['parent']['#attributes']['class'] = array('term-parent');
        $term['depth']['#attributes']['class'] = array('term-depth');
        $row[0] .= drupal_render($term['parent']) . drupal_render($term['tid']) . drupal_render($term['depth']);
      }
      //$term['weight']['#attributes']['class'] = array('term-weight');
      //$row[] = drupal_render($term['weight']);
      $row[] = $term['#term']['description'];
      $row[] = drupal_render($term['edit']);
      $row = array('data' => $row);
      $rows[$key] = $row;
    }
  }

  // Add necessary classes to rows.
  $row_position = 0;
  foreach ($rows as $key => $row) {
    $rows[$key]['class'] = array();
    if (isset($form['#parent_fields'])) {
      $rows[$key]['class'][] = 'draggable';
    }

    // Add classes that mark which terms belong to previous and next pages.
    if ($row_position < $back_step || $row_position >= $page_entries - $forward_step) {
      $rows[$key]['class'][] = 'taxonomy-term-preview';
    }

    if ($row_position !== 0 && $row_position !== count($rows) - 1) {
      if ($row_position == $back_step - 1 || $row_position == $page_entries - $forward_step - 1) {
        $rows[$key]['class'][] = 'taxonomy-term-divider-top';
      }
      elseif ($row_position == $back_step || $row_position == $page_entries - $forward_step) {
        $rows[$key]['class'][] = 'taxonomy-term-divider-bottom';
      }
    }

    // Add an error class if this row contains a form error.
    foreach ($errors as $error_key => $error) {
      if (strpos($error_key, $key) === 0) {
        $rows[$key]['class'][] = 'error';
      }
    }
    $row_position++;
  }

  if (empty($rows)) {
    $rows[] = array(array(
      'data' => $form['#empty_text'],
      'colspan' => '3',
    ));
  }
  
  unset($form['#weight']);
  //dsm($form);
  //$header = array(t('Term'), t('Weight'), t('Definition'), t('Operations'));
  $header = array(t('Term'), t('Definition'), t('Operations'));
  $output = theme('table', array('header' => $header, 'rows' => $rows, 'attributes' => array('id' => 'taxonomy')));
  $output .= drupal_render_children($form);
  $output .= theme('pager');

  return $output;
}


/**
 * Implements hook_form_FORM_ID_alter().
*/
function smartparticipation_core_form_taxonomy_overview_terms_alter(&$form, &$form_state, $form_id) {

  if (smartparticipation_core_is_glossary_vocabulary($form['#vocabulary']->machine_name)) {
    smartparticipation_core_form_glossary_overview_terms_alter($form, $form_state, $form_id);
  }

}