#
import time
import telepot
import MySQLdb
import RPi.GPIO as GPIO
import picamera

from telepot.loop import MessageLoop
from picamera import PiCamera
from time import sleep
from PIL import Image

camera = picamera.PiCamera()

location = "/home/pi/project/captured/%s_video.h264"

#telegram chat id
chat_id = ********
#telegram bot token 
telegramToken = 'enter your token here'

#file naming
def remove(string): 
    return "".join(string.split())

camera.framerate = 20
def captureSendCopy():
    
    #get system time
    localtime = str(time.asctime(time.localtime(time.time())))
    fileName = remove(localtime)
    
    #capture video
    print ("recording video for 20")
    camera.start_recording(location % fileName)
    camera.wait_recording(20)
    camera.stop_recording()

    #upload video to telegram
    print ("connecting to telegram")
    telegram_bot = telepot.Bot (telegramToken)
    print (telegram_bot.getMe())
    print ("uploading to telegram")
    telegram_bot.sendDocument (chat_id, document=open(location % fileName))
    print ("uploaded to telegram")
    print ("verify alert on telegram")

def sqlReaderWriter():
    #connect to MariaDB
    db = MySQLdb.connect(host="localhost", user="alpha", passwd="alpha", db="Project")
    cur = db.cursor()
    print ("connection:")
    print(cur)
    sql1 = "SELECT * FROM ActionTable WHERE `Action ID` > 6005 AND `Action ID` < 8000"
    sql2 = "UPDATE ActionTable SET `Action Value` = 1 WHERE `Action ID` = 8001"
    sql3 = "UPDATE ActionTable SET `Action Value` = 0 WHERE `Action ID` = 8001"
    sql4 = "UPDATE ActionTable SET `Action Value` = 0 WHERE `Action ID` = "
    cur.execute (sql1)
    for row in cur.fetchall():
        print (str(row[0])+" "+str(row[1])+" "+str(row[2])+" "+str(row[3]))
        #check if action value is 1 or 0
        if row[2] == 1:
            print ("motion detected in: " + str(row[1]) + " at " + str(row[3]))
            cur.execute (sql2)
            db.commit()
            captureSendCopy()
            cur.execute (sql3)
            sql4 = sql4 + str(row[0])
            cur.execute (sql4)
            db.commit()
try:
    counter = 1
    
    #program runs 10000 times to reduce system load, program is restarted through shell script
    while(counter != 10000):
        sqlReaderWriter()
        counter += 1
except KeyboardInterrupt:
    GPIO.cleanup()
 
