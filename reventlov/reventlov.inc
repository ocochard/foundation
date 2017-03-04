<?php

/*function _Reventlov_getprobe() {
  $list_probe = array();

  db_set_active('cdpweather');
  $result = db_query('SELECT probename FROM {new_v4_sonde}');
  db_set_active();

  foreach($result as $item) {
  $list_probe[] = $item->probename;
  }

  return drupal_map_assoc($list_probe);
}*/

function reventlov_getnetwork() {
   $tmp_network = array() ;

   $query = "SELECT networkname FROM {network}";

   db_set_active('lxc');
   $result = db_query($query);
   db_set_active();

   foreach($result as $row){
      $tmp_network[] = $row->networkname;
   }
   return drupal_map_assoc(
      $tmp_network);
}



function _Reventlov_gethypervisors() {
   $tmp_hypervisor = array();

   db_set_active('lxc');
   $result = db_query('SELECT hypername FROM {hypervisor}');
   db_set_active();

   foreach($result as $item) {
      $tmp_hypervisor[] = $item->hypername;
   }

   return drupal_map_assoc(
      $tmp_hypervisor);
}

function _Reventlov_gethypervisor($hyper) {
   $result = array();


   db_set_active('lxc');
   $queryhyper = db_select('hypervisor','h')
      ->fields('h')
      ->condition('hyperid',$hyper)
      ->execute();
   db_set_active();

   foreach($queryhyper as $rowhyper) {
      $result[] = $rowhyper;
   }

   //drupal_set_message("<pre>" . print_r($result, true) . "</pre>");

   return $result;
}

function _Reventlov_getcontainers($hyper) {
   $list_container = array();
   // Init list container

   db_set_active('lxc');
   //$result = db_query("SELECT f.extip, f.dns FROM {container} f, {hypervisor} h  WHERE f.hyperid = h.hyperid and h.hypername = :nom", array(':nom' => $hyper));
   $result = db_query("SELECT f.containername FROM {container} f inner join {hypervisor} h on f.hyperid = h.hyperid  WHERE h.hypername = :nom", array(':nom' => $hyper));
   db_set_active();

   foreach($result as $item) {
      //$extip = str_replace("-", "_", $item->extip);
      //$extip = str_replace(".", "_", $extip);
      //$dns = str_replace("-", "_", $item->dns);
      //$dns = str_replace(".", "_", $dns);

      //$name = $hyper . "_" . $extip . "_" . $dns;
      //$list_container[$name] = $name;
      $name = $item->containername;
      $list_container[$name] = $name;
   }

   //file_put_contents("/var/www/html/drupal.debug", print_r($result, TRUE), FILE_APPEND);
   return $list_container;
}

function _Reventlov_getdisabledcontainers($hyper) {
   $list_container = array();
   db_set_active('lxc');
   $query = db_select('container', 'c')
      ->fields('c')
      ->condition('status',0,'=')
      ->execute();
   db_set_active();

   foreach($query as $item) {
      $extip = str_replace("-", "_", $item->extip);
      $extip = str_replace(".", "_", $extip);
      $dns = str_replace("-", "_", $item->dns);
      $dns = str_replace(".", "_", $dns);

      $name = $hyper . "_" . $extip . "_" . $dns;
      #$list_container[$name] = $name;
      $list_container[$name] = $item->containername;

   }

   return $list_container;
}

function _Reventlov_getenabledcontainers($hyper) {
   $list_container = array();
   db_set_active('lxc');
   $query = db_select('container', 'c')
      ->fields('c')
      ->condition('status',1,'=')
      ->execute();
   db_set_active();



   foreach($query as $item) {
      $extip = str_replace("-", "_", $item->extip);
      $extip = str_replace(".", "_", $extip);
      $dns = str_replace("-", "_", $item->dns);
      $dns = str_replace(".", "_", $dns);

      $name = $hyper . "_" . $extip . "_" . $dns;
      #$list_container[$name] = $name;
      $list_container[$name] = $item->containername;
   }

   return $list_container;
}

function _Reventlov_getreadfile($file) {
   $list_container = array();
   $handle = fopen($filename, "r");

   while(!feof($handle)) {
      $line = fgets($myfile);
      $list_container[$line] = $line;
   }

   fclose($handle);

   //file_put_contents("/var/www/html/drupal.debug", print_r($list_container, TRUE), FILE_APPEND);
   return $list_container;
}

