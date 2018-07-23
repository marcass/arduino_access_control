from influxdb import InfluxDBClient
from datetime import timedelta
import datetime
import time
# initialise as None status
state = None

client = InfluxDBClient(host='localhost', port=8086)
client.create_database('boiler')
client.switch_database('boiler')
# client.drop_database('boiler')

value_types = ['water', 'auger', 'setpoint', 'burn', 'fan', 'feed', 'pause']

def update_status(new_state):
    global state
    state = new_state

def write_data(data_type, group, data):
    global state
    global value_types
    print str(value_types)
    if data_type not in value_types:
        value_types.append(data_type)
    json_data = [
        {
            'measurement': 'boilerEvents',
            'data group': group,
            'tags': {
                'status': state,
                'value_type': data_type
            },
            'fields': {
                'value': data,
            },
            'time': datetime.datetime.now().strftime("%Y-%m-%dT%H:%M:%S.%fZ")
            }
        ]
    print json_data
    client.write_points(json_data)

def get_values():
    global value_types
    return value_types

def get_data():
    q_time = (datetime.datetime.now() - datetime.timedelta(days=1)).strftime("%Y-%m-%dT%H:%M:%S.%f000Z")
    # Need stupid single quites around timestamp to nanoscecond precision
    results = client.query("SELECT * FROM boiler.autogen.boilerEvents WHERE time > %s" %("'"+q_time+"'"))
    water_data = results.get_points(tags={'value_type':'water'})
    wtimes = []
    wvalues = []
    atimes = []
    avalues = []
    for i in water_data:
        wtimes.append(i['time'])
        wvalues.append(i['value'])
    water = {'marker': {'color': 'red', 'size': '10', 'symbol': 104}, 'name': 'water', 'type': 'line', 'x': wtimes, 'y': wvalues}
    auger_data = results.get_points(tags={'value_type':'auger'})
    # for i in auger_data:
    #     atimes.append(i['time'])
    #     avalues.append(i['value'])
    # auger = {'marker': {'color': 'blue', 'size': '10', 'symbol': 104}, 'name': 'auger', 'x': atimes, 'y': avalues}
    # ftimes = []
    # fvalues = []
    # fan_data = results.get_points(tags={'value_type':'fan', 'status': 'Heating'})
    # for i in fan_data:
    #     ftimes.append(i['time'])
    #     fvalues.append(i['value'])
    # fan = {'marker': {'color': 'yellow', 'size': '10', 'symbol': 104}, 'name': 'fan', 'x': ftimes, 'y': fvalues, 'yaxis': 'y2'}
    # fdtimes = []
    # fdvalues = []
    # feed_data = results.get_points(tags={'value_type':'feed', 'status': 'Heating'})
    # for i in feed_data:
    #     fdtimes.append(i['time'])
    #     fdvalues.append(i['value'])
    # feed = {'marker': {'color': 'black', 'size': '10', 'symbol': 104}, 'name': 'feed', 'x': fdtimes, 'y': fdvalues, 'yaxis': 'y2'}
    # ptimes = []
    # pvalues = []
    # pause_data = results.get_points(tags={'value_type':'pause', 'status': 'Heating'})
    # for i in pause_data:
    #     ptimes.append(i['time'])
    #     pvalues.append(i['value'])
    # pause = {'marker': {'color': 'green', 'size': '10', 'symbol': 104}, 'name': 'pause', 'x': ptimes, 'y': pvalues, 'yaxis': 'y2'}
    # state_plot = {}
    # return [water, auger, fan, feed, pause]
    return [water]

def custom_data(payload):
    # client.query('SELECT "duration" FROM "pyexample"."autogen"."brushEvents" WHERE time > now() - 4d GROUP BY "user"')
    # payload = {'items':graph_items, 'range':range, 'period':period}
    try:
        if payload['range'] == 'days':
            timestamp = (datetime.datetime.now() - datetime.timedelta(days=int(payload['period']))).strftime("%Y-%m-%dT%H:%M:%S.%f000Z")
        if payload['range'] == 'hours':
            timestamp = (datetime.datetime.now() - datetime.timedelta(hours=int(payload['period']))).strftime("%Y-%m-%dT%H:%M:%S.%f000Z")
    except:
        timestamp = (datetime.datetime.now() - datetime.timedelta(hours=int(payload['period']))).strftime("%Y-%m-%dT%H:%M:%S.%f000Z")
    results = client.query('SELECT * FROM "boiler"."autogen"."boilerEvents" WHERE time > %s' %("'"+timestamp+"'"))
    res = []
    colours = ['red', 'blue', 'green', 'black', 'yellow', 'orange']
    count = 0
    for i in payload['items']:
        times = []
        values = []
        out = {'marker': {'color': '', 'size': '10', 'symbol': 104}, 'name': i, 'type': 'line', 'x': '', 'y': ''}
        data = results.get_points(tags={'value_type':i})
        for a in data:
            times.append(a['time'])
            values.append(a['value'])
        out['colour'] = colours[count]
        count += 1
        out['x'] = times
        out['y'] = values
        res.append(out)
    return res
