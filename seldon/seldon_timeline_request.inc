<?php

function _timeline_request_callback() {
   if(isset($_SESSION['probe']) and isset($_SESSION['url']) and isset($_SESSION['id']) ) {
      //print_r ($_SESSION['url']);
      $data = array();
      $url = array();
      $data['id'] = $_SESSION['id'];
      foreach ($_SESSION['url'] as $url_key => $url_value){
         $tmpurl = array();
         $tmpurl['content'] = $url_value; $tmpurl['start'] = 0; $tmpurl['end'] = 0;
         foreach ($_SESSION['probe'] as $probe_key => $probe_value){
            db_set_active('cdpweather');
            $result = db_query("SELECT table_name FROM information_schema.tables where table_name like 'new_v4_times_%"  . $probe_value . "%'");
            db_set_active();

            foreach($result as $row){
               $table = $row->table_name;
               db_set_active('cdpweather');
               $resultmax = db_query("select time.time from `" . $table . "` as time inner join new_v4_url as url on time.urlid = url.urlid where url.urlname = :name order by time desc limit 1", array(':name' => $url_value));
               db_set_active();

               foreach($resultmax as $itemmax) {
                  if ($tmpurl['end'] < $itemmax->time)
                     $tmpurl['end'] = $itemmax->time;
               }

               db_set_active('cdpweather');
               $resultmin = db_query("select time.time from `" . $table . "` as time inner join new_v4_url as url on time.urlid = url.urlid where url.urlname = :name order by time asc limit 1", array(':name' => $url_value));
               db_set_active();

               foreach($resultmin as $itemmin) {
                  if ($tmpurl['start'] > $itemmin->time or $tmpurl['min'] == 0)
                     $tmpurl['start'] = $itemmin->time;
               }
            }
         }
         $tmpurl['start'] = date('Y-m-d', $tmpurl['start']);
         $tmpurl['end'] = date('Y-m-d', $tmpurl['end']);
         $url[] = $tmpurl;
      }
      $data['data'] = $url;
   }

   /*$data = array();
   $data["name"]  = "olivier";
   $data["date"]  = time();
   $data["admin"] = true;
   echo json_encode( $data );*/
   /*$data = array();
   $data[] = array(id => 1, content => 'item 1', start => '2013-04-20');
   $data[] = array(id => 2, content => 'item 2', start => '2013-04-14');
   $data[] = array(id => 3, content => 'item 3', start => '2013-04-18');
   $data[] = array(id => 4, content => 'item 4', start => '2013-04-18', end => '2013-04-19');
   $data[] = array(id => 5, content => 'item 5', start => '2013-04-25');
   $data[] = array(id => 6, content => 'item 6', start => '2013-04-27');
    */
   //print_r($data);
   echo json_encode( $data );
}
