#!/bin/bash

FILE="/home/spike/route.txt"

# Init table
function _init_table()
{
   # Flush /sbin/iptables
   /sbin/iptables -F 2>/dev/null

   # Suppression des chaînes utilisateurs 
   /sbin/iptables -X 2>/dev/null

   # Interdire toutes connexions entrantes et sortantes
   /sbin/iptables -t filter -P INPUT DROP
   /sbin/iptables -t filter -P FORWARD DROP
   /sbin/iptables -t filter -P OUTPUT DROP

   ## Permettre à une connexion ouverte de recevoir du trafic en entrée.
   /sbin/iptables -A INPUT -m state --state ESTABLISHED,RELATED -j ACCEPT

   ## Permettre à une connexion ouverte de recevoir du trafic en sortie.
   /sbin/iptables -A OUTPUT -m state ! --state INVALID -j ACCEPT

   # On accepte la boucle locale en entrée.
   /sbin/iptables -A INPUT -i lo -j ACCEPT
}

# Create route with iptables
function _create_route()
{
   #iptables -t nat -A POSTROUTING -o eth0 -s $1 -j SNAT --to-source $2
   #iptables -t nat -A PREROUTING -i eth0 -d $2 -j DNAT --to-destination $1
   #iptables -A FORWARD -s $2 -j ACCEPT
   #iptables -A FORWARD -d $1 -j ACCEPT
   ip route add $1/32 via $2
}

# Read the file including ip and dns addresses
function _read_file()
{
   #_init_table
   # Delete old static route
   _del_static_route

   exec 3<> $1
   while read line <&3
   do {
      extip=$(echo $line | cut -d" " -f4)
      intip=$(echo $line | cut -d" " -f5)
      echo "mapping $extip $intip"
      _create_route $extip $intip
   }
done
exec 3>&-
}

# Delete static routes
function _del_static_route()
{
   for i in `ip route show | grep via | grep -v "default" | awk '{print $1;}'`
   do 
      ip route del $i;
   done
}

# Read the database
function _search_new_routes()
{
   #assign user, password etc..
   #username="spike"
   #password="valentine"
   #hostname="192.168.2.12"
   #database="hyperdb"
   directory=$1

   # mysql -N -h${hostname} -D${database} -u${username} -p${password}  <<< 'select extip, intip from container where status = 1' >  ${directory}
   curl --proxy http://http-proxy.rd.francetelecom.fr:3128 --data $data http://p-vite2.riv.rd.francetelecom.fr/newapi/getcontainers.php > ${directory}

   _read_file $directory
}


sleep 60
_search_new_routes $FILE

exit 0
