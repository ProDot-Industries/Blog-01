import time, datetime
import telepot
from telepot.loop import MessageLoop

now = datetime.datetime.now()
token = 'enter your token here'
def action(msg):
    chat_id = msg['chat']['id']
    command = msg['text']
    print chat_id

    if command == '/hi':
        telegram_bot.sendMessage (chat_id, str("Hi from ProDot Industries"))

telegram_bot = telepot.Bot(token)
print (telegram_bot.getMe())
MessageLoop(telegram_bot, action).run_as_thread()
while 1:
    time.sleep(10)
    print 'Up and Running....'