(function($) 
 {
     $.getJSON('/AmarylGetUser',
               function(data) {
                   $('#tableuser').DataTable( {
                       data: data,
                       "bFilter" : false, 
                       //"bJQueryUI" : true, 
                       "sPaginationType" : "full_numbers", 
                       "bPaginate": false,
                       "bInfo": false,
                       columns: [ 
                           { title: "uid" },
                           { title: "name" },
                           { title: "mail" },
                           { title: "login" },
                           { title: "status" },
                       ]
                   });
               });
 }(jQuery));
