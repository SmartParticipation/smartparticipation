<?php


namespace SmartParticipation;


use SmartParticipation\model\ShowMoreCommentsResult;

/**
 * Defines and processes a show more request for featured comments.
 *
 * Class ShowMoreFeaturedComments
 * @package SmartParticipation
 */
class ShowMoreFeaturedComments
{
  const DEFAULT_COMMENT_COUNT = 3;

  /** @var int return comment ids greater than max */
  private $max_id = null;

  /** @var int number of comments ids to return */
  private $count = self::DEFAULT_COMMENT_COUNT;

  /** @var int Drupal node id */
  private $proposal_nid = null;

  /**
   * Return Drupal menu route for featured comments ajax request.
   * @return array
   */
  public static function getMenuRoute()
  {
    // return ajax request route
    $menu_item['ajax/featured_comments/%'] = array(
      'title' => 'Get comment tips state',
      'page callback' => 'smartparticipation_show_more_featured_comments',
      'page arguments' => array(2),
      'access callback' => TRUE,
    );
    return $menu_item;
  }

  /**
   * Return JSON containing comment ids and metadata.
   * @param string $comment_type
   */
  public function request($comment_type)
  {

    $this->setPropertiesFromArguments();

    switch ($comment_type) {

      case 'recent':
        $show_more_request = new ShowMoreRecentCommentsRequest;
        $featured_comments_service = new RecentCommentsService;
        break;

      case 'recommended':
        $show_more_request = new ShowMoreRecommendedCommentsRequest;
        $featured_comments_service = new RecommendedCommentsService;
        break;

      default:
        return drupal_not_found();
    }

    /** @var ShowMoreCommentsResult $show_more_result */
    $show_more_result = $show_more_request->run(
      $this->proposal_nid,
      $this->max_id,
      $this->count
    );

    $cids = $show_more_result->getCids();
    $comment_items = $featured_comments_service->getData($cids);

    // Drupal render array for Featured comments items.
    $featured_comments_items = $featured_comments_service->getItemsRenderArray(
      $comment_type,
      $comment_items,
      $show_more_result->isMoreComments(),
      $show_more_result->getMaxId(),
      $show_more_result->getShowMoreButtonId()
    );

    $output = render($featured_comments_items);

    $show_more_button_selector = '#' . $show_more_request->getShowMoreButtonId();
    $commands[] = ajax_command_remove($show_more_button_selector);
    $commands[] = ajax_command_append($show_more_result->getCommentsColumnSelector(), $output);
    ajax_deliver(array(
      '#type' => 'ajax',
      '#commands' => $commands
    ));
  }

  /**
   * Set properties from query string.
   */
  private function setPropertiesFromArguments()
  {
    if (!empty($_GET['max_id'])) {
      $this->max_id = (int) $_GET['max_id'];
    }
    if (!empty($_GET['count'])) {
      $this->count = (int) $_GET['count'];
    }
    if (!empty($_GET['proposal_nid'])) {
      $this->proposal_nid = (int) $_GET['proposal_nid'];
    }
  }
}