<?php


namespace SmartParticipation;


use PDOException;
use SmartParticipation\model\ABTest;
use SmartParticipation\model\ABTestUserVariant;
use SmartParticipation\model\User;

class ABTestUserVariantService
{
  const AB_TEST_USER_VARIANT_TABLE = 'sp_ab_test_user_variant';

  public function find(ABTest $ab_test, User $user)
  {
    $query = db_select(self::AB_TEST_USER_VARIANT_TABLE, 'abuv')
      ->fields('abuv', array('variant','assigned'))
      ->condition('abid', $ab_test->abid)
      ->condition('uid', $user->uid);
    $result = $query->execute()->fetchAssoc();
    if (!$result) {
      $user_variant = $this->assignUser($ab_test, $user);
    } else {
      $user_variant = new ABTestUserVariant(
        $ab_test->abid,
        $user->uid,
        $result['variant'],
        $result['assigned']
      );
    }
    return $user_variant;
  }

  /**
   * @return ABTestUserVariant[]
   */
  public function findAll()
  {
    $query = db_select(self::AB_TEST_USER_VARIANT_TABLE, 'abuv')
      ->fields('abuv', array(
        'abid',
        'uid',
        'variant',
        'assigned'
      ))
      ->orderBy('assigned', 'DESC');

    $result = $query->execute()->fetchAll();

    $user_variants = array();
    foreach ($result as $row) {
      $user_variants[] = new ABTestUserVariant(
        $row->abid,
        $row->uid,
        $row->variant,
        $row->assigned
      );
    }

    return $user_variants;
  }

  public function findLastVariant(ABTest $ab_test)
  {
    $query = db_select(self::AB_TEST_USER_VARIANT_TABLE, 'abuv')
      ->fields('abuv', array('variant'))
      ->condition('abid', $ab_test->abid)
      ->orderBy('assigned', 'DESC')
      ->range(0, 1);
    //$query->addExpression('MAX(assigned)', 'max_assigned');
    $last_variant = $query->execute()->fetchField();
    return $last_variant;
  }

  public function save(ABTest $ab_test, User $user, $variant)
  {
    try {
      $assigned = time();
      db_insert(self::AB_TEST_USER_VARIANT_TABLE)
        ->fields(array(
          'abid' => $ab_test->abid,
          'uid' => $user->uid,
          'variant' => $variant,
          'assigned' => $assigned,
        ))
        ->execute();
      return $assigned;
    } catch (PDOException $e) {
      return FALSE;
    }
  }

  protected function assignUser(ABTest $ab_test, User $user)
  {
    $variant = ABTestUserVariant::VARIANT_A;
    // Look up last assignment - assign user the opposite
    if ($last_variant = $this->findLastVariant($ab_test)) {
      $variant = $last_variant != ABTestUserVariant::VARIANT_A ? ABTestUserVariant::VARIANT_A : ABTestUserVariant::VARIANT_B;
    }
    if ($assigned = $this->save($ab_test, $user, $variant)) {
      return new ABTestUserVariant(
        $ab_test->abid,
        $user->uid,
        $variant,
        $assigned
      );
    }
    return null;
  }
}