function getlistprofiles() {
   $tmp_profile = array();

   db_set_active('lxc');
   $query = db_select('profile', 'p')
      ->fields('p')
      ->execute();
   db_set_active();

   if ($query->rowCount() > 0)
      foreach ($query as $row)
         $tmp_profile[] = $row->profilename;

   //drupal_set_message('<pre>'.print_r($tmp_profile,true).'</pre>');

   return drupal_map_assoc(
      $tmp_profile);
}


function add_profile($data) {
   $profilename         = $data['profilename'];
   $profileinformation  = $data['profileinformation'];
   $content             = $data['content'];

   db_set_active('lxc');
   $result = db_select('profile', 'p')
      ->fields('p')
      ->condition('profilename',$profilename)
      ->execute();
   db_set_active();

   if ($result->rowCount() == 0) {
      db_set_active('lxc');
      $id = db_insert('profile')
         ->fields(array(
            'profilename' => $profilename,
            'information' => $profileinformation,
            'content'     => $content,
         ))
         ->execute();
      db_set_active();
   } else {
      db_set_active('lxc');
      db_update('profile')
         ->fields(array( 'content'   => $content,))
         ->condition('profilename', $profilename, '=')
         ->execute();
      db_set_active();
   }
}

function reventlov_add_containers($content, $data) {
   foreach($content as $key => $line){
      drupal_set_message('Le fichier: <strong>"' . $line . '"</strong> est bien sur le serveur');
      reventlov_add_one_containers($data);
   }
}

function reventlov_add_one_container($data) {
   //drupal_set_message('<pre>'.print_r($data, true).'</pre>');

   //$hyper = $info[0];
   $hyper         = $data['hypervisor'];
   $cpu           = $data['cpu'];
   $memory        = $data['memory'];
   $period        = $data['period'];
   $bridge        = $data['bridge'];
   $startprocess  = $data['startprocess'];
   $endprocess    = $data['endprocess'];
   $crontab       = $data['crontab'];

   $imageid    = $data['imageid'];

   $extip   = $data['extip'];
   $intip   = $data['intip'];
   $dns     = $data['dnsip'];

   db_set_active('lxc');
   $queryhyper = db_select('hypervisor', 'h')
      ->fields('h')
      ->condition('hyperid',$hyper,'=')
      ->execute();
   db_set_active();

   db_set_active('lxc');
   $queryimage = db_select('image', 'i')
      ->fields('i')
      ->condition('imageid',$imageid,'=')
      ->execute();
   db_set_active();

   foreach($queryhyper as $rowhyper) {
      foreach($queryimage as $rowimage) {

         $hypername  = $rowhyper->hypername;
         $hyperid    = $rowhyper->hyperid;
         $command    = $rowimage->command;
         //$crontab    = $crontab . " " . $command;
         $crontab    = $crontab . " ";
         $name       = uniqid();

         $data['containername'] = $name;

         //drupal_set_message('<pre>'.print_r($data, true).'</pre>');
         db_set_active('lxc');


         /***********************************************/

         $id = db_insert('container')
            ->fields(array(
               'containername'   => $name,
               'hyperid'         => $hyperid,
               'imageid'         => $imageid,
               'cpu'             => $cpu,
               'rammemory'       => $memory,
               'hdmemory'        => "0",
               'bridge'          => $bridge,
               'extip'           => $extip,
               'intip'           => $intip,
               'dns'             => $dns,
               'status'          => 0,
               'period'          => $period,
               'startprocess'    => $startprocess,
               'endprocess'      => $endprocess,
               'crontab'         => $crontab,
            ))
            ->execute();

         /***********************************************/

         db_set_active();

         add_profile_container($data);
      }
   }
}

