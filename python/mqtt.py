import paho.mqtt.client as mqtt
import paho.mqtt.publish as publish
import requests
import creds
import time
import json

broker = creds.mosq_auth['broker']
auth = creds.mosq_auth
#API_URL = 'https://skibo.duckdns.org/api/usekey'
API_URL = 'http://localhost:5000/usekey'
auth = {'username':creds.mosq_auth['username'], 'password':creds.mosq_auth['password']}
broker = creds.mosq_auth['broker']

def check_key(door, pin):
    payload ={'door':door, 'pincode': pin}
    # print payload
    r = requests.post(API_URL, json=payload)
    print r.json()
    topic = 'doors/response/'+door
    try:
        resp = r.json()['pin_correct']
        publish.single(topic, resp, qos=2, auth=auth, hostname=broker)
    except:
        print 'failed to publish'

# The callback for when the client receives a CONNACK response from the server.
def on_connect(client, userdata, flags, rc):
    print("Connected with result code "+str(rc))

    # Subscribing in on_connect() means that if we lose the connection and
    # reconnect then subscriptions will be renewed.
    client.subscribe("doors/request/#", qos=2)
    #client.subscribe("doors/requests/#")

# The callback for when a PUBLISH message is received from the server.
def on_message(client, userdata, msg):
    print(msg.topic+' '+msg.payload)
    door = msg.topic.split('/')[-1]
    print 'Door is '+door
    check_key(door, msg.payload)


#subscribe to broker and test for messages below alert values
client = mqtt.Client("Python_doors")
client.username_pw_set(username=creds.mosq_auth['username'], password=creds.mosq_auth['password'])
client.on_connect = on_connect
client.on_message = on_message
client.connect(broker, 1883, 60)
client.loop_start()
