from influxdb import InfluxDBClient
import datetime
# initialise as None status
state = None

client = InfluxDBClient(host='localhost', port=8086)
client.create_database('boiler')
client.switch_database('boiler')
# client.drop_database('boiler')

def update_status(new_state):
    global state
    state = new_state

def write_data(data_type, group, data):
    global state
    json_data = [
        {
            'measurement': 'boilerEvents',
            'data group': group,
            'tags': {
                'status': state,
                'value_type': data_type
            },
            'fields': {
                'value': data
            },
            'time': datetime.datetime.now().strftime("%Y-%m-%dT%H:%M:%S.%fZ")
            }
        ]
    print json_data
    client.write_points(json_data)
    # print 'Update success'

def get_data():
    results = client.query('SELECT * FROM "boiler"."autogen"."boilerEvents"')
    # lists = results.raw['series'][0]['values']
    # print results.raw
    water_data = results.get_points(tags={'value_type':'water'})
    wtimes = []
    wvalues = []
    atimes = []
    avalues = []
    for i in water_data:
        wtimes.append(i['time'])
        wvalues.append(i['value'])
    water = [{'x': wtimes, 'y': wvalues}]
    auger_data = results.get_points(tags={'value_type':'auger'})
    for i in auger_data:
        atimes.append(i['time'])
        avalues.append(i['value'])
    auger = [{'x': atimes, 'y': avalues}]
    # , 'value_type':value_type
    print [water, auger]
    return water
    # return results.get_points({'value_type':'water'})
    # return results.raw['series'][0]['values']
