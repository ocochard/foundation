#!/bin/bash

#URL="--proxy http://http-proxy.rd.francetelecom.fr:3128 http://p-vite2.riv.rd.francetelecom.fr/"
URL=$MAAS_URL
DIRECTORY="/home/spike"
FILE=$DIRECTORY"/delresult.txt"

# Stop container
function _stop_container()
{
   echo "lxc-wait -n $1 -s RUNNING -t 0"
   if lxc-wait -n $1 -s RUNNING -t 0
   then
      echo "lxc-stop -n $1"
      lxc-stop -n $1
   fi

   COUNTER=0
   while ! lxc-wait -n $1 -s STOPPED -t 0
   do
      echo "$1 isn't stopped yet !!!"
      if [ $COUNTER -lt 10 ]
      then
         echo "Stop process counter $COUNTER"
         let COUNTER=COUNTER+1
      else
         exit 0
      fi
      sleep 5
   done

   echo "$1 stopped"
}

# Delete the container
function _del_container()
{
   # Test if the container exists
   EXIST=`lxc-ls -1 | grep $1`

   if [ -z "$EXIST" ]
   then
      echo "$1 doesn't exits !!!"
   else
      echo "lxc-destroy  -n $1"
      lxc-destroy  -n $1
   fi
}

function _delete_container()
{
   _stop_container $1

   _del_container $1
}

# Read the file including ip and dns addresses
function _read_file()
{
   exec 3<> $1
   while read line <&3
   do {
      name=$(echo $line  | cut -d" " -f2)
      image=$(echo $line | cut -d" " -f3)
      dnsip=$(echo $line | cut -d" " -f7)
      extip=$(echo $line | cut -d" " -f8)
      echo "Container $name $extip $dnsip $image"
      _delete_container $name $extip $dnsip $image
   }
done
exec 3>&-

}


# Read dbase
function _search_del_container()
{
   NAME=$HOSTNAME
   ACTION="del"
   directory=$1
   data="name=$NAME&action=$ACTION"
   :> ${directory}

   #curl --data $data --proxy http://http-proxy.rd.francetelecom.fr:3128 http://p-vite2.riv.rd.francetelecom.fr/newapi/getcontainers.php > ${directory}
   curl --data $data $URL/maasapi/getcontainers.php > ${directory}

   _read_file $1
}
_search_del_container $FILE

exit 0
