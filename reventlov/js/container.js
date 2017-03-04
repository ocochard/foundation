(function($)
   {
      Drupal.behaviors.reventlovbis = {
         attach: function (context, settings) {
            // console.log('container');
            if (Drupal.settings.reventlovctn != undefined ) {
               var id = Drupal.settings.reventlovctn.hyperid;
               $.getJSON('/ListContainers/' + id,
                  function(data) {
                     var rows_selected = [];
                     var dictionary = {};
                     // console.log("probleme");
                     var table = $('#tablecontainer').DataTable( {
                        data: data,
                        "bFilter" : false, 
                        //"bJQueryUI" : true, 
                        "sPaginationType" : "full_numbers", 
                        "bPaginate": false,
                        "bInfo": false,
                        columns: [
                           //{ title: "containerid" },
                           { title: "select" },
                           { title: "containerid" },
                           { title: "containername" },
                           { title: "hyper" },
                           { title: "image" },
                           //{ title: "networkid" },
                           { title: "arch" },
                           { title: "bridge" },
                           { title: "dns" },
                           { title: "extip" },
                           { title: "intip" },
                           { title: "crontab" },
                           { title: "status" },
                           //{ title: "button" },
                        ],
                        'columnDefs': [{
                           'targets': 0,
                           'searchable':false,
                           'orderable':false,
                           'width':'1%',
                           'className': 'dt-body-center',
                           'render': function (data, type, full, meta){
                              return '<input type="checkbox">';
                           }
                        }],
                        'order': [[1, 'asc']],
                        'rowCallback': function(row, dat, dataIndex){
                           // Get row ID
                           var rowId = dat[0];

                           // If row ID is in the list of selected row IDs
                           if($.inArray(rowId, rows_selected) !== -1){
                              $(row).find('input[type="checkbox"]').prop('checked', true);
                              $(row).addClass('selected');
                           }
                        },
                        "fnRowCallback": function( nRow, aData, iDisplayIndex, iDisplayIndexFull ){
                           switch(aData[11]){
                              case '0':
                                 $(nRow).css('color', '#009933');
                                 break;
                              case '1':
                                 $(nRow).css('color', '#1487d4');
                                 break;
                              case '2':
                                 $(nRow).css('color', '#FF0000');
                                 break;
                           }
                        }
                     });
                     $('#tablecontainer tbody').on('click', 'tr', function() {
                        var containerid = table.row( this ).data()[1];
                        console.log(containerid);
                        $.getJSON('/cdpweather/GetLoadContainer/'+ containerid, function(data){
                           container_graphic_memory(data);
                           container_graphic_network(data);
                        });

                     });
                     /*                     $('#tablecontainer').on('click', 'tbody td', function(e) { */
                     // e.preventDefault();  
                     // var cell = table.cell(this);
                     // console.log(cell.index());
                     // console.log(cell.data());
                     // console.log("lolo");
                     /* }); */
                     $('#tablecontainer').on( 'click', 'input[type="checkbox"]', function () {
                        var $row = $(this).closest('tr');
                        var madata = table.row($row).data();

                        if (dictionary.hasOwnProperty(madata[1])) {
                           delete dictionary[madata[1]];
                        } else {
                           dictionary[madata[1]] = madata[1] + "#" + madata[8];
                        }

                        var result = '';
                        for (var key in dictionary) {
                           result += dictionary[key] + ";";
                        }
                        console.log(JSON.stringify(dictionary));
                        $("#hiddenid input").attr('value',result);
                        console.log(result);
                     } );

                     $('#btn1').on( 'click', function () {
                        // var data = table.row( $(this).parents('tr') ).data();
                        // alert( data );
                        var $row = $(this).closest('tr');
                        var madata = table.row($row).data();
                        alert(this.id);
                     } );

                  });
            }
         }
      }

      function container_graphic_memory(data) {
         var cont = "#containermemoryid";
         var result = new Array();
         // console.log(JSON.stringify(data.cputime));
         result.push(data.cputime);
         result.push(data.memory);
         $(cont).highcharts({
            chart: {
               zoomType: 'xy',
               spacingRight: 20,
               //width: 800,
                             //height:300,
               defaultSeriesType:'spline',
               backgroundColor:'transparent',
            },
            xAxis:[{
               type: 'datetime',
               minRange: 5,
               title: {text: 'Date'},
            }],
            yAxis: [{
               labels: { format: '{value}Mbytes' },
               title: { text: 'RAM used' },
               //style: { color: Highcharts.getOptions().colors[1] }
            },{
               labels: { format: '{value}Mbytes' },
               title: { text: 'Harddisk used' },
               //style: { color: Highcharts.getOptions().colors[2] },
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
            },series: result

         }); 
      }

      function container_graphic_network(data) {
         var cont = "#containernetworkid";
         var result = new Array();
         // console.log(JSON.stringify(data.rx));
         result.push(data.tx);
         result.push(data.rx);
         $(cont).highcharts({
            chart: {
               zoomType: 'xy',
               spacingRight: 20,
               //width: 800,
               //               //height:300,
               defaultSeriesType:'spline',
               backgroundColor:'transparent',
            },
            xAxis:[{
               type: 'datetime',
               minRange: 5,
               title: {text: 'Date'},
            }],
            yAxis: [{
               labels: { format: '{value}Mbytes' },
               title: { text: 'Data sent' },
               //style: { color: Highcharts.getOptions().colors[1] }
            },{
               labels: { format: '{value}Mbytes' },
               title: { text: 'Data received' },
               //style: { color: Highcharts.getOptions().colors[2] },
               opposite: true
            }],
            title: {
               text: 'Tx/Rx on the container',
               style: {
                  color: 'blue'
               }
            },
            exporting:{
               buttons:{
                  contextButton:{ enabled:false },
               }
            },
            series: result
         }); 
      }

   }(jQuery));
