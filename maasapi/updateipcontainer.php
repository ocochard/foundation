<?php

require './ctnmethods.php';

try
{
   $bdd = new PDO('mysql:host=192.168.2.12;dbname=hyperdb;charset=utf8', 'spike', 'valentine');
   //$result = $_POST["data"];
   
   update_ip_ctn($bdd, $_POST);

   //echo json_encode($_POST);
}
catch (Exception $e)
{
   die('Erreur : ' . $e->getMessage());
}
?>
