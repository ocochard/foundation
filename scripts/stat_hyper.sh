#!/bin/bash

heure=$(date +%H:%M:%S)
jour=$(date +%Y%m%d)

memoire=$(netstat -i | grep '^eth0' |  tr -s ' ' | cut -d ' ' -f 4)
sleep 60

while [ 1 ]; do
   ###############################################################
   # MEMOIRE
   ###############################################################
   mlibre=$(free | grep 'Mem:' | tr -s ' ' | cut -d ' ' -f 4)
   mutilise=$(free | grep 'Mem:' | tr -s ' ' | cut -d ' ' -f 3)

   echo "$jour  $heure  $mlibre  $mutilise" >> stat_mem.txt

   ###############################################################
   # OCTETS RECUS
   ###############################################################
   recu=$(netstat -i | grep '^eth0' |  tr -s ' ' | cut -d ' ' -f 4)
   resultat=$(($recu-$memoire))
   echo "$jour  $heure  $resultat" >> stat_rx.txt 
   memoire=$recu
   sleep 60
done
