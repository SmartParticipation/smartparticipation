<?php

require_once "SmartParticipationSelenium.php";

class LogInSelenium extends SmartParticipationSelenium
{
  public function testLogIn()
  {
    $this->logInAsAdmin();
    $content = $this->byXPath('//*[@id="nice-menu-sp-user-menu"]/li/a')->text();
    $this->assertEquals('admin', $content);
    //fwrite(STDERR, print_r($content, TRUE));
  }
}