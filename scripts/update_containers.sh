#!/bin/bash

#URL="--proxy http://http-proxy.rd.francetelecom.fr:3128 http://p-vite2.riv.rd.francetelecom.fr/"
URL=$URL_MAAS
DIRECTORY="/tmp"
FILE=$DIRECTORY"/majresult.txt"
#set -x 


function _stop_container()
{
   if [ ! -z "$VIRT_LXC" ]; 
   then
      if lxc-wait -n $name -s RUNNING -t 0 
      then
         echo "lxc-stop -n $name"
         lxc-stop -n $name
      fi

      COUNTER=0
      while ! lxc-wait -n $name -s STOPPED -t 0
      do
         echo "$name isn't stopped yet !!!"
         if [ $COUNTER -lt 10 ]
         then
            echo "Stop process counter $COUNTER"
            let COUNTER=COUNTER+1
         else
            exit 0
         fi
         sleep 5
      done
   fi
   if [ ! -z "$VIRT_DOCKER" ]; 
   then
      RUNNING=$(docker ps | grep $name)
      if [ ! -z "$RUNNING" ]
      then
         echo "docker stop $name"
         docker stop $name
      fi
   fi
   echo "$name stopped"
}

function _reload_crontab()
{
   if [ ! -z "$VIRT_LXC" ]; 
   then
      The_config_lxc
   fi

   if [ ! -z "$VIRT_DOCKER" ];
   then
      The_config_docker
   fi
}

function The_config_lxc()
{
   CONTAINER_ID=$name
   echo "name: $name"
   echo "iso: $iso"
   echo "crontab: $crontab"
   echo "#!/bin/bash" > /var/lib/lxc/$name/rootfs/tmp/$CONTAINER_ID
   echo "" >> /var/lib/lxc/$name/rootfs/tmp/$CONTAINER_ID
   # echo "croncmd=\"/opt/dockbaley/scripts/process.sh\"" >> /tmp/$CONTAINER_ID
   echo "croncmd=\"$command\"" >> /var/lib/lxc/$name/rootfs/tmp/$CONTAINER_ID
   echo "cronjob=\"$crontab \$croncmd\"" >> /var/lib/lxc/$name/rootfs/tmp/$CONTAINER_ID
   echo "( crontab -l | grep -v \"\$croncmd\" ; echo \"\$cronjob\" ) | crontab -" >> /var/lib/lxc/$name/rootfs/tmp/$CONTAINER_ID
}

function The_config_docker()
{
   CONTAINER_ID=$(docker ps | grep $name | cut -d" " -f1)
   echo "name: $name"
   echo "iso: $iso"
   echo "crontab: $crontab"
   echo "#!/bin/bash" > /tmp/$CONTAINER_ID
   echo "" >> /tmp/$CONTAINER_ID
   # echo "croncmd=\"/opt/dockbaley/scripts/process.sh\"" >> /tmp/$CONTAINER_ID
   echo "croncmd=\"$command\"" >> /tmp/$CONTAINER_ID
   echo "cronjob=\"$crontab \$croncmd\"" >> /tmp/$CONTAINER_ID
   echo "( crontab -l | grep -v \"\$croncmd\" ; echo \"\$cronjob\" ) | crontab -" >> /tmp/$CONTAINER_ID
}


function _start_container()
{
   if [ ! -z "$VIRT_LXC" ]; 
   then
      if lxc-wait -n $name -s RUNNING -t 0
      then
         echo "$name started"
      else
         lxc-start -n $name -d
      fi
   fi

   if [ ! -z "$VIRT_DOCKER" ]; 
   then
      docker start $name
   fi

   echo "$name started"
}

function _del_container()
{
   # Test if the container exists
   if [ ! -z "$VIRT_LXC" ]; 
   then
      EXIST=`lxc-ls -1 | grep $name`

      if [ -z "$EXIST" ]
      then
         echo "$name doesn't exits !!!"
      else
         echo "lxc-destroy  -n $name"
         lxc-destroy  -n $name
      fi
   fi

   if [ ! -z "$VIRT_DOCKER" ]; 
   then
      EXIST=`docker ps -a | grep $name`

      if [ -z "$EXIST" ]
      then
         echo "$name doesn't exits !!!"
      else
         echo "docker rm $name"
         docker rm $name
      fi

   fi
}

