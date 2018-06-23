import paho.mqtt.client as mqtt
import paho.mqtt.publish as publish
import creds
import middleman

broker = creds.broker
auth = creds.mosq_auth
# = 'https://skibo.duckdns.org/api/usekey'
# auth = {'username':creds.mosq_auth['username'], 'password':creds.mosq_auth['password']}

def notify_door(resp, door):
    topic = 'doors/response/'+door
    publish.single(topic, resp, qos=2, auth=auth, hostname=broker)

# The callback for when the client receives a CONNACK response from the server.
def on_connect(client, userdata, flags, rc):
    print("Connected with result code "+str(rc))

    # Subscribing in on_connect() means that if we lose the connection and
    # reconnect then subscriptions will be renewed.
    client.subscribe([("doors/request/#", 2), ("doors/status/#", 2)])

# The callback for when a PUBLISH message is received from the server.
def on_message(client, userdata, msg):
    # print(msg.topic+' '+msg.payload)
    door = msg.topic.split('/')[-1]
    # print 'Door is '+door
    if 'request' in msg.topic:
        # print 'Checking door key'
        if (middleman.use_key(msg.payload, door)):
            resp = "1"
        else:
            resp = "0"
        try:
            notify_door(resp, door)
        except:
            print 'failed to publish'
    if 'status' in msg.topic:
        #publish status
        try:
            # print msg.payload
            middleman.update_door_status(door, msg.payload)
        except:
            print 'Status error'

def on_message_api(client, userdata, msg):
    door = msg.topic.split('/')[-1]
    if 'request' in msg.topic:
        key = msg.payload
        resp = middleman.use_key_api(key, door)['pin_correct']
        print 'key rsponse is '+str(resp)
        try:
            notify_door(resp, door)
            print 'key sent successfully'
        except:
            print 'failed to publish'
    if 'status' in msg.topic:
        try:
            resp = middleman.update_door_status_api(door, msg.payload)
            print resp
        except:
            print 'Status error'
    return resp

#subscribe to broker and test for messages below alert values
client = mqtt.Client("Python_doors")
client.username_pw_set(username=auth['username'], password=auth['password'])
client.on_connect = on_connect
client.on_message = on_message
client.connect(broker, 1883, 60)
client.loop_start()
