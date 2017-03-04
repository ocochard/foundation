(function($)
 {
    $.getJSON('/VenabiliLocationRequest', 
    //$.getJSON('/VenabiliLocationRequest', 
              function(data) {
                 $('#location').highcharts('Map', {
                    title : {
                       text : 'The probes location'
                    },
                    subtitle : {
                       text : 'The map locates all probes around the world'
                    },
                    mapNavigation: {
                       enabled: true,
                       buttonOptions: {
                          verticalAlign: 'bottom'
                       }
                    },
                    colorAxis: {
                       min: 0,
                    },
                    series : [{
                       data : data ,
                       mapData: Highcharts.maps['custom/world-highres'],
                       joinBy: 'hc-key',
                       name: 'Probes quantity',
                       states: {
                          hover: {
                             color: '#BADA55'
                          }
                       },
                       dataLabels: {
                          enabled: false,
                          format: '{point.name}'
                       }
                    }]
                 });
              }
             );
 }(jQuery));
