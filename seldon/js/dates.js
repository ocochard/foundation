(function ($) {
   Drupal.behaviors.seldon = {
      attach: function (context, settings) {
         if (Drupal.settings.date_content) {
            var content = Drupal.settings.date_content;

            var min = Drupal.settings.date_content.data.min;
            var max = Drupal.settings.date_content.data.max;

            var minDate = new Date(min * 1000);
            var maxDate = new Date(max * 1000);

            $('#dateid').datetimepicker({
               //minDate: minDate,
               //maxDate: maxDate,
               //onSelect: function (selectedDateTime){
                  //minDate = $(this).datetimepicker('getDate');
                  //$('.enddateid').datetimepicker('option', 'minDate', minDate );
                  //$('#edit-end-date').datetimepicker('option', 'minDate', minDate );
                  //$('.enddateid').datetimepicker('option', 'maxDate', maxDate );
                  //$('#edit-end-date').datetimepicker('option', 'maxDate', maxDate );
               //}
            });
         }
      }
   }
})(jQuery);