function add_profile_container($data) {
   //drupal_set_message('<pre>'.print_r($data,true).'</pre>');

   $profile = $data['profile'];
   $date = new DateTime();
   $timestamp = $date->getTimestamp();

   db_set_active('lxc');
   $queryprofile  = db_select('profile', 'p')
      ->fields('p')
      ->condition('profilename',$profile,'=')
      ->execute();
   db_set_active();

   db_set_active('cdpweather');
   $nid = db_insert('new_v4_sonde')
      ->fields(array(
         'probename' => $data['containername'],
         'ip'        => '0.0.0.0',
         'ext'       => $data['extip'],
         'dns'       => $data['dnsip'],
         'uptime'    => '0',
         'lasttime'  => $timestamp,  
         'mac'       => '00:00:00:00:00:00',
         'cc'        => 'XX',
         'country'   => 'XX',
         'version'   => 'XX',
         'status'    => '0',
      ))
      ->execute();
   db_set_active();

   db_set_active('cdpweather');
   $queryprobe = db_select('new_v4_sonde','s')
      ->fields('s')
      ->condition('probename',$data['containername'],'=')
      ->execute();
   db_set_active();

   $infoprobe = array();

   foreach($queryprobe as $rowprobe) {
      $infoprobe['probeid'] = $rowprobe->probeid;
      $infoprobe['probename'] = $rowprobe->probename;

      db_set_active('cdpweather');
      db_insert('new_v4_users_probes')
         ->fields(array(
            'userid'    => $data['userid'],
            'probeid'   => $infoprobe['probeid'],
         ))
         ->execute();
      db_set_active();

      db_set_active('drupal');
      $mails = db_query("SELECT mail FROM users WHERE uid = :uid", array(':uid' => $data['userid']));
      db_set_active();


      foreach($mails as $mail) {
         db_set_active('cdpweather');
         db_insert('new_v4_users_probes_edit')
            ->fields(array(
               'userid'    => $data['userid'],
               'probeid'   => $infoprobe['probeid'],
               'email'     => $mail->mail,
               'ref'       => 0,
            ))
            ->execute();
         db_set_active();
      }
   }


   foreach($queryprofile as $rowprofile) {
      $urllist = $rowprofile->content;
      $tablisturl = explode(";", $urllist);
      foreach($tablisturl as &$url) {
         if ($url != ''){
            db_set_active('cdpweather');
            $queryurl = db_select('new_v4_url', 'u')
               ->fields('u')
               ->condition('urlname',$url,'=')
               ->execute();
            db_set_active();
            if ($queryurl->rowcount() == 1) {
               foreach($queryurl as $rowurl) {
                  db_set_active('cdpweather');
                  db_insert('new_v4_fqdnuser')
                     ->fields(array(
                        'urlid'  => $rowurl->urlid,
                        'probeid'=> $infoprobe['probeid'],
                        'status' => 1,
                     ))
                     ->execute();
                  db_set_active();
               }
            } else {
               //drupal_set_message("URL doesn't exists.");
               db_set_active('cdpweather');
               db_insert('new_v4_url')
                  ->fields(array(
                     'urlname'   => $url,
                  ))
                  ->execute();
               db_set_active();

               db_set_active('cdpweather');
               $queryurl = db_select('new_v4_url', 'u')
                  ->fields('u')
                  ->condition('urlname',$url,'=')
                  ->execute();
               db_set_active();

               foreach($queryurl as $rowurl) {
                  db_set_active('cdpweather');
                  db_insert('new_v4_fqdnuser')
                     ->fields(array(
                        'urlid'  => $rowurl->urlid,
                        'probeid'=> $infoprobe['probeid'],
                        'status' => 1,
                     ))
                     ->execute();
                  db_set_active();
               }
            }
         }
      }
   }
}