function _info_tx()
{
   if [ ! -z "$VIRT_LXC" ]
   then
      if [ "$status" == "1" ]
      then
         result=$(lxc-info -n $name| grep TX | tr -s " " | cut -d " " -f4)
         result="tx=$result"
      else
         result="tx=0"
      fi
   fi
   if [ ! -z "$VIRT_DOCKER" ]
   then
      if [ "$status" == "1" ]
      then
         result=$(docker stats $name --no-stream=true | grep -v 'CONTAINER' | tr -s " " | cut -d" " -f9)
         result="tx=$result"
      fi
   fi
   echo $result;
}  

function _info_rx()
{
   if [ ! -z "$VIRT_LXC" ]
   then
      if [ "$status" == "1" ]
      then
         result=$(lxc-info -n $name| grep RX | tr -s " " | cut -d " " -f4)
         result="rx=$result"
      else
         result="rx=0"
      fi
   fi
   if [ ! -z "$VIRT_DOCKER" ]
   then
      if [ "$status" == "1" ]
      then
         result=$(docker stats $name --no-stream=true | grep -v 'CONTAINER' | tr -s " " | cut -d" " -f12)
         result="rx=$result"
      fi
   fi

   echo $result;
} 

function _info_bloc()
{
   if [ ! -z "$VIRT_LXC" ]
   then
      if [ "$status" == "1" ]
      then
         result=$(lxc-info -n $name| grep BlkIO | tr -s " " | cut -d " " -f3)
         result="bloc=$result"
      else
         result="bloc=0"
      fi
   fi
   if [ ! -z "$VIRT_DOCKER" ]
   then
      result=$(docker stats $name --no-stream=true | grep -v 'CONTAINER' | tr -s " " | cut -d" " -f14)
      result="bloc=$result"
   fi
   echo $result;
}

function _info_memory()
{
   if [ ! -z "$VIRT_LXC" ]
   then
      if [ "$status" == "1" ]
      then
         result=$(lxc-info -n $name| grep Memory | tr -s " " | cut -d " " -f3)
         result="memory=$result"
      else
         result="memory=0"
      fi
   fi
   if [ ! -z "$VIRT_DOCKER" ]
   then
      result=$(docker stats $name --no-stream=true | grep -v 'CONTAINER' | tr -s " " | cut -d" " -f3)
      result="memory=$result"
   fi
   echo $result;
}

function _info_cpu()
{
   if [ ! -z "$VIRT_LXC" ]
   then
      if [ "$status" == "1" ]
      then
         result=$(lxc-info -S -n $name | grep 'CPU' | tr -s " " | cut -d " " -f3)
         result="cpu=$result"
      else
         result="cpu=0"
      fi
   fi
   if [ ! -z "$VIRT_DOCKER" ]
   then
      result=$(docker stats $name --no-stream=true | grep -v 'CONTAINER' | tr -s ' ' | cut -d' ' -f2 | cut -d'%' -f1)
      result="cpu=$result"
   fi
   echo $result;
}

function _info_ethernet()
{
   if [ ! -z "$VIRT_LXC" ]
   then
      if [ "$status" == "1" ]
      then
         for i in $(lxc-info -i -n $name | tr -s " " | cut -d " " -f2); do
            foo="$foo;$i"
         done
         foo="ipaddress=$foo"
      else
         foo="ipaddress=-.-.-.-"
      fi
   fi
   if [ ! -z "$VIRT_DOCKER" ]
   then
      foo="ipaddress=;$(docker inspect --format '{{ .NetworkSettings.IPAddress }}' $name)"
   fi
   echo $foo;
}

function _info_container()
{
   result_info_ethernet=$(_info_ethernet $name)
   result_info_cpu=$(_info_cpu $name)
   result_info_memory=$(_info_memory $name)
   result_info_bloc=$(_info_bloc $name)
   result_info_tx=$(_info_tx $name)
   result_info_rx=$(_info_rx $name)
   result="container=$name&$result_info_ethernet&$result_info_cpu&$result_info_memory&$result_info_bloc&$result_info_tx&$result_info_rx"
   # echo $result;
   data=$result
   curl --data $data $URL/maasapi/updatecontainer.php
}

