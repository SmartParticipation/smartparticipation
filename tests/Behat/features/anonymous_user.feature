@api
Feature: Anonymous user
  Check that I am an anonymous user

Scenario: I am an anonymous user
  Given I am an anonymous user
  Then I should see the text "Log in"
