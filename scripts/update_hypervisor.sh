#!/bin/bash

URL=$URL_MAAS

function update_hypervisor()
{

   STATUS="Unknown"
   STATUS_LXC=$(dpkg -l | grep -i lxc)
   STATUS_DOCKER=$(dpkg -l | grep -i docker)

   if [ ! -z "$STATUS_LXC" ]
   then
      STATUS="Lxc"
      INSTANCE=$(lxc-ls --running | wc -l)
      send_data $STATUS $INSTANCE
   elif [ ! -z "$STATUS_DOCKER" ]; then
      STATUS="Docker"
      INSTANCE=$(docker ps | wc -l)
      INSTANCE=$(($INSTANCE - 1))
      send_data $STATUS $INSTANCE
   fi

}

function send_data()
{
   STATUS=$1
   INSTANCE=$2

   BRIDGE=""
   for i in `/sbin/brctl show | grep -v '^bridge' | tr -s " " | cut -f1`
   do
      BRIDGE="$i $BRIDGE"
   done 

   ARCHI=$(uname -m)
   CPU=$(cat /proc/cpuinfo | grep 'processor' | wc -l)
   # TOTALHD=$(df -BM | awk '{sub(/ /,""); sum += $4;} END {print sum}')
   TOTALHD=$(df | awk '{sub(/ /,""); sum += $4;} END {print sum}')
   # USEHD=$(df -BM | awk '{sub(/ /,""); sum += $3;} END {print sum}')
   USEHD=$(df | awk '{sub(/ /,""); sum += $3;} END {print sum}')
   TOTALRAM=$(free | egrep 'Mem:.*' | awk '{sub(/ /,""); print $2}')
   USERAM=$(free | egrep 'Mem:.*' | awk '{sub(/ /,""); print $3}')

   data="name=$HOSTNAME&totalhd=$TOTALHD&usehd=$USEHD&totalram=$TOTALRAM&useram=$USERAM&typevirt=$STATUS&instances=$INSTANCE&cpu=$CPU&architecture=$ARCHI&bridges=$BRIDGE"

   #echo $data
   #echo "curl --data $data $URL/maasapi/updatehypervisor.php"
   curl --data $data $URL/maasapi/updatehypervisor.php 
}

update_hypervisor
