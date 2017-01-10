<?php

use SmartParticipation\model\ABTest;
use SmartParticipation\model\ABTestUserVariant;

function _smartparticipation_export_ab_test_csv($proposal)
{
  $proposal_name = smartparticipation_core_slug($proposal->title);
  $current_time = date('Y-m-d-h-i', time()); // year-month-date-hour-minute
  $csv_filename = 'ab-test-descriptions-' . $proposal_name . '-' . $current_time . '.csv';

  drupal_add_http_header('Content-Type', 'text/csv; utf-8');
  drupal_add_http_header('Content-Disposition', 'attachment; filename=' . $csv_filename);

  $output = fopen("php://output", 'w');

  $headings = array(
    ABTest::getLabel('abid'),
    ABTest::getLabel('title'),
    ABTest::getLabel('description'),
    ABTest::getLabel('variant_a_title'),
    ABTest::getLabel('variant_b_title'),
    ABTest::getLabel('created'),
    ABTest::getLabel('modified'),
  );

  fputcsv($output, $headings);

//  $min_start_time = _smartparticipation_core_get_proposal_min_start_time ($proposal);
//  $max_end_time = _smartparticipation_core_get_proposal_max_end_time($proposal);

  $ab_test_service = new \SmartParticipation\ABTestService;
  $ab_tests = $ab_test_service->findAll();
  foreach ($ab_tests as $ab_test) {

//    if ($ab_test->created < $min_start_time || $ab_test->created > $max_end_time) {
//      continue;
//    }
    $row = array (
      $ab_test->abid,
      $ab_test->title,
      $ab_test->description,
      $ab_test->variant_a_title,
      $ab_test->variant_b_title,
      getNiceDateFormat($ab_test->created),
      getNiceDateFormat($ab_test->modified),
    );
    fputcsv($output, $row);
  }

  fclose($output);

  exit;
}

function _smartparticipation_export_ab_test_assignments_csv($proposal)
{
  $proposal_name = smartparticipation_core_slug($proposal->title);
  $current_time = date('Y-m-d-h-i', time()); // year-month-date-hour-minute
  $csv_filename = 'ab-test-assignments-' . $proposal_name . '-' . $current_time . '.csv';

  drupal_add_http_header('Content-Type', 'text/csv; utf-8');
  drupal_add_http_header('Content-Disposition', 'attachment; filename=' . $csv_filename);

  $output = fopen("php://output", 'w');

  $headings = array(
    ABTestUserVariant::getLabel('abid'),
    ABTestUserVariant::getLabel('uid'),
    t('Username'),
    ABTestUserVariant::getLabel('variant'),
    ABTestUserVariant::getLabel('assigned'),
  );

  fputcsv($output, $headings);

  $ab_test_user_variant_service = new \SmartParticipation\ABTestUserVariantService;
  $ab_test_assignments = $ab_test_user_variant_service->findAll();
  $user_service = new \SmartParticipation\UserService;
  foreach ($ab_test_assignments as $ab_test_assignment) {
    $username = '';
    if ($ab_test_assignment->uid) {
      $user = $user_service->find($ab_test_assignment->uid);
      if (!empty($user->name)) {
        $username = $user->name;
      }
    }
    $row = array (
      $ab_test_assignment->abid,
      $ab_test_assignment->uid,
      $username,
      $ab_test_assignment->variant,
      getNiceDateFormat($ab_test_assignment->assigned),
    );
    fputcsv($output, $row);
  }

  fclose($output);

  exit;
}