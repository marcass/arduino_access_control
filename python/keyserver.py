# todo
#- front end https://auth0.com/blog/vuejs2-authentication-tutorial/

# USAGE:
# INSTALL: sudo pip install flask
# START API with:
#    FLASK_APP=keyserver.py flask run

# In another terminal:
# curl -X GET http://127.0.0.1:5000/listall

# curl -X GET http://127.0.0.1:5000/listallowed

# curl -X GET http://127.0.0.1:5000/getstatus

# curl -X POST -H "Content-Type: application/json" -d '{"door":"topgarage", "pincode":"000"}' http://127.0.0.1:5000/usekey
# Response: {"pin_correct": False}

# curl -X POST -H "Content-Type: application/json" -d '{"door":"topgarage", "pincode":"1111"}' http://127.0.0.1:5000/usekey
# Response:  {"pin_correct": True}

# curl -X POST -H "Content-Type: application/json" -d '{"door":"topgarage", "pincode":"1111"}' http://127.0.0.1:5000/usekey
# Response:  {"pin_correct": False} # NOW FALSE because it was a burn code!

# curl -X POST -H "Content-Type: application/json" -d '{"username": "max", "keycode": "AAB23", "doorlist":["topgarage",  "frontdoor", "bottomgarage"], "enabled":"1"}' http://127.0.0.1:5000/addkey
# Response:  {  "Status": "Added key" }

# curl -X POST -H "Content-Type: application/json" -d '{"username": "burner", "keycode": "1111", "doorlist":["topgarage",  "frontdoor", "bottomgarage"], "enabled":"1"}' http://127.0.0.1:5000/addkey
# Response:  {  "Status": "Added key" }

#allowed values are: topgarage, frontdoor, bottomgarage
#curl -X POST -H "Content-Type: application/json" -d '{"door":"topgarage", "pincode":"00003"}' http://127.0.0.1:5000/usekey
# Response:  {  "pin_correct": true}

#curl -X PUT -H "Content-Type: application/json" -d '{"door":"topgarage","status":"open"}' http://127.0.0.1:5000/doorstatus
# Response:  { "door":"top garage", "status": "open"}

#curl -X GET -H "Content-Type: application/json" -d '{"days":"10"}' http://127.0.0.1:5000/getlog
# Response:  {  log stuff in here }

import re
import sql
from flask import Flask, request, jsonify

def get_allowed_users():
    d = sql.build_allowed_users()
    return d

def get_all_keys():
    d = sql.build_all_keys()
    return d

def use_key(key, door):
    d = sql.validate_key(key, door)
    if d is None:
        x = sql.insert_actionLog('Pinpad', door, key)
        print x
        return False
    else:       
        if d == 'burner':
            print 'user tested true for burner'
            conn, c = sql.get_db()
            #remove burnkey row
            c.execute("DELETE FROM canOpen WHERE userallowed=?", ('burner',))
            #c.execute("DELETE FROM canOpen WHERE userallowed=burner")
            conn.commit()
        print 'username = '+str(d)+' for '+door
        y = sql.insert_actionLog('Pinpad', door, key, d)
        print y
        return True        

def add_user_to_db(username, keycode, enabled, doorlist, timeStart, timeEnd):
    sql.setup_db()
    d = sql.build_user_dict_query()
    print d
    if (keycode in d['keycode']) or username in d['username']:
        print 'failed to add user as exists'
        return False
    else:
        print 'moving on down'
        print doorlist
        sql.add_user(username, keycode, doorlist, enabled, timeStart, timeEnd)
        return True

def get_access_log(days):
    d = sql.get_doorlog(days)
    return d
    
        

sql.setup_db()
app = Flask(__name__)

@app.route("/")
def hello():
    return "Hello World!"

@app.route("/getstatus", methods=['GET',])
def getStatus():
    conn, c = sql.get_db()
    c.execute("SELECT * FROM doorStates GROUP BY door")
    #c.execute("SELECT * FROM doorStates")
    resp = c.fetchall()
    return jsonify(resp), 200

@app.route("/listallowed", methods=['GET',])
def list_allowed_keys():
    resp = get_allowed_users()
    return jsonify(resp), 200

@app.route("/listall", methods=['GET',])
def list_all_keys():
    resp = get_all_keys()
    return jsonify(resp), 200

@app.route("/usekey", methods=['POST',])
def usekey():
    content = request.get_json(silent=False)
    door = content['door']
    pin = content['pincode']
    if use_key(pin, door):
        resp = {'pin_correct':True}
    else:
        resp = {'pin_correct':False}
    return jsonify(resp), 200

@app.route("/addkey", methods=['POST',])
def addkey():
    content = request.get_json(silent=False)
    #{"username":pell", "keycode":"00003", "doorlist":{"topgarage":"1",  "frontdoor":"1", "bottomgarage":"1"}, "enabled":"1"}
    timeStart = None
    timeEnd = None
    if content.has_key('timeStart'):
        timeStart = content['timeStart'] # parse this to datetime
    if content.has_key('timeEnd'):
        timeEnd = content['timeEnd'] # parse this to datetime
    if (len(content['keycode']) > 3) and (len(content['keycode']) < 11) and (re.match("^[A-D1-9]+$", content['keycode'])):
        #print content["doorlist"]
        ret = add_user_to_db(content['username'], content['keycode'], int(content['enabled']), content["doorlist"], timeStart, timeEnd)
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
    door = content['door']
    status = content["status"]
    sql.update_doorstatus(status, door)
    resp = {"door":door,"status":status}
    return jsonify(resp), 200

@app.route("/getlog", methods=['GET',])
def getAccessLog():
    content = request.get_json(silent=False)
    days = content['days']
    resp = get_access_log(days)
    return jsonify(resp), 200
    
