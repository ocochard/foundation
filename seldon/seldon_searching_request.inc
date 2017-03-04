<?php



function _get_color($code) {
   $result = '';

   if ( $code != 'XX') {

      $firstLetter = substr($code,0,1);
      $secondLeter = substr($code,-1);

      $base  = 20;
      $index = 5;
      $incre = 10;

      // Bleu vert
      $resA = $base;
      $resB = (ord($firstLetter) - ord('A')) * $index;
      $resC = (ord($secondLeter) - ord('A')) * $index;

      // Rose violet   
      //$resA = (ord($firstLetter) - ord('A')) * $index;
      //$resB = $base;
      //$resC = (ord($secondLeter) - ord('A')) * $index;

      // Moutarde vert   
      //$resA = (ord($firstLetter) - ord('A')) * $index;
      //$resB = (ord($secondLeter) - ord('A')) * $index;
      //$resC = $base;

      //$resA = (ord($firstLetter) - ord('A')) * $index;
      //$resB = (ord($secondLeter) - ord('A')) * $index;
      //$resC = (round((ord($firstLetter) + ord($secondLeter))/2) -  ord('A')) * $index;

   } else { 
      $resA = 100; $resB = 100; $resC = 100;
   }

   $hex = rgb2hex(array($resA, $resB, $resC));

   if (!array_key_exists($code, $_SESSION['color'])) {
      $_SESSION['color'][$code] = array($resA, $resB, $resC);
      return rgb2hex($_SESSION['color'][$code]);
   } else {
      $_SESSION['color'][$code][0] += $incre;
      if ($_SESSION['color'][$code][0] > 255) {$_SESSION['color'][$code][0] -= 255;}
      //$_SESSION['color'][$code][0] -= 5;
      //if ($_SESSION['color'][$code][0] < 0) {$_SESSION['color'][$code][0] = 0;}

      $_SESSION['color'][$code][1] += $incre;
      if ($_SESSION['color'][$code][1] > 255) {$_SESSION['color'][$code][1] -= 255;}
      //$_SESSION['color'][$code][1] -= 5;
      //if ($_SESSION['color'][$code][1] < 0) {$_SESSION['color'][$code][1] = 0;}

      $_SESSION['color'][$code][2] += $incre;
      if ($_SESSION['color'][$code][2] > 255) {$_SESSION['color'][$code][2] -= 255;}
      //$_SESSION['color'][$code][2] -= 5;
      //if ($_SESSION['color'][$code][2] < 0) {$_SESSION['color'][$code][2] = 0;}

      $result = rgb2hex($_SESSION['color'][$code]);
      return $result;
   }
}

//$rgb = array( 255, 255, 255 );
//$hex = rgb2hex($rgb);
//echo $hex;

function rgb2hex($rgb) {
   $hex = "#";
   $hex .= str_pad(dechex($rgb[0]), 2, "0", STR_PAD_LEFT);
   $hex .= str_pad(dechex($rgb[1]), 2, "0", STR_PAD_LEFT);
   $hex .= str_pad(dechex($rgb[2]), 2, "0", STR_PAD_LEFT);

   return $hex; // returns the hex value including the number sign (#)
}


