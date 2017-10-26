# todo
#- front end https://auth0.com/blog/vuejs2-authentication-tutorial/

# USAGE:
# INSTALL: sudo pip install flask flask-cors pyopenssl
# START REST API with:
#    FLASK_APP=keyserver.py flask run
# START REST and WebSockets with:
#    gunicorn -k flask_sockets.worker keyserver:app

# In another terminal:
# curl -X GET http://127.0.0.1:5000/users

# curl -X GET http://127.0.0.1:5000/listallowed

# curl -X GET http://127.0.0.1:5000/door/status

# curl -X POST -H "Content-Type: application/json" -d '{"door":"topgarage", "pincode":"000"}' http://127.0.0.1:5000/usekey
# Response: {"pin_correct": False}

# curl -X POST -H "Content-Type: application/json" -d '{"door":"topgarage", "pincode":"1111"}' http://127.0.0.1:5000/usekey
# Response:  {"pin_correct": True}

# curl -X POST -H "Content-Type: application/json" -d '{"door":"topgarage", "pincode":"1111"}' http://127.0.0.1:5000/usekey
# Response:  {"pin_correct": False} # NOW FALSE because it was a burn code!

# curl -X POST -H "Content-Type: application/json" -d '{"username": "max", "keycode": "AAB23", "doorlist":["topgarage", "frontdoor", "bottomgarage"], "enabled":"1"}' http://127.0.0.1:5000/user
# Response:  {  "Status": "Added key" }

# curl -X POST -H "Content-Type: application/json" -d '{"username": "mw", "keycode": "1111", "doorlist":["topgarage", "frontdoor"], "enabled":"1"}' http://127.0.0.1:5000/user

# curl -X POST -H "Content-Type: application/json" -d '{"username": "burner", "keycode": "1111", "doorlist":["topgarage"], "enabled":"1"}' http://127.0.0.1:5000/user

# curl -X POST -H "Content-Type: application/json" -d '{"username": "burner", "keycode": "1111", "doorlist":["topgarage",  "frontdoor", "bottomgarage"], "enabled":"1"}' http://127.0.0.1:5000/user
# Response:  {  "Status": "Added key" }

#allowed values are: topgarage, frontdoor, bottomgarage
#curl -X POST -H "Content-Type: application/json" -d '{"door":"topgarage", "pincode":"00003"}' http://127.0.0.1:5000/usekey
# Response:  {  "pin_correct": true}

#curl -X PUT -H "Content-Type: application/json" -d '{"door":"topgarage","status":"opened"}' http://127.0.0.1:5000/door/status
# Response:  { "door":"top garage", "status": "opened"}

#curl -X GET -H "Content-Type: application/json" -d '{"days":"10"}' http://127.0.0.1:5000/getlog
# Response:  {  log stuff in here }

# curl -X DELETE -H "Content-Type: application/json" -d '{"username":"mw"}' http://127.0.0.1:5000/user

# curl -X GET -H "Content-Type: application/json" -d '{"username":"max"}' http://127.0.0.1:5000/user

# curl -X POST -H "Content-Type: application/json" -d '{"username":"admin", "password":"password"}' http://127.0.0.1:5000/auth
#response = {
#  "access_token": "eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJqdGkiOiIyMGU4OTg2NS0wNjM5LTQ3ZDEtYWU0YS1hYTg4ODQxNDIwNjciLCJleHAiOjE1MDg0NTU2NDgsImZyZXNoIjpmYWxzZSwiaWF0IjoxNTA4NDU0NzQ4LCJ0eXBlIjoiYWNjZXNzIiwibmJmIjoxNTA4NDU0NzQ4LCJpZGVudGl0eSI6ImFkbWluIn0.NT7t_17Hd3hT6_uTwy5FgGSN-koq8UeybEEKaLbRjIk", 
#  "refresh_token": "eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJqdGkiOiI2MWZmMDcxMC1hYjFlLTRhMTYtYTU1NS0yZjY0NjdlZDgyZjgiLCJleHAiOjE1MTEwNDY3NDgsImlhdCI6MTUwODQ1NDc0OCwidHlwZSI6InJlZnJlc2giLCJuYmYiOjE1MDg0NTQ3NDgsImlkZW50aXR5IjoiYWRtaW4ifQ.MMOMZCLxJbW9v2GwIgndtDZq_VpCKsueiqwXLgU04eg"
#}

