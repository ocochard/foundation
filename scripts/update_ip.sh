#!/bin/bash

INTIP=$(ip addr show dev eth0 | grep "inet " | grep -v "eth0:1" | cut -d " " -f6 | cut -d "/" -f1)
#ipinterface=$(ip addr show dev eth0 | grep "inet " | grep "eth0:1" | cut -d " " -f6 | cut -d "/" -f1)
#ipdns=$(cat /etc/resolv.conf | cut -d " " -f2)

NAME="$HOSTNAME"

# Read dbase
function _update_IP()
{
   
   data="containername=$NAME&intip=$INTIP"
   #echo $query
   curl --proxy http://http-proxy.rd.francetelecom.fr:3128 --data $data http://p-vite2.riv.rd.francetelecom.fr/newapi/updatecontainer.php
}

# Run process

sleep 60
_update_IP
