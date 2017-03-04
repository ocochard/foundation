<?php

function gettables($bdd, $reader, $probe) {
   date_default_timezone_set('UTC');

   $map = array("null", "00", "01", "02", "03", "04", "05", "06", "07", "08", "09", "10", "11");
   $month = $map[date('n')];
   $day = date('Y') . "_" . $month;

   //$datenow = date('Y_m');

   //print_r($datenow."\n");

   $urlTemplate      = "new_v4_url";
   $probeTemplate    = "new_v4_sonde";
   $fqdnuserTemplate = "new_v4_fqdnuser";
   $timeTemplate     = "new_v4_times";

   $timeNow = $timeTemplate . "_" .  $day . "_" . $probe["name"];

   //print_r($timeNow."\n");

   $tableslist = array(
      "urlNow" => array(
         "table" 	=> $urlTemplate,
         "template"	=> $urlTemplate
      ),
      "probeNow" => array(
         "table" 	=> $probeTemplate,
         "template"	=> $probeTemplate
      ),
      "fqdnuserNow" => array(
         "table" 	=> $fqdnuserTemplate,
         "template"	=> $fqdnuserTemplate
      ),
      "timeNow" => array(
         "table" 	=> $timeNow,
         "template"	=> $timeTemplate
      ),
   );

   foreach($tableslist as $cle => $table) {
      $query = "SHOW TABLES LIKE '" . $table["table"] . "'";

      //print_r($query."\n");

      $reponse = $bdd->query($query);
      $rows = $reponse->fetchAll();

      if (sizeof($rows) == 0) {
         $query = "CREATE TABLE `" . $table["table"] . "` LIKE `" . $table["template"] . "`";
         //print_r($query."\n");
         $reponse = $bdd->query($query);
      }

      $reponse->closeCursor();
   }

   return  $tableslist;
}

function getprobe($bdd, $reader, $tableslist, $probe) {
   $date = new DateTime();
   $timestamp = $date->getTimestamp();

   if ($probe["extip"] != "0.0.0.0") {
      $record       = $reader->country($probe["extip"]);
      $isoCode      = $record->country->isoCode;
      $countryname  = $record->country->name;
   } else {
      $isoCode      = "XX";
      $countryname  = "XXXX";
   }

   $query = "SELECT * FROM " . $tableslist["probeNow"]["table"] . " WHERE " . $tableslist["probeNow"]["table"] . ".probename = '" . $probe["name"] . "'";
   $reponse = $bdd->query($query);
   $rows = $reponse->fetchAll();

   if (count($rows) == 0) {
      $status = 0;
      $query = "INSERT INTO `" . $tableslist["probeNow"]["table"] . "` " .
         "(`probename`, `ip`, `ext`, `dns`, `uptime`, `lasttime`, `mac`, `cc`, `country`, `version`, `status`) VALUES(" . 
         "\"" 	  . $probe["name"] . 
         "\",\""  . $probe["intip"] . 
         "\",\""  . $probe["extip"] . 
         "\",\""  . $probe["dns"] .
         "\","    . $probe["uptime"] .
         ","      . $timestamp .
         ",\"" 	  . $probe["macaddress"] .
         "\",\""  . $isoCode .
         "\",\""  . $countryname .
         "\",\""  . $probe["version"] . 
         "\"," 	  . $status . ")";
      $reponse = $bdd->query($query);
   } else {
      $query = "UPDATE `"        . $tableslist["probeNow"]["table"] . "`  SET " .
         "`ip`=\"" 	         . $probe["intip"] .
         "\",`ext`=\"" 	         . $probe["extip"] .
         "\",`dns`=\""           . $probe["dns"] .
         "\",`uptime`="          . $probe["uptime"] .
         ",`lasttime`="          . $timestamp .
         ",`country`=\""         . $countryname .
         "\",`cc`=\"" 	         . $isoCode .
         "\",`version`=\""       . $probe["version"] .
         "\" WHERE `probeid`="   . $rows[0]["probeid"];
      $reponse = $bdd->query($query);
   }

   $query = "SELECT * FROM " . $tableslist["probeNow"]["table"] . " WHERE " . $tableslist["probeNow"]["table"] . ".probename = '" . $probe["name"] . "'";
   $reponse = $bdd->query($query);
   $rows = $reponse->fetchAll();

   $reponse->closeCursor();
   return $rows[0];
}