function _iptables_add_rules() 
{
   if [ "$dnsip" != "-.-.-.-" ] && [ "$extip" != "-.-.-.-" ] && [ "$intip" != "-.-.-.-" ] && [  "$status" == "1" ]
   then
      echo "iptable process"      
      rulesnat=$(iptables -L -t nat | grep SNAT | grep to:$extip)
      rulesnat=$(iptables -L -t nat | grep DNAT | grep to:$intip)
      if [ "$rulesnat" == "" ] && [ "$rulednat" == "" ]
      then
         echo "add iptables -t nat -A POSTROUTING -o $bridge -j SNAT --to $extip"
         iptables -t nat -A POSTROUTING -o $bridge ! -s 192.168.49.49 -s $intip  -j SNAT --to $extip
         iptables -t nat -A PREROUTING -i $bridge -d $extip   -j DNAT --to $intip 
      fi
   fi
}


function _iptables_del_rules()
{
   if [ "$dnsip" != "-.-.-.-" ] && [ "$extip" != "-.-.-.-" ] && [ "$intip" != "-.-.-.-" ] && [  "$status" == "2" ]
   then
      echo "iptable process"
      rulesnat=$(iptables -L -t nat | grep SNAT | grep to:$extip)
      rulesnat=$(iptables -L -t nat | grep DNAT | grep to:$intip)
      if [ "$rulesnat" != "" ] && [ "$rulednat" != "" ]
      then
         echo "del iptables -t nat -A POSTROUTING -o $bridge -j SNAT --to $extip"
         iptables -t nat -D POSTROUTING -o $bridge -s $intip  -j SNAT --to $extip
         iptables -t nat -D PREROUTING -i $bridge -d $extip   -j DNAT --to $intip
      fi
   fi
}

# Read the file including ip and dns addresses
function _read_file()
{
   while read line 
   do 
      export name=$(echo $line   | cut -d";" -f2)
      export dnsip=$(echo $line  | cut -d";" -f7)
      export extip=$(echo $line  | cut -d";" -f9)
      export intip=$(echo $line  | cut -d";" -f10)
      export bridge=$(echo $line | cut -d";" -f11)
      export crontab=$(echo $line | cut -d";" -f12)
      export status=$(echo $line | cut -d";" -f13)
      export iso=$(echo $line | cut -d";" -f14)
      export command=$(echo $line | cut -d";" -f15)

      export VIRT_LXC=$(dpkg -l| grep -i lxc)
      export VIRT_DOCKER=$(dpkg -l| grep -i docker)

      if [ "$status" == "1" ]
      then
         echo "Start the container $name"
         _reload_crontab
         _start_container 
         _info_container
         _iptables_add_rules

      elif [ "$status" == "0" ]
      then
         echo "Stop the container $name"
         _stop_container 
      elif [ "$status" == "2" ]
      then
         echo "Delete the container $name"
         _stop_container 
         _del_container
         _iptables_del_rules
      fi

      #echo $name
      #echo "Container $name $extip $dnsip $bridge $status"
      #_info_container $name $extip $dnsip $image
      #_info_container
      #_iptables
   done < $1
}


# Read dbase
function _search_container()
{
   NAME=$HOSTNAME
   ACTION="get"
   directory=$1
   data="name=$NAME&action=$ACTION"
   :> ${directory}

   #echo $data
   #curl --data $data --proxy http://http-proxy.rd.francetelecom.fr:3128 http://p-vite2.riv.rd.francetelecom.fr/newapi/getcontainers.php > ${directory}
   #echo "curl --data $data $URL/newapi/getcontainers.php"

   echo "curl --data $data $URL/maasapi/getcontainers.php > ${directory}"
   curl --data $data $URL/maasapi/getcontainers.php > ${directory}

   _read_file $1
}

_search_container $FILE

exit 0

#1;5846a2340d89b;cdpweather.maas;x86_64;1;256;0;-.-.-.-;-.-.-.-;-.-.-.-;br0;*/10 5-6 * * *  /opt/dockbaley/dockbaley -p path;1
#2;5846a2c6c1a30;cdpweather.maas;x86_64;1;256;0;-.-.-.-;-.-.-.-;-.-.-.-;br0;* * * * *  /opt/dockbaley/dockbaley -p path;0
