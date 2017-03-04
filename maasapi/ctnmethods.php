<?php

function update_hyper($bdd, $data) {
   //echo $data["name"];
   $query = "SELECT * FROM hypervisor h WHERE h.hypername = '" . $data["name"] . "'";
   //echo $query . "\n";
   $reponse = $bdd->query($query);
   $rows = $reponse->fetchAll();
   $reponse->closeCursor();

   date_default_timezone_set("Europe/Paris");
   $time = time();


   if (sizeof($rows) == 0) {
      $query = "INSERT INTO hypervisor (`hypername`, `cpu`, `architecture`, `bridge`, `typevirt`, `totalHD`, `useHD`, `totalRAM`, `useRAM`, `timestamp`) VALUES(\"" . $data["name"] . "\",\"" . $data["cpu"] . "\",\"" . $data["architecture"] . "\",\"" . $data['bridges'] . "\",\"" . $data["typevirt"] . "\",\""  . $data["totalhd"] . "\",\"" . $data["usehd"] . "\",\"" . $data["totalram"] . "\",\"" . $data["useram"] . "\",\"" . $time . "\")";
      echo $query . "\n";
      $reponse = $bdd->query($query);
      $reponse->closeCursor(); 
   } else {
      foreach ($rows as $rowhyper){
         $query = "UPDATE hypervisor SET  architecture=\"" . $data["architecture"] . "\", bridge=\"" . $data["bridges"] . "\", cpu=\"" . $data["cpu"] . "\", typevirt=\"" . $data["typevirt"] . "\", totalHD=\"" . $data["totalhd"] . "\", useHD= \"" . $data["usehd"] . "\", totalRAM= \"" . $data["totalram"] . "\", useRAM= \"" . $data["useram"] .  "\", timestamp= \"" . $time . "\" WHERE hyperid = \"" . $rowhyper["hyperid"] . "\"";
         echo $query . "\n";
         $bdd->query($query);
         $query="INSERT INTO hypervisorusage (`time`,`hyperid`,`instances`,`totalHD`,`useHD`,`totalRAM`,`useRAM`) VALUES(\"" . $time ."\",\""  . $rowhyper["hyperid"] . "\",\"" . $data["instances"] . "\",\"" . $data["totalhd"] . "\",\"" . $data["usehd"] . "\",\"" . $data["totalram"] . "\",\"" . $data["useram"] . "\")";
         echo $query . "\n";
         $reponse = $bdd->query($query);
         $reponse->closeCursor(); 
      }
   }
}

function usage_ctn($bdd, $data) {
   date_default_timezone_set("Europe/Paris");
   $time = time();
   $query="SELECT * FROM container WHERE containername=\"" . $data['container'] . "\"";
   $reponse = $bdd->query($query);
   $rows = $reponse->fetchAll();
   if (sizeof($rows) == 1) {
      foreach ($rows as $rowhyper){
         $query="INSERT INTO containerusage (`time`,`containerid`,`cputime`,`memory`,`bloc`,`tx`,`rx`) VALUES(\"" . $time . "\",\"" . $rowhyper['containerid'] . "\",\"" . $data['cpu'] . "\",\"" . $data['memory'] . "\",\"" . $data['bloc'] . "\",\"" . $data['tx'] . "\",\"" . $data['rx'] . "\" )";
         $reponse = $bdd->query($query);
         $reponse->closeCursor();
      }
   }  
   //echo $query;
}

function update_ctn($bdd, $data) {
   print_r($data);
   $container=$data['container'];
   $ip=explode(";",$data['ipaddress']);
   foreach($ip as $value) {
      if ($value != "250.250.250.250") {
         $param = array("intip" => $value, "container"=> $container); 
         //echo "update_ip"
         update_ip($bdd, $param);
      }
   }

   usage_ctn($bdd, $data);
   //print_r($ip);
}

function update_ip($bdd, $data) {
   $query = "UPDATE container SET intip = \"" . $data["intip"] . "\" WHERE containername = \"" . $data["container"] . "\"";
   //echo $query . "\n";
   $reponse = $bdd->query($query);
   $reponse->closeCursor(); 
}

function getctn($bdd, $data) {

   if ($data["action"] == "add") {
      $action = 1;
   } else if ($data["action"] == "del") {
      $action = 2;
   } else if ($data["action"] == "get") {
      $action = 5;
   } else {
      $action = 5;
   }

   $query = "SELECT c.containerid as containerid, c.containername as containername, i.path as path, i.architecture as architecture, i.imagename as imagename, i.command as command, c.cpu as cpu, c.rammemory as rammemory, c.hdmemory as hdmemory, c.dns as dns, c.extip as extip, c.intip as intip, c.bridge as bridge, c.crontab as crontab, c.status as status FROM container c INNER JOIN hypervisor h ON c.hyperid = h.hyperid INNER JOIN image i ON c.imageid = i.imageid WHERE h.hypername = \"" . $data["name"] . "\" AND c.status = " . $action;

   if ($action == 5) {
      $query = "SELECT c.containerid as containerid, c.containername as containername, i.path as path, i.architecture as architecture, i.imagename as imagename, i.command as command, c.cpu as cpu, c.rammemory as rammemory, c.hdmemory as hdmemory, c.dns as dns, c.extip as extip, c.intip as intip, c.bridge as bridge, c.crontab as crontab, c.status as status FROM container c INNER JOIN hypervisor h ON c.hyperid = h.hyperid INNER JOIN image i ON c.imageid = i.imageid WHERE h.hypername = \"" . $data["name"] . "\"";
   }
   //echo $query . "\n";

   foreach  ($bdd->query($query) as $row) {
      print $row['containerid']     . ";";
      print $row['containername']   . ";";
      print $row['path']            . ";";
      print $row['architecture']    . ";";
      print $row['cpu']             . ";";
      print $row['rammemory']       . ";";
      print $row['hdmemory']        . ";";
      print $row['dns']             . ";";
      print $row['extip']           . ";";
      print $row['intip']           . ";";
      print $row['bridge']          . ";";
      print $row['crontab']         . ";";
      print $row['status']          . ";";
      print $row['imagename']       . ";";
      print $row['command']         . "\n";
   }
}

function getimage($bdd) {
   // Select all images approved by admin
   $query = "SELECT imagename FROM image WHERE status=1";

   foreach  ($bdd->query($query) as $row) {
      print $row['imagename']   . "\n";
   }
}

?>
