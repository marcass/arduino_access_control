#import matplotlib
#matplotlib.use('Agg')
import pytz
import sys
import time
import datetime
import sqlite3
#import matplotlib.pyplot as plt
##from matplotlib.dates import DayLocator, HourLocator, DateFormatter, drange
#import matplotlib.dates as md


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
    c.execute('''CREATE TABLE IF NOT EXISTS actionLog
                    (actiontime TIMESTAMP, message TEXT, type TEXT)''')
    c.execute('''CREATE TABLE IF NOT EXISTS doorUsers
                    (username TEXT, keycode TEXT, enabled INTEGER, timeStart TIMESTAMP, timeEnd TIMESTAMP )''')
    c.execute('''CREATE TABLE IF NOT EXISTS doorStates
                    (timestamp TIMESTAMP, state TEXT )''')
    conn.commit() # Save (commit) the changes

def update_doorstatus(status):
    conn, c = get_db()
    utcnow = datetime.datetime.utcnow()
    c.execute("INSERT INTO doorStates VALUES (?,?)", (utcnow,status) )
    conn.commit()
    
def add_user(username, keycode, enabled, timeStart, timeEnd):
    # Insert a row of data
    conn, c = get_db()
    utcnow = datetime.datetime.utcnow()
    if timeStart is None:
        timeStart = utcnow
    if timeEnd is None:
        from dateutil.relativedelta import relativedelta
        timeEnd = utcnow + relativedelta(years=20)
    
    c.execute("INSERT INTO doorUsers VALUES (?,?,?,?,?)",\
        (username, keycode, enabled, timeStart,timeEnd) )
    conn.commit() # Save (commit) the changes
    
def insert_actionLog(access_type, pin=None, username='Someone'):
    if username is not 'Someone':
        pin = '****'
    if pin is None:
        pin = 'mqtt'
    message = '{} attempted access with {}'.format(username, pin)
    conn, c = get_db()
    c.execute("INSERT INTO actionLog VALUES (?,?,?)",\
        (datetime.datetime.utcnow(), message, access_type) )
    conn.commit()

def localtime_from_response(resp):
    ts = datetime.datetime.strptime(resp, "%Y-%m-%d %H:%M:%S.%f")
    ts = ts.replace(tzinfo=pytz.UTC)
    return ts.astimezone(pytz.timezone(tz))

def build_user_dict_query():
    conn, c = get_db()
    c.execute("SELECT * FROM doorUsers")
    ret = c.fetchall()
    username = [i[0] for i in ret]
    keycode = [i[1] for i in ret]
    #enabled = [i[2] for i in ret]
    #timeStart = [i[3] for i in ret]
    #timeEnd = [i[4] for i in ret]
    ret_dict = {'username':username, 'keycode':keycode}
    print ret_dict
    return ret_dict

def build_allowed_keys():
    utcnow = datetime.datetime.utcnow()
    conn, c = get_db()
    c.execute("SELECT * FROM doorUsers WHERE ? BETWEEN timeStart AND timeEnd", (utcnow,))
    ret = c.fetchall()
    username = [i[0] for i in ret]
    keycode = [i[1] for i in ret]
    valid_keys = []
    burner_key = []
    key_user_list = {}
    for i in ret:
        #user enabled?
        if i[2] == 1:
            #build list of dicts
            valid_keys.append(i[1])
            key_user_list.update({i[1]:i[0]})
        if i[0] == 'burner':
            burner_key.append(i[1])
    print 'valid_keys = '+str(valid_keys)
    ret_dict = {'valid_keys':valid_keys, 'key_name':key_user_list, 'burner':burner_key}
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
