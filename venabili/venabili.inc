<?php

function _Venabili_bandwidth_request() {
   $data = array();
   $output = array();
   //$_SESSION['probe'] = 'c5e029b83cd9';

   $map = array("null", "00", "01", "02", "03", "04", "05", "06", "07", "08", "09", "10", "11");

   if(isset($_SESSION['probe'])) {
      $probe = $_SESSION['probe'];
      $month = $map[date('n')];
      $day = date('Y') . "-" . $month;

      $datenow = new DateTime($date);
      //$datenow->add(new DateInterval('P1D'));
      $dmax = $datenow ->getTimestamp();

      $datenow->sub(new DateInterval('PT2H'));
      $dmin = $datenow ->getTimestamp();

      db_set_active('cdpweather');
      $result = db_query("SELECT table_name FROM information_schema.tables where table_name like 'new_v4_times_%"  . $probe . "' order by table_name desc limit 1");
      db_set_active();

      //print_r($result);

      foreach($result as $row){
         $table = $row->table_name;
         $tab = explode("_", $row->table_name);

         //print_r($tab);

         //print_r($tab);
         if ($tab[4] == $month) {
            $query = "select time, octet from `" . $table . "` where time >= " . $dmin . " and time <= " . $dmax . " order by time";   
            db_set_active('cdpweather');
            $timeresult = db_query($query);
            //echo $query;
            db_set_active();
            foreach($timeresult as $timerow){
               $data[$timerow->time] += $timerow->octet;
            }
         }
      }
      foreach($data as $key => $value){
         $output[] = array($key, $value);
      }
   }

   $datas = array();
   $datas['data'] = $output;
   $datas['container'] = $_GET['container'];

   echo json_encode($datas);
   //echo $month ;
}

function _Venabili_location_request() {
   global $user;
   $userId = $user->uid;

   $data = array();
   $output = array();

   db_set_active('cdpweather');
   $result = db_query("SELECT * from {new_v4_sonde} s inner join {new_v4_users_probes_edit} u on s.probeid = u.probeid where u.userid = :userid", array(':userid' => $userId));
   db_set_active();

   foreach($result as $row){
      $cc = $row->cc;
      if (array_key_exists($cc, $data)){
         $data[$cc] += 1;
      } else {
         $data[$cc] = 1;
      }
   }

   foreach($data as $key => $value){
      $output[] =  array("hc-key" => strtolower($key), "value" => $value);
   }

   echo json_encode($output);
}



