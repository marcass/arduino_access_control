from influxdb import InfluxDBClient
from datetime import timedelta
import datetime
import time

db_name = 'sensors'
# setup reteniton plicy list
retention_policies = ['24_hours', '7_days','2_months', '1_year', '5_years']
# setup retention policy detail
durations = {'24_hours': {'dur':'1d', 'default':True}, '7_days': {'dur':'7d', 'default':False},
             '2_months': {'dur':'4w', 'default':False}, '1_year': {'dur':'52w','default':False}, '5_years': {'dur':'260w','default':False}}
# orgainse graphing periods
periods = {'hours': ['24_hours'], 'days': ['7_days', '2_months'], 'months': ['1_year'], 'years': ['5_years']}
# setup db
client = InfluxDBClient(host='localhost', port=8086)
# setup db if it ins't already:
def setup_db():
    print 'checking for db'
    flag = False
    dbs = client.get_list_database()
    print dbs
    for i in client.get_list_database():
        if db_name in i['name']:
            flag = True
        else:
            print 'doing nothing'
    if not flag:
        print 'making db'
        client.create_database(db_name)

# setup db and use currnet db
setup_db()
client.switch_database(db_name)
# client.drop_database(db_name)

# values we want top preserve in retention policies:
rp_fields = ['temp', 'location', 'humidity']

def setup_RP():
    global retention_policies
    RP_list = []
    try:
        RP = client.get_list_retention_policies(db_name)
        for i in RP:
            # produce list of existing retention policies
            RP_list.append(i['name'])
    except:
        print 'No retention polices here'
    for i in retention_policies:
        if i not in RP_list:
            client.create_retention_policy(i, durations[i]['dur'], 1, database='boiler', default=durations[i]['default'])
    # https://influxdb-python.readthedocs.io/en/latest/api-documentation.html
    # https://docs.influxdata.com/influxdb/v1.6/guides/downsampling_and_retention/
    try:
        client.query('CREATE CONTINUOUS QUERY "cq_7_days" ON %s BEGIN SELECT mean(temp) AS "temp", mean(humidity) AS "humidity", "location" AS "location" INTO "7_days".values_7d FROM "sensorData" GROUP BY time(1m), * END' %(db_name))
        client.query('CREATE CONTINUOUS QUERY "cq_2_months" ON %s BEGIN SELECT mean(temp) AS "temp", mean(humidity) AS "humidity", "location" AS "location" INTO "2_months".values_2mo FROM "sensorData" GROUP BY time(10m), * END' %(db_name))
        client.query('CREATE CONTINUOUS QUERY "cq_1_year" ON %s BEGIN SELECT mean(temp) AS "temp", mean(humidity) AS "humidity", "location" AS "location" INTO "1_year".values_1y FROM "sensorData" GROUP BY time(30m), * END' %(db_name))
        client.query('CREATE CONTINUOUS QUERY "cq_5_years" ON %s BEGIN SELECT mean(temp) AS "temp", mean(humidity) AS "humidity", "location" AS "location" INTO "5_years".values_5y FROM "sensorData" GROUP BY time(1h), * END' %(db_name))
    except:
        # already exist
        pass

def write_data(json):
    if data_type not in value_types:
        value_types.append(data_type)
    json_data = [
        {
            'measurement': 'sensorData',
            'tags': {
                'group': json['group'],
                'sensorID': json['sensor']
            },
            'fields': {
                json['type']: json['value']
            },
            'time': datetime.datetime.utcnow().strftime("%Y-%m-%dT%H:%M:%S.%fZ")
            }
        ]
    print json_data
    client.write_points(json_data)


