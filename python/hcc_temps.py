import paho.mqtt.client as mqtt
import paho.mqtt.publish as publish
import creds
import ast
import requests


# {"timestamp":1549074071149,"name":"5A00080205816410","signature":"T5A00080205816410","signal":22.25}
# {"timestamp":1549074072304,"name":"60000003AEF86C28","signature":"T60000003AEF86C28","signal":20.9375}
# {"timestamp":1549074073250,"name":"9A000003AEE20728","signature":"T9A000003AEE20728","signal":24.3125}
# {"timestamp":1549074074328,"name":"DE0008018486AF10","signature":"TDE0008018486AF10","signal":23.4375}
# {"timestamp":1549074067500,"name":"01000801848CC710","signature":"T01000801848CC710","signal":22.3125}
# {"timestamp":1549074068704,"name":"01000802055D6B10","signature":"T01000802055D6B10","signal":24.875}
# {"timestamp":1549074069936,"name":"110008020565B910","signature":"T110008020565B910","signal":23.75}
# {"timestamp":1549074075548,"name":"01000801848CC710","signature":"T01000801848CC710","signal":22.3125}
# {"timestamp":1549074076755,"name":"01000802055D6B10","signature":"T01000802055D6B10","signal":24.875}
# {"timestamp":1549074077979,"name":"110008020565B910","signature":"T110008020565B910","signal":23.75}

sensor_names = {'5A00080205816410':'hall', '60000003AEF86C28': 'downstairs', '9A000003AEE20728': 'outside',
                'DE0008018486AF10': 'lounge', '01000801848CC710': 'harris', '01000802055D6B10': 'spare',
                '110008020565B910': 'marcus'}

# data = {'measurement': 'tablename', 'tags':{'type':'meastype', 'sensorID':'sensor name', 'site': 'thissite'}, 'value':value}

broker = creds.broker
auth = creds.mosq_auth
AUTH_URL = 'https://skibo.duckdns.org/api/auth/login'
DATA_URL = 'https://skibo.duckdns.org/api/data'
headers = ''
jwt = ''
jwt_refresh = ''
refresh_headers = {"Authorization": "Bearer %s" %jwt_refresh}

def getToken():
    global jwt
    global jwt_refresh
    global headers
    r = requests.post(AUTH_URL, json = {'username': creds.user, 'password': creds.password})
    tokens = r.json()
    #print 'token data is: ' +str(tokens)
    try:
        jwt = tokens['access_token']
        jwt_refresh = tokens['refresh_token']
        headers = {"Authorization":"Bearer %s" %jwt}
        print('got token')
    except:
        print('oops, no token for you')

def post_data(data):
    global jwt
    global jwt_refresh
    global headers
    if (jwt == ''):
        print('Getting token')
        getToken()
    ret = requests.post(DATA_URL, json = data, headers = headers)
    #print 'JWT = '+str(jwt)
    #print 'First response is: ' +str(ret)
    # if '200' in str(ret):
    #     print('Success')
        # print(data)
    if '200' not in str(ret):
        print('Oops, not authenticated')
        try:
            getToken()
            requests.post(DATA_URL, json = data, headers = headersi)
            ret = {'Status': 'Error', 'Message': 'Got token'}
            print('Post NOT 200 response is: ' +str(r))
        except:
            ret =  {'Status': 'Error', 'Message': 'Failed ot get token, so cannot perform request'}


# The callback for when the client receives a CONNACK response from the server.
def on_connect(client, userdata, flags, rc):
    print("Connected with result code "+str(rc))
    # Subscribing in on_connect() means that if we lose the connection and
    # reconnect then subscriptions will be renewed.
    client.subscribe("hcc/#", 2)

# use to auth through endpoint
def on_message(client, userdata, msg):
    try:
        message = ast.literal_eval(msg.payload.decode('UTF-8'))
        sensor = sensor_names[message['name']]
        temp = float(message['signal'])
        data = {'measurement': 'things', 'tags':{'type':'temp',
                'sensorID':sensor, 'site': 'marcus'}, 'value':temp}
        post_data(data)
    except:
        print('unable to fomrat message for posting')


#subscribe to broker and test for messages below alert values
client = mqtt.Client("Python_temps")
# not using auth wiht hcc data
# client.username_pw_set(username=auth['username'], password=auth['password'])
client.on_connect = on_connect
client.on_message = on_message
client.connect(broker, 1883, 60)
#client.loop_start()
client.loop_forever()
