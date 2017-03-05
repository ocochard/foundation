<?php

function _getListProbes( $date ) {
   global $_SESSION;
   //2016-05-09
   //2016-03-19
   $tabdate = explode("-", $date); 
   $year    = $tabdate[0];
   $month   = $tabdate[1];
   $day     = $tabdate[2];

   //print_r($date);

   unset($_SESSION);

   $listprobes = getprobes();

   $map = array("null", "00", "01", "02", "03", "04", "05", "06", "07", "08", "09", "10", "11");

   $_SESSION['date'] 		= $date;
   $_SESSION['listprobes'] 	= array();
   $_SESSION['probes'] 		= array();
   //$_SESSION['tables'] = array();

   foreach ($listprobes as $probe) {
      db_set_active('cdpweather');
      //new_v4_times_2016_03_amiens01
      $query = "SELECT table_name FROM information_schema.tables where table_name like 'new_v4_times_" . $year . "_" . $map[intval($month)] . "_" . $probe . "'";
      $result = db_query($query);
      db_set_active();

      foreach($result as $row){
         $urls = _getListUrlsByProbe( $date, $probe, $row->table_name);
         if (count($urls) > 0) {
            $_SESSION['listprobes'][] = $probe;
            $_SESSION['probes'][$probe] = array(
               'date'   => $date,
               'table'  => $row->table_name,
               'urls'   => array(),
            );
            $_SESSION['probes'][$probe]['urls'] = $urls;
         }
      }
   }
   //print_r($_SESSION['date']);
}

function _getListUrlsByProbe( $date, $probe, $table) {
   $format = 'Y-m-d H:i:s';
   $mindate = DateTime::createFromFormat($format, $date . " 00:00:00");
   //$mindate = new DateTime($date);
   $mindatetimestamp = $mindate->getTimestamp();

   //print_r("timestamp [" . $mindatetimestamp . "]");
   $maxdate = DateTime::createFromFormat($format, $date . " 00:00:00");
   //$maxdate = new DateTime($date);
   $maxdate->add(new DateInterval('P1D'));
   $maxdatetimestamp = $maxdate->getTimestamp();

   // print_r("timestamp [" . $maxdatetimestamp . "]");

   db_set_active('cdpweather');
   $query = "SELECT DISTINCT u.urlname from {new_v4_url} u inner join {" . $table . "} p on u.urlid = p.urlid WHERE p.time > " . $mindatetimestamp . " AND p.time < " . $maxdatetimestamp;
   $result = db_query($query);  
   //print_r($query);
   db_set_active();

   $tmp_url = array();

   foreach($result as $item) {
      $tmp_url[] = $item->urlname;
   }

   return $tmp_url;
}

