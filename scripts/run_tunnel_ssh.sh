#!/bin/bash

for i in $(grep "^Host " $HOME/.ssh/config | cut -d" " -f2)
do
   RUNNING=$(ps -aux | grep autossh | grep $i)
   if [ -z "$RUNNING" ];
   then
      echo "autossh -M 0 -f -T -N $i"
      autossh -M 0 -f -T -N $i
   fi
done
