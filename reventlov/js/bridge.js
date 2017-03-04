/*(function($)
  {
  $('#bridge').highcharts({
backgroundColor: 'white',
events: {
}
},
title: {
text: 'Highcharts export server overview',
style: {
color: 'black'
}
}
}(jQuery));*/


(function ($, Drupal) {
    //console.log("lolo");
    Drupal.behaviors.graphiccmd = {
        attach: function(context, settings) {
            if (Drupal.settings.reventlov.hypervisor != undefined ) {
                var hyper = Drupal.settings.reventlov.hypervisor;
                var url = "/ListContainers/" + hyper;
                //console.log(url);
                $.getJSON(url, 
                          function(data) {
                              //console.log(JSON.stringify(data));
                              //'',
                              //$row->containerid,
                              //$row->containername,
                              //$rowhyper->hypername,
                              //$rowimage->imagename,
                              //$row->networkid,
                              //$rowimage->architecture,
                              //$row->dns,
                              //$row->extip,
                              //$row->intip,
                              //$row->status,

                              //for (i=0; i< data.length; i++){ 
                              //    console.log(data[i]);   
                              //}

                              bridge_graphic(data);
                              multidns_graphic(data);
                              $("#bridgegraphic").show();
                              $("#multidnsgraphic").hide();

                              $(":input[name=network]").bind("change", function(event, ui) {
                                  //console.log($(this).val());
                                  if ($(this).val() == "multidns") {
                                      $("#multidnsgraphic").show();
                                      $("#bridgegraphic").hide();
                                  } else {
                                      $("#bridgegraphic").show();
                                      $("#multidnsgraphic").hide();
                                  }
                              });
                          }
                         );
            }
        }
    };

    function bridge_graphic(data){
        var cont = "#bridgegraphic";

        var line = 500;
        $(cont).highcharts({
            chart: {
                backgroundColor: 'white',
                events: {
                    load: function () {
                        var ren = this.renderer;
                        var colors = Highcharts.getOptions().colors;
                        var rightArrow = ['M', 0, 0, 'L', 100, 0, 'L', 95, 5, 'M', 100, 0, 'L', 95, -5];
                        var leftArrow = ['M', 100, 0, 'L', 0, 0, 'L', 5, 5, 'M', 0, 0, 'L', 5, -5];

                        var margin = 100;
                        var height = 100;
                        var length = 500;
                        //Subnet Draw
                        ren.path(['M', margin, height, 'L', length, height])
                        .attr({
                            'stroke-width': 2,
                            stroke: 'silver',
                        })
                        .add();

                        //Label
                        ren.label('subnet 192.168.0.0/24', 200, 80)
                        .css({
                            fontWeight: 'bold'
                        })
                        .add();

                        var size = data.length;
                        var step = length / (size*2);

                        for (i=0; i< size; i++){
                              console.log( margin + step*(1+i) );
                            //Container 50
                            ren.label(data[i][2], margin + step * ( 1 + i ) - 50, height + 100)
                            .attr({
                                fill: colors[0],
                                stroke: 'white',
                                'stroke-width': 2,
                                padding: 5,
                                r: 5
                            })
                            .css({
                                color: 'white'
                            })
                            .add()
                            .shadow(true);
                            // link from subnet to container
                            ren.path(['M', margin + step*(1+i), 100, 'L', margin + step*(1+i), 200])
                            .attr({
                                'stroke-width': 2,
                                stroke: 'silver',
                            })
                            .add();
                        }

                    }
                }
            },
            title: {
                text: 'Network Bridge Overview',
                style: {
                    color: 'blue'
                }
            }    
        });
    }

    function multidns_graphic(data){
        var cont = "#multidnsgraphic";

        $(cont).highcharts({
            chart: {
            },
            title: {
                text: 'Network Multidns Overview',
                style: {
                    color: 'blue'
                }
            }    
        });
    }
}(jQuery, Drupal));
