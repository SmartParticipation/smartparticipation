<?php

/**
 * Track an outreach id value in customized links. The cookie value will be
 * sent as a custom variable to Google Analytics (GA) on subsequent page views, 
 * allowing tracking of all subsequent page views of these users by the outreach 
 * id.
 *
 * REMOVED 
 * This doesn't work due to Drupal page caching.  The page is cached with the
 * customVar value, so the results aren't accurate.
 */
/* 
function smartparticipation_core_set_cookie_from_query_param() {

  $query = drupal_get_query_parameters();  
  
  if (isset($query['oid'])) {

    user_cookie_save(array(
      'oid' => $query['oid'],
    ));

    // Remove the triggering query to prevent an infinite loop of redirection
    unset($query['oid']);   

    // Reload the page to send GA the custom variable.
    // TODO is there an API call to send it to GA without a reload?
    // Unfortunately, a status code of 200 is not valid.
    drupal_goto(request_path(), array(
      'query' => $query,
    ));

  }
}
*/

/**
 * Add the GA oid custom variable based on a cookie value.
 *
 * REMOVED 
 * This doesn't work due to Drupal page caching.  The page is cached with the
 * customVar value, so the results aren't accurate.
 */
/* 
function smartparticipation_core_add_ga_custom_variables() {

  if (module_exists('googleanalytics') &&
      smartparticipation_core_is_anonymous_or_basic_authenticated_user() &&
      isset($_COOKIE['Drupal_visitor_oid'])) {
       
    $oid = $_COOKIE['Drupal_visitor_oid'];
        
    $ga_script = 'var _gaq = _gaq || [];' .                     
                 "_gaq.push(['_setCustomVar', 5, 'oid', '$oid', 3]);";  
        
    drupal_add_js($ga_script, array(
      'type' => 'inline',
      'scope' => 'header',
    ));
  } 
}
*/