#!/usr/bin/env python


import mysql.connector 
import smtplib
import datetime
import time

from email.mime.text import MIMEText

############# Define global variable #######################

conn = mysql.connector.connect(host="localhost",\
      user="spike",\
      password="valentine",\
      database="content_delivery_weather")

cursor = conn.cursor()

############################################################

def sendmail(to, subject, text):
   username = 'spynet.rd.francetelecom.fr@orange.com'
   #password = 'XXXXX'

   msg = MIMEText(text)
   msg['Subject'] = '%s' % subject
   msg['From'] = username
   msg['To'] = '%s' % to
   #msg['To'] = 'XXXXXX@XXXXXX'

   #server = smtplib.SMTP('smtp.gmail.com:587')
   server = smtplib.SMTP('mailhost:2525')
   #server.starttls()
   #server.login(username,password)
   server.sendmail(username, [to], msg.as_string())
   server.quit()

def warm(probeid):
   print("warm probeid " + str(probeid))
   cursor.execute("""SELECT e.email, s.probename FROM new_v4_users_probes_edit e INNER JOIN new_v4_sonde s ON e.probeid = s.probeid WHERE s.probeid = %s AND ref = %s""",(probeid, 1))
   rows = cursor.fetchall()

   for row in rows:
      #print('{0} to warm from {1} probe'.format(row[0],row[1]))
      if row[0] != None:
          text = 'It seems that %s doesn\'t work' % row[1]
          sendmail(row[0], '%s disabled' % row[1], text)      

def status(probeid):
   cursor.execute("""SELECT status FROM new_v4_sonde WHERE probeid = %s""", (probeid,))
   rows = cursor.fetchall()

   for row in rows:
      if row[0] == 2:
         # We disable the probe
         cursor.execute("""UPDATE new_v4_sonde SET status = %s WHERE probeid = %s""", (3, probeid))
      if row[0] == 0:
         # The  probe was received
         cursor.execute("""UPDATE new_v4_sonde SET status = %s WHERE probeid = %s""", (1, probeid))
      if row[0] in [1,3]:
          # We enable the probe
         cursor.execute("""UPDATE new_v4_sonde SET status = %s WHERE probeid = %s""", (2, probeid))

def update():
   global cursor
   delay = 86400

   now = datetime.datetime.now()
   timestamp = time.mktime(now.timetuple())
   dt = datetime.datetime.fromtimestamp(timestamp)

   cursor.execute("""SELECT probeid, probename, uptime, lasttime, status FROM new_v4_sonde""")
   rows = cursor.fetchall()

   for row in rows:
       result = timestamp - row[3]
       #print('{0} - {1} = {2}'.format(timestamp, row[3], result))

       if result > delay and row[4] == 2:
          status(row[0])
          warm(row[0])

       if result < delay and row[4] in [0,1,3]:
          status(row[0])

update()
conn.close()
