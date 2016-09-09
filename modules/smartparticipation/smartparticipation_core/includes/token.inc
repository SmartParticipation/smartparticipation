<?php 

/**
 * RegRoom custom token replacement.
 * 
 * TODO Generalize this into an overall strategy.
 */

/**
 * Token replacement of [comment:permalink] and [comment:parent:permalink].
 * 
 * @param $text - the text 
 * @param $comment - the comment
 * 
 * @return - text with tokens replaced
 */
function smartparticipation_core_token_replace_comment_permalink($text, $comment) {

  $comment_permalink = smartparticipation_core_get_comment_permalink_from_cid($comment->cid);
  $text = str_replace('[comment:permalink]', $comment_permalink, $text);
  
  if ($comment->pid) {
    $parent_permalink = smartparticipation_core_get_comment_permalink_from_cid($comment->pid);
    $text = str_replace('[comment:parent:permalink]', $parent_permalink, $text);
  }
  
  return $text;
  
}

/**
 * Custom replacement of comment:author:name token for moderators.
 *
 * @param $text - the text
 * @param $comment - the comment
 *
 * @return - text with tokens replaced
 */
function smartparticipation_core_token_replace_comment_author_moderator_name($text, $comment) {
  
  if (smartparticipation_core_is_moderator_comment($comment)) { 
    $text = str_replace('[comment:author:name]', t('the site moderator'), $text);
  } 

  return $text;
}

/**
 * Replace multiple several tokens using Drupal framework
 */
/*
 function _smartparticipation_core_replace_multiple_tokens($token_text, $comment) {

preg_match_all('/
    \[             # [ - pattern start
    ([^\s\[\]:]*)  # match $type not containing whitespace : [ or ]
    :              # : - separator
    ([^\s\[\]]*)   # match $name not containing whitespace [ or ]
    \]             # ] - pattern end
    /x', $token_text, $matches);

$final_text = $token_text;

for ($i = 0; $i < count($matches[0]); $i++) {

if ($matches[1][$i] == 'comment') {
$aux = $comment;
}

$replacements[$matches[0][$i]] = token_replace($matches[0][$i], array($matches[1][$i] => $aux));
}

foreach ($replacements as $index_replace => $replace_value) {
$final_text = str_replace($index_replace, $replace_value, $final_text);
}

return $final_text;
}
*/