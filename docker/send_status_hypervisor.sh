#!/bin/bash

# Send status information to mothership
function send_data()
{
   heure=$(date +%H:%M:%S)
   jour=$(date +%Y-%m-%d)

   memory_usage=$(free -m | awk 'NR==2{printf "Memory Usage: %s/%sMB (%.2f%%)\n", $3,$2,$3*100/$2 }')
   disk_usage=$(df -h | awk '$NF=="/"{printf "Disk Usage: %d/%dGB (%s)\n", $3,$2,$5}')
   cpu=$(top -bn1 | grep load | awk '{printf "CPU Load: %.2f\n", $(NF-2)}')

   data="{\"heure\":\"$heure\",\"jour\":\"$jour\",\"memory_usage\":\"$memory_usage\",\"disk_usage\":\"$disk_usage\",\"cpu\":\"$cpu\"}"
   curl -k -H "Content-Type: application/json" -X POST -d $data https://127.0.0.1/docker/status.php
}

# get data of new containers
function get_data()
{
}