function _request_callback($days) {
   $debug = "NO"; 
   if ($debug == "NO") {
      $probe      = $_GET['probe'];
      $date       = $_GET['date'];
      $container  = $_GET['container'];
      $url        = $_GET['url'];
      $level      = $_GET['level'];  
      $days       = $_GET['days'];
   } else {
      $probe      = 'romania01';
      $date       = '2017-02-10';
      $container  = 'romania01';
      $url        = 'www.google.fr';
      $level      = '0';
      $days       = '1';
   }

   /*$tabdate = explode("-", $date); 
   $year    = $tabdate[0];
   $month   = $tabdate[1];
   $day     = $tabdate[2];*/


   $_SESSION['color'] = array(
      //'FR'  => array(255,140,0),
   );

   // Define the minimum of information memorized
   //$level = 6000;
   //$level = $_SESSION['level']['value'];

   $output = array();
   $map = array("null", "00", "01", "02", "03", "04", "05", "06", "07", "08", "09", "10", "11");

   $tabdate = explode("-", $date);
   //$month = $map[intval($tabdate[1])];
   $month   = $tabdate[1];
   $year    = $tabdate[0];

   $INTERVAL="P" . $days . "D";

   // drupal_set_message($INTERVAL);

   $paris = new DateTimeZone('Europe/Paris');
   $datemax = new DateTime($date, $paris);
   // $datemax->add(new DateInterval('P1D'));
   $datemax->add(new DateInterval($INTERVAL));
   $dmax = $datemax ->getTimestamp();

   $datemin = new DateTime($date, $paris);
   $dmin = $datemin ->getTimestamp();

   db_set_active('cdpweather');
   $query = "SELECT table_name FROM information_schema.tables where table_name like 'new_v4_times_" . $year . "_" . $map[intval($month)] . "_" . $probe . "'";
   // $query = "SELECT table_name FROM information_schema.tables where table_name like 'new_v4_times_%" . $probe . "'";
   $result = db_query($query);
   // echo $query;
   db_set_active();

   // drupal_set_message('<pre>'.print_r($query,true).'</pre>');

   foreach($result as $row){
      $table = $row->table_name;
      $tab = explode("_", $row->table_name);
      //$tab = explode("-", $tab[3]);

      //if ($tab[3] == $year and $tab[4] == $month) {
      $query = "select"
         . " t.time as time,"
         . " t.contentaddress as address,"
         . " t.contenttype as contenttype,"
         . " t.firstbyte as firstbyte,"
         . " t.cc as times_cc,"
         . " t.country as country,"
         . " t.prefix as prefix,"
         . " t.asnum as asnum,"
         . " t.asname as asname,"
         . " t.starttime as starttime,"
         . " t.wait as wait,"
         . " t.receive as receive,"
         . " t.duration as duration,"
         . " t.octet as octet"
         . " from `" . $table . "` t"
         . " join `new_v4_url` u on"
         . " t.urlid = u.urlid"
         . " where u.urlname = '" . $url . "'"
         . " and t.time >= " . $dmin
         . " and t.time <= " . $dmax
         . " order by time, asnum";

      // echo $query;
      // drupal_set_message('<pre>'.print_r($query,true).'</pre>');

      db_set_active('cdpweather');
      $tmpresult = db_query($query);
      db_set_active();
      foreach($tmpresult as $tmprow){
         if ($tmprow->times_cc == '') { $tmprow->times_cc = 'XX'; }

         //if ($tmprow->octet > $level) {
         if (!$tmprow->prefix == '' && !$tmprow->asnum == '' && !$tmprow->asname == '') {
            $output[] = $tmprow;
         }
         //}
      }
      //} 
   }
   // echo '<pre>';
   // print_r($output);
   // echo  '</pre>';

   $country = array();
   foreach($output as $row) {
      if (array_key_exists($row->times_cc, $country)) {
         if (array_key_exists($row->asname, $country[$row->times_cc])) {
            if (!array_key_exists($row->prefix, $country[$row->times_cc][$row->asname])){
               $country[$row->times_cc][$row->asname][$row->prefix] = _get_color($row->times_cc);
            } 
         } else {
            $country[$row->times_cc][$row->asname] = array();
            $country[$row->times_cc][$row->asname][$row->prefix] = _get_color($row->times_cc);
         }
      } else {
         $country[$row->times_cc] = array();
         $country[$row->times_cc]['color'] = _get_color($row->times_cc);
         $country[$row->times_cc][$row->asname] = array();
         $country[$row->times_cc][$row->asname][$row->prefix] = _get_color($row->times_cc);
      }
   }

   foreach($output as $row) {
      $row->color       = $country[$row->times_cc][$row->asname][$row->prefix];
      $row->color_cc    = $country[$row->times_cc]['color'];
   }

 /*   echo '<pre>'; */
   // print_r($output);
   // echo  '</pre>';

   return $output;
}

