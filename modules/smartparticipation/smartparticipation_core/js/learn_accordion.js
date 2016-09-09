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
      learnVideo = '.learn-video',
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
    //this.learnVideoIds = learnVideoIds;
    this.desktopSettings = desktopSettings;
    this.activeBreakpoint = activeBreakpoint;
    this.lastActiveBreakpoint = null;
    this.videoCreated = false; // jwplayer applied?
    this.mobileVideoCreated = false; // jwplayer applied?

    var _self = this;

    // build
    this.build();

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
          // Adjust the video for the new desktop breakpoint size.
          this.resizeVideo();
        }

        // Create a desktop (horizontal) accordion using liteAccordion.
        this.createAccordion();

        // Have videos already been setup for the desktop accordion?
        if (!this.videoCreated) {
          // Set up JW Player videos for the first time.
          this.createVideo();
          this.videoCreated = true;
        }

        this.lastActiveBreakpoint = this.activeBreakpoint;

      }
    } else {

      // This is mobile device, so setup mobile videos if they haven't already.
      if (!this.mobileVideoCreated) {
        this.createMobileVideo();
        this.mobileVideoCreated = true;
      }

    }

  },

  resizeVideo: function () {

    var _self = this;

    this.$learnVideo.each(function (index) {

      var $div = _self.$(this),
        id = $div.attr('id');

      jwplayer(id).resize(
        _self.desktopSettings[_self.activeBreakpoint].video.width,
        _self.desktopSettings[_self.activeBreakpoint].video.height
      );

    });
  },

  createVideo: function () {

    var _self = this,
      learnVideoIds = this.$learnVideo.map(function () {
        return this.id;
      });

    this.$learnVideo.each(function (index) {

      _self.setupVideo(
        _self.$(this),
        _self.desktopSettings[_self.activeBreakpoint].video.width,
        _self.desktopSettings[_self.activeBreakpoint].video.height
      );

    });

    this.applyStopVideoOnNewSlide(this.$learnAccordion, 'h2', learnVideoIds);

  },

  createMobileVideo: function () {

    var _self = this,
      learnVideoIds = this.$learnVideoMobile.map(function () {
        return this.id;
      });

    this.$learnVideoMobile.each(function (index) {

      _self.setupVideo(
        _self.$(this),
        "100%",
        null,
        "16:9"
      );

    });

    this.applyStopVideoOnNewSlide(this.$learnAccordionMobile, 'a.accordion-toggle', learnVideoIds);

  },

  /**
   * Set up a single JW Player video.
   * @param $learnVideo
   * @param width
   * @param height
   * @param aspectratio
   */
  setupVideo: function ($learnVideo, width, height, aspectratio) {

    var id = $learnVideo.attr('id'),
      options = {
        file: $learnVideo.data('video'),
        image: $learnVideo.data('image'),
        primary: "flash"
      };

    // width is required
    options.width = width;

    if (height !== undefined && height) {
      options.height = height;
    }
    if (aspectratio !== undefined && aspectratio) {
      options.aspectratio = aspectratio;
    }

    jwplayer(id).setup(options).onPlay(function () {

      var nodeId = id.split('-').pop();

      SP.logEvent({
        'name': 'play video',
        'entity_type': 'node',
        'entity_id': nodeId
      });

    });

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

    // Stop a video playing on one slide when another slide is opened
    if (this.slideCount > 1) {
      // Would be nicer if this were triggered by an accordion slide close event,
      // but the liteAccordion doesn't provide such an event.
      $slides.on('click', function (event) {

        /*$slides.data('active', false);
         _self.$(this).data('active', true);
         */
        // NB We can no longer iterate over $learnVideo: when a jwplayer is
        // loaded onto the page, the div.learn-video element is replaced. The
        // container element for the player varies from browser to browser. Thus
        // the best way to get the player is via the id it was created with.
        _self.$.each(learnVideoIds, function (index, value) {
          var jwPlayer = jwplayer(value);//,
          //$container = _self.$(jwPlayer.container);
          //if (! $container.closest('#'+$accordion.attr('id')).find('h2').hasClass('selected')) {
          jwPlayer.stop();
          //}
        });
      });
    }

  }

});