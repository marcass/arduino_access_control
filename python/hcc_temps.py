import paho.mqtt.client as mqtt
import paho.mqtt.publish as publish
import creds

{"timestamp":1549074071149,"name":"5A00080205816410","signature":"T5A00080205816410","signal":22.25}
{"timestamp":1549074072304,"name":"60000003AEF86C28","signature":"T60000003AEF86C28","signal":20.9375}
{"timestamp":1549074073250,"name":"9A000003AEE20728","signature":"T9A000003AEE20728","signal":24.3125}
{"timestamp":1549074074328,"name":"DE0008018486AF10","signature":"TDE0008018486AF10","signal":23.4375}
{"timestamp":1549074067500,"name":"01000801848CC710","signature":"T01000801848CC710","signal":22.3125}
{"timestamp":1549074068704,"name":"01000802055D6B10","signature":"T01000802055D6B10","signal":24.875}
{"timestamp":1549074069936,"name":"110008020565B910","signature":"T110008020565B910","signal":23.75}
{"timestamp":1549074075548,"name":"01000801848CC710","signature":"T01000801848CC710","signal":22.3125}
{"timestamp":1549074076755,"name":"01000802055D6B10","signature":"T01000802055D6B10","signal":24.875}
{"timestamp":1549074077979,"name":"110008020565B910","signature":"T110008020565B910","signal":23.75}

sensor_names = {'5A00080205816410':'hall', '60000003AEF86C28': 'downstairs', '9A000003AEE20728': 'outside',
                'DE0008018486AF10': 'lounge', '01000801848CC710': 'harris', '01000802055D6B10': 'spare',
                '110008020565B910': 'marcus'}

broker = creds.broker
auth = creds.mosq_auth

# The callback for when the client receives a CONNACK response from the server.
def on_connect(client, userdata, flags, rc):
    print("Connected with result code "+str(rc))
    # Subscribing in on_connect() means that if we lose the connection and
    # reconnect then subscriptions will be renewed.
    client.subscribe([("hcc/#", 2)])

# use to auth through endpoint
def on_message_api(client, userdata, msg):
    print(msg.payload)
    print(type(msg.payload))
    # if 'request' in msg.topic:
    #     key = msg.payload
    #     resp = middleman.use_key_api(key, door)
    #     print 'key response is '+str(resp)
    #     try:
    #         notify_door(resp, door)
    #         print 'key sent successfully'
    #     except:
    #         print 'failed to publish'
    # if 'status' in msg.topic:
    #     try:
    #         resp = middleman.update_door_status_api(door, msg.payload)
    #         # print resp
    #         return resp
    #     except:
    #         print 'Status error'

#subscribe to broker and test for messages below alert values
client = mqtt.Client("Python_temps")
client.username_pw_set(username=auth['username'], password=auth['password'])
client.on_connect = on_connect
client.on_message = on_message_api
client.connect(broker, 1883, 60)
#client.loop_start()
client.loop_forever()
