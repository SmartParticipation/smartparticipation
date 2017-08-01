/**
 * Script for homepage.
 */
 
/*SP.jQuery(document).ready(function ($) {

  if ($('#learn-accordion').length) {
    var learnAccordion = new SP.LearnAccordion('#learn-accordion', '#learn-accordion-mobile');
  }

});*/

SP.LearnAccordion = Object.subClass({

  init: function (accordionSelector, mobileAccordionSelector) {

    var $ = SP.jQuery,
      $learnAccordion = $(accordionSelector),
      $slides = $learnAccordion.find('li.learn-item'),
      slideCount = $slides.length,
      learnVideo = '.learn-video-wrapper video',
      $learnVideo = $learnAccordion.find(learnVideo),
      $learnAccordionMobile = $(mobileAccordionSelector),
      $learnVideoMobile = $learnAccordionMobile.find(learnVideo),
    //learnVideoIds = $learnVideo.map(function () {
    //  return this.id;
    //}),
      desktopSettings = {
        'mdDevice': {
          'accordion': {'width': 940, 'height': 297},
          'video': {'width': 528, 'height': 297}
        },
        'lgDevice': {
          'accordion': {'width': 1170, 'height': 369},
          'video': {'width': 657, 'height': 370}
        }
      },
      activeBreakpoint = SP.Breakpoint.getActiveBreakpoint();

    // Assign public properties
    this.$ = $;
    this.$learnAccordion = $learnAccordion;
    this.slideCount = slideCount;
    this.$learnVideo = $learnVideo;
    this.$learnAccordionMobile = $learnAccordionMobile;
    this.$learnVideoMobile = $learnVideoMobile;
    this.desktopSettings = desktopSettings;
    this.activeBreakpoint = activeBreakpoint;
    this.lastActiveBreakpoint = null;

    var _self = this;

    var learnVideoIds = this.$learnVideo.map(function () {
      return this.id;
    });

    var learnVideoIdsMobile = this.$learnVideoMobile.map(function () {
       return this.id;
    });

    // build
    this.build();

    // Pause video when changing slides
    // Desktop
    this.applyStopVideoOnNewSlide(this.$learnAccordion, 'h2', learnVideoIds);
    // Mobile
    this.applyStopVideoOnNewSlide(this.$learnAccordionMobile, 'a.accordion-toggle', learnVideoIdsMobile);

    // on resize, build again
    $(window).resize(function () {
      _self.refresh();
    });

  },

  refresh: function () {
    this.activeBreakpoint = SP.Breakpoint.getActiveBreakpoint();
    this.build();
  },

  build: function () {

    // Is this a desktop device?
    if (this.desktopSettings.hasOwnProperty(this.activeBreakpoint)) {
      // Are we at a new desktop breakpoint?
      if (this.lastActiveBreakpoint != this.activeBreakpoint) {

        // Was accordion already built for a desktop?
        if (this.lastActiveBreakpoint != null) {
          // Destroy the existing accordion.
          this.$learnAccordion.liteAccordion("destroy");
        }

        // Create a desktop (horizontal) accordion using liteAccordion.
        this.createAccordion();

        this.lastActiveBreakpoint = this.activeBreakpoint;

      }
    }

  },

  createAccordion: function () {

    this.$learnAccordion.liteAccordion({
      containerHeight: this.desktopSettings[this.activeBreakpoint].accordion.height,
      containerWidth: this.desktopSettings[this.activeBreakpoint].accordion.width,
      firstSlide: this.slideCount
    }).show();

  },

  applyStopVideoOnNewSlide: function ($accordion, slideSelector, learnVideoIds) {
    var _self = this,
      $slides = $accordion.find(slideSelector);
    // Pause a video playing on one slide when another slide is opened
    if (this.slideCount > 1) {
      // Would be nicer if this were triggered by an accordion slide close event,
      // but the liteAccordion doesn't provide such an event.
      $slides.on('click', function (event) {
        _self.$.each(learnVideoIds, function (index, videoId) {
          var video = document.getElementById(videoId);
          video.pause();
        });
      });
    }

  }

});