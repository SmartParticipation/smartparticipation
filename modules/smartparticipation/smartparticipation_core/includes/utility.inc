<?php

/** 
 * RegRoom utility functions.
 */

/**
 * @return Remove spaces from site name and append '.org'
 * Example: Regulation Room => RegulationRoom.org
 */
function smartparticipation_core_nice_domain_name() {
  $site_name = variable_get('site_name'); 
  return str_replace(' ', '', $site_name) . '.org';
}
