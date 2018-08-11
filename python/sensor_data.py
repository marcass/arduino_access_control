from influxdb import InfluxDBClient
from datetime import timedelta
import datetime
import time

measurement = []

db_name = 'sensors'
# setup reteniton plicy list must match index of durations
retention_policies = ['24_hours', '7_days','2_months', '1_year', '5_years']
# setup retention policy detail
durations = [{'name':'24_hours', 'dur':'1d', 'default':True},
             {'name': '7_days', 'dur':'7d', 'default':False},
             {'name': '2_months', 'dur':'4w', 'default':False},
             {'name': '1_year', 'dur':'52w','default':False},
             {'name': '5_years', 'dur':'260w','default':False}]
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

def setup_RP(meas):
    global retention_policies
    global measurement
    RP_list = []
    meas_rps = []
    try:
        RP = client.get_list_retention_policies(db_name)
        for i in RP:
            # produce list of existing retention policies
            RP_list.append(i['name'])
    except:
        print 'No retention polices here'
    for i in retention_policies:
        meas_rps.append(meas+'_'+i)
    for i in meas_rps:
        val = meas_rps.index(i)
        if i not in RP_list:
            client.create_retention_policy(i, durations[val]['dur'], 1, database='sensors', default=durations[val]['default'])
    # https://influxdb-python.readthedocs.io/en/latest/api-documentation.html
    # https://docs.influxdata.com/influxdb/v1.6/guides/downsampling_and_retention/
    try:
        # client.query('CREATE CONTINUOUS QUERY \"%s\" ON %s BEGIN SELECT mean(temp) AS "temp", mean(humidity) AS "humidity", mean(light) AS "light" INTO \"%s\".values_7d FROM \"%s\" GROUP BY time(5m), * END' %(meas+'_cq_7_days', db_name, meas+'_7_days', meas))
        # client.query('CREATE CONTINUOUS QUERY \"%s\" ON %s BEGIN SELECT mean(temp) AS "temp", mean(humidity) AS "humidity", mean(light) AS "light" INTO \"%s\".values_2mo FROM \"%s\" GROUP BY time(10m), * END' %(meas+'_cq_2_months', db_name, meas+'_2_months', meas))
        # client.query('CREATE CONTINUOUS QUERY \"%s\" ON %s BEGIN SELECT mean(temp) AS "temp", mean(humidity) AS "humidity", mean(light) AS "light" INTO \"%s\".values_1y FROM \"%s\" GROUP BY time(20m), * END' %(meas+'_cq_1_year', db_name, meas+'_1_year', meas))
        # client.query('CREATE CONTINUOUS QUERY \"%s\" ON %s BEGIN SELECT mean(temp) AS "temp", mean(humidity) AS "humidity", mean(light) AS "light" INTO \"%s\".values_5y FROM \"%s\" GROUP BY time(30m), * END' %(meas+'_cq_5_years', db_name, meas+'_5_years', meas))
        client.query('CREATE CONTINUOUS QUERY \"%s\" ON %s BEGIN SELECT mean(value) AS "value" INTO \"%s\".values_7d FROM \"%s\" GROUP BY time(5m), * END' %(meas+'_cq_7_days', db_name, meas+'_7_days', meas))
        client.query('CREATE CONTINUOUS QUERY \"%s\" ON %s BEGIN SELECT mean(value) AS "value" INTO \"%s\".values_2mo FROM \"%s\" GROUP BY time(10m), * END' %(meas+'_cq_2_months', db_name, meas+'_2_months', meas))
        client.query('CREATE CONTINUOUS QUERY \"%s\" ON %s BEGIN SELECT mean(value) AS "value" INTO \"%s\".values_1y FROM \"%s\" GROUP BY time(20m), * END' %(meas+'_cq_1_year', db_name, meas+'_1_year', meas))
        client.query('CREATE CONTINUOUS QUERY \"%s\" ON %s BEGIN SELECT mean(value) AS "value" INTO \"%s\".values_5y FROM \"%s\" GROUP BY time(30m), * END' %(meas+'_cq_5_years', db_name, meas+'_5_years', meas))
    except:
        # already exist
        print "Failed to create CQ for "+i+", as it already exists"

# setup_RP()

# def write_data(json):
#     # ensure RP's and CQ's in place for new sites
#     global sites
#     if json['group'] not in sites:
#         sites.append(json['group'])
#         setup_RP(json['group'])
#     try:
#         json_data = [
#             {
#                 'measurement': json['group'],
#                 'tags': {
#                     'sensorID': json['sensor']
#                 },
#                 'fields': {
#                     json['type']: float(json['value'])
#                 },
#                 'time': datetime.datetime.utcnow().strftime("%Y-%m-%dT%H:%M:%S.%fZ")
#                 }
#             ]
#         client.write_points(json_data)
#         return {'Status': 'success', 'Message': 'successfully wrote data points'}
#     except:
#         return {'Status': 'error', 'Messgage': 'failed to wrote data points'}
def write_data(json):
    # ensure RP's and CQ's in place for new sites
    global measurement
    if json['type'] not in measurement:
        measurement.append(json['type'])
        setup_RP(json['type'])
    try:
        json_data = [
            {
                'measurement': json['type'],
                'tags': {
                    'sensorID': json['sensor'],
                    'site': json['group']
                },
                'fields': {
                    json['type']: json['value']
                },
                'time': datetime.datetime.utcnow().strftime("%Y-%m-%dT%H:%M:%S.%fZ")
                }
            ]
        client.write_points(json_data)
        return {'Status': 'success', 'Message': 'successfully wrote data points'}
    except:
        return {'Status': 'error', 'Messgage': 'failed to wrote data points'}

