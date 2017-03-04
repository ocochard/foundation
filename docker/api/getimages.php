<?php

try {
   $bdd = new PDO('mysql:host=localhost;dbname=content_delivery_weather;charset=utf8', 'spike', 'valentine');

   $data = array();
   $output = array();


   $rest_json = file_get_contents("php://input");
   $_POST = json_decode($rest_json, true);

   //print_r($_POST['name'] . "\n");

   if ( isset($_POST['name']) ) {
      $query = "SELECT * FROM new_v4_hyper WHERE hypername = '" . $_POST['name'] . "'";

      //print_r($query . "\n");

      $reponse = $bdd->query($query);

      $rows = $reponse->fetchAll();

      $reponse->closeCursor();

      if (count($rows) == 0) {
         //print_r("nothing \n"); 
         $query = "INSERT INTO new_v4_hyper (`hypername`) VALUES (\"" . $_POST['name'] . "\")";
         //print_r($query . "\n");   
         $reponse = $bdd->query($query);
      } 

      //print_r($rows[0]["hypername"] . "\n");
      $query = "SELECT i.imagename FROM new_v4_hyper h inner join new_v4_hyperimage c on h.hyperid = c.hyperid inner join new_v4_image i on c.imageid = i.imageid WHERE h.hypername = '" . $_POST['name'] . "'";
      //print_r($query . "\n");   
      $reponse = $bdd->query($query);
      $rows = $reponse->fetchAll();

      foreach($rows as $row) {
         $data[] = array( 
            "imagename"    => $row['imagename'],
         );
      }

      $reponse->closeCursor();

      $output['images']   = $data;

      echo json_encode($output);
   } else {
      $output['images']   = $data;
      echo json_encode($output) . "\n";
   }


} catch (Exception $e) {
   die('Erreur : ' . $e->getMessage());
}

?>
