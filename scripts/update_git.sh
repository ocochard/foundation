#!/bin/bash

DIRECTORY="/opt/foundation"
CMD="git pull"

# Test for all hypervisors
if [ -d "$DIRECTORY" ]
then
   cd $DIRECTORY
   git pull
fi

# Test for the MaaS server
DIRECTORY="/var/www/html/cdpweather/sites/all/modules/foundation"

if [ -d "$DIRECTORY" ]
then
   cd $DIRECTORY
   git pull
fi

