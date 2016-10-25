<?php


class SmartParticipationSelenium extends PHPUnit_Extensions_Selenium2TestCase
{
  /**
   * Function which performs the setup tasks
   */
  public function setUp() {
    $this->setHost('127.0.0.1');
    $this->setPort(4444);
    $this->setBrowserUrl('http://smartparticipation.local');
    $this->setBrowser('chrome');
  }

  /**
   * Function which shutdowns our session.
   */
  public function tearDown() {
    $this->stop();
  }

  protected function logInAsAdmin() {
    $this->url('/');
    $login_link = $this->byXPath('//*[@id="nice-menu-sp-user-menu"]/li[2]/a')->click();
    $this->waitUntil(function () {
      if ($this->byXPath('//*[@id="user-login"]')) {
        return TRUE;
      }
      return NULL;
    }, 5000);
    // Enter URL Creds.
    $this->byXPath('//*[@id="edit-name"]')->value('admin');
    $this->byXPath('//*[@id="edit-pass"]')->value('admin');
    // Hit Submit.
    $this->byXPath('//*[@id="user-login"]')->submit();
  }

}