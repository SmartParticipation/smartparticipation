<?php


namespace SmartParticipation;


use PDO;
use PDOException;
use SelectQuery;
use SmartParticipation\model\ABTest;
use SmartParticipation\model\User;

/**
 * Class ABTestService
 * @package SmartParticipation
 */
class ABTestService
{
  const AB_TEST_TABLE = 'sp_ab_test';

  /**
   * @param ABTest $ab_test
   * @return int num records affected
   */
  public function save(ABTest $ab_test)
  {
    $fields = array(
      'abid' => $ab_test->abid,
      'title' => $ab_test->title,
      'description' => $ab_test->description,
      'variant_a_title' => $ab_test->variant_a_title,
      'variant_b_title' => $ab_test->variant_b_title,
    );
    // Create a new A/B test.
    if (!$this->testExists($ab_test->abid)) {
      try {
        db_insert(self::AB_TEST_TABLE)
          ->fields(array_merge(array('created' => time()), $fields))
          ->execute();
        return TRUE;
      } catch (PDOException $e) {
        return FALSE;
      }
    }
    // Update an A/B test.
    else {
      try {
        return db_update(self::AB_TEST_TABLE)
          ->fields(array_merge(array('modified' => time()), $fields))
          ->condition('abid', $ab_test->abid)
          ->execute();
      } catch (PDOException $e) {
        return FALSE;
      }
    }
  }

  /**
   * @param array $form_values Drupal array of values (see mapSubmit)
   */
  public function saveSubmit($form_values)
  {
    $ab_test = $this->mapSubmit($form_values);
    // TODO this saves but returns 0
    return $this->save($ab_test);
  }

  /**
   * @param string $abid
   * @return ABTest
   */
  public function find($abid)
  {
    $ab_test = $this->_find(array('abid' => $abid));
    return $ab_test[$abid];
  }

  /**
   * @return ABTest[]
   */
  public function findAll()
  {
    return $this->_find();
  }

  /**
   * @param array $conditions
   * @return ABTest[]
   */
  private function _find(array $conditions = null)
  {
    $query = db_select(self::AB_TEST_TABLE, 'ab')
      ->fields('ab', array(
        'abid',
        'title',
        'description',
        'variant_a_title',
        'variant_b_title',
        'created',
        'modified',
      ));

    if (!empty($conditions)) {
      foreach ($conditions as $field => $value) {
        $query->condition($field, $value);
      }
    }

    $result = $query->execute()
      ->fetchAllAssoc('abid', PDO::FETCH_BOTH);

    $ab_tests = array();
    foreach ($result as $abid => $ab_test) {
      $ab_tests[$abid] = $this->mapQuery($ab_test);
    }

    return $ab_tests;
  }

  /**
   * @return array
   */
  public function getOptionsList()
  {
    $options = array();
    $ab_tests = $this->findAll();
    foreach ($ab_tests as $ab_test) {
      $options[$ab_test->abid] = $ab_test->title;
    }
    return $options;
  }

  /**
   * @param $abid
   * @return int num records deleted
   */
  public function delete($abid)
  {
    // Delete the A/B test with matching ID.
    return db_delete(self::AB_TEST_TABLE)
      ->condition('abid', $abid)
      ->execute();
  }

  /**
   * @param string $value
   * @return bool
   */
  public function testExists($abid)
  {
    return db_query_range('SELECT 1 FROM {sp_ab_test} WHERE abid = :abid', 0, 1, array(':abid' => $abid))->fetchField();
  }

  /**
   * @param array $data submit data from Drupal
   * @return ABTest
   *
   * Map form submission values to an ABTest object.
   *
   * Example Drupal submit data:
   *
   * array (size=10)
   * 'abid' => string 'comment_tips_test_1' (length=19)
   * 'title' => string 'Effectiveness of comment tips' (length=29)
   * 'description' => string 'Only show comment tips for users assigned to variant B.' (length=55)
   * 'variant_a_title' => string 'Don't show comment tips' (length=23)
   * 'variant_b_title' => string 'Show comment tips' (length=17)
   * 'submit' => string 'Save' (length=4)
   * 'form_build_id' => string 'form-iB2RFdcfFJqpl72LYlrGFjh3zmVqPqLbnBhRr7opYBk' (length=48)
   * 'form_token' => string 'O3xfgDzsi5RHT74HmSGvzV4MLeg8rZNVgjJF6Vseo6g' (length=43)
   * 'form_id' => string 'smartparticipation_core_ab_test_form' (length=20)
   * 'op' => string 'Save' (length=4)
   */
  protected function mapSubmit($data)
  {
    return new ABTest(
      $data['abid'],
      $data['title'],
      $data['description'],
      $data['variant_a_title'],
      $data['variant_b_title']
    );
  }

  /**
   * @param array $result
   * @return ABTest
   *
   * Map query result values to an ABTest object.
   *
   */
  protected function mapQuery($result)
  {
    return new ABTest(
      $result['abid'],
      $result['title'],
      $result['description'],
      $result['variant_a_title'],
      $result['variant_b_title'],
      $result['created'],
      $result['modified']
    );
  }

  public function userIsEligible(User $user)
  {
    $drupal_user = user_load($user->uid);
    return smartparticipation_core_is_basic_authenticated_user($drupal_user);
  }

}