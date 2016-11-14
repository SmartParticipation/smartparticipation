<?php

/**
 * @file
 * Create the proposal phase and proposal phase schema vocabularies and add 
 * initial vocabulary terms.
 * 
 * @ingroup smartparticipation_core
 */


/**
 * Create the proposal phase vocabulary.
 * 
 */
function smartparticipation_core_create_proposal_phase_vocabulary($weight = 1) {
  smartparticipation_core_add_vocabulary('Proposal Phases', 'sp_proposal_phase',
    'Phases of the proposal participation process.', $weight);

  // Create the fields we are adding to the vocabulary.
  foreach (smartparticipation_core_proposal_phase_vocabulary_installed_fields() as $field) {
    $field['entity_types'] = array('taxonomy_term');  
    field_create_field($field);
  }

  // Create all the instances for our fields.
  foreach (smartparticipation_core_proposal_phase_vocabulary_installed_instances() as $instance) {
    $instance['bundle'] = 'sp_proposal_phase';
    $instance['entity_type'] = 'taxonomy_term';
    field_create_instance($instance);
  }
 
}


/**
 * Returns a structured array defining the fields defined for terms in this
 * vocabulary.
 *
 * This is factored into a function so it can be used in both
 * smartparticipation_core_install() and smartparticipation_core_uninstall().
 *
 * @return
 *  An associative array specifying the fields we wish to add to our
 *  new node type.
 *
 */
function smartparticipation_core_proposal_phase_vocabulary_installed_fields() {
  $t = get_t();
  
  return array(

    'field_sp_phase_allows_comments' => array(
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'field_name' => 'field_sp_phase_allows_comments',
      'foreign keys' => array(),
      'indexes' => array(
        'value' => array(
          0 => 'value',
        ),
      ),
      'locked' => '1',
      'module' => 'list',
      'settings' => array(
        'allowed_values' => array(
          0 => $t('Topics in this phase do not accept comments'),
          1 => $t('Topics in this phase accept comments'),
        ),
        'allowed_values_function' => '',
      ),
      'translatable' => '1',
      'type' => 'list_boolean',
    ),

    'field_sp_phase_text_heading' => array(
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'field_name' => 'field_sp_phase_text_heading',
      'foreign keys' => array(
        'format' => array(
          'columns' => array(
            'format' => 'format',
          ),
          'table' => 'filter_format',
        ),
      ),
      'indexes' => array(
        'format' => array(
          0 => 'format',
        ),
      ),
      'locked' => '0',
      'module' => 'text',
      'settings' => array(
        'max_length' => '100',
      ),
      'translatable' => '0',
      'type' => 'text',
    ),
      
  );

}

/**
 * Returns a structured array defining the field instances for terms in this
 * vocabulary.
 *
 * The instance lets Drupal know which widget to use to allow the user to enter
 * data and how to react in different view modes. 
 *
 * This is factored into this function so it can be used in both
 * smartparticipation_core_install() and smartparticipation_core_uninstall().
 *
 * @return
 *  An associative array specifying the instances we wish to add to our new
 *  node type.
 *
 */
function smartparticipation_core_proposal_phase_vocabulary_installed_instances() {
  $t = get_t();
  return array(
  
    // This key is an arbitrary string  
    'field_sp_phase_allows_comments' => array(
      'default_value' => NULL,
      'deleted' => '0',
      'description' => '',
      'display' => array(
        'default' => array(
          'label' => 'hidden',
          'module' => 'list',
          'settings' => array(),
          'type' => 'list_default',
          'weight' => '0',
        ),
      ),
      'field_name' => 'field_sp_phase_allows_comments',
      'label' => $t('Allows comments'),
      'required' => 1,
      'settings' => array(
        'user_register_form' => FALSE,
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'options',
        'settings' => array(),
        'type' => 'options_buttons',
        'weight' => '10',
      ),
    ),

    'field_sp_phase_text_heading' => array(
      'default_value' => NULL,
      'deleted' => '0', 
      'description' => $t('Heading used above subtopic text. Case-sensitive. Defaults to term name.'),
      'display' => array(
        'default' => array(
          'type' => 'hidden',
        ),
        'teaser' => array(
          'type' => 'hidden',
        ),
      ), 
      'field_name' => 'field_sp_phase_text_heading',
      'label' => $t('Subtopic text heading'),
      'required' => 0,
      'settings' => array(
        'text_processing' => '0',
        'user_register_form' => FALSE,
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'text',
        'settings' => array(
          'size' => '75',
        ),
        'type' => 'text_textfield',
        'weight' => '0',
      ),   
    ),
        

  );  
}

