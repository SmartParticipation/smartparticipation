<?php


namespace SmartParticipation;


use SmartParticipation\model\ABTest;
use SmartParticipation\model\ABTestUserVariant;
use SmartParticipation\model\CommentTips;

class ABTestCommentTipsService extends ABTestService
{
  const ACTIVE_TEST_VAR_NAME = 'smartparticipation_comment_support_tips_active_ab_test';

  const TEST_1 = 'comment_tips_effectiveness';

  //const TEST_2 = 'comment_tips_test_2';

  /**
   * @return null|ABTest
   */
  public function getActiveABTest()
  {
    $ab_test_key = variable_get(self::ACTIVE_TEST_VAR_NAME, '');
    if ($ab_test_key) {
      return $this->find($ab_test_key);
    }
    return null;
  }

  /**
   * Return the variant information based on the active A/B test.
   * @param ABTestUserVariant $user_variant
   * @param CommentTipsService $comment_tips_service
   * @param int $proposal_nid
   * @param int $phase_tid
   * @return array|null
   */
  public function getTestInfo(ABTestUserVariant $user_variant, CommentTipsService $comment_tips_service, $proposal_nid, $phase_tid)
  {
    switch ($user_variant->abid) {

      // A/B test: Effectiveness of comment tips
      case self::TEST_1:

        switch ($user_variant->variant) {

          // Variant A:  Don't show comment tips
          case ABTestUserVariant::VARIANT_A:

            return array(
              'show_comment_tips' => FALSE
            );

            break;

          // Variant B:  Show comment tips
          case ABTestUserVariant::VARIANT_B:

            // Get comment tip object

            return array(
              'show_comment_tips' => TRUE
            );

            break;

        }

        break;

      /*
       * NOTE: The following case is not in use but is an example of how to run
       *       a future test where different versions of comment tips are used.
       */

      // A/B test: Compare effectiveness of different comment tips
      /*case self::TEST_2:

        switch ($user_variant->variant) {

          // Variant A: Use version 1 of comment tips
          case ABTestUserVariant::VARIANT_A:

            return array(
              'show_comment_tips' => TRUE
            );

            break;

          // Variant B: User version 2 of comment tips
          case ABTestUserVariant::VARIANT_B:

            $comment_tips = $comment_tips_service->findAll($proposal_nid, $phase_tid);
            // Use the second comment tips node for this proposal phase
            $comment_tips = $comment_tips[1];

            return array(
              'show_comment_tips' => TRUE,
              'comment_tips' => $comment_tips
            );

            break;

        }

        break;*/
    }

    return null;
  }

  /**
   * @param array $ab_test_info
   * @return bool
   */
  public function showCommentTips($ab_test_info)
  {
    return !empty($ab_test_info['show_comment_tips']);
  }

  /**
   * Return the alternate comment tips where the test is comparison of different versions.
   * @param array $ab_test_info
   * @return CommentTips|bool
   */
  public function getVariantCommentTips($ab_test_info)
  {
    return !empty($ab_test_info['comment_tips']) ? $ab_test_info['comment_tips'] : false;
  }
}