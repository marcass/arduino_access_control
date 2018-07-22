from influxdb import InfluxDBClient
import datetime

client = InfluxDBClient(host='localhost', port=8086)
client.create_database('boiler')
client.switch_database('boiler')

def write_data(data_type, group, data):
    json_data = [
        {
            'measurement': 'boilerEvents',
            'data group': group,
            'tag': data_type,
            'value': data,
            'time': datetime.datetime.now().strftime("%Y-%m-%dT%H:%M:%S.%fZ")
            }
        ]
    client.write_points(json_data)

def get_data():
    client.query('SELECT "duration" FROM "boiler"."autogen"."boilerEvents" WHERE time > now() - 4d')
    return results.raw