function _global_object_type_per_as_callback(&$msg) {
   $level      = $_GET['level'];  
   //$level = $_SESSION['per']['value'];
   //$level = 0;

   // Colors mapping
   $_SESSION['color'] = array(
      // #6195a0
      "GREY"  => array(100,100,100),
   );


   $data = array();
   $data['sum'] = 0;

   foreach($msg as $row){
      $contenttype = explode(";", $row->contenttype);
      $contenttype = $contenttype[0];
      if (array_key_exists($contenttype, $data)){
         if (array_key_exists($row->asnum, $data[$contenttype])){
            if (array_key_exists($row->prefix, $data[$contenttype][$row->asnum])){
               $data['sum'] += intval($row->octet);
               $data[$contenttype]['sum'] += intval($row->octet);
               $data[$contenttype][$row->asnum]['sum'] += intval($row->octet);
               $data[$contenttype][$row->asnum][$row->prefix]['sum'] += intval($row->octet);
            } else {
               $data['sum'] += intval($row->octet);
               $data[$contenttype]['sum'] += intval($row->octet);
               $data[$contenttype][$row->asnum]['sum'] += intval($row->octet);
               $data[$contenttype][$row->asnum][$row->prefix] = array();
               $data[$contenttype][$row->asnum][$row->prefix]['sum'] += intval($row->octet);
            }
         } else {
            $data['sum'] += intval($row->octet);
            $data[$contenttype]['sum'] += intval($row->octet);
            $data[$contenttype][$row->asnum] = array();
            $data[$contenttype][$row->asnum]['sum'] = intval($row->octet);
            $data[$contenttype][$row->asnum]['color'] = $row->color;
            $data[$contenttype][$row->asnum][$row->prefix] = array();
            $data[$contenttype][$row->asnum][$row->prefix]['sum'] += intval($row->octet);
         }
      } else {
         $data['sum'] += intval($row->octet);
         $data[$contenttype] = array();
         $data[$contenttype]['sum'] += intval($row->octet);
         $data[$contenttype]['color'] = _get_color('GREY');
         $data[$contenttype][$row->asnum] = array();
         $data[$contenttype][$row->asnum]['sum'] = intval($row->octet);
         $data[$contenttype][$row->asnum]['color'] = $row->color;
         $data[$contenttype][$row->asnum][$row->prefix] = array();
         $data[$contenttype][$row->asnum][$row->prefix]['sum'] += intval($row->octet);
      }
   }

   $datafiltered = array();
   foreach($data as $keyA => $valueA) {
      switch($keyA) {
      case 'sum':
         $datafiltered['sum'] = $data['sum'];
         break;
      default:
         $datafiltered[$keyA] = array();
         foreach($data[$keyA] as $keyB => $valueB) {
            switch($keyB) {
            case 'sum':
               $datafiltered[$keyA]['sum'] = $valueB; 
               break;
            case 'color':
               $datafiltered[$keyA]['color'] = $valueB;
               break;
            default:
               $datafiltered[$keyA][$keyB] = array();
               foreach($data[$keyA][$keyB] as $keyC => $valueC) {
                  switch($keyC) {
                  case 'sum':
                     $datafiltered[$keyA][$keyB]['sum'] = $data[$keyA][$keyB]['sum'];
                     break;
                  case 'color':
                     $datafiltered[$keyA][$keyB]['color'] = $valueC;
                     break;
                  default:
                     $result = ($data[$keyA][$keyB][$keyC]['sum'] / $data['sum']) * 100;
                     if ($result < $level) {
                        $datafiltered['sum'] -= $data[$keyA][$keyB][$keyC]['sum'];
                        $datafiltered[$keyA]['sum'] -= $data[$keyA][$keyB][$keyC]['sum'];
                        $datafiltered[$keyA][$keyB]['sum'] -= $data[$keyA][$keyB][$keyC]['sum'];
                        if ($datafiltered[$keyA][$keyB]['sum'] == 0) {
                           unset($datafiltered[$keyA][$keyB]);
                        }
                        if ($datafiltered[$keyA]['sum'] == 0) {
                           unset($datafiltered[$keyA]);
                        }
                     }
                  }
               }
               //break;
            }
         }
      }
   }

   //print "<pre>";
   //print_r($data);
   //print_r($datafiltered);
   //print "</pre>";
   //return json_encode($data);
   return $datafiltered;
}

