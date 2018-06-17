import requests

URL = 'http://localhost/api'

jwt = ''
jwt_refresh = ''
headers = {"Authorization":"Bearer %s" %jwt}
refresh_headers = {"Authorization":"Bearer %s" %jwt_refresh}

def getToken():
    global jwt
    global jwt_refresh
    r = requests.post(URL+'/auth/login', data = {'username': creds.user, 'password': creds.password})
    print 'token data is: ' +str(r)
    try:
        jwt = r['access_token']
        jwt_refresh = r['refresh_token']
    except:
        print 'oops, no token for you'

def parseData(data, method, route):
    # catch all for furst use
    if (jwt == ''):
        getToken()
    r = requests.method(URL+route, data = data)
    print 'First response is: ' +str(r)
    if (r['code'] != '200'):
        try:
            getToken()
            r = requests.method(URL+route, data = data)
            print 'Subsequent response is: ' +str(r)
        except:
            print 'Could not get data on retry'
    else:
        return r
