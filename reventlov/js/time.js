(function ($, Drupal) {
    Drupal.behaviors.reventlovtime = {
        attach: function (context, settings) {

            $('#edit-cronperiod').timepicker({
                hourMin: 0,
                hourMax: 0,
                showHour: false,
                stepMinute: 10,
            }); 

            $('#edit-cronminute').timepicker({
                hourMin: 0,
                hourMax: 0,
                showHour: false,
                stepMinute: 10,
            }); 

            var startTimeTextBox    = $('#edit-cronstart');
            var endTimeTextBox      = $('#edit-cronend');


            $.timepicker.timeRange(
                    startTimeTextBox,
                    endTimeTextBox,
                    {
                        minInterval: (1000*60*60), // 1hr
                        timeFormat: 'HH:mm',
                        stepHour: 1,
                        showMinute: false,
                        start: {}, // start picker options
                        end: {} // end picker options
                    });

        }
    }
}(jQuery, Drupal));
