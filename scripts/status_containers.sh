#!/bin/bash

#Start or stop containers depending the status on cdpw
FILE="/home/spike/status.txt"

# Start the container
function _start_container()
{
   if lxc-wait -n $1 -s RUNNING -t 0
   then
      echo "$1 yet started"
   else
      lxc-start -n $1 -d
      echo "$1 started"
   fi
}

# Stop the container
function _stop_container()
{
   if lxc-wait -n $1 -s STOPPED -t 0
   then
      echo "$1 yet stopped"
   else
      lxc-stop -n $1
      echo "$1 stopped"
   fi

}

# Read the container depending the status
function _modify_container()
{   
   #infoip=$1
   #infodns=$2
   #IPname=${infoip//./_}
   #IPdns=${infodns//./_}
   #Host=${HOSTNAME}
   #NODE=$Host":"$infoip":"$infodns

   local name=$NODE
   if [ "$2" == "1" ]
   then
      echo "Start the container $name"
      _start_container $1 
   else
      echo "Stop the container $name"
      _stop_container $1
   fi
}

# Read the file including ip and dns addresses
function _read_file()
{
   exec 3<> $1
   while read line <&3
   do {
      name=$(echo $line| cut -d" " -f2)
      #add=$(echo $line | cut -d" " -f4)
      #dns=$(echo $line | cut -d" " -f3)
      sts=$(echo $line | cut -d" " -f6)
      echo "Container $name status $sts"
      # _modify_container $add $dns $sts
      _modify_container $name $sts
   }
done
exec 3>&-
}

# Read status on mysql
function _search_status_containers()
{
   directory=$1

   NAME=$HOSTNAME
   data="name=$NAME"

   curl --proxy http://http-proxy.rd.francetelecom.fr:3128 --data $data http://p-vite2.riv.rd.francetelecom.fr/newapi/getcontainers.php > ${directory}

   _read_file $1
}


_search_status_containers $FILE

exit 0
