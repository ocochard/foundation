(function($)
 {
     $.getJSON('/VenabiliGetProbe', 
               function(data) {
                   //console.log(data);
                   var matable = $('#tableprobe').DataTable({
                       data: data,
                       "bFilter" : false, 
                       //"bJQueryUI" : true, 
                       "sPaginationType" : "full_numbers", 
                       "bPaginate": false,
                       "bInfo": false,
                       columns: [
                           { title: "Name" },
                           { title: "Ip" },
                           { title: "Ext" },  
                           { title: "Dns" },
                           { title: "New Dns" },
                           { title: "Uptime" },
                           { title: "Firsttime" },
                           { title: "Lasttime" },
                           { title: "Mac" },
                           { title: "CC" },
                           { title: "Version" },
                           { title: "Status" },
                       ],
                       "fnRowCallback": function( nRow, aData, iDisplayIndex, iDisplayIndexFull ){
                           switch(aData[11]){
                               case 'plugged':
                                   $(nRow).css('color', '#009933');
                                   break;
                               case 'disconnected':
                                   $(nRow).css('color', '#FF0066');
                                   break;
                               case 'posted':
                                   $(nRow).css('color', '#3366FF');
                                   break;
                               case 'received':
                                   $(nRow).css('color', '#CC3399');
                                   break;
                               case 'decommissioned':
                                   $(nRow).css('color', '#666699');
                                   break;
                           }
                       },
                   });
               });
 }(jQuery));
