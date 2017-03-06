#!/bin/bash

#URL="--proxy http://http-proxy.rd.francetelecom.fr:3128 http://p-vite2.riv.rd.francetelecom.fr/"
URL=$URL_MAAS
IPPROBE="250.250.250.250"
ETHPROBE="eth0"
GATEWAY="192.168.2.10"
DIRECTORY="/tmp"
FILE=$DIRECTORY"/addresult.txt"


# Stop container
function _stop_container()
{
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

# Start container
function _start_container()
{
   if lxc-wait -n $1 -s RUNNING -t 0
   then
      echo "$1 started"
   else
      lxc-start -n $1 -d
   fi

   echo "$1 started"
}
function The_creation_docker()
{
   EXIST=$(docker ps | grep -i $name)

   if [ -z "$EXIST" ]
   then
      echo "docker -h $name run -d -v /tmp:/tmp --name $name $iso" 
      docker run -h $name -d -v /tmp:/tmp --name $name $iso

      echo "Crontab configuration"
      The_config_docker
   else
      echo "The container $name exist"
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

# The_creation $NODE $EXTIP $DNSIP $NEWIMG $NEWIMAGE result
#$name $extip $dnsip $image $bridge $crontab
#The_creation $NEWIMG $NEWIMAGE result
function The_creation()
{
   DIRECTORY="/var/lib/lxc/$name"
   CRONFILE=$DIRECTORY"/rootfs/root/crontab"
   RESOLV=$DIRECTORY"/rootfs/etc/resolv.conf"
   NETWORK=$DIRECTORY"/rootfs/etc/network/interfaces"
   IPPROBE="250.250.250.250"

   #echo "--> $crontab"
   #echo "$crontab" > /root/crontab

   #Return test
   local __result=$3

   # Test if the container exists
   EXIST=`lxc-ls -1 | grep $name`

   if [ ! -z "$EXIST" ]
   then
      echo "$name exits yet !!!"
      #echo "false"
      local myresult='false'
      eval $__result="'$myresult'"
      # exit 0
      echo ""
   else
      echo ""
      echo "mkdir /var/lib/lxc/$name"
      mkdir /var/lib/lxc/$name
      echo ""
      echo "cp $1 /var/lib/lxc/$name"
      cp $1 /var/lib/lxc/$name
      echo ""
      echo "cd /var/lib/lxc/$name"
      cd /var/lib/lxc/$name
      echo ""
      echo "tar --numeric-owner -xzf $2"
      tar --numeric-owner -xzf $2
      echo ""
      #echo "sed s/cdpweather/$name config > config.new"
      #sed s/cdpweather/$name/ config > config.new
      #echo ""
      #echo "sed s/br0/$bridge config.new > config"
      #sed s/br0/$bridge/ config.new > config
      #echo ""
      _create_config
      echo "-->$crontab"
      # echo "$crontab" > $CRONFILE
      The_config_lxc
      echo ""
      echo "rm -f $2 config.new"
      rm -f $2
      echo ""
      #echo "cp -n config.new config"
      #cp -f config.new config

      if [ "$dnsip" != "-.-.-.-" ]
      then
         echo ""
         echo "modify dns"
         #_modify_conf_dns "nameserver" $dnsip $RESOLV
         echo "nameserver $dnsip" > $RESOLV
         echo ""
         #echo "modify address extern"
         #_modify_conf_address $IPPROBE $extip $NETWORK
      fi
      
      echo $name > /var/lib/lxc/$name/rootfs/etc/hostname

      echo "_start_container $name"
      _start_container $name
      echo ""
   fi 
}

function _create_config() 
{
   echo "# The $name config" > config
   echo "lxc.network.type = veth" >> config
   echo "lxc.network.flags = up" >> config
   echo "lxc.network.link = $bridge" >> config
   echo "lxc.network.ipv4 = 0.0.0.0/24" >> config
   echo "lxc.rootfs = /var/lib/lxc/$name/rootfs" >> config
   echo "" >> config
   echo "# Common configuration" >> config
   echo "lxc.include = /usr/share/lxc/config/debian.common.conf" >> config
   echo "" >> config
   echo "# Container specific configuration" >> config
   echo "#lxc.mount = /var/lib/lxc/$name/fstab" >> config
   echo "lxc.utsname = $name" >> config
   echo "lxc.arch = $archi" >> config
   echo "lxc.autodev = 1" >> config
   echo "#lxc.kmsg = 0" >> config
}

# Clone the container
function _clone_container()
{
   #Return test
   local __result=$3

   # Test if the container exists
   EXIST=`lxc-ls -1 | grep $2`

   if [ ! -z "$EXIST" ]
   then
      echo "$2 exits yet !!!"
      #echo "false"
      local myresult='false'
      eval $__result="'$myresult'"
      # exit 0
   else
      echo "lxc-clone -o $1 -n $2"
      lxc-clone -o $1 -n $2

      COUNTER=0
      while ! lxc-wait -n $2 -s STOPPED -t 0
      do
         echo "$2 isn't created yet !!!"
         if [ $COUNTER -lt 10 ]
         then
            echo "Create process counter $COUNTER"
            let COUNTER=COUNTER+1
         else
            #echo "false"
            local myresult='false'
            eval $__result="'$myresult'"
         fi
         sleep 5
      done
      #echo "true"
      local myresult='true'
      eval $__result="'$myresult'"
   fi
}

# Modify config file
# _modify_conf_dns "nameserver" $dnsip $RESOLV
function _modify_conf_dns()
{
   DETECT=$1
   ADDRESS="${DETECT} ${2}"

   # sed -i "s/\($DETECT\).*/$ADDRESS/" $3

   echo "nameserver $2" > $3
}

function _modify_conf_address()
{
   DETECT=$1
   ADDRESS="${2}"

   sed -i "s/\($DETECT\).*/$ADDRESS/" $3
}

#IPPROBE="250.250.250.250"
#ETHPROBE="eth1"
#GATEWAY="192.168.1.2"

# Set of functions to create the container
#_create_container $name $extip $dnsip $image

#$name $extip $dnsip $image $bridge $crontab
function _create_container()
{
   #NODE=$1
   #EXTIP=$2
   #DNSIP=$3
   #IMAGE=$4
   #BRIDGE=$5
   #CRONTAB=$6
   IMG="$DIRECTORY/$image"
   #NEWIMG=$(echo $IMG | sed s/maas/tar.gz/)
   #NEWIMAGE=$(echo $image | sed s/maas/tar.gz/)
   echo "->>image $IMG"
   if [ ! -f "$IMG" ]
   then
      echo "This image des not exist!!"
      WGETURL="$URL/maasapi/templates/$image" 
      echo "$WGETURL > $IMG"
      curl $WGETURL > $IMG
      mv $IMG $NEWIMG

      if [ ! -z "$VIRT_DOCKER" ]; then
         echo "zcat $NEWIMG | docker load"
         zcat $IMG | docker load
      fi

      #IMG=$(echo $IMG | sed s/maas/tar.gz/)
   fi
   #echo "--> $crontab"
   #echo "$crontab" > /root/crontab
   #The_creation $NODE $EXTIP $DNSIP $NEWIMG $NEWIMAGE $BRIDGE $CRONTAB result
   if [ ! -z "$VIRT_LXC" ]; then
      The_creation $NEWIMG $NEWIMAGE result
   fi
   if [ ! -z "$VIRT_DOCKER" ]; then
      echo "lolo"
      The_creation_docker $NEWIMG $NEWIMAGE result
   fi

   #echo $NEWIMG
   #echo $NEWIMAGE
}


function _create_container_docker()
{
   docker run -d -v /tmp:/tmp $image
}

function __create_container()
{
   NODE=$1
   infoip=$2
   infodns=$3
   #IPname=${infoip//./_}
   #IPdns=${infodns//./_}
   #Host=${HOSTNAME}
   #NODE=$Host"_"$IPname"_"$IPdns
   #NODE=$Host":"$IPname":"$IPdns
   #echo "->> $NODE"
   #CONTAINER="template"
   DIRECTORY="/var/lib/lxc/$NODE"
   RESOLV=$DIRECTORY"/rootfs/etc/resolv.conf"
   NETWORK=$DIRECTORY"/rootfs/etc/network/interfaces"

   echo "Stop the master container $CONTAINER"
   _stop_container $CONTAINER 


   echo "Clone the container $CONTAINER"
   _clone_container $CONTAINER $NODE result

   #if [ "$result" == "true" ]
   if [ "$result" == "inutil" ]
   then
      echo "Modify $RESOLV from $NODE"
      _modify_conf_dns "nameserver" $3 $RESOLV

      echo "Modify $NETWORK from $NODE"
      _modify_conf_address $IPPROBE $2 $NETWORK

      echo "" >> $NETWORK
      echo "# Static route" >> $NETWORK
      echo "post-up ip route del 0/0" >> $NETWORK
      echo "post-up ip route add default via $GATEWAY src $2 dev $ETHPROBE" >> $NETWORK

      sleep 2

      echo "Start $NODE"
      #_start_container $NODE
   fi

   echo "Start $CONTAINER"
   _start_container $CONTAINER 

}

# Read the file including ip and dns addresses
function _read_file()
{
   while read line 
   do 
      export name=$(echo "$line"     | cut -d";" -f2)
      export image=$(echo "$line"    | cut -d";" -f3)
      export archi=$(echo "$line"    | cut -d";" -f4)
      export dnsip=$(echo "$line"    | cut -d";" -f8)
      export extip=$(echo "$line"    | cut -d";" -f9)
      export bridge=$(echo "$line"   | cut -d";" -f11)
      export crontab=$(echo "$line"  | cut -d";" -f12)
      export command=$(echo "$line"  | cut -d";" -f15)
      export iso=$(echo "$line"  | cut -d";" -f14)

      export VIRT_LXC=$(dpkg -l| grep -i lxc)
      export VIRT_DOCKER=$(dpkg -l| grep -i docker)
      #export $crontab
      #echo "$crontab" > /root/crontab
      #echo "Container $name $extip $dnsip $image $bridge $crontab"
      #_create_container $name $extip $dnsip $image $bridge $crontab
      
      _create_container
      # if [ -z "$VIRT_LXC" ]; then
         # _create_container
      # fi
   done < $1
}

# Read dbase
function _search_new_container()
{
   NAME=$HOSTNAME
   ACTION="add"
   directory=$1
   :> ${directory}

   data="name=$NAME&action=$ACTION"
   #curl --data $data --proxy http://http-proxy.rd.francetelecom.fr:3128 http://p-vite2.riv.rd.francetelecom.fr/newapi/getcontainers.php > ${directory}
   echo "curl --data $data $URL/maasapi/getcontainers.php"
   curl --data $data $URL/maasapi/getcontainers.php > ${directory}

   _read_file $1
}

# Test arguments

_search_new_container $FILE

exit 0
