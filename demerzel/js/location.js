(function($)
   {
   $.getJSON('/DemerzelGetHyper',
      function(data) {
         console.log(JSON.stringify(data)); });
   }(jQuery))
