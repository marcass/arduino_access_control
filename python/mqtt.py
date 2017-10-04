import paho.mqtt.publish as mqtt
import requests
import creds

broker = creds.mosq_auth['broker']
auth = creds.mosq_auth
API_URL = 'https://skibo.duckdns.org/api/usekey'

def check_key(door, pin):
    r = requests.post(API_URL, data = {'door':door, 'pincode': pin})
    print r
    topic = '/'+door
    try:
        resp = r['pin_correct']
        publish.single(topic, resp, qos=2, auth=auth, hostname=broker)
    except:
        print 'failed to publish'
        
# The callback for when the client receives a CONNACK response from the server.
def on_connect(client, userdata, flags, rc):
    print("Connected with result code "+str(rc))

    # Subscribing in on_connect() means that if we lose the connection and
    # reconnect then subscriptions will be renewed.
    client.subscribe("/door/#")
    
# The callback for when a PUBLISH message is received from the server.
def on_message(client, userdata, msg):
    print(msg.topic+' '+msg.payload)
    door = topic.split(/)[1]
    print 'Door is '+door
    check_key(door, msg.payload)
    
        
#subscribe to broker and test for messages below alert values
client = mqtt.Client()
client.username_pw_set(username=creds.mosq_auth['username'], password=creds.mosq_auth['password'])
client.on_connect = on_connect
client.on_message = on_message
client.connect(creds.mosq_auth['broker'], 1883, 60)
#client.loop_start()


#while 1:
    #time.sleep(5)


