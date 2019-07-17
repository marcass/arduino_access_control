import paho.mqtt.client as mqtt
import paho.mqtt.publish as publish
import creds

broker = creds.broker

def notify_door(resp, door):
    topic = 'doors/response/'+door
    print('sending '+resp+' to '+topic)
    publish.single(topic, resp, qos=2)

# The callback for when the client receives a CONNACK response from the server.
def on_connect(client, userdata, flags, rc):
    print("Connected with result code "+str(rc))
    # Subscribing in on_connect() means that if we lose the connection and
    # reconnect then subscriptions will be renewed.
    client.subscribe([("doors/request/#", 2), ("doors/status/#", 2)])

# The callback for when a PUBLISH message is received from the server.
def on_message(client, userdata, msg):
    print('stuff')
    print(msg.topic)
    print(msg.payload)
    data = msg.payload.decode("utf-8")
    door = msg.topic.split('/')[-1]
    # print 'Door is '+door
    if 'request' in msg.topic:
        print ('Checking door key')
        if (data in creds.users):
            resp = "1"
        else:
            resp = "0"
        print('response is: '+resp)
        try:
            notify_door(resp, door)
        except:
            print ('failed to publish')

#subscribe to broker and test for messages below alert values
client = mqtt.Client("Python_doors")
client.on_connect = on_connect
client.on_message = on_message
client.connect(broker, 1883, 60)
#client.loop_start()
client.loop_forever()