# def get_data():
#     q_time = (datetime.datetime.utcnow() - datetime.timedelta(days=1)).strftime("%Y-%m-%dT%H:%M:%S.%f000Z")
#     # Need stupid single quites around timestamp to nanoscecond precision
#     results = client.query('SELECT water FROM "24_hours".sensorData WHERE time > \'%s\'' %(q_time))
#     water_data = results.get_points()
#     wtimes = []
#     wvalues = []
#     for i in water_data:
#         wtimes.append(i['time'])
#         wvalues.append(i['water'])
#     water = {'marker': {'color': 'red', 'size': '10', 'symbol': 104}, 'name': 'water', 'type': 'line', 'x': wtimes, 'y': wvalues}
#     auger_data = results.get_points(tags={'value_type':'auger'})
#     # for i in auger_data:
#     #     atimes.append(i['time'])
#     #     avalues.append(i['value'])
#     # auger = {'marker': {'color': 'blue', 'size': '10', 'symbol': 104}, 'name': 'auger', 'x': atimes, 'y': avalues}
#     # ftimes = []
#     # fvalues = []
#     # fan_data = results.get_points(tags={'value_type':'fan', 'status': 'Heating'})
#     # for i in fan_data:
#     #     ftimes.append(i['time'])
#     #     fvalues.append(i['value'])
#     # fan = {'marker': {'color': 'yellow', 'size': '10', 'symbol': 104}, 'name': 'fan', 'x': ftimes, 'y': fvalues, 'yaxis': 'y2'}
#     # fdtimes = []
#     # fdvalues = []
#     # feed_data = results.get_points(tags={'value_type':'feed', 'status': 'Heating'})
#     # for i in feed_data:
#     #     fdtimes.append(i['time'])
#     #     fdvalues.append(i['value'])
#     # feed = {'marker': {'color': 'black', 'size': '10', 'symbol': 104}, 'name': 'feed', 'x': fdtimes, 'y': fdvalues, 'yaxis': 'y2'}
#     # ptimes = []
#     # pvalues = []
#     # pause_data = results.get_points(tags={'value_type':'pause', 'status': 'Heating'})
#     # for i in pause_data:
#     #     ptimes.append(i['time'])
#     #     pvalues.append(i['value'])
#     # pause = {'marker': {'color': 'green', 'size': '10', 'symbol': 104}, 'name': 'pause', 'x': ptimes, 'y': pvalues, 'yaxis': 'y2'}
#     # state_plot = {}
#     # return [water, auger, fan, feed, pause]
#     return [water]
#
# q_dict = {'24_hours': {'rp_val':'sensorData'}, '7_days': {'rp_val':'values_7d'}, '2_months': {'rp_val':'values_2mo'}, '1_year': {'rp_val':'values_1y'}, '5_years': {'rp_val':'values_5y'}}
# def custom_data(payload):
#     print 'payload for graph is:'
#     print payload
#     global periods
#     # payload = {"items": ["water", "auger", "setpoint"], "range": "24_hours", "period": "3"}
#     try:
#         if payload['range'] in periods['hours']:
#             timestamp = (datetime.datetime.utcnow() - datetime.timedelta(hours=int(payload['period']))).strftime("%Y-%m-%dT%H:%M:%S.%f000Z")
#         if payload['range'] in periods['days']:
#             timestamp = (datetime.datetime.utcnow() - datetime.timedelta(days=int(payload['period']))).strftime("%Y-%m-%dT%H:%M:%S.%f000Z")
#         if payload['range'] in periods['months']:
#             timestamp = (datetime.datetime.utcnow() - datetime.timedelta(months=int(payload['period']))).strftime("%Y-%m-%dT%H:%M:%S.%f000Z")
#         if payload['range'] in periods['years']:
#             timestamp = (datetime.datetime.utcnow() - datetime.timedelta(years=int(payload['period']))).strftime("%Y-%m-%dT%H:%M:%S.%f000Z")
#     except:
#         timestamp = (datetime.datetime.utcnow() - datetime.timedelta(hours=int(payload['period']))).strftime("%Y-%m-%dT%H:%M:%S.%f000Z")
#     # results = client.query('SELECT * FROM "boiler"."autogen"."boilerEvents" WHERE time > %s' %("'"+timestamp+"'"))
#     # target = payload["range"]+"."+q_dict[payload["range"]]["rp_val"]
#     # print target
#     res = []
#     colours = ['red', 'blue', 'green', 'black', 'yellow', 'orange']
#     count = 0
#     for i in payload['items']:
#         # get value name:
#
#         results = client.query('SELECT %s FROM %s WHERE time > \'%s\'' %(i, '"'+payload["range"]+'"'+'.'+'"'+q_dict[payload["range"]]["rp_val"]+'"', timestamp))
#         # print results.raw
#         times = []
#         values = []
#         if i in temps:
#             out = {'marker': {'color': '', 'size': '10', 'symbol': 104}, 'name': i, 'type': 'line', 'x': '', 'y': '', 'yaxis': 'yaxis'}
#             # data = results.get_points()
#         if i in pids:
#             out = {'marker': {'color': '', 'size': '10', 'symbol': 104}, 'name': i, 'type': 'line', 'x': '', 'y': '', 'yaxis': 'yaxis2'}
#             # data = results.get_points(tags={'status': 'Heating'})
#         data = results.get_points()
#         print data
#         for a in data:
#             times.append(a['time'])
#             values.append(a[i])
#         out['colour'] = colours[count]
#         count += 1
#         out['x'] = times
#         out['y'] = values
#         res.append(out)
#     print res
#     return res
#
# setup_RP()