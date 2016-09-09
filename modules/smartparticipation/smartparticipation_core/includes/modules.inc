<?php

/* WEBFORM  */

function smartparticipation_core_configure_webform() {
  // enable webform functionality for sp_interest_survey content type
  $types = variable_get('webform_node_types', array('webform'));
  $types[] = 'sp_interest_survey';
  variable_set('webform_node_types', $types);
}

function smartparticipation_core_unconfigure_webform() {
  variable_del('webform_node_types');
}


/* HONEYPOT */
  
function smartparticipation_core_configure_honeypot() {
  variable_set('honeypot_element_name', 'name');
}

function smartparticipation_core_unconfigure_honeypot() {
  // Restore default value
  variable_set('honeypot_element_name', 'url');
}
