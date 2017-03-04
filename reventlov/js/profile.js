(function($)
   {
      $.getJSON('/ReventlovGetProfile',
         function(data) {
            var selected = [];

            var dt = $('#tableprofile').DataTable( {
               data: data,
               "bFilter" : false,
               //"bJQueryUI" : true,
               "sPaginationType" : "full_numbers",
               "bPaginate": false,
               "bInfo": false,
               columns: [
                  //{ title: "select" },
                  { title: "profileid" },
                  { title: "profilename" },
                  { title: "information" },
                  { title: "content" },
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
            $('#tableprofile tbody').on('click', 'tr', function() {

               console.log(dt.rows('.selected').data());
               //$('#testid').text("lili");
            });
         });
   }(jQuery));