/**
 * Add default proposal phase schema and phases.
 */
function smartparticipation_core_add_default_proposal_phases() {

  smartparticipation_core_create_term('sp_proposal_phase', 'Discussion', 0, '', 0,
    smartparticipation_core_set_term_field_values(
      array(
        'field_sp_phase_allows_comments' => 1,
        'field_sp_phase_text_heading' => '',
      )
    )
  );

  smartparticipation_core_create_term('sp_proposal_phase', 'Draft Discussion Summary', 0, '', 0,
    smartparticipation_core_set_term_field_values(
      array(
        'field_sp_phase_allows_comments' => 1,
        'field_sp_phase_text_heading' => 'Draft Summary of Discussion',
      )
    )
  );

  smartparticipation_core_create_term('sp_proposal_phase', 'Final Discussion Summary', 0, '', 0,
    smartparticipation_core_set_term_field_values(
      array(
        'field_sp_phase_allows_comments' => 0,
        'field_sp_phase_text_heading' => 'Final Summary of Discussion',
      )
    )
  );

  smartparticipation_core_create_term('sp_proposal_phase', 'Final Rule', 0, '', 0,
    smartparticipation_core_set_term_field_values(
      array(
        'field_sp_phase_allows_comments' => 0,
        'field_sp_phase_text_heading' => 'Final Rule',
      )
    )
  );
  
}


/** 
 * Delete the proposal phase vocabulary.
 */
function smartparticipation_core_delete_proposal_phase_vocabulary() {
     
  foreach (smartparticipation_core_proposal_phase_installed_fields() as $key => $field) {
    field_delete_field($field['field_name']);
  }

  /* Not needed. field_delete_field() already marks these for deletion.
  // Delete field instances defined for this vocabulary.
  foreach (smartparticipation_core_proposal_phase_installed_fields() as $key => $instance) {
    field_delete_instance($instance);
  }
  */

  smartparticipation_core_delete_vocabulary('sp_proposal_phase');
  
  field_purge_batch(1000);  
}


/**
 * Customize the proposal phase term form.
 */
function smartparticipation_core_proposal_phase_vocabulary_form_term_alter(&$form, &$form_state, $form_id) {
  
  /* Set default description format to plain text for future use as hover text, 
   * etc. Doesn't prevent users from switching formats, but encourages the use 
   * of plain text. Drupal 8 will convert description to an ordinary field so 
   * that it can be configured like other fields. For now we will use JavaScript
   * to hide the format selector.
   */
  $form['description']['#format'] = 'plain_text';
  $form['relations']['#access'] = FALSE;
  $form['#validate'][] = 'smartparticipation_core_proposal_phase_vocabulary_form_term_validate';
  drupal_add_js(drupal_get_path('module', 'smartparticipation_core') . '/js/admin/proposal_phase_form_term.js');

}

/**
 * Custom validation for proposal phase vocabulary term form
 * 
 */
function smartparticipation_core_proposal_phase_vocabulary_form_term_validate($form, &$form_state) {

  // If no label has been specified, use the term name
  // NB Changing $form_state in a custom submit handler happens AFTER the data
  // has been saved to the db, so must use a validation function.
  /*if (! $form_state['values']['field_sp_phase_text_heading'][LANGUAGE_NONE][0]['value']) {
    $form_state['values']['field_sp_phase_text_heading'][LANGUAGE_NONE][0]['value'] =
        $form_state['values']['name'];
  }*/
}

/**
 * Before saving a proposal phase term, if the label field is unset, set it 
 * equal to the term name.
 */  
function smartparticipation_core_proposal_phase_term_presave($term) {
  
  $field_name = 'field_sp_phase_text_heading';
  // This handles the terms created in code, but not through the term add form.
  // That case is handled through hook_form_FORM_ID_alter().
  if (!isset($term->$field_name)) {
    $term->$field_name = array(
      LANGUAGE_NONE => array(
        0 => array(
          'value' => $term->name,
        ),
      ),
    );
  }
}  

/**
 * Return the proposal phase term name
 * 
 * @param $tid - the term tid
 */
function smartparticipation_core_proposal_phase_term_name($tid) {
  
  $term = taxonomy_term_load($tid);
  return $term->name;
  
}