#curl -X GET -H "Authorization: JWT eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJqdGkiOiIyMGU4OTg2NS0wNjM5LTQ3ZDEtYWU0YS1hYTg4ODQxNDIwNjciLCJleHAiOjE1MDg0NTU2NDgsImZyZXNoIjpmYWxzZSwiaWF0IjoxNTA4NDU0NzQ4LCJ0eXBlIjoiYWNjZXNzIiwibmJmIjoxNTA4NDU0NzQ4LCJpZGVudGl0eSI6ImFkbWluIn0.NT7t_17Hd3hT6_uTwy5FgGSN-koq8UeybEEKaLbRjIk" http://127.0.0.1:5000/listallowed

import re
import sql
from flask import Flask, request, jsonify
# from flask_cors import CORS
# from flask_jwt_extended import JWTManager
import json
import mqtt
import keycheck
import views_auth
from init import app, jwt
from flask_jwt_extended import jwt_required, \
    create_access_token, jwt_refresh_token_required, \
    create_refresh_token, get_jwt_identity


def get_access_log(days):
    d = sql.get_doorlog(days)
    return d

def keycode_validation(keycode):
    # keycodes = sql.get_doorUser_col('keycode')
    # if keycode in keycodes:
    #     return False
    if (len(keycode) > 3) and (len(keycode) < 11) and (re.match("^[A-D1-9]+$", keycode)):
        return True
    else:
        return False

# sql.setup_db()
# app = Flask(__name__)
# CORS(app)

# app.secret_key = 'ksajdkhsadulaulkj1092830983no1y24'  # Change this!
# app.config['JWT_HEADER_TYPE'] = 'JWT'
# jwt = JWTManager(app)
#
# import views_auth

# Provide a method to create access tokens. The create_access_token()
# function is used to actually generate the token
# place @jwt_required above route to protect it
# @app.route('/login', methods=['POST'])
# def login():
#     if not request.is_json:
#         return jsonify({"msg": "Missing JSON in request"}), 400
#
#     params = request.get_json()
#     username = params.get('username', None)
#     password = params.get('password', None)
#
#     if not username:
#         return jsonify({"msg": "Missing username parameter"}), 400
#     if not password:
#         return jsonify({"msg": "Missing password parameter"}), 400
#
#     if username != 'test' or password != 'test':
#         return jsonify({"msg": "Bad username or password"}), 401
#
#     # Identity can be any data that is json serializable
#     ret = {'access_token': create_access_token(identity=username)}
    # return jsonify(ret), 200

@app.route("/")
def hello():
    return "Hello World!"

@app.route("/listallowed", methods=['GET',])
@jwt_required
def list_allowed_keys():
    '''
    List doors with allowed users
    ["topgarage":["max", "mw", "etc"], "bottomgarage":[...]]
    '''
    return jsonify(sql.get_allowed()), 200


@app.route("/usekey", methods=['POST',])
def usekey():
    try:
        content = request.get_json(silent=False)
        print content
    except:
        print 'failed to get data'
    door = content['door']
    pin = content['pincode']
    #use_key(pin, door)
    if keycheck.use_key(pin, door):
        mqtt.notify_door(1, door)
        resp = {'pin_correct':1}
    else:
        mqtt.notify_door(0, door)
        resp = {'pin_correct':0}
    return jsonify(resp), 200

@app.route("/user", methods=['POST',])
def add_user():
    '''
    Add a new user to everything.
    '''
    content = request.get_json(silent=False)
    #{"username":invalid", "keycode":"invalid", "doorlist":["topgarage","frontdoor","bottomgarage"], "enabled":"1"}
    #{"username":pell", "keycode":"00003", "doorlist":["topgarage","frontdoor","bottomgarage"], "enabled":"1"}
    timeStart = None
    timeEnd = None
    doorlist = None
    if content.has_key('timeStart'):
        # print 'has time start'
        timeStart = content['timeStart'] # parse this to datetime in sql script
    else:
        content.update({'timeStart':0})
        print 'making timeStart content = '+str(content)
    if content.has_key('timeEnd'):
        timeEnd = content['timeEnd'] # parse this to datetime in sql scrip
    else:
        content.update({'timeEnd':0})
    if not keycode_validation(content['keycode']):
        return jsonify({'Status':'keycode failure'}), 200
    sql.write_userdata(content)
    return jsonify(sql.write_userdata(content)), 200

@app.route("/user/<username>", methods=['DELETE',])
def remove_user(username):
    '''
    Remove Username in user doorUsers table, and update all tables...
    {'username':'mw'}
    '''
    sql.delete_user(username)
    resp = {}
    return jsonify(resp), 200

@app.route("/user", methods=['GET',])
def get_user():
    '''
    Receives: {'username':'max'}
    Returns {'username':, 'keycode':, enabled:'', timeStart:, timeEnd, doors: [...]}
    '''
    content = request.get_json(silent=False)
    return jsonify(sql.fetch_user_data(content['username'])), 200