/*function __reventlov_add_one_container($line, $data) {

   $info = explode(";", $line);

   if (count($info) == 2) {
      //$hyper = $info[0];
      $hyper = $data['hypervisor'];

      db_set_active('lxc');
      $queryfirst = db_select('hypervisor', 'h')
         ->fields('h')
         ->condition('hyperid',$hyper)
         ->execute();
      db_set_active();

      foreach($queryfirst as $row) {
         $networkname = $data['network'];
         db_set_active('lxc');
         $querynetwork = db_select('network', 'n')
            ->fields('n')
            ->condition('networkname',$networkname)
            ->execute();
         db_set_active();

         foreach($querynetwork as $rownetwork) {

            $hypername  = $row->hypername;
            $hyperid    = $row->hyperid;
            $ip         = trim($info[0]);
            $dns        = trim($info[1]);
            $imageid    = $data['imageid'];
            $networkid  = $rownetwork->networkid;
            $name       = uniqid();

            //drupal_set_message('<pre>' . print_r($image, true) . '</pre>');
            //drupal_set_message('<pre>' . print_r($archi, true) . '</pre>');
            if(filter_var($ip, FILTER_VALIDATE_IP) && filter_var($dns, FILTER_VALIDATE_IP)) {
               db_set_active('lxc');
               $querycontainerexist = db_select('container', 'c')
                  ->fields('c')
                  ->condition('hyperid',$hyperid)
                  ->condition('imageid',$imageid)
                  ->condition('extip',$ip)
                  ->condition('dns',$dns)
                  ->execute();
               db_set_active();

               if ($querycontainerexist->rowCount() == 0) {
                  //foreach($queryimageexist as $rowimage) {
                  //$imageid = $rowimage->imageid;
                  db_set_active('lxc');
                  $id = db_insert('container')
                     ->fields(array(
                        'containername'   =>$name,
                        'hyperid'         =>$hyperid,
                        'imageid'         =>$imageid,
                        'cpu'              => "0",
                        'rammemory'        => "0",
                        'hdmemory'         => "0",
                        //'networkid'       =>$networkid,
                        'extip'           =>$ip,
                        'intip'           =>'0.0.0.0',
                        'dns'             => $dns,
                        'status'          =>0,
                     ))
                     ->execute();
                  drupal_set_message($id);
               }
            }
         }
      }
   }
}*/

function _Reventlov_disable_ctn($form, $form_state) {
   if(isset($form_state) and isset($form_state['values']))   $_SESSION['Reventlov'] = $form_state['values'];
   $_SESSION['Reventlov'] = $form_state['values'];

   if(isset($form_state) and isset($form_state['values']) and isset($form_state['values']['selected']) ) {
      $containers = $_SESSION['Reventlov']['selected'];
      foreach ($containers as $container) {
         $container = str_replace("-", "_", $container);
         $container = str_replace(".", "_", $container);

         $info = explode("_", $container);


         //drupal_set_message('Le fichier: <strong>"' . $container . '"</strong> est bien sur le serveur');
         //
         db_set_active('lxc');
         $queryhyper = db_select('hypervisor', 'h')
            ->fields('h')
            ->condition('hypername',$info[0])
            ->execute();
         db_set_active();

         $extip = $info[1] . "." . $info[2] . "." . $info[3] . "." . $info[4];
         $dns = $info[5] . "." . $info[6] . "." . $info[7] . "." . $info[8];

         foreach($queryhyper as $hyper) {
            db_set_active('lxc');
            db_update('container')
               ->fields(array( 'status' => 0,))
               ->condition('hyperid', $hyper->hyperid)
               ->condition('extip',$extip)
               ->condition('dns',$dns)
               ->execute();
            db_set_active();
         }
      }
   }
}

function _Reventlov_enable_ctn($form, $form_state) {
   if(isset($form_state) and isset($form_state['values']))   $_SESSION['Reventlov'] = $form_state['values'];
   $_SESSION['Reventlov'] = $form_state['values'];

   if(isset($form_state) and isset($form_state['values']) and isset($form_state['values']['unselected']) ) {
      $containers = $_SESSION['Reventlov']['unselected'];
      foreach ($containers as $container) {
         $container = str_replace("-", "_", $container);
         $container = str_replace(".", "_", $container);

         $info = explode("_", $container);

         db_set_active('lxc');
         $queryhyper = db_select('hypervisor', 'h')
            ->fields('h')
            ->condition('hypername',$info[0])
            ->execute();
         db_set_active();

         $extip = $info[1] . "." . $info[2] . "." . $info[3] . "." . $info[4];
         $dns = $info[5] . "." . $info[6] . "." . $info[7] . "." . $info[8];


         foreach($queryhyper as $hyper) {
            db_set_active('lxc');
            db_update('container')
               ->fields(array( 'status' => 1,))
               ->condition('hyperid', $hyper->hyperid)
               ->condition('extip',$extip)
               ->condition('dns',$dns)
               ->execute();
            db_set_active();
         }
      }
   }
}
