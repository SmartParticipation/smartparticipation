<?php

namespace SmartParticipation;


class DrupalPageService
{
  /**
   * @param array $page_variables Drupal preprocess_page variables
   * @return bool
   */
  public function pageContainsInterestSurvey(array $page_variables)
  {
    if (!empty($page_variables['page']['content']['system_main']['nodes'])) {
      $system_main_nodes = $page_variables['page']['content']['system_main']['nodes'];
      foreach ($system_main_nodes as $node) {
        if (!empty($node['interest_survey'])) {
          return TRUE;
        }
      }
    }
    return FALSE;
  }

  /**
   * @param array $page_variables Drupal preprocess_page variables
   */
  public function disablePageMessages(array &$page_variables)
  {
    $page_variables['show_messages'] = FALSE;
  }
}