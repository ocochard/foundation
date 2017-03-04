(function($) 
   { 
      update();


      function update() {
         $.getJSON('/ReventlovGetHyper',
            function(data) {
               var selected = [];
               var timestamp = Math.round(new Date().getTime()/1000);
               var dt = $('#tablehyper').DataTable( {
                  data: data,
                  "bFilter" : false, 
                  //"bJQueryUI" : true, 
                  "sPaginationType" : "full_numbers", 
                  "bPaginate": false,
                  "bInfo": false,
                  columns: [
                     { title: "hyperid" },
                     { title: "hypername" },
                     { title: "cpu" },
                     { title: "architecture" },
                     { title: "typevirt" },
                     { title: "bridge" },
                     { title: "totalHD" },
                     { title: "useHD" },
                     { title: "totalRAM" },
                     { title: "useRAM" },
                     { title: "timestamp" },
                     { title: "edit/view" },
                  ],
                  "rowCallback": function( row, data ) {
                     if ( $.inArray(data.DT_RowId, selected) !== -1 ) {
                        $(row).addClass('selected');
                     }
                  },
                  "fnRowCallback": function( nRow, aData, iDisplayIndex, iDisplayIndexFull ){
                     $(nRow).css('color', '#1487d4');
                  }
               });
               $('#tablehyper tbody').on('click', 'tr', function() { 
                  var hyperid = dt.row( this ).data()[0];
                  $.getJSON('/cdpweather/GetLoadHyper/'+ hyperid, function(data){
                     hyper_graphic(data, hyperid);
                  }); 
               }); 
            });
      }


      function request(id) {
         $.getJSON('/cdpweather/GetLoadHyper/'+ id, function(data){
            return data;
         });
      }

      function hyper_graphic(data, id) {
         var cont = "#statushyperid";
         $(cont).highcharts({
            chart: {
               zoomType: 'xy',
               spacingRight: 20,
               //width: 800,
               //height:300,
               defaultSeriesType:'spline',
               backgroundColor:'transparent',
               events: {
                   load: function () { 
                      var series = request(id);
                      setInterval(function () {},1000);
                   } 
               }
            },
            xAxis:[{
               type: 'datetime',
               minRange: 5,
               title: {text: 'Date'},
            }],
            yAxis: [{
               labels: { format: '{value}bytes' },
               title: { text: 'RAM used' },
               //style: { color: Highcharts.Highcharts.getOptions().colors[1] }
            },{
               labels: { format: '{value}bytes' },
               title: { text: 'Harddisk used' },
               //style: { color: Highcharts.Highcharts.getOptions().colors[2] },
               opposite: true
            }],
            title: {
               text: 'Process',
               style: {
                  color: 'blue'
               }
            },
            exporting:{
               buttons:{
                  contextButton:{ enabled:false },
               }
            }, series: data
         });
      }

      function before_container_hdisk(data) {
         var result = "";
         container_hdisk(result);
      }

      function container_hdisk(data) {
         var cont = "#statuscontainerid";

         $(cont).highcharts({
            chart: {
               plotBackgroundColor: null,
               plotBorderWidth: null,
               plotShadow: false,
               type: 'pie'
            },
            title: {
               text: 'Harddisk used per container'
            },
            plotOptions: {
               pie: {
                  allowPointSelect: true,
                  cursor: 'pointer',
                  dataLabels: {
                     enabled: true,
                     format: '<b>{point.name}</b>: {point.percentage:.1f} %',
                     style: {
                        color: (Highcharts.theme && Highcharts.theme.contrastTextColor) || 'black'
                     }
                  }
               }
            },
            tooltip: {
               headerFormat: '<span style="font-size:11px">{series.name}</span><br>',
               pointFormat: '<span style="color:{point.color}">{point.name}</span>: <b>{point.y:.2f}%</b> of total<br/>'
            },
            exporting:{
               buttons:{
                  contextButton:{ enabled:false },
               }
            },
            series: data
         });
      }

   }(jQuery));
