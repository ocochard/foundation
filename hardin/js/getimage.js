(function($)
   {
      $.getJSON('/HardinGetImages',
         function(data) {
            var selected = [];

            var dt = $('#tableimage').DataTable( {
               data: data,
               "bFilter" : false,
               "sPaginationType" : "full_numbers",
               "bPaginate": false,
               "bInfo": false,
               columns: [
                  { title: "imageid" },
                  { title: "imagename" },
                  { title: "path" },
                  { title: "vtype" },
                  { title: "information" },
                  { title: "architecture" },
                  { title: "command" },
                  { title: "status" },
               ],
               "rowCallback": function( row, data ) {
                  if ( $.inArray(data.DT_RowId, selected) !== -1 ) {
                     $(row).addClass('selected');
                  }
               },
               "fnRowCallback": function( nRow, aData, iDisplayIndex, iDisplayIndexFull ){
                  switch(aData[6]){
                     case '0':
                        $(nRow).css('color', '#009933');
                        break;
                     case '1':
                        $(nRow).css('color', '#0033CC');

                        break;
                     case '2':
                        $(nRow).css('color', '#FF0000');
                        break;
                  }
               }
            });

            $('#tableimage tbody').on('click', 'tr', function() {
               console.log(dt.rows('.selected').data());
            });
         });
   }(jQuery));
