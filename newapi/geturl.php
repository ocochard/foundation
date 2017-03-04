<?php

require '../vendor/autoload.php';
require './methods.php';

use GeoIp2\Database\Reader;

try
{
   $reader = new Reader('../data/GeoLite2-Country.mmdb');

   $bdd = new PDO('mysql:host=dbserver;dbname=content_delivery_weather;charset=utf8', 'spike', 'valentine');

   $rest_json = file_get_contents("php://input");
   $_POST = json_decode($rest_json, true);

   //print_r($_POST);

   $tables = gettables($bdd, $reader, $_POST);

   if (count($tables) > 0) {
      $probe = getprobe($bdd, $reader, $tables, $_POST);
      if (count($probe) > 0) {
         geturl($bdd, $reader, $tables, $probe);
      }
   }
}

catch (Exception $e)
{
   die('Erreur : ' . $e->getMessage());
}

?>
