/*(function ($) {
  Drupal.behaviors.form_testjs = {
attach: function(context, settings) {
var container = $("#objectid");
var items = new vis.DataSet([
{id: 1, content: 'item 1', start: '2013-04-20'},
{id: 2, content: 'item 2', start: '2013-04-14'},
{id: 3, content: 'item 3', start: '2013-04-18'},
{id: 4, content: 'item 4', start: '2013-04-16', end: '2013-04-19'},
{id: 5, content: 'item 5', start: '2013-04-25'},
{id: 6, content: 'item 6', start: '2013-04-27'}
]);

var options = {};

var timeline = new vis.Timeline(container, items, options);
}
};
})(jQuery);*/

//Drupal.ajax.prototype.commands.nameOfCommand =  function(ajax, response, status) {
//   var type = Drupal.settings.seldon.type;
//alert("Hello I'm Maged ..." + type)  ; // this will be executed after the ajax call
/*var container = document.getElementById('globalid');
  var items = new vis.DataSet([
  {id: 1, content: 'item 1', start: '2013-04-20'},
  {id: 2, content: 'item 2', start: '2013-04-14'},
  {id: 3, content: 'item 3', start: '2013-04-18'},
  {id: 4, content: 'item 4', start: '2013-04-16', end: '2013-04-19'},
  {id: 5, content: 'item 5', start: '2013-04-25'},
  {id: 6, content: 'item 6', start: '2013-04-27'}
  ]);

  var options = {};

  var timeline = new vis.Timeline(container, items, options);*/

//}
//

(function($, Drupal)
 {
    Drupal.ajax.prototype.commands.searchingcmd=  function(ajax, response, status) {
       //var container = document.getElementById('globalid');
       //var container = $('#globalid')[0];
       //var listurl   = Drupal.settings.seldon.url;
       //var listprobe = Drupal.settings.seldon.probe;

       //console.log(listurl);
       //console.log(listprobe);
       $.get('SeldonSearchingRequest') 
       .done(myFunction);
       //$.get('SeldonRequest', 
       //      { listurl: listurl, listprobe:listprobe }) 
       //      .done(myFunction);
       /*var xmlhttp = new XMLHttpRequest();
         var url = "seldon_request";
         xmlhttp.onreadystatechange=function() {
         if (xmlhttp.readyState == 4 && xmlhttp.status == 200) {
         myFunction(xmlhttp.responseText);
         }
         }
         xmlhttp.open("GET", url, true);
         xmlhttp.send();*/
//$.get('request','false', 'myFunction', 'text');
/*var items = new vis.DataSet([
  {id: 1, content: 'item 1', start: '2013-04-20'},
  {id: 2, content: 'item 2', start: '2013-04-14'},
  {id: 3, content: 'item 3', start: '2013-04-18'},
  {id: 4, content: 'item 4', start: '2013-04-16', end: '2013-04-19'},
  {id: 5, content: 'item 5', start: '2013-04-25'},
  {id: 6, content: 'item 6', start: '2013-04-27'}
  ]);

  var options = {};

  var timeline = new vis.Timeline(container, items, options);*/
//func01(container);
    };

       function myFunction(data) {
          var container = $('#timelineid')[0];

          //console.log(data);

          var arr = JSON.parse(data);

          //console.log(arr);

          var items = new vis.DataSet(arr);

          var options = {};

          var timeline = new vis.Timeline(container, items, options);

       } 
    } (jQuery, Drupal));


