(function($, Drupal)
 {
    Drupal.behaviors.bandwidthcmd = {
       attach: function(context, settings) {
          if (Drupal.settings.bandwidthcmd != undefined ) {
             var information = Drupal.settings.bandwidthcmd.container;
             //console.log(containers);
             $('#bandwidth', context).once('bandwidthcmd', function () {
                
                   //console.log(" -> " + information.probe);
                   $.getJSON('/Bandwidth', 
                    { container: information.container, 
                      probe: information.probe}, 
                                function(data) {
                                   bandwidth(data);
                                }
                            );
             })

          }
       }
    };

    function bandwidth(data) {
      
      var cont = "#" + data.container;
      //console.log(data.data);

      $(cont).highcharts({
        chart: {
            type: 'spline'
        },
        title: {
            text: 'Activity 5H earlier'
        },
        subtitle: {
            text: 'Show the probe activity with data sent to the master ship'
        },
        xAxis: {
            type: 'datetime',
            dateTimeLabelFormats: { // don't display the dummy year
                month: '%e. %b',
                year: '%b'
            },
            title: {
                text: 'Date'
            }
        },
        yAxis: {
            title: {
                text: 'Octets'
            },
            min: 0
        },
        tooltip: {
            headerFormat: '<b>{series.name}</b><br>',
            pointFormat: '{point.x:%e. %b}: {point.y} octets'
        },

        plotOptions: {
            spline: {
                marker: {
                    enabled: true
                }
            }
        },
        series: [{
          name: 'Probe activity',
          data: data.data
        }]
      });
    }
} (jQuery, Drupal));
