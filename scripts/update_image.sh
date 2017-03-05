#!/bin/bash

DIR_IMAGE="/var/www/html/p-trolette.orange-labs.fr/newapi/template/"
DIR_DRUPAL="/var/www/html/p-trolette.orange-labs.fr/cdpweather/sites/default/files/"

function _read_file()
{
   rm -rf $DIR_IMAGE/*

   exec 3<> $1

   while read line <&3
   do {
      ln -s $DIR_DRUPAL/$line $DIR_IMAGE/$line
   }
done
exec 3>&-
}

function _search_new_images()
{
   directory=$1

   curl --proxy http://http-proxy.rd.francetelecom.fr:3128 http://p-vite2.riv.rd.francetelecom.fr/newapi/getimages.php > ${directory}

   _read_file $directory
}

_search_new_images $FILE

exit 0
