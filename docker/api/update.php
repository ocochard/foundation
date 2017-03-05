<?php

try {
   $bdd = new PDO('mysql:host=localhost;dbname=content_delivery_weather;charset=utf8', 'spike', 'valentine');

   $rest_json = file_get_contents("php://input");
   $_POST = json_decode($rest_json, true);

   //print_r($_POST['name'] . "\n");

   if ( isset($_POST['name']) && isset($_POST['containerid']) && isset($_POST['dockerid'])) {
      $query = "SELECT * FROM new_v4_hyper WHERE hypername = '" . $_POST['name'] . "'";

      //print_r($query . "\n");

      $reponse = $bdd->query($query);

      $rows = $reponse->fetchAll();

      foreach($rows as $row) {
         //$row['hyperid'];
         $query = "UPDATE new_v4_container SET dockerid = '" . $_POST['dockerid'] . "' WHERE containerid = '" . $_POST['containerid'] . "' AND hyperid = " . $row['hyperid'];
         //print_r($query);
         $reponse = $bdd->query($query);
      }

      $reponse->closeCursor();
   }  
} catch (Exception $e) {
   die('Erreur : ' . $e->getMessage());
}

?>