function _global_prefix_per_as_per_country_callback(&$msg) {
   $level      = $_GET['level'];  
   //$level = $_SESSION['per']['value'];
   //$level = 10;

   $data = array();
   $data['sum'] = 0;

   foreach($msg as $row){
      if (array_key_exists($row->times_cc, $data)){
         if (array_key_exists($row->asnum, $data[$row->times_cc])){
            if (array_key_exists($row->prefix, $data[$row->times_cc][$row->asnum])){
               $data['sum'] += intval($row->octet);
               $data[$row->times_cc]['sum'] += intval($row->octet);
               $data[$row->times_cc][$row->asnum]['sum'] += intval($row->octet);
               $data[$row->times_cc][$row->asnum][$row->prefix]['sum'] += intval($row->octet);
            } else {
               $data['sum'] += intval($row->octet);
               $data[$row->times_cc]['sum'] += intval($row->octet);
               $data[$row->times_cc][$row->asnum]['sum'] += intval($row->octet);
               $data[$row->times_cc][$row->asnum][$row->prefix] = array();
               $data[$row->times_cc][$row->asnum][$row->prefix]['sum'] = intval($row->octet);
               $data[$row->times_cc][$row->asnum][$row->prefix]['color'] = $row->color; 
            }
         } else {
            $data['sum'] += intval($row->octet);
            $data[$row->times_cc]['sum'] += intval($row->octet);
            $data[$row->times_cc][$row->asnum] = array();
            $data[$row->times_cc][$row->asnum]['sum'] = intval($row->octet);
            $data[$row->times_cc][$row->asnum][$row->prefix] = array();
            $data[$row->times_cc][$row->asnum][$row->prefix]['sum'] = intval($row->octet);
            $data[$row->times_cc][$row->asnum][$row->prefix]['color'] = $row->color;
         }
      } else {
         $data['sum'] += intval($row->octet);
         $data[$row->times_cc] = array();
         $data[$row->times_cc]['sum'] = intval($row->octet);
         $data[$row->times_cc]['color'] = $row->color_cc;
         $data[$row->times_cc][$row->asnum] = array();
         $data[$row->times_cc][$row->asnum]['sum'] = intval($row->octet);
         $data[$row->times_cc][$row->asnum][$row->prefix] = array();
         $data[$row->times_cc][$row->asnum][$row->prefix]['sum'] = intval($row->octet);
         $data[$row->times_cc][$row->asnum][$row->prefix]['color'] = $row->color;
      }
   }

   $datafiltered = array();
   foreach($data as $keyA => $valueA) {
      switch($keyA) {
      case 'sum':
         $datafiltered['sum'] = $data['sum'];
         break;
      default:
         $datafiltered[$keyA] = array();
         foreach($data[$keyA] as $keyB => $valueB) {
            switch($keyB) {
            case 'sum':
               $datafiltered[$keyA]['sum'] = $valueB;
               break;
            case 'color':
               $datafiltered[$keyA]['color'] = $valueB;
               break;
            default:
               $datafiltered[$keyA][$keyB] = array();
               foreach($data[$keyA][$keyB] as $keyC => $valueC) {
                  switch($keyC) {
                  case 'sum':
                     $datafiltered[$keyA][$keyB]['sum'] = $valueC;
                     break;
                  case 'color':
                     $datafiltered[$keyA][$keyB]['color'] = $valueC;
                     break;
                  default:
                     if (!array_key_exists('color', $datafiltered[$keyA][$keyB])){
                        $datafiltered[$keyA][$keyB]['color'] = $data[$keyA][$keyB][$keyC]['color']; 
                     }
                     $result = ($data[$keyA][$keyB][$keyC]['sum'] / $data['sum']) * 100;
                     //$tmp = $data[$keyA][$keyB][$keyC]['sum'];
                     if ($result < $level) {
                        $datafiltered['sum'] -= $data[$keyA][$keyB][$keyC]['sum'];
                        $datafiltered[$keyA]['sum'] -= $data[$keyA][$keyB][$keyC]['sum'];
                        $datafiltered[$keyA][$keyB]['sum'] -= $data[$keyA][$keyB][$keyC]['sum'];
                        if ($datafiltered[$keyA][$keyB]['sum'] == 0) {
                           unset($datafiltered[$keyA][$keyB]);
                        }
                        if ($datafiltered[$keyA]['sum'] == 0) {
                           unset($datafiltered[$keyA]);
                        }
                     }
                  }
               }
            }
         }
      }
   }

   //print "<pre>";
   //print_r($data);
   //print "--------------------------------------------------------------------------------------------<br>";
   //print_r($datafiltered);
   //print "</pre>";

   //return json_encode($data);
   return $datafiltered;
}


function _percentage_as_country_callback(&$msg) {
   $level      = $_GET['level'];  
   //$level = $_SESSION['per']['value'];

   $tmp  = array();
   $data = array();
   $time = array();
   foreach($msg as $row){
      ($row->times_cc == '')?$name = substr($row->asname,0,15) . " " . $row->asnum . " " . "XX":$name = substr($row->asname,0,15) . " " . $row->asnum . " " . $row->times_cc;
      if (array_key_exists($name, $tmp)){
         if (array_key_exists($row->time, $tmp[$name])){
            $tmp[$name][$row->time]['octet'] += intval($row->octet);
            $tmp[$name][$row->time]['asnum']    = $row->asnum;
            $tmp[$name][$row->time]['asname']   = $row->asname;
            $tmp[$name][$row->time]['color']    = $row->color;
         } else {
            $tmp[$name][$row->time]['octet']    = intval($row->octet);
            $tmp[$name][$row->time]['asnum']    = $row->asnum;
            $tmp[$name][$row->time]['asname']   = $row->asname;
            $tmp[$name][$row->time]['color']    = $row->color;
         }
      } else {
         $tmp[$name][$row->time]['octet']     = intval($row->octet);
         $tmp[$name][$row->time]['asnum']    = $row->asnum;
         $tmp[$name][$row->time]['asname']   = $row->asname;
         $tmp[$name][$row->time]['color']    = $row->color;
      }
      if (!array_key_exists($row->time, $time))
         $time[$row->time] = $row->time;
   }

   foreach($msg as $row){
      ($row->times_cc == '')?$name = substr($row->asname,0,15) . " " . $row->asnum . " " . "XX":$name = substr($row->asname,0,15) . " " . $row->asnum . " " . $row->times_cc;
      foreach($time as $_time_){
         if (!array_key_exists($_time_, $tmp[$name])){
            $tmp[$name][$_time_]['octet']    = intval(0);
            $tmp[$name][$_time_]['asnum']    = $row->asnum;
            $tmp[$name][$_time_]['asname']   = $row->asname;
            $tmp[$name][$_time_]['color']    = $row->color;
         }
      }
   }

   $globalsum = 0;
   foreach($tmp as $keycc => $valuecc){
      $d = array();
      $d['data'] = array();
      ($keycc=='') ? $d['name']  = 'XX' : $d['name']  = $keycc;
      $sum = 0;
      foreach($tmp[$keycc] as $keytime => $valuetime) {
         if (!array_key_exists('color', $d)) { $d['color']  = $tmp[$keycc][$keytime]['color']; }
         $sum += $tmp[$keycc][$keytime]['octet'];
         $e = array();
         $tmptime = 1000 * intval($keytime);
         $e['x'] = $tmptime;
         $e['y'] = $tmp[$keycc][$keytime]['octet'];
         $e['asnum']  = $tmp[$keycc][$keytime]['asnum'];
         $e['asname'] = $tmp[$keycc][$keytime]['asname'];
         $d['data'][] = $e;
      }
      $d['sum'] = $sum;
      $globalsum += $sum;

      $data[] = $d;
   }

   $datafiltered = array();
   foreach($data as $keyA => $valueA) {
      $result = ($data[$keyA]['sum'] / $globalsum) * 100;

      if ($result > $level) {
         $datafiltered[] = $data[$keyA];
      }
   }

   //print_r($data);
   //echo json_encode($data);
   $_data_['data'] = $datafiltered;
   return $_data_;
}

