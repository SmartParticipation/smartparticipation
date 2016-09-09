<?php

namespace SmartParticipation\model;


class CommentTipsState extends Model
{
  public $useCommentTipsPrompt = FALSE;

  public $useCommentTipsButton = FALSE;

  public function __construct($use_prompt, $use_button)
  {
    $this->useCommentTipsPrompt = $use_prompt;
    $this->useCommentTipsButton = $use_button;
  }
}