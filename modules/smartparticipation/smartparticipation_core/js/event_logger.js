/**
 * Script for SmartParticipation custom event logging.
 */


/*SP.jQuery(document).ready(function() {
  SP.EventLogger.init();
});*/

(function (Self, $) {
  
  Self.init = function() {
    
    // Attach listeners to events
    $('[data-rr-event]').each(function() {

      var el = $(this),
          event = el.data('rr-event');
      
      el.on(event, null, function() {
        Self.logElementData($(this));
      });
      
    });
    
  };
  
  Self.logElementData = function(el) {

    var element_data = el.data(),
        data = {};

    $.each(element_data, function(index, value) {
      /*
       * Convert jQuery data indices to keys corresponding to the event log
       * table field names. For example:
       * HTML: data-rr-event_entity-id =>
       * jQuery data() index: rrEvent_entityId =>
       * table field: entity_id
       * Throw away the index rrEvent
       */
      var key = index.replace(/^rrEvent_?/, '');
      if (key) {
        key = key.replace(/([A-Z])/g, '_$1');
        key = key.toLowerCase();
        data[key] = value;
      }
    });
    
    Self.log(data);

  };
  
  Self.log = function(data, async) {

    data['path'] = window.location.pathname;

    if ($('body').data('instanceStart')) {
      data['instance_start'] = $('body').data('instanceStart');
    }

    if (typeof async == 'undefined') {
      async = true;
    }
    
    $.ajax({
      type: 'POST',
      url: '/ajax/log_event',
      data: data,
      dataType: 'json',
      async: async
    }); 
    
  };
  
})(SP.EventLogger = {}, SP.jQuery);
