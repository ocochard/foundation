(function($, Drupal)
 {
    Drupal.behaviors.slidercmd = {
       attach: function(context, settings) {
          $('#slider').slider({
             range: "min",
             value: 37,
             min: 0,
             max: 500,
             slide: function( event, ui ) {
                $( "#level" ).val( "$" + ui.value );
             }
          });
          $( "#level" ).val( "$" + $( "#slider" ).slider( "value" ) );
       }
    };
 }(jQuery, Drupal));
