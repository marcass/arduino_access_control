# todo
# - expire burners after use (done) and after time period
# - ensure telegram bot can instatiate class User
# - pickle instances


# USAGE:
# INSTALL: sudo pip install flask
# START API with:
#    FLASK_APP=keyserver.py flask run

# In another terminal:
# curl -X GET http://127.0.0.1:5000/list

# curl -X POST -H "Content-Type: application/json" -d '{"pincode":"000"}' http://127.0.0.1:5000/usekey
# Response: {"pin_correct": False}

# curl -X POST -H "Content-Type: application/json" -d '{"pincode":"0000"}' http://127.0.0.1:5000/usekey
# Response:  {"pin_correct": True}

# curl -X POST -H "Content-Type: application/json" -d '{"pincode":"0000"}' http://127.0.0.1:5000/usekey
# Response:  {"pin_correct": False} # NOW FALSE because it was a burn code!

# curl -X POST -H "Content-Type: application/json" -d '{"pincode":"00003","burnkey":"True"}' http://127.0.0.1:5000/addkey
# Response:  {  "Status": "Added key" }

#curl -X POST -H "Content-Type: application/json" -d '{"pincode":"00003"}' http://127.0.0.1:5000/usekey
# Response:  {  "pin_correct": true}

import users
import sql
from flask import Flask, request, jsonify

#ALWAYS_ALLOWED_KEYS_LIST = users.ALWAYS_ALLOWED_KEYS_LIST
#BURN_KEYS_LIST = users.BURN_KEYS_LIST

def get_allowed_keys():
    global d
    d = sql.build_user_dict_query()
    print d
    #{'username': [u'marcass'], 'name': [u'Marcus Wells'], 'keycode_en': [1], 'mqtt_id': [u'mqttdash-mi4c'], 'mqtt_id_en': [1], 'burner': [0], 'createtime': [datetime.datetime(2017, 9, 9, 0, 19, 4, 703059, tzinfo=<DstTzInfo 'Pacific/Auckland' NZST+12:00:00 STD>)], 'enabled': [1], 'keycode': [u'1234A']}
    keys_buy_user = {a : d['username'] for a in d['keycode']}
    print keys_by_user
    
    #add rfid if works
    
    #build list of allowed users
    user_list = []
    for i in d['enabled']:
        if i == 1:
            user_list.append(i['user_name'])
            #test kecode enabled
        
    print user_list
    allowed_keys_list = []
    
    #global ALWAYS_ALLOWED_KEYS_LIST, BURN_KEYS_LIST
    #return ALWAYS_ALLOWED_KEYS_LIST+BURN_KEYS_LIST
    return d

def use_key(key):
    global d
    global ALWAYS_ALLOWED_KEYS_LIST, BURN_KEYS_LIST
    if key in ALWAYS_ALLOWED_KEYS_LIST:
        return True
    elif key in BURN_KEYS_LIST:
        BURN_KEYS_LIST.remove(key)
        return True
    return False

def add_to_keys(key, typeburnkey=True):
    global ALWAYS_ALLOWED_KEYS_LIST, BURN_KEYS_LIST
    if typeburnkey:
        BURN_KEYS_LIST.append(key)
    else:
        ALWAYS_ALLOWED_KEYS_LIST.append(key)

app = Flask(__name__)

@app.route("/")
def hello():
    return "Hello World!"

@app.route("/list", methods=['GET',])
def list_keys():
    resp = get_allowed_keys()
    return jsonify(resp), 200

@app.route("/usekey", methods=['POST',])
def usekey():
  content = request.get_json(silent=False)
  pin = content['pincode']
  if use_key(pin):
    resp = {'pin_correct':True}
  else:
    resp = {'pin_correct':False}
  return jsonify(resp), 200

@app.route("/addkey", methods=['POST',])
def addkey():
  content = request.get_json(silent=False)
  key = content['pincode']
  if content.has_key('burnkey'):
    add_to_keys(key, typeburnkey=content['burnkey'])
  else:
    add_to_keys(key, typeburnkey=True)
  resp = {'Status':'Added key'}
  return jsonify(resp), 200