function _getCommonUrls($listprobes, $listkeys) {
   $list_url = array();

   db_set_active('cdpweather');
   $result = db_query("SELECT u.urlname FROM {new_v4_url} u");
   db_set_active();

   foreach($result as $item) {
      $list_url[$item->urlname] = $item->urlname;
   }


   $someprobes = array();
   foreach($listkeys as $key) {
      $someprobes[] = $listprobes[$key];
   }

   foreach ($someprobes as $probe) {
      $tmp_url = array();
      foreach($_SESSION['probes'][$probe]['urls'] as $item) {
         if (array_key_exists($item,$list_url)) {
            $tmp_url[$item] = $item;
         }
      }

      $list_url = $tmp_url;
   }
   //print_r($list_url);
   return drupal_map_assoc($list_url);
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//

function _getListUrls( $date, $probes) {
   global $_SESSION;

   $_SESSION['urls'] = array();

   $mindate = new DateTime($date);
   $mindatetimestamp = $mindate->getTimestamp();

   $maxdate = new DateTime($date);
   $maxdate->add(new DateInterval('P1D'));
   $maxdatetimestamp = $maxdate->getTimestamp();

   $list_url = array();

   db_set_active('cdpweather');
   $result = db_query("SELECT u.urlname FROM {new_v4_url} u");
   db_set_active();

   foreach($result as $item) {
      $list_url[$item->urlname] = $item->urlname;
   }


   foreach ($probes as $probe) {
      db_set_active('cdpweather');
      $query = "SELECT DISTINCT u.urlname from {new_v4_url} u inner join {" . $_SESSION['tables'][$probe] . "} p on u.urlid = p.urlid WHERE p.time > " . $mindatetimestamp . " AND p.time < " . $maxdatetimestamp;
      $result = db_query($query);  
      db_set_active();

      $tmp_url = array();
      foreach($result as $item) {
         if (array_key_exists($item->urlname,$list_url)) {
            $tmp_url[$item->urlname] = $item->urlname;
         }
      }

      $list_url = $tmp_url;
   } 

   foreach($list_url as $row){
      $_SESSION['urls'][] = $row; 
   }

   return drupal_map_assoc($_SESSION['urls']);
}


/*function oldgetListProbes( $date ) {
   //2016-05-09
   $tabdate = explode("-", $date); 
   $year    = $tabdate[0];
   $month   = $tabdate[1];
   $day     = $tabdate[2];

   $listprobes = getprobes();

   $map = array("null", "00", "01", "02", "03", "04", "05", "06", "07", "08", "09", "10", "11");

   $_SESSION['date'] = $date;    
   $_SESSION['probes'] = array();
   $_SESSION['tables'] = array();

   foreach ($listprobes as $probe) {
      db_set_active('cdpweather');
      //new_v4_times_2016_03_amiens01
      $query = "SELECT table_name FROM information_schema.tables where table_name like 'new_v4_times_" . $year . "_" . $map[intval($month)] . "_" . $probe . "'";
      //print_r($query);
      $result = db_query($query);
      db_set_active();

      foreach($result as $row){
         $_SESSION['probes'][] = $probe;
         $_SESSION['tables'][$probe] = $row->table_name;
      }   
   }
   return drupal_map_assoc($_SESSION['probes']);
}*/

function getprobes() {
   global $user;
   $userId = $user->uid;

   $tmp_probe = array();

   db_set_active('cdpweather');

   $result = db_query("SELECT s.probename FROM {new_v4_sonde} s inner join {new_v4_users_probes} u on s.probeid = u.probeid where u.userid = :userid", array(':userid' => $userId));
   //$query = db_select('new_v4_sonde', 's');
   //$query->fields('s', array('probename;'));
   //$result = $query->execute();

   db_set_active();

   foreach ($result as $record) {
      $tmp_probe[] = $record->probename;
   }

   return drupal_map_assoc($tmp_probe);
}

function geturls($probes) {
   $list_url = array();
   db_set_active('cdpweather');
   $result = db_query("SELECT u.urlname FROM {new_v4_url} u");
   db_set_active();

   foreach($result as $item) {
      $list_url[$item->urlname] = $item->urlname;
   }

   foreach($probes as $key => $value) {
      db_set_active('cdpweather');
      $result = db_query("SELECT u.urlname FROM {new_v4_url} u join {new_v4_fqdnuser} f on u.urlid = f.urlid join {new_v4_sonde} s on f.probeid = s.probeid WHERE f.status = 1 AND s.probename = :nom order by u.urlname", array(':nom' => $value));
      db_set_active();
      $tmp_url = array();
      foreach($result as $item) {
         if (array_key_exists($item->urlname,$list_url)) {
            $tmp_url[$item->urlname] = $item->urlname;
         }
      }

      $list_url = $tmp_url;
   }

   return $list_url;
}

/*function getintervals($probes) {
   date_default_timezone_set('UTC');

   $year['min'] = NULL; $year['max'] = NULL;
   $datemin = NULL; $datemax = NULL;

   $map = array("null", "00", "01", "02", "03", "04", "05", "06", "07", "08", "09", "10", "11");

   foreach($_SESSION['probes'] as $probe) {
      db_set_active('cdpweather');
      $query = "SELECT table_name FROM information_schema.tables where table_name like 'new_v4_times_%"  . $probe . "' order by table_name asc limit 1";
      //dsm($query);
      $result = db_query($query);
      db_set_active();

      foreach($result as $row){
         //drupal_set_message('probe: ' . print_r($row));
         $timeNow = $row->table_name;
         $tabtimeNow = explode("_", $timeNow);
         if($year['min'] < $tabtimeNow[3] || $year['min'] == NULL) {
            $year['min']   = $tabtimeNow[3];
            $month['min']  = $tabtimeNow[4];
            $prob['min']  = $probe;
         } else if($year['min'] == $tabtimeNow[3] and $month['min'] < $tabtimeNow[4]) {
            $month['min']  = $tabtimeNow[4];
            $prob['min']  = $probe;
         }
      }

      db_set_active('cdpweather');
      $query = "SELECT table_name FROM information_schema.tables where table_name like 'new_v4_times_%"  . $probe . "' order by table_name desc limit 1";
      $result = db_query($query);
      db_set_active();

      foreach($result as $row){
         //drupal_set_message('probe: ' . print_r($row));
         $timeNow = $row->table_name;
         $tabtimeNow = explode("_", $timeNow);
         if($year['max'] > $tabtimeNow[3] || $year['max'] == NULL) {
            $year['max']   = $tabtimeNow[3];
            $month['max']  = $tabtimeNow[4];
            $prob['max']  = $probe;
         } else if($year['max'] == $tabtimeNow[3] and $month['max'] > $tabtimeNow[4]) {
            $month['max']  = $tabtimeNow[4];
            $prob['max']  = $probe;
         }
      }
   }


   if (isset($prob['min']) and isset($prob['max']) and isset($year['min']) and isset($year['max']) and isset($month['min']) and isset($month['max']) ) {
      $table['min'] = 'new_v4_times_' . $year['min'] . '_' . $month['min'] . '_' . $prob['min'];
      $table['max'] = 'new_v4_times_' . $year['max'] . '_' . $month['max'] . '_' . $prob['max'];

      db_set_active('cdpweather');
      $timestamp['min'] = db_query("SELECT time FROM {`" . $table['min']. "`} ORDER BY time ASC LIMIT 1");
      $timestamp['max'] = db_query("SELECT time FROM {`" . $table['max']. "`} ORDER BY time DESC LIMIT 1");
      db_set_active();

      foreach($timestamp['min'] as $row){
         $datemin = date_create();
         date_timestamp_set($datemin,$row->time);
         //drupal_set_message("min " . print_r($row->time, TRUE), 'warning');
      }
      foreach($timestamp['max'] as $row){
         $datemax = date_create();
         date_timestamp_set($datemax,$row->time);
         //drupal_set_message("max " . print_r($row->time, TRUE), 'warning');
      }

      if ($datemin == NULL or $datemax == NULL) {
         return array('minDate'=> 0, 'maxDate' => 0);        
      } else {

         date_default_timezone_set("UTC");
         $time = time();
         $datenow=date_create();
         date_timestamp_set($datenow,$time);

         //drupal_set_message("time " . print_r($time, TRUE), 'warning');

         $aa = date_diff($datemin, $datenow);
         $bb = date_diff($datemax, $datenow);

         return array( 'minDate'=>'-'.$aa->format("%a"), 'maxDate' => '-'.$bb->format("%a") );
      }
   } else {
      return array('minDate'=> 0, 'maxDate' => 0);
   }

}*/
