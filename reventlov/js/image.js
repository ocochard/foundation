(function($)
   {
      Drupal.behaviors.reventlovplop = {
         attach: function (context, settings) {
            if (Drupal.settings.reventlovimg != undefined ) {
               var id = Drupal.settings.reventlovimg.hyperid;
               $.getJSON('/ReventlovGetImage/'+ id,
                  function(data) {
                     var selected = [];
                     var dt = $('#tableimage').DataTable( {
                        data: data,
                        "bFilter" : false,
                        //"bJQueryUI" : true,
                        "sPaginationType" : "full_numbers",
                        "bPaginate": false,
                        "bInfo": false,
                        columns: [
                           //{ title: "select" },
                           { title: "imageid" },
                           { title: "imagename" },
                           { title: "path" },
                           { title: "virtualtype" },
                           { title: "information" },
                           { title: "architecture" },
                           { title: "clone" },
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
                     $('#tableimage tbody').on('click', 'tr', function() {

                        console.log(dt.rows('.selected').data());
                        //$('#testid').text("lili");
                     });
                  });
            }}}
   }(jQuery));
