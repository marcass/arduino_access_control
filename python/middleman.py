import sql
import requests
import creds
import json

URL = 'http://localhost/api'

jwt = ''
jwt_refresh = ''
headers = {"Authorization":"Bearer %s" %jwt}
refresh_headers = {"Authorization": "Bearer %s" %jwt_refresh}

def getToken():
    global jwt
    global jwt_refresh
    r = requests.post(URL+'/auth/login', json = {'username': creds.user, 'password': creds.password})
    tokens = json.loads(r.content)
    print 'token data is: ' +str(tokens)
    try:
        jwt = tokens['access_token']
        jwt_refresh = tokens['refresh_token']
    except:
        print 'oops, no token for you'

def parseData(data, method, route):
    global jwt
    global jwt_refresh
    print 'Auth header is: '+str(headers)
    # catch all for furst use
    if (jwt == ''):
        print 'Getting token'
        getToken()
    r = requests.method(URL+route, json = data, headers = headers)
    print 'First response is: ' +str(r)
    if (r['code'] != '200'):
        try:
            getToken()
            r = requests.method(URL+route, json = data, headers = headers)
            print 'Post NOT 200 response is: ' +str(r)
        except:
            print 'Could not get data on retry'
    else:
        print 'Successful request'

# def use_key(key, door):
#     d = sql.validate_key(key, door)
#     if d is None:
#         x = sql.insert_actionLog('Pinpad', door, key)
#         print x
#         return False
#     else:
#         if d == 'burner':
#             print 'user tested true for burner'
#             sql.remove_disable_key(d)
#         print 'username = '+str(d)+' for '+door
#         y = sql.insert_actionLog('Pinpad', door, key, d)
#         print y
#         return True

def update_door_status(door, status):
    sql.update_doorstatus(status, door)

def use_key_api(key, door):
    method = 'post'
    parseData({'door': door, 'pincode': key}, method, '/usekey')

def update_door_status_api(door, status):
    method = 'put'
    data = {'door': door, 'status': status}
    parseData(data, method, '/door/status')
