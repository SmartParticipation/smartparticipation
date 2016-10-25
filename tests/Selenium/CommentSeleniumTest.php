<?php

require_once "SmartParticipationSelenium.php";

class CommentSelenium extends SmartParticipationSelenium
{
  public function testComment()
  {
    $this->logInAsAdmin();
    $this->url('/proposals/consumer-debt-collection-practices-anprm/discussion/telling-consumers-whats-happening');
    $subtopic_header = $this->byXPath('//*[@id="subtopic-title-1"]')->click();
    $this->waitUntil(function () {
      if ($this->byXPath('//*[@id="edit-comment-body"]')) {
        return TRUE;
      }
      return NULL;
    }, 2000);
    $this->execute(array(
      'script'=>"CKEDITOR.instances[\"edit-comment-body-und-0-value\"].setData('<p>Hello world</p>');jQuery('#edit-submit').mousedown();",
      'args'   => array()
    ));
    //$this->byXPath('//*[@id="edit-comment-body"]/html/body')->value('Hello world!');
    //$this->byXPath('//*[@id="comment-form"]')->submit();
    //$this->byXPath('//*[@id="edit-submit"]')->click();
    $this->waitUntil(function () { return NULL; }, 5000);
  }
}