function _percentage_prefix_country_callback(&$msg) {
   $level      = $_GET['level'];  
   //$level = $_SESSION['per']['value'];

   $tmp  = array();
   $data = array();
   $time = array();
   foreach($msg as $row){
      ($row->times_cc == '') ? $name = $row->prefix . " " . "XX" : $name = $row->prefix . " " . $row->times_cc;
      if (array_key_exists($name, $tmp)){
         if (array_key_exists($row->time, $tmp[$name])){
            $tmp[$name][$row->time]['octet'] += intval($row->octet);
            $tmp[$name][$row->time]['asnum']    = $row->asnum;
            $tmp[$name][$row->time]['asname']   = $row->asname;
            $tmp[$name][$row->time]['color']    = $row->color;
         } else {
            $tmp[$name][$row->time]['octet']    = intval($row->octet);
            $tmp[$name][$row->time]['asnum']    = $row->asnum;
            $tmp[$name][$row->time]['asname']   = $row->asname;
            $tmp[$name][$row->time]['color']    = $row->color;
         }
      } else {
         $tmp[$name][$row->time]['octet']     = intval($row->octet);
         $tmp[$name][$row->time]['asnum']    = $row->asnum;
         $tmp[$name][$row->time]['asname']   = $row->asname;
         $tmp[$name][$row->time]['color']    = $row->color;
      }
      if (!array_key_exists($row->time, $time))
         $time[$row->time] = $row->time;
   }
   foreach($msg as $row){
      ($row->times_cc == '') ? $name = $row->prefix . " " . "XX" : $name = $row->prefix . " " . $row->times_cc;
      foreach($time as $_time_){
         if (!array_key_exists($_time_, $tmp[$name])){
            $tmp[$name][$_time_]['octet']    = intval(0);
            $tmp[$name][$_time_]['asnum']    = $row->asnum;
            $tmp[$name][$_time_]['asname']   = $row->asname;
            $tmp[$name][$_time_]['color']    = $row->color;
         }
      }
   }
   $globalsum = 0;
   foreach($tmp as $keycc => $valuecc){
      $d = array();
      $d['data'] = array();
      ($keycc=='') ? $d['name']  = 'XX' : $d['name']  = $keycc;
      $sum = 0;
      foreach($tmp[$keycc] as $keytime => $valuetime) {
         if (!array_key_exists('color', $d)) { $d['color']  = $tmp[$keycc][$keytime]['color']; }
         $sum += $tmp[$keycc][$keytime]['octet'];
         $e = array();
         $tmptime = 1000 * intval($keytime);
         $e['x'] = $tmptime;
         $e['y'] = $tmp[$keycc][$keytime]['octet'];
         $e['asnum']  = $tmp[$keycc][$keytime]['asnum'];
         $e['asname'] = $tmp[$keycc][$keytime]['asname'];
         $d['data'][] = $e;
      }
      $d['sum'] = $sum;
      $globalsum += $sum;

      $data[] = $d;
   }

   $datafiltered = array();
   foreach($data as $keyA => $valueA) {
      $result = ($data[$keyA]['sum'] / $globalsum) * 100;

      if ($result > $level) {
         $datafiltered[] = $data[$keyA];
      }
   }

   //print_r($data);
   $_data_['data'] = $datafiltered;
   return $_data_;
}

function _percentage_prefix_as_country_callback(&$msg) {
   $level      = $_GET['level'];  
   //$level = $_SESSION['per']['value'];

   $tmp  = array();
   $data = array();
   $time = array();
   foreach($msg as $row){
      ($row->times_cc == '') ? $name = $row->prefix . " " . $row->asnum . " " . "XX" : $name = $row->prefix . " " . $row->asnum . " " . $row->times_cc;
      if (array_key_exists($name, $tmp)){
         if (array_key_exists($row->time, $tmp[$name])){
            $tmp[$name][$row->time]['octet'] += intval($row->octet);
            $tmp[$name][$row->time]['asnum']    = $row->asnum;
            $tmp[$name][$row->time]['asname']   = $row->asname;
            $tmp[$name][$row->time]['color']    = $row->color;
         } else {
            $tmp[$name][$row->time]['octet']    = intval($row->octet);
            $tmp[$name][$row->time]['asnum']    = $row->asnum;
            $tmp[$name][$row->time]['asname']   = $row->asname;
            $tmp[$name][$row->time]['color']    = $row->color;
         }
      } else {
         $tmp[$name][$row->time]['octet']     = intval($row->octet);
         $tmp[$name][$row->time]['asnum']    = $row->asnum;
         $tmp[$name][$row->time]['asname']   = $row->asname;
         $tmp[$name][$row->time]['color']    = $row->color;
      }
      if (!array_key_exists($row->time, $time))
         $time[$row->time] = $row->time;
   }
   foreach($msg as $row){
      ($row->times_cc == '') ? $name = $row->prefix . " " . $row->asnum . " " . "XX" : $name = $row->prefix . " " . $row->asnum . " " . $row->times_cc;
      foreach($time as $_time_){
         if (!array_key_exists($_time_, $tmp[$name])){
            $tmp[$name][$_time_]['octet']    = intval(0);
            $tmp[$name][$_time_]['asnum']    = $row->asnum;
            $tmp[$name][$_time_]['asname']   = $row->asname;
            $tmp[$name][$_time_]['color']    = $row->color;
         }
      }
   }
   $globalsum = 0;
   foreach($tmp as $keycc => $valuecc){
      $d = array();
      $d['data'] = array();
      ($keycc=='') ? $d['name']  = 'XX' : $d['name']  = $keycc;
      $sum = 0;
      foreach($tmp[$keycc] as $keytime => $valuetime) {
         if (!array_key_exists('color', $d)) { $d['color']  = $tmp[$keycc][$keytime]['color']; }
         $sum += $tmp[$keycc][$keytime]['octet'];
         $e = array();
         $tmptime = 1000 * intval($keytime);
         $e['x'] = $tmptime;
         $e['y'] = $tmp[$keycc][$keytime]['octet'];
         $e['asnum']  = $tmp[$keycc][$keytime]['asnum'];
         $e['asname'] = $tmp[$keycc][$keytime]['asname'];
         $d['data'][] = $e;
      }
      $d['sum'] = $sum;
      $globalsum += $sum;

      $data[] = $d;
   }

   $datafiltered = array();
   foreach($data as $keyA => $valueA) {
      $result = ($data[$keyA]['sum'] / $globalsum) * 100;

      if ($result > $level) {
         $datafiltered[] = $data[$keyA];
      }
   }


   $_data_['data'] = $datafiltered;
   return $_data_;
}

function _cumulated_ip_address_country_callback(&$msg) {
   $tmp  = array();
   $data = array();
   $time = array();
   foreach($msg as $row){
      ($row->times_cc == '') ? $name = "Solaria" . " " . "XX" : $name = $row->country . " " . $row->times_cc;
      if (array_key_exists($name, $tmp)){
         if (!array_key_exists($row->address, $tmp[$name]['addresses'])){
            $tmp[$name]['addresses'][$row->address] = $row->address;
            $tmp[$name][$row->time] += 1;
         }
      } else {
         $tmp[$name]['addresses'][$row->address] = $row->address;
         $tmp[$name][$row->time] += 1;
         $tmp[$name]['color'] = $row->color;
      }
      if (!array_key_exists($row->time, $time))
         $time[$row->time] = $row->time;
   }
   foreach($msg as $row){
      ($row->times_cc == '') ? $name = "Solaria" . " " . "XX" : $name = $row->country . " " . $row->times_cc;
      foreach($time as $_time_){
         if (!array_key_exists($_time_, $tmp[$name])){
            $tmp[$name][$_time_] = 0;
         }
      }
   }
   foreach($tmp as $keycc => $valuecc){
      $listkeys = array_keys($tmp[$keycc]);
      sort($listkeys);
      $d = array();
      $d['data'] = array();
      ($keycc=='') ? $d['name']  = 'XX' : $d['name']  = $keycc;
      $d['color']  = $tmp[$keycc]['color'];
      $count = 0;
      foreach($listkeys as $key) {
         if ($key != 'addresses') {
            if ($key != 'color') {
               $count += $tmp[$keycc][$key];            
               $e = array();
               $tmptime = 1000 * intval($key);
               $e['x'] = $tmptime;
               $e['y'] = $count;
               $d['data'][] = $e;
            }
         }
      }
      $data[] = $d;
   }

   $_data_['data'] = $data;
   return $_data_;
}

function _global_time_callback(&$msg) {
   $time = array();
   $data = array();

   foreach($msg as $row){
      //echo '<pre>';
      //print_r($row->duration);
      //echo  '</pre>';

      if (array_key_exists($row->time, $time)) {
         //if ($time[$row->time]['sum'] < (intval($row->starttime) * 1000 - intval($row->time) * 1000 + intval($row->duration))) {
         if ($time[$row->time]['sum'] < (intval($row->starttime) * 1000 + intval($row->duration) - intval($row->time) * 1000)) {
            $time[$row->time]['sum'] = intval($row->starttime) * 1000 + intval($row->duration) - intval($row->time) * 1000;
         }
         if ($time[$row->time]['laststart'] < intval($row->starttime)) {
            //if ($time[$row->time]['sumstart'] < ( intval($row->starttime) * 1000 - intval($row->time) * 1000 )) {
            //   $time[$row->time]['sumstart'] = intval($row->starttime) * 1000 - intval($row->time) * 1000 - $time[$row->time]['sumstart'];
            //}
            $time[$row->time]['sumstart'] += ( intval($row->starttime) * 1000 - $time[$row->time]['laststart'] * 1000) ;
            $time[$row->time]['laststart'] = intval($row->starttime); 
         }
         $time[$row->time]['weight']      += intval($row->firstbyte); 
      } else {
         $time[$row->time] = array();
         $time[$row->time]['sum']         = intval($row->duration) * 1000;
         $time[$row->time]['weight']      = intval($row->firstbyte);
         $time[$row->time]['sumstart']    = 0;
         $time[$row->time]['laststart']   = intval($row->time);
      }
   }

   $serietime  = array(); 
   $serietime['name'] = 'Times to load all the page';
   $serietime['yAxis'] = 0;
   $serietime['data'] = array();

   $seriesum   = array(); 
   $seriesum['name'] = 'Accumated Times of Waiting';
   $seriesum['type'] = 'spline';
   $seriesum['dashStyle'] = 'shortdot';
   $seriesum['yAxis'] = 1;
   $seriesum['data'] = array();

   $serieweight   = array();
   $serieweight['name']   = 'Page Weight';
   $serieweight['type'] = 'spline';
   $serieweight['dashStyle'] = 'shortdot';
   $serieweight['yAxis'] = 1;
   $serieweight['data'] = array();



   foreach($time as $key => $value) {
      //echo '<pre>';
      //print_r($key);
      //print_r($value['sum']);
      //echo  '</pre>';
      //print_r($value['sum']);
      //print_r($value['sumstart']);
      $e = array();
      $e['x'] = 1000 * $key;
      $e['y'] = round($value['sum'] / 1000);
      $serietime['data'][] = $e; 

      $f = array();
      $f['x'] = 1000 * $key;
      $f['y'] = round($value['sumstart']/1000);
      $seriesum['data'][] = $f;

      $g = array();
      $g['x'] = 1000 * $key;
      $g['y'] = $value['weight'];
      $serieweight['data'][] = $g;
   }
   $data[] = $serietime;
   // $data[] = $seriesum;
   $data[] = $serieweight;

   //$_data_['data'] = $time;
   $_data_['data'] = $data;

   //echo '<pre>';
   //print_r($_data_);
   //echo  '</pre>';

   return $_data_;   
}

function _global_delay_callback(&$msg) {
   $time = array();
   $data = array();

   foreach($msg as $row){
      //echo '<pre>';
      //print_r($row->duration);
      //echo  '</pre>';

      if (array_key_exists($row->time, $time)) {
         //if ($time[$row->time]['sum'] < (intval($row->starttime) * 1000 - intval($row->time) * 1000 + intval($row->duration))) {
         if ($time[$row->time]['sum'] < (intval($row->starttime) * 1000 + intval($row->duration) - intval($row->time) * 1000)) {
            $time[$row->time]['sum'] = intval($row->starttime) * 1000 + intval($row->duration) - intval($row->time) * 1000;
         }
         if ($time[$row->time]['laststart'] < intval($row->starttime)) {
            //if ($time[$row->time]['sumstart'] < ( intval($row->starttime) * 1000 - intval($row->time) * 1000 )) {
            //   $time[$row->time]['sumstart'] = intval($row->starttime) * 1000 - intval($row->time) * 1000 - $time[$row->time]['sumstart'];
            //}
            $time[$row->time]['sumstart'] += ( intval($row->starttime) * 1000 - $time[$row->time]['laststart'] * 1000) ;
            $time[$row->time]['laststart'] = intval($row->starttime); 
         }
         $time[$row->time]['object']      += 1; 
      } else {
         $time[$row->time] = array();
         $time[$row->time]['sum']         = intval($row->duration) * 1000;
         $time[$row->time]['object']      = 1;
         $time[$row->time]['sumstart']    = 0;
         $time[$row->time]['laststart']   = intval($row->time);
      }
   }

   $serietime  = array(); 
   $serietime['name'] = 'Times to load all the page';
   $serietime['yAxis'] = 0;
   $serietime['data'] = array();

   $seriesum   = array(); 
   $seriesum['name'] = 'Accumated Times of Waiting';
   $seriesum['type'] = 'spline';
   $seriesum['dashStyle'] = 'shortdot';
   $seriesum['yAxis'] = 0;
   $seriesum['data'] = array();

   $serieobject   = array();
   $serieobject['name']   = 'Page Object Quantity';
   $serieobject['type'] = 'spline';
   $serieobject['dashStyle'] = 'shortdot';
   $serieobject['yAxis'] = 1;
   $serieobject['data'] = array();



   foreach($time as $key => $value) {
      //echo '<pre>';
      //print_r($key);
      //print_r($value['sum']);
      //echo  '</pre>';
      //print_r($value['sum']);
      //print_r($value['sumstart']);
      $e = array();
      $e['x'] = 1000 * $key;
      $e['y'] = round($value['sum'] / 1000);
      $serietime['data'][] = $e; 

      $f = array();
      $f['x'] = 1000 * $key;
      $f['y'] = round($value['sumstart']/1000);
      $seriesum['data'][] = $f;

      $g = array();
      $g['x'] = 1000 * $key;
      $g['y'] = $value['object'];
      $serieobject['data'][] = $g;
   }
   // $data[] = $serietime;
   $data[] = $seriesum;
   $data[] = $serieobject;

   //$_data_['data'] = $time;
   $_data_['data'] = $data;

   //echo '<pre>';
   //print_r($_data_);
   //echo  '</pre>';

   return $_data_;   
}

