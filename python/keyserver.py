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

# curl -X POST -H "Content-Type: application/json" -d '{"pincode":"1111"}' http://127.0.0.1:5000/usekey
# Response:  {"pin_correct": True}

# curl -X POST -H "Content-Type: application/json" -d '{"pincode":"1111"}' http://127.0.0.1:5000/usekey
# Response:  {"pin_correct": False} # NOW FALSE because it was a burn code!

# curl -X POST -H "Content-Type: application/json" -d '{"username": "max", "keycode": "AAB23", "enabled":"1"}' http://127.0.0.1:5000/addkey
# Response:  {  "Status": "Added key" }

# curl -X POST -H "Content-Type: application/json" -d '{"username": "burner", "keycode": "1111", "enabled":"1"}' http://127.0.0.1:5000/addkey
# Response:  {  "Status": "Added key" }

#curl -X POST -H "Content-Type: application/json" -d '{"pincode":"00003"}' http://127.0.0.1:5000/usekey
# Response:  {  "pin_correct": true}

#curl -X PUT -H "Content-Type: application/json" -d '{"status":"open"}' http://127.0.0.1:5000/doorstatus
# Response:  {  "doorstatus": "open"}


import re
import sql
from flask import Flask, request, jsonify

def get_allowed_keys():
    d = sql.build_allowed_keys()
    return d

def use_key(key):
    d = sql.build_allowed_keys()
    if key in d['valid_keys']:
        if key in d['burner']:
            conn, c = sql.get_db()
            #remove burnkey row
            c.execute("UPDATE doorUsers SET enabled=? WHERE username=?", (0, 'burner'))
            conn.commit()
        print 'username = '+d['key_name'][key]
        sql.insert_actionLog('Pinpad', key, d['key_name'][key])
        return True
    else:
        sql.insert_actionLog('Pinpad', key)
        return False

def add_user_to_db(username, keycode, enabled, timeStart, timeEnd):
    sql.setup_db()
    d = sql.build_user_dict_query()
    if (keycode in d['keycode']) or username in d['username']:
        return False
    else:
        sql.add_user(username, keycode, enabled, timeStart, timeEnd)
        return True


def add_to_keys(key, typeburnkey=True):
    global ALWAYS_ALLOWED_KEYS_LIST, BURN_KEYS_LIST
    if typeburnkey:
        BURN_KEYS_LIST.append(key)
    else:
        ALWAYS_ALLOWED_KEYS_LIST.append(key)
        

sql.setup_db()
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
    #{"name": "pellets", "username":pell", "keycode":"00003","keycode_en": "1", "mqtt_id": "pell-guy", "mqtt_id_en":"0", "burner":"1", "enabled":"1"}
    timeStart = None
    timeEnd = None
    if content.has_key('timeStart'):
        timeStart = content['timeStart'] # parse this to datetime
    if content.has_key('timeEnd'):
        timeEnd = content['timeEnd'] # parse this to datetime
    print content['keycode']
    print len(content['keycode'])
    if (len(content['keycode']) > 3) and (len(content['keycode']) < 11) and (re.match("^[A-D1-9]+$", content['keycode'])):
        ret = add_user_to_db(content['username'], content['keycode'], int(content['enabled']), timeStart, timeEnd)
        if ret:
            resp = {'Status':'Added key for {}'.format(content['username'])}
        else:
            resp = {'Status':'Did not at key for {}. Either username or pin already in use'.format(content['username'])}
            
    else:
        resp = {'Status':'Did not at key for {}. Pin must use numbers 1-9 and letters A-D and be at between 4 and 10 alphanumeric characters long'.format(content['username'])}
    return jsonify(resp), 200

@app.route("/doorstatus", methods=['PUT',])
def update_status():
    content = request.get_json(silent=False)
    status = content["status"]
    sql.update_doorstatus(status)
    resp = {"doorstatus":status}
    return jsonify(resp), 200
    
