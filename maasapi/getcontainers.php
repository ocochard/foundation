<?php

require './ctnmethods.php';

try
{
   $bdd = new PDO('mysql:host=dbserver;dbname=hyperdb;charset=utf8', 'spike', 'valentine');
   //$result = $_POST["data"];
   
   getctn($bdd, $_POST);

   //echo json_encode($_POST);
}
catch (Exception $e)
{
   die('Erreur : ' . $e->getMessage());
}
?>
