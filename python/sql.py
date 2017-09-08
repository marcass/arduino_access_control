import matplotlib
matplotlib.use('Agg')
import users
import pytz
import sys
import time
import datetime
import sqlite3
import matplotlib.pyplot as plt
#from matplotlib.dates import DayLocator, HourLocator, DateFormatter, drange
import matplotlib.dates as md


#sqlite stuff
users_db = '/home/mw/git/arduino_access_control/python/door_database.db'
tz = 'Pacific/Auckland'

def get_db():
    conn = sqlite3.connect(users_db)
    c = conn.cursor()
    return conn, c


def setup_db():
    # Create table
    conn, c = get_db()
    #type is of 'mqtt', 'keycode' or 'rfid'
    c.execute('''CREATE TABLE IF NOT EXISTS doorAccess
                    (acesstime TIMESTAMP, name TEXT, username TEXT, type TEXT)''')
    conn.commit()
    c.execute('''CREATE TABLE IF NOT EXISTS doorUsers
                    (timestamp TIMESTAMP, name TEXT, username TEXT, keycode TEXT, keycode_en INTEGER, mqtt_id TEXT, mqtt_id_en INTEGER, burner INTEGER, enabled INTEGER )''')
    conn.commit() # Save (commit) the changes

    
def add_user(name, username, keycode, keycode_en, mqtt_id, mqtt_id_en, burner, enabled):
    # Insert a row of data
    conn, c = get_db()
    c.execute("INSERT INTO doorUsers VALUES (?,?,?,?,?,?,?,?,?)", (datetime.datetime.utcnow(),name, username, keycode, keycode_en, mqtt_id, mqtt_id_en, burner, enabled) )
    conn.commit() # Save (commit) the changes
    
def add_access(name, username, access_type):
    conn, c = get_db()
    c.execute("INSERT INTO doorAccess VALUES (?,?,?,?)", (datetime.datetime.utcnow(),name, username, access_type) )
    conn.commit()

def localtime_from_response(resp):
    ts = datetime.datetime.strptime(resp, "%Y-%m-%d %H:%M:%S.%f")
    ts = ts.replace(tzinfo=pytz.UTC)
    return ts.astimezone(pytz.timezone(tz))

def build_user_dict_query():
    conn, c = get_db()
    c.execute("SELECT * FROM doorUsers")
    ret = c.fetchall()
    createtime = [localtime_from_response(i[0]) for i in ret]
    name = [i[1] for i in ret]
    username = [i[2] for i in ret]
    keycode = [i[3] for i in ret]
    keycode_en = [i[4] for i in ret]
    mqtt_id = [i[5] for i in ret]
    mqtt_id_en = [i[6] for i in ret]
    burner = [i[7] for i in ret]
    enabled = [i[8] for i in ret]
    ret_dict = {'createtime':createtime, 'name':name, 'username':username, 'keycode':keycode, 'keycode_en':keycode_en, 'mqtt_id':mqtt_id, 'mqtt_id_en':mqtt_id_en, 'burner':burner, 'enabled':enabled}
    print ret_dict
    return ret_dict
    
def query_via_user(user, days_str, q_range):
    time_range = int(days_str)
    plot_range = days_str+' '+q_range
    print 'plot range = '+plot_range
    conn, c = get_db()
    c.execute("SELECT * FROM doorAccess WHERE user=? AND accesstime BETWEEN datetime('now', '-%i %s') AND datetime('now','localtime')" % (time_range, q_range), (user,))
    #else:
        #c.execute("SELECT * FROM doorAccess WHERE tank_id=? AND timestamp BETWEEN datetime('now', '-1 days') AND datetime('now','localtime')", (tank_id,))
    ret = c.fetchall()
    accesstime = [localtime_from_response(i[0]) for i in ret]
    username = [i[2] for i in ret]
    name = [i[1] for i in ret]
    access_type = [i[3] for i in ret]
    ret_dict = {'timestamp':accesstime, 'username':username, 'name':name, 'access_type':access_type, 'burner':burner}
    print ret_dict 
    return ret_dict

def query_via_type(in_type, days_str, q_range):
    time_range = int(days_str)
    plot_range = days_str+' '+q_range
    print 'plot range = '+plot_range
    conn, c = get_db()
    c.execute("SELECT * FROM doorAccess WHERE type=? AND accesstime BETWEEN datetime('now', '-%i %s') AND datetime('now','localtime')" % (time_range, q_range), (in_type,))
    ret = c.fetchall()
    accesstime = [localtime_from_response(i[0]) for i in ret]
    username = [i[2] for i in ret]
    name = [i[1] for i in ret]
    access_type = [i[3] for i in ret]
    burner = [i[4] for i in ret]
    ret_dict = {'timestamp':accesstime, 'username':username, 'name':name, 'access_type':access_type, 'burner':burner}
    print ret_dict 
    return ret_dict
