<?php

use SmartParticipation\ABTestService;
use SmartParticipation\model\ABTest;

function smartparticipation_ab_test_admin_list()
{
  $titles = ABTest::getAttributeLabels();

  $header = array(
    array('data' => $titles['abid'], 'field' => 'abid'),
    array('data' => $titles['title'], 'field' => 'title'),
    array('data' => $titles['description'], 'field' => 'description'),
    array('data' => $titles['variant_a_title'], 'field' => 'variant_a_title'),
    array('data' => $titles['variant_b_title'], 'field' => 'variant_b_title'),
    array('data' => $titles['created'], 'field' => 'created'),
    array('data' => $titles['modified'], 'field' => 'modified', 'sort' => 'desc'),
    array('data' => t('Actions')),
  );

  $query = db_select(ABTestService::AB_TEST_TABLE, 'ab')->fields('ab', array(
    'abid',
    'title',
    'description',
    'variant_a_title',
    'variant_b_title',
    'created',
    'modified',
  ));

  $table_sort = $query->extend('TableSort')->orderByHeader($header);
  $pager = $table_sort->extend('PagerDefault')->limit(100);
  $result = $pager->execute();

  $rows = array();
  foreach($result as $row) {

    $row_actions =
      "<a href=\"ab_test/$row->abid/edit\">Edit</a>"
      . " | "
      // Something strange is going on with the confirm (Drupal?!?) - it was triggering regardless of choice
      // ...so setting href in roundabout way to avoid delete when clicking Cancel
      . "<a href=\"#\" onclick=\"javascript:if (confirm('Are you sure?')) { this.href='ab_test/$row->abid/delete'; }\">Delete</a>";

    $rows[] = array(
      $row->abid,
      $row->title,
      $row->description,
      $row->variant_a_title,
      $row->variant_b_title,
      format_date($row->created, 'short'),
      !empty($row->modified) ? format_date($row->modified, 'short') : '-',
      $row_actions,
    );
  }

  // If rows are not empty theme and display the rows.
  $output = '';
  if (!empty($rows)) {
    $output = theme('table', array('header' => $header, 'rows' => $rows, 'attributes' => array('id' => 'sort-table')));
    $output .= theme('pager');
  }
  else {
    $output .= t("No results found.");
  }
  $output .= '<br /><a href="ab_test/add">Add new A/B test</a>';
  return $output;
}

function smartparticipation_ab_test_admin_form($form, &$form_state, $abid = null)
{
  $form = array();

  /** @var ABTest|null $ab_test */
  $ab_test = null;
  if (!empty($abid)) {
    $ab_test_service = new \SmartParticipation\ABTestService;
    $ab_test = $ab_test_service->find($abid);
  }
  $titles = ABTest::getAttributeLabels();

  $form['abid'] = array(
    '#type' => 'machine_name',
    /*'#default_value' => !empty($ab_test->abid) ? $ab_test->abid : '',*/
    '#maxlength' => 32,
    '#machine_name' => array(
      'exists' => 'smartparticipation_ab_test_id_exists',
    ),
    '#weight' => 1,
  );
  if (!empty($ab_test->abid)) {
    $form['abid']['#default_value'] = $ab_test->abid;
    $form['abid']['#attributes'] = array('disabled' => 'disabled');
  }

  $form['title'] = array(
    '#type' => 'textfield',
    '#title' => $titles['title'],
    '#description' => t('The A/B test title.'),
    '#required' => TRUE,
    '#default_value' => !empty($ab_test->title) ? $ab_test->title : '',
    '#size' => 60,
    '#weight' => 5,
  );

  $form['description'] = array(
    '#type' => 'textarea',
    '#title' => $titles['description'],
    '#description' => t('Describe the test variants.'),
    '#required' => FALSE,
    '#default_value' => !empty($ab_test->description) ? $ab_test->description : '',
    '#weight' => 10,
    '#rows' => 10,
  );

  $form['variant_a_title'] = array(
    '#type' => 'textfield',
    '#title' => $titles['variant_a_title'],
    '#description' => 'The title for the first variant.',
    '#default_value' => !empty($ab_test->variant_a_title) ? $ab_test->variant_a_title : '',
    '#required' => TRUE,
    '#weight' => 15,
  );

  $form['variant_b_title'] = array(
    '#type' => 'textfield',
    '#title' => $titles['variant_b_title'],
    '#description' => 'The title for the second variant.',
    '#default_value' => !empty($ab_test->variant_b_title) ? $ab_test->variant_b_title : '',
    '#required' => TRUE,
    '#weight' => 20,
  );

  $form['actions']['#type'] = 'actions';
  $form['actions']['submit'] = array('#type' => 'submit', '#value' => t('Save'));

  $form['#submit'][] = 'smartparticipation_ab_test_admin_submit';

  return $form;
}

function smartparticipation_ab_test_admin_submit($form, &$form_state)
{
  $ab_test_service = new \SmartParticipation\ABTestService;
  /** @var \SmartParticipation\model\ABTest $ab_test */
  //$ab_test = $ab_test_service->mapSubmit($form_state['values']);
  if ($ab_test_service->saveSubmit($form_state['values'])) {
    drupal_set_message('The A/B test has been saved.');
  } else {
    drupal_set_message('Error - unable to save A/B test.', 'error');
  }
  drupal_goto('admin/config/system/ab_test');
}

function smartparticipation_core_ab_test_id_exists($value)
{
  $ab_test_service = new \SmartParticipation\ABTestService;
  return $ab_test_service->testExists($value);
}

function smartparticipation_ab_test_admin_delete($abid)
{
  $ab_test_service = new ABTestService;
  if ($ab_test_service->delete($abid)) {
    drupal_set_message('The A/B test has been deleted.');
  } else {
    drupal_set_message('Error - unable to delete A/B test.', 'error');
  }
  drupal_goto('admin/config/system/ab_test');
}