# Telegram messaging
import telepot
import telepot.helper
from telepot.loop import MessageLoop
from telepot.namedtuple import InlineKeyboardMarkup, InlineKeyboardButton
from telepot.delegate import (
    per_chat_id, create_open, pave_event_space, include_callback_query_chat_id)
import telepot.api
import creds

TOKEN = creds.botAPIKey
botID = creds.bot_ID

# ########### Alert stuff ########################
# #fix for protocol error message ( see https://github.com/nickoala/telepot/issues/242 )
def always_use_new(req, **user_kw):
    return None

telepot.api._which_pool = always_use_new

def send_alert(text):
    bot.sendMessage(botID, text)

def on_chat_message(msg, payload):
    content_type, chat_type, chat_id = telepot.glance(msg)
    try:
        text = msg['text']
        help_text = "This bot will alert you to boiler malfunctions. Any message you send will be replied to by the bot. If it is not formatted correctly you will get this message again. Sending the following will give you a result:\n'/status' to get the status of the boiler. Type '/turn on' to turn boiler on, or '/turn off' to turn it off"
        if ('/status' in text) or ('/Status' in text):
            message = "State: "+payload['State']+"\r\nWater temp: "+str(payload['Water temp'])+"\r\nAuger temp: "+str(payload['Auger temp'])+"\r\nSetpoint: "+str(payload['Setpoint'])
            # print message
            send_alert(message)
        else:
            message = bot.sendMessage(chat_id, "I'm sorry, I don't recongnise that request (=bugger off, that does nothing). " +help_text)
    except KeyError:
        bot.sendMessage(chat_id, "There's been a cock-up. Please let Marcus know what you just did (if it wasn't adding somebody to the chat group)")

def on_callback_query(msg):
    query_id, from_id, query_data = telepot.glance(msg, flavor='callback_query')
    print('Callback Query:', query_id, from_id, query_data)
    #print msg
    target_id = msg['message']['chat']['id']
    if query_data == 'turn off':
        print 'turn off'
    if query_data == 'turn on':
        print 'turn on'

#TOKEN = creds.testbotAPIKey
bot = telepot.Bot(TOKEN)

# #start the message bot
# MessageLoop(bot, {'chat': on_chat_message, 'callback_query': on_callback_query}).run_as_thread()
# print('Listening ...')
