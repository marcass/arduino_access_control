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


from flask import Flask, request, jsonify

class User:
    global ALWAYS_ALLOWED_KEYS_LIST, BURN_KEYS_LIST, USER_LIST, username_list, mqtt_id_list, username_by_keycode
    def __init__(self, name, username, keycode, mqtt_id, burner):
        self.name = name
        self.username = username
        if self.username in username_list:
            raise exception(self.username)
        self.keycode = keycode
        if (self.keycode in ALWAYS_ALLOWED_KEYS_LIST) or(self.keycode in BURN_KEYS_LIST) :
            raise exception(self.keycode)        
        self.mqtt_id = mqtt_id
        if self.mqtt_id in mqtt_id_list:
            raise exception(self.mqtt_id)
        self.burner = burner
    
    USER_LIST.append(self)
        
    if burner = True:
        BURN_KEYS_LIST.append(self.keycode)
        
    if burner = False:
        ALWAYS_ALLOWED_KEYS_LIST.append(self.keycode)
        
    def exception(item):
        print item+' is not unique. Please try again'

ALWAYS_ALLOWED_KEYS_LIST = ['1234','5123']
BURN_KEYS_LIST = ['0000',]
USER_LIST = []
username_list = [i.name for i in USER_LIST]
mqtt_id_list = [i.mqtt_id for i in USER_LIST]
username_by_keycode = {i.keycode : i.username for in USER_LIST}

m = User('Marcus Wells', 'marcass', '1234A', 'mqttdash-mi4c', False)

def get_allowed_keys():
  global ALWAYS_ALLOWED_KEYS_LIST, BURN_KEYS_LIST
    # query db here and return allowed keys
  return ALWAYS_ALLOWED_KEYS_LIST+BURN_KEYS_LIST

def use_key(key):
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
