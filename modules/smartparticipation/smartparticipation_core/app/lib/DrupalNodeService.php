<?php

namespace SmartParticipation;


class DrupalNodeService {
  /**
   * Get a node by its url alias.
   *  http://drupal.stackexchange.com/a/15247
   *
   * @param $alias
   * @return \stdClass a Drupal node
   */
  public function findByAlias($alias)
  {
    $path = drupal_lookup_path("source", $alias);
    $node = menu_get_object("node", 1, $path);
    return $node;
  }
}