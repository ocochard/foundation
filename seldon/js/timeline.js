
(function($, Drupal)
 {
    Drupal.ajax.prototype.commands.timelinecmd =  function(ajax, response, status) {
       //var container = document.getElementById('globalid');
       //var container = $('#globalid')[0];
       //var listurl   = Drupal.settings.seldon.url;
       //var listprobe = Drupal.settings.seldon.probe;
       //console.log(listurl);
       //console.log(listprobe);
       $.get('SeldonTimelineRequest') 
       .done(myFunction);
    };

    function myFunction(data) {
       //var container = $('#newtimeline')[0];

       //console.log($('#newtimeline'));

       var arr = JSON.parse(data);

       console.log(arr['data']);
       var container = $('#timeview')[0];
       
       //var container = $('#newtimeline')[0];

       //console.log(container);

       var items = new vis.DataSet(arr['data']);

       var options = {};

       var timeline = new vis.Timeline(container, items, options);
      
    } 
 } (jQuery, Drupal));


