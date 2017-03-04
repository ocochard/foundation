<?php
require '../vendor/autoload.php';
require './methods.php';

use GeoIp2\Database\Reader;

try{
   $reader = new Reader('../data/GeoLite2-Country.mmdb');

   $bdd = new PDO('mysql:host=dbserver;dbname=content_delivery_weather;charset=utf8', 'spike', 'valentine');

   $rest_json = file_get_contents("php://input");
   $_POST = json_decode($rest_json, true);

   $probe = array(
      "name"         => $_POST["log"]["probe"]["name"],
      "intip"        => $_POST["log"]["probe"]["intip"],
      "extip"        => $_POST["log"]["probe"]["extip"],
      "dns"          => $_POST["log"]["probe"]["dns"],
      "uptime"       => $_POST["log"]["probe"]["uptime"],
      "macaddress"   => $_POST["log"]["probe"]["macaddress"],
      "version"      => $_POST["log"]["probe"]["version"]
   );

   //print_r($probe);

   $tables = gettables($bdd, $reader, $probe);

   $probe = getprobe($bdd, $reader, $tables, $probe);

   if (count($probe) > 0) {
      $pageref = getpageref($bdd, $reader, $tables, $_POST, $probe);

      if (sizeof($pageref) > 0) {
         for ($i=0;$i<sizeof($_POST["log"]["entries"]); $i++) {
            $har = '';
            if ($i == 0) {
               // $har = '';
               $har = $rest_json;
            }
            gettimeshot($bdd, $reader, $tables, $_POST["log"]["entries"][$i], $probe, $pageref, $_POST["log"]["pages"][0]["startedDateTime"], $har);
         }
      }
   }
}

catch (Exception $e) {
   die('Erreur : ' . $e->getMessage());
}
?>