def get_sensorIDs():
    measurements = get_measurements()
    meas = []
    for i in measurements:
        print i
        results = client.query('SHOW TAG VALUES ON "sensors" FROM \"%s\" WITH KEY = site' %(i))
        meas_types = results.get_points()
        for a in meas_types:
            print a

# def get_sensorIDs1():
#     global sites
#     ret = []
#     for a in sites:
#         print a
#         res_dict = {'site': a, 'location': []}
#         # results = client.query('SHOW TAG VALUES ON "sensors" WITH KEY = sensorID')
#         # results = client.query('SHOW TAG VALUES ON "sensors" WITH KEY = sensorID WHERE "group" = \"%s\"' %(location))
#         results = client.query('SHOW TAG VALUES ON "sensors" FROM \"%s\" WITH KEY = sensorID' %(a))
#         locations = results.get_points()
#         # ids = []
#         # ret.append(res_dict)
#         for i in locations:
#             # this_dict = {'id': i['value'], 'fields': []}
#             this_res = client.query('SHOW FIELD KEYS ON "sensors" FROM \"%s\"' %(a))
#             sensors = this_res.get_points()
#             fields = [x['fieldKey'] for x in sensors]
#             this_dict = {'id': i['value'], 'fields': fields}
#             # for x in sensors:
#             #     fields =
#             #     this_dict['fields'].append(x['fieldKey'])
#             # ids.append(i['value'])
#             res_dict['location'].append(this_dict)
#         ret.append(res_dict)
#     print ret
#     # [{site: marcus, locatons: [{id: lounge, fields: []}, {id: kitchen, fields: []}]
#     # returns:
#     # [{'site': 'marcus', 'location': [{'fields': [u'light', u'temp'], 'id': u'downhall'}, {'fields': [u'light', u'temp'], 'id': u'kitchen'}, {'fields': [u'light', u'temp'], 'id': u'lounge'}, {'fields': [u'light', u'temp'], 'id': u'spare'}, {'fields': [u'light', u'temp'], 'id': u'window'}]}]
#     return ret

def get_measurements():
    results = client.query('SHOW MEASUREMENTS ON "sensors"')
    measurement = results.get_points()
    locs = []
    for i in measurement:
        locs.append(i['name'])
    return locs

q_dict = {'24_hours': {'rp_val':'sensorData', 'period_type': 'hours'}, '7_days': {'rp_val':'values_7d', 'period_type': 'days'}, '2_months': {'rp_val':'values_2mo', 'period_type': 'days'}, '1_year': {'rp_val':'values_1y', 'period_type': 'months'}, '5_years': {'rp_val':'values_5y', 'period_type': 'years'}}
def custom_data(payload):
    # want to graph sensors from one site, so payload should be in this format:
    # {"site":"marcus", "sensorIDs":[], "range":<RP to graph from>, "period": int}
    # payload = {"measurement": [{"location": <location1>, "sensors":[{'id': <sens1>, 'type': <temp/hum>}........]},....], "range":<RP to graph from>, "period": int}
    # print 'payload for graph is:'
    # print payload
    try:
        arg_dict = {q_dict[payload['range']]['period_type']: payload['period']}
        timestamp = (datetime.datetime.utcnow() - datetime.timedelta(**arg_dict)).strftime("%Y-%m-%dT%H:%M:%S.%f000Z")
    except:
    # timestamp = (datetime.datetime.utcnow() - datetime.timedelta(hours=int(payload['period']))).strftime("%Y-%m-%dT%H:%M:%S.%f000Z")
        timestamp = (datetime.datetime.utcnow() - datetime.timedelta(hours=24)).strftime("%Y-%m-%dT%H:%M:%S.%f000Z")
    # results = client.query('SELECT * FROM "boiler"."autogen"."boilerEvents" WHERE time > %s' %("'"+timestamp+"'"))
    # target = payload["range"]+"."+q_dict[payload["range"]]["rp_val"]
    # print target
    res = []
    colours = ['red', 'blue', 'green', 'black', 'yellow', 'orange']
    count = 0
    # results = client.query('SELECT * FROM "24_hours".marcus WHERE time > \'%s\'' %(timestamp))
    results = client.query('SELECT * FROM "marcus_7_days".values_7d WHERE time > \'%s\'' %(timestamp))
    # results = client.query('SELECT * FROM \"%s\".%s WHERE time > \'%s\'' %(payload['range'], q_dict[payload['range']['rp_val']], timestamp))
    # print results.raw
    for x in payload['measurement']:
        for i in x['sensors']:
            # results = client.query('SELECT %s FROM \"%s\".\"%s\" WHERE time > \'%s\'' %(i['type'], payload["range"], payload['measurement'], timestamp))
            times = []
            values = []
            # if i in temps:
            # out = {'marker': {'color': '', 'size': '10', 'symbol': 104}, 'name': i['id'], 'type': 'line', 'x': '', 'y': '', 'yaxis': 'yaxis'}
            out = {'marker': {'color': 'red', 'size': '10'}, 'name': i['id'], 'type': 'line', 'x': '', 'y': ''}
                # data = results.get_points()
            # if i in pids:
            #     out = {'marker': {'color': '', 'size': '10', 'symbol': 104}, 'name': i, 'type': 'line', 'x': '', 'y': '', 'yaxis': 'yaxis2'}
                # data = results.get_points(tags={'status': 'Heating'})
            data = results.get_points(tags={"sensorID": i['id']})
            # print data
            for a in data:
                times.append(a['time'])
                values.append(a[i['type']])
            # out['colour'] = colours[count]
            count += 1
            out['x'] = times
            out['y'] = values
            # print times
            # print values
            res.append(out)
    # print res
    return res
