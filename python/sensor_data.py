from influxdb import InfluxDBClient
from datetime import timedelta
import datetime
import time

locations = ['julian', 'marcus']

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
            client.create_retention_policy(i, durations[i]['dur'], 1, database='sensors', default=durations[i]['default'])
    # https://influxdb-python.readthedocs.io/en/latest/api-documentation.html
    # https://docs.influxdata.com/influxdb/v1.6/guides/downsampling_and_retention/
    try:
        for i in locations:
            client.query('CREATE CONTINUOUS QUERY \"%s\" ON %s BEGIN SELECT mean(temp) AS "temp", mean(humidity) AS "humidity", mean(light) AS "light" INTO "7_days".values_7d FROM \"%s\" GROUP BY time(5m), * END' %(i+'_cq_7_days', db_name, i))
            client.query('CREATE CONTINUOUS QUERY \"%s\" ON %s BEGIN SELECT mean(temp) AS "temp", mean(humidity) AS "humidity", mean(light) AS "light" INTO "2_months".values_2mo FROM \"%s\" GROUP BY time(10m), * END' %(i+'_cq_2_months', db_name, i))
            client.query('CREATE CONTINUOUS QUERY \"%s\" ON %s BEGIN SELECT mean(temp) AS "temp", mean(humidity) AS "humidity", mean(light) AS "light" INTO "1_year".values_1y FROM \"%s\" GROUP BY time(20m), * END' %(i+'_cq_1_year', db_name, i))
            client.query('CREATE CONTINUOUS QUERY \"%s\" ON %s BEGIN SELECT mean(temp) AS "temp", mean(humidity) AS "humidity", mean(light) AS "light" INTO "5_years".values_5y FROM \"%s\" GROUP BY time(30m), * END' %(i+'_cq_5_years', db_name, i))
    except:
        # already exist
        print "Failed to create CQ's, do they already exist?"

setup_RP()

def write_data(json):
    try:
        json_data = [
            {
                'measurement': json['group'],
                'tags': {
                    'sensorID': json['sensor']
                },
                'fields': {
                    json['type']: float(json['value'])
                },
                'time': datetime.datetime.utcnow().strftime("%Y-%m-%dT%H:%M:%S.%fZ")
                }
            ]
        client.write_points(json_data)
        return {'Status': 'success', 'Message': 'successfully wrote data points'}
    except:
        return {'Status': 'error', 'Messgage': 'failed to wrote data points'}

def get_sensorIDs():
    locations = get_measurements()
    ret = []
    for a in locations:
        # results = client.query('SHOW TAG VALUES ON "sensors" WITH KEY = sensorID')
        # results = client.query('SHOW TAG VALUES ON "sensors" WITH KEY = sensorID WHERE "group" = \"%s\"' %(location))
        results = client.query('SHOW TAG VALUES ON "sensors" FROM \"%s\" WITH KEY = sensorID' %(a))
        sensors = results.get_points()
        ids = []
        for i in sensors:
            ids.append(i['value'])
        res = {a: ids}
        ret.append(res)
    print ret
    return ret

def get_measurements():
    results = client.query('SHOW MEASUREMENTS ON "sensors"')
    sites = results.get_points()
    locs = []
    for i in sites:
        locs.append(i['name'])
    return locs

q_dict = {'24_hours': {'rp_val':'sensorData', 'period_type': 'hours'}, '7_days': {'rp_val':'values_7d', 'period_type': 'days'}, '2_months': {'rp_val':'values_2mo', 'period_type': 'days'}, '1_year': {'rp_val':'values_1y', 'period_type': 'months'}, '5_years': {'rp_val':'values_5y', 'period_type': 'years'}}
def custom_data(payload):
    # payload = {"measurement": [{"location": <location1>, "sensors":[{'id': <sens1>, 'type': <temp/hum>}........]},....], "range":<RP to graph from>, "period": int}
    print 'payload for graph is:'
    print payload
    # try:
    #     arg_dict = {q_dict[payload['range']]['period_type']: payload['period']}
    #     timestamp = (datetime.datetime.utcnow() - datetime.timedelta(**arg_dict)).strftime("%Y-%m-%dT%H:%M:%S.%f000Z")
    # except:
    # timestamp = (datetime.datetime.utcnow() - datetime.timedelta(hours=int(payload['period']))).strftime("%Y-%m-%dT%H:%M:%S.%f000Z")
    timestamp = (datetime.datetime.utcnow() - datetime.timedelta(hours=24)).strftime("%Y-%m-%dT%H:%M:%S.%f000Z")
    # results = client.query('SELECT * FROM "boiler"."autogen"."boilerEvents" WHERE time > %s' %("'"+timestamp+"'"))
    # target = payload["range"]+"."+q_dict[payload["range"]]["rp_val"]
    # print target
    res = []
    colours = ['red', 'blue', 'green', 'black', 'yellow', 'orange']
    count = 0
    results = client.query('SELECT * FROM "24_hours".marcus WHERE time > \'%s\'' %(timestamp))
    # print results.raw
    for x in payload['measurement']:
        for i in x['sensors']:
            # results = client.query('SELECT %s FROM \"%s\".\"%s\" WHERE time > \'%s\'' %(i['type'], payload["range"], payload['measurement'], timestamp))
            times = []
            values = []
            # if i in temps:
            # out = {'marker': {'color': '', 'size': '10', 'symbol': 104}, 'name': i['id'], 'type': 'line', 'x': '', 'y': '', 'yaxis': 'yaxis'}
            out = {'marker': {'color': '', 'size': '10', 'symbol': 104}, 'name': i['id'], 'type': 'line', 'x': '', 'y': ''}
                # data = results.get_points()
            # if i in pids:
            #     out = {'marker': {'color': '', 'size': '10', 'symbol': 104}, 'name': i, 'type': 'line', 'x': '', 'y': '', 'yaxis': 'yaxis2'}
                # data = results.get_points(tags={'status': 'Heating'})
            data = results.get_points(tags={"sensorID": i['id']})
            # print data
            for a in data:
                times.append(a['time'])
                values.append(a[i['type']])
            out['colour'] = colours[count]
            count += 1
            out['x'] = times
            out['y'] = values
            # print times
            # print values
            res.append(out)
    # print res
    return res
