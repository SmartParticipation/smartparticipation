<?php 

/**
 * SmartParticipation custom event logging.
 */

/**
 * Return true iff events should be logged: event logging is turned on, and
 * the current user is either an anonymous or a basic authenticated user.
 */
function smartparticipation_core_log_events() {

  return variable_get('smartparticipation_log_events') &&
      smartparticipation_core_is_anonymous_or_basic_authenticated_user();
}

/**
 * Log client events submitted via an Ajax request. We do not need to check 
 * smartparticipation_core_log_events(), because the JavaScript used to log the event is not
 * loaded if those conditions are not satisfied. Event data is retrieved from
 * the $_POST variable. Send success or failure message back to the client.
 */
function smartparticipation_core_log_client_event() {
  
  // TODO - no access in full page view - only via Ajax
  
  $result = smartparticipation_core_log_event($_POST);
  drupal_json_output($result);
}

/**
 * Log server-side events. Check first whether the conditions for event logging
 * are satified.
 * 
 * @param $event - an associative array of event data
 * 
 * @return false if the event should not be logged, otherwise the return value
 * of smartparticipation_core_log_event().
 */
function smartparticipation_core_log_server_event($event) {
  
  if (!smartparticipation_core_log_events()) {  
    return false;
  }
  
  return smartparticipation_core_log_event($event);
}

/**
 * Write event data to the smartparticipation_core_event_log. Handles both client-side and
 * server-side events. Applies default values that are not included in the 
 * event data.
 * 
 * @param $event - an associative array of event data
 * 
 * @return the return value of drupal_write_record (1 or 0, depending on whether
 * the event was successfully written to the database or not).
 */
function smartparticipation_core_log_event($event) {
  
  global $user;

  $defaults = array(
    'hostname' => isset($user->hostname) ? $user->hostname : ip_address(),
    'path' => '/' . request_path(),
    //'sid' => $user->sid,
    'timestamp' => time(),
    'uid' => $user->uid,
  );

  $session_id = session_id();
  if (!empty($session_id)) {
    $defaults['sid'] = $session_id;
  }
  
  $event += $defaults;
  
  return drupal_write_record('sp_event_log', $event);
  
}