#!/bin/bash

cd $HOME/.ssh

for i in `cat config | grep "^Host " | cut -d" " -f2`
do
   RUNNING=$(ps -aux | grep autossh | grep $i)
   if [ -z "$RUNNING" ];
   then
      echo "autossh -M 0 -f -T -N $i"
      autossh -M 0 -f -T -N $i
   fi
done
