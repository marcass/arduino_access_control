import sql
import requests
import creds
import json

URL = 'http://localhost/api'

headers = ''
jwt = ''
jwt_refresh = ''
refresh_headers = {"Authorization": "Bearer %s" %jwt_refresh}

def getToken():
    global jwt
    global jwt_refresh
    global headers
    r = requests.post(URL+'/auth/login', json = {'username': creds.user, 'password': creds.password})
    tokens = r.json()
    #print 'token data is: ' +str(tokens)
    try:
        jwt = tokens['access_token']
        jwt_refresh = tokens['refresh_token']
        headers = {"Authorization":"Bearer %s" %jwt}
    except:
        print 'oops, no token for you'

def post(data, route):
    global headers
    return requests.post(URL+route, json = data, headers = headers)

def put(data, route):
    global headers
    return requests.put(URL+route, json = data, headers = headers)

def parseData(data, method, route):
    global jwt
    global jwt_refresh
    global headers
    #print 'Auth header is: '+str(headers)
    # catch all for furst use
    if (jwt == ''):
        print 'Getting token'
        getToken()
    if (method == 'POST'):
        r = post(data, route)
    if (method == 'PUT'):
        r = put(data, route)
    #print 'First response is: ' +str(r)
    if '200' not in str(r):
        print 'Oops, not matchin'
    # if (r.json()['status'] != '200'):
    # if (r.json()['msg'] == The token has expired'):
        # do refresh stuff
        try:
            getToken()
            if (method == 'POST'):
                r = post(data, route)
            if (method == 'PUT'):
                r = put(data, route)
            print 'Post NOT 200 response is: ' +str(r)
        except:
            r =  {'Status': 'Error', 'Message': 'Failed ot get token, so cannot perform request'}
    print r.json()
    return r.json()

def use_key(key, door):
    d = sql.validate_key(key, door)
    if d is None:
        x = sql.insert_actionLog('Pinpad', door, key)
        print x
        return False
    else:
        if d == 'burner':
            print 'user tested true for burner'
            sql.remove_disable_key(d)
        print 'username = '+str(d)+' for '+door
        y = sql.insert_actionLog('Pinpad', door, key, d)
        print y
        return True

def update_door_status(door, status):
    sql.update_doorstatus(status, door)

def use_key_api(key, door):
    method = 'POST'
    ret = parseData({'door': door, 'pincode': key}, method, '/usekey')
    return ret.json()


def update_door_status_api(door, status):
    method = 'PUT'
    data = {'door': door, 'status': status}
    ret = parseData(data, method, '/door/status')
    return ret.json()
