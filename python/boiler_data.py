from influxdb import InfluxDBClient
from datetime import timedelta
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
            'time': int(datetime.datetime.now().strftime("%s")) * 1000
            # 'time': datetime.datetime.now().strftime("%Y-%m-%dT%H:%M:%S.%fZ")
            }
        ]
    print json_data
    client.write_points(json_data)
    # print 'Update success'

def get_data():
    date_stop = datetime.datetime.now() - timedelta(days=1)
    date_string = date_stop.strftime("%Y-%m-%dT%H:%M:%S.%fZ")
    # results = client.query('SELECT "duration" FROM "boiler"."autogen"."boilerEvents" WHERE time > %s' %(date_string))
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
    water = {'marker': {'color': 'red', 'size': '10', 'symbol': 104}, 'name': 'water', 'type': 'line', 'x': wtimes, 'y': wvalues}
    auger_data = results.get_points(tags={'value_type':'auger'})
    for i in auger_data:
        atimes.append(i['time'])
        avalues.append(i['value'])
    auger = {'marker': {'color': 'blue', 'size': '10', 'symbol': 104}, 'name': 'auger', 'x': atimes, 'y': avalues}
    ftimes = []
    fvalues = []
    fan_data = results.get_points(tags={'value_type':'fan', 'status': 'Heating'})
    for i in fan_data:
        ftimes.append(i['time'])
        fvalues.append(i['value'])
    fan = {'marker': {'color': 'yellow', 'size': '10', 'symbol': 104}, 'name': 'fan', 'x': ftimes, 'y': fvalues, 'yaxis': 'y2'}
    fdtimes = []
    fdvalues = []
    feed_data = results.get_points(tags={'value_type':'feed', 'status': 'Heating'})
    for i in feed_data:
        fdtimes.append(i['time'])
        fdvalues.append(i['value'])
    feed = {'marker': {'color': 'black', 'size': '10', 'symbol': 104}, 'name': 'feed', 'x': fdtimes, 'y': fdvalues, 'yaxis': 'y2'}
    ptimes = []
    pvalues = []
    pause_data = results.get_points(tags={'value_type':'pause', 'status': 'Heating'})
    for i in pause_data:
        ptimes.append(i['time'])
        pvalues.append(i['value'])
    pause = {'marker': {'color': 'green', 'size': '10', 'symbol': 104}, 'name': 'pause', 'x': ptimes, 'y': pvalues, 'yaxis': 'y2'}
    state_plot = {}
    # , 'value_type':value_type
    # print [water, auger]
    return [water, auger, fan, feed, pause]
    # return results.get_points({'value_type':'water'})
    # return results.raw['series'][0]['values']