@app.route("/user", methods=['PUT',])
def update_user():
    '''
    Select Username and update in user doorUsers table. Json must contain old username
    #{"old_username":"pell", "old_keycode":"1234", "username":pell", "keycode":"00003", "timeStart":"blah", "endTime":"blah", "doorlist":["topgarage","frontdoor","bottomgarage"], "enabled":"1"}

    curl -X PUT -H "Content-Type: application/json" -d '{"username":"max","keycode":"234A","enabled":"0","doorlist":["frontdoor", "bottomgarage"], "timeStart":"2017-09-11T03:03:27.860Z", "timeEnd":"2037-09-11T03:03:27.860Z"}' http://127.0.0.1:5000/user
    '''
    content = request.get_json(silent=False)
    print content
    timeStart = None
    timeEnd = None
    doorlist = None
    if content.has_key('timeStart'):
        # print 'has time start'
        timeStart = content['timeStart'] # parse this to datetime
    else:
        content.update({'timeStart':0})
    if content.has_key('timeEnd'):
        timeEnd = content['timeEnd'] # parse this to datetime
    else:
        content.update({'timeEnd':0})
    if not keycode_validation(content['keycode']):
        return jsonify({'Status':'keycode failure'}), 200
    return jsonify(sql.write_userdata(content)), 200

@app.route("/user/keycode", methods=['PUT',])
def update_user_keycode():
    '''
    Select Username and update in user doorUsers table
    {"username":pell", "keycode":"00003"}
    '''
    content = request.get_json(silent=False)
    print content
    if not keycode_validation(content['keycode']):
        return jsonify({'Status':'keycode failure'}), 200
    else:
        sql.update_doorUsers(content['username'], 'keycode', content['keycode'])
        resp = {}
        return jsonify(resp), 200

@app.route("/user/enabled", methods=['PUT',])
def update_user_enabled():
    '''
    Select Username and update in user doorUsers table
    {"username":pell", "enabled":"1"}
    '''
    content = request.get_json(silent=False)
    sql.update_doorUsers(content['username'], 'enabled', int(content['enabled']))
    resp = {}
    return jsonify(resp), 200

@app.route("/user/timeStart", methods=['PUT',])
def update_user_timestart():
    '''
    Select Username and update in user doorUsers table
    '''
    content = request.get_json(silent=False)
    sql.update_doorUsers(content['username'], 'timeStart', content['timeStart'])
    resp = {}
    return jsonify(resp), 200

@app.route("/user/timeEnd", methods=['PUT',])
def update_user_timeend():
    '''
    Select Username and update in user doorUsers table
    '''
    content = request.get_json(silent=False)
    sql.update_doorUsers(content['username'], 'timeEnd', content['timeEnd'])
    resp = {}
    return jsonify(resp), 200

@app.route("/user/doors", methods=['PUT',])
def update_user_doors():
    '''
    Select Username and update canOpen table
    '''
    content = request.get_json(silent=False)
    sql.update_canOpen(content['username'], content['doorlist'])
    resp = {}
    return jsonify(resp), 200

@app.route("/users", methods=['GET',])
def get_users():
    '''
    Returns [{'username':, 'keycode':, enabled:'', timeStart:, timeEnd, doors: [...]}, {...}, ...]
    '''
    return jsonify(sql.get_all_users()), 200

@app.route("/doors", methods=['GET',])
def get_doors():
    '''
    Returns all possible door names in db as a list['door1','door2',...]
    '''
    content = request.get_json(silent=False)
    return jsonify(sql.get_all_doors()), 200

@app.route("/door/status", methods=['GET',])
def getStatus():
    content = request.get_json(silent=False)
    return jsonify(sql.get_doorstatus()), 200

@app.route("/door/status", methods=['PUT',])
def update_status():
    content = request.get_json(silent=False)
    sql.update_doorstatus(content["status"], content['door'])
    return jsonify(content), 200

@app.route("/getlog", methods=['GET',])
def getAccessLog():
    '''
    curl -X GET -H "Content-Type: application/json" -d '{"days":"5"}' http://127.0.0.1:5000/getlog
    '''
    content = request.get_json(silent=False)
    resp = get_access_log(content['days'])
    return jsonify(resp), 200

if __name__ == "__main__":
    app.run()
#    app.run(ssl_context='adhoc')
#    from gevent import pywsgi
#    from geventwebsocket.handler import WebSocketHandler
#    server = pywsgi.WSGIServer(('', 5000), app, handler_class=WebSocketHandler)
#    server.serve_forever()