function geturl($bdd, $reader, $tableslist, $probe) {
   $data = array();
   $output = array();
   $query = " SELECT u.urlname"
      . " FROM " . $tableslist["urlNow"]["table"] . " u"
      . " inner join " . $tableslist["fqdnuserNow"]["table"]   . " f on u.urlid = f.urlid"
      . " inner join " . $tableslist["probeNow"]["table"]      . " s on f.probeid = s.probeid"
      . " where s.probeid = " . $probe['probeid']; 
   //print_r($query);
   $reponse = $bdd->query($query);
   $rows = $reponse->fetchAll();
   
   foreach($rows as $row) {
      $data[] = $row['urlname'];
   }
   $reponse->closeCursor();

   $output['urls']   = $data;
   $output['probe']  = $probe;
   echo json_encode($output);
}

function getpageref($bdd, $reader, $tableslist, $array, $probe) {
   $pagename   = $array["log"]["pages"][0]["id"];
   $pagename = str_replace("http://", "", $pagename);

   $query = "SELECT " . $tableslist["urlNow"]["table"]  . ".urlid"
      . " FROM "  . $tableslist["fqdnuserNow"]["table"]  . "," .  $tableslist["urlNow"]["table"] 
      . " WHERE " . $tableslist["urlNow"]["table"]       . ".urlname LIKE '"  . $pagename  . "'"
      . " AND "   . $tableslist["fqdnuserNow"]["table"]  . ".probeid = "      . $probe['probeid'] 
      . " AND "   . $tableslist["fqdnuserNow"]["table"]  . ".urlid = "        . $tableslist["urlNow"]["table"]  . ".urlid";

   $reponse = $bdd->query($query);
   $rows = $reponse->fetchAll();
   $reponse->closeCursor();

   $rows[0] = array_merge($rows[0], array("startedDateTime" => $array["log"]["pages"][0]["startedDateTime"]));
   $rows[0] = array_merge($rows[0], array("onLoad" => $array["log"]["pages"][0]["pageTimings"]["onLoad"]));

   return $rows[0];
}

function gettimeshot($bdd, $reader, $tableslist, $entry, $probe, $pageref, $starteddatetime, $har) {
   $contentaddress   = $entry["serverIPAddress"];
   $contentname      = $entry["request"]["url"];
   $contenttype      = $entry["response"]["content"]["mimeType"];
   $firstbyte        = firstbyte($contentaddress);  

   $datefqdn = new DateTime($entry["startedDateTime"]);
   $unixstampfqdn = date_timestamp_get($datefqdn);   
   $datepage = new DateTime($starteddatetime);
   $unixstamppage = date_timestamp_get($datepage);  

   if ($contentaddress != "0.0.0.0") {
      $record       = $reader->country($contentaddress);
      $isoCode      = $record->country->isoCode;
      $countryname  = $record->country->name;
   } else {
      $isoCode      = "XX";
      $countryname  = "XXXX";
   }

   $har = $bdd->quote($har);

   $query = "INSERT INTO `" . $tableslist["timeNow"]["table"] . "` " .
      "(`time`, `probeid`, `urlid`, `contentname`, `contenttype`, `contentaddress`, `firstbyte`, `prefix`, `asnum`, `asname`, `cc`, `country`, `duration`, `octet`, `starttime`, `wait`, `receive`, `har`) VALUES(" . 
      ""       . $unixstamppage .
      ","      . $probe['probeid'] .
      ","      . $pageref['urlid'] .
      ",\""    . $contentname .
      "\",\""  . $contenttype .
      "\",\""  . $contentaddress .
      "\","    . $firstbyte .
      ",\"\",\"\",\"\"" .
      ",\""    . $isoCode .
      "\",\""  . $countryname .
      "\","    . $entry["time"] .
      ","      . $entry["response"]["bodySize"] .
      ","      . $unixstampfqdn .
      ","      . $entry["timings"]["wait"] .
      ","      . $entry["timings"]["receive"] .
      ",\""    . $har . "\")";

   $reponse = $bdd->query($query);
}

function firstbyte($IP) {
   $data = explode(".", $IP);
   return $data[0];
}

?>
