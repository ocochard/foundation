<?php

//header( 'content-type: text/html; charset=utf-8' );
//
// Version 0.2.0
//
require '../vendor/autoload.php';
require './methods.php';

use GeoIp2\Database\Reader;

try
{
   //$reader = new Reader('../data/GeoLite2-Country.mmdb');

   $bdd = new PDO('mysql:host=dbserver;dbname=content_delivery_weather;charset=utf8', 'spike', 'valentine');

   //$tmp = array("name" => $_GET["probe"]);
   //$tables = newgettables($bdd, $reader, $tmp);

   /*if (count($tables) > 0) {
      $probe = newgetprobe($bdd, $reader, $tables, $_POST);
      if (count($probe) > 0) {
         newgeturl($bdd, $reader, $tables, $probe);
      }
   }*/
   date_default_timezone_set('UTC');

   $timestamp = $_GET['time'];
   $namefqdn = $_GET['url'];

   $year   = date('Y',$timestamp);
   $month  = date('m',$timestamp);



   $probe = strtolower ( $_GET['probe'] );
   $probe = str_replace("-", "_", $probe);
   $probe = str_replace(".", "_", $probe);   

   $probeTemplate     = "new_v4_sonde";
   $fqdnuserTemplate  = "new_v4_fqdnuser";
   $timeTemplate      = "new_v4_times";

   $query = "SHOW TABLES LIKE '%" . $probe . "%'";

   //print_r($query);
   $response = $bdd->query($query);

   $result= array();

   while($timeNow = $response->fetch()) {
      $date = explode("_", $timeNow[0]);
      $timeNow  = $timeNow[0];

      //print_r($date[3] . "=>" . $year );
      //print_r($date[4] . "=>" . $month );

      if ($date[3] == $year and ($date[4]+1) == $month) {
         //print_r($date[3] . " " . $date[4]);
         $query = "select " . $timeNow . ".har"
            . " from " . $timeNow . " inner join new_v4_url"
            . " where " . $timeNow . ".urlid = new_v4_url.urlid"
            . " and new_v4_url.urlname = '" . $namefqdn . "'"
            . " and " . $timeNow . ".starttime = " . $timestamp 
            . " and har != '' limit 1";


         $interesponse = $bdd->query($query);
         $interesponse->setFetchMode(PDO::FETCH_OBJ);
         while ($donnees = $interesponse->fetch()) {
            $rep = substr($donnees->har, 1, -1);
            $tmp = array( "data"   => $rep);

            array_push($result, $tmp);
         }
      }


      //print_r($query);

   }

   $response->closeCursor();
   $interesponse->closeCursor();

   //echo json_encode($result);
   //print_r($result);


   header('Content-Type: text/html; charset=UTF-8');

   mb_internal_encoding('UTF-8');
   mb_http_output('UTF-8');
   mb_http_input('UTF-8');
   mb_regex_encoding('UTF-8'); 

   $output = $result[0]['data'];
   $output = 'onInputData(' . $output . ')';
   $tmp = str_replace('null', '""', $tmp);
   $file = fopen("file.har","w") or die("Unable to open file!!");
   fwrite($file, $output);
   fclose($file);
   
   header('Location: http://www.softwareishard.com/har/viewer?inputUrl=https://p-trolette.orange-labs.fr/api/file.har');
   
   //print $output;
}
catch (Exception $e)
{
   die('Erreur : ' . $e->getMessage());
}

?>