function _statistic_callback(&$msg) {
   $time = array();
   $data = array(); 
   $serieplot = array();
   $serieplot['name'] = 'normal';
   $serieplot['type'] = 'scatter';
   //$serieplot['yAxis'] = 1;
   $serieplot['data'] = array();
   $seriebar = array();
   $seriebar['name'] = 'normal';
   $seriebar['type'] = 'column';
   $seriebar['pointPadding'] = 0;
   $seriebar['groupPadding'] = 0;
   $seriebar['pointPlacement'] = 'between';
   //$seriebar['yAxis'] = 0;
   $seriebar['data'] = array();


   foreach($msg as $row){
      if (array_key_exists($row->time, $time)) {
         if ($time[$row->time]['sum'] < (intval($row->starttime) * 1000 + intval($row->duration) - intval($row->time) * 1000)) {
            $time[$row->time]['sum'] = intval($row->starttime) * 1000 + intval($row->duration) - intval($row->time) * 1000;
         }
      } else {
         $time[$row->time] = array();
         $time[$row->time]['sum']      = intval($row->duration) * 1000;
      }
   }

   $tmp = array();
   foreach($time as $row) {
      ++$tmp[$row['sum']];
   }

   ksort($tmp);
   $histo = array();

   foreach ($tmp as $key => $value) {
      $e = array();
      $e['x'] = $key;
      $e['y'] = $value;
      $serieplot['data'][] = $e;
      $x = floor($key/50000) * 50000;
      ++$histo[$x];
   }

   /*foreach($tmp as $key => $value) {
      $x = floor($key/50000) * 50000;
      ++$histo[$x];
   }*/
   ksort($histo);

   foreach ($histo as $key => $value) {
      $e = array();
      $e['x'] = $key;
      $e['y'] = $value;
      $seriebar['data'][] = $e;
   }

   //sort($serie);
   $data[] = $serieplot;
   $data[] = $seriebar;

   $_data_['data'] = $data;
   return $_data_;   
}


function _searching_request_callback() {
   $data = array();

   $probe      = $_GET['probe'];
   $date       = $_GET['date'];
   $container  = $_GET['container'];
   $url        = $_GET['url'];
   $level      = $_GET['level'];  
   $days       = $_GET['days'];  

   $data['information']['probe']       = $probe;
   $data['information']['date']        = $date;
   $data['information']['container']   = $container;
   $data['information']['url']         = $url;
   $data['information']['level']       = $level;
   $data['information']['days']        = $days;

   $msg = _request_callback(intval($days));

   $data['gotas']  = _global_object_type_per_as_callback($msg);
   $data['gpasc']  = _global_prefix_per_as_per_country_callback($msg);
   $data['pasc']   = _percentage_as_country_callback($msg);
   $data['pprc']   = _percentage_prefix_country_callback($msg);
   $data['ppasc']  = _percentage_prefix_as_country_callback($msg);   
   $data['cipc']   = _cumulated_ip_address_country_callback($msg);
   $data['gtc']    = _global_time_callback($msg);
   $data['gdc']    = _global_delay_callback($msg);
   // $data['stat']   = _statistic_callback($msg);


/*    echo '<pre>';   */
   // print_r($data);
   // echo  '</pre>';

   echo json_encode($data);
}
