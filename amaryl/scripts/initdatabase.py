#!/usr/bin/env python
# -*- coding: utf-8 -*-

import mysql.connector
import time
import datetime

conn = mysql.connector.connect(host="localhost",user="spike",password="valentine", database="drupal")
cann = mysql.connector.connect(host="localhost",user="spike",password="valentine", database="content_delivery_weather")
cursor = conn.cursor()
cursar = cann.cursor()


cursor.execute("""SELECT uid, mail FROM users""")
rows = cursor.fetchall()

for row in rows:
   if row[0] != 0:
      print('{0} : {1} '.format(row[0], row[1]))
      #print('UPDATE new_v4_users_probes_edit SET email = {0}  WHERE uid = {1}'.format(row[1], row[0]))
      cursar.execute("""UPDATE new_v4_users_probes_edit SET email = %s  WHERE userid = %s""",(row[1], row[0]))

cursar.execute("""SELECT probename, probeid FROM new_v4_sonde""")
rows = cursar.fetchall()

for row in rows:
   cursar.execute("""SHOW TABLES LIKE %s""",("%" + row[0] + "%",))
   rowsbis = cursar.fetchall()
   for rowbis in rowsbis:
      result = rowbis[0].split("_")
      month = 1 + int(result[4])
      s = "01/" + str(month) + "/" + result[3]
      timestamp = time.mktime(datetime.datetime.strptime(s, "%d/%m/%Y").timetuple())
      print('{0} : {1} year: {2} month: {3} timestamp: {4}'.format(row[0], rowbis[0], result[3], result[4], round(timestamp,0)))
      cursar.execute("""SELECT firsttime FROM new_v4_sonde WHERE probeid = %s""",(row[1],))
      rowsbisbis = cursar.fetchall()
      for rowbisbis in rowsbisbis:
         if rowbisbis[0] == None:
            cursar.execute("""UPDATE new_v4_sonde SET firsttime = %s  WHERE probeid = %s""",(timestamp,row[1]))
            print('firsttime: {0}'.format(rowbisbis[0],))


conn.close()
cann.close()
