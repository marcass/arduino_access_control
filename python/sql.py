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

def get_db():
    conn = sqlite3.connect(users_db)
    c = conn.cursor()
    return conn, c


def setup_db():
    # Create table
    conn, c = get_db()
    #type is of 'mqtt', 'keycode' or 'rfid'
    c.execute('''CREATE TABLE IF NOT EXISTS doorUsers
                    (acesstime TIMESTAMP, name TEXT, username TEXT, type TEXT, burner TEXT)''')
    conn.commit() # Save (commit) the changes

    
def add_user(username, name, keycode, mqtt_id, burner):
    # Insert a row of data
    conn, c = get_db()
    c.execute("INSERT INTO doorUsers VALUES (?,?,?,?,?,?,?)", (datetime.datetime.utcnow(),username, name, keycode, mqtt_id, burner) )
    conn.commit() # Save (commit) the changes

def localtime_from_response(resp):
    ts = datetime.datetime.strptime(resp, "%Y-%m-%d %H:%M:%S.%f")
    ts = ts.replace(tzinfo=pytz.UTC)
    return ts.astimezone(pytz.timezone(tanks.tz))
    
def query_via_user(user, days_str, q_range):
    time_range = int(days_str)
    plot_range = days_str+' '+q_range
    print 'plot range = '+plot_range
    conn, c = get_db()
    c.execute("SELECT * FROM doorUsers WHERE user=? AND accesstime BETWEEN datetime('now', '-%i %s') AND datetime('now','localtime')" % (time_range, q_range), (user,))
    #else:
        #c.execute("SELECT * FROM doorUsers WHERE tank_id=? AND timestamp BETWEEN datetime('now', '-1 days') AND datetime('now','localtime')", (tank_id,))
    ret = c.fetchall()
    accesstime = [localtime_from_response(i[0]) for i in ret]
    username = [i[2] for i in ret]
    name = [i[1] for i in ret]
    access_type = [i[3] for i in ret]
    burner = [i[4] for i in ret]
    ret_dict = {'timestamp':accesstime, 'username':username, 'name':name, 'access_type':access_type, 'burner':burner}
    print ret_dict 
    return ret_dict

def query_via_type(in_type, days_str, q_range):
    time_range = int(days_str)
    plot_range = days_str+' '+q_range
    print 'plot range = '+plot_range
    conn, c = get_db()
    c.execute("SELECT * FROM doorUsers WHERE type=? AND accesstime BETWEEN datetime('now', '-%i %s') AND datetime('now','localtime')" % (time_range, q_range), (in_type,))
    #else:
        #c.execute("SELECT * FROM doorUsers WHERE tank_id=? AND timestamp BETWEEN datetime('now', '-1 days') AND datetime('now','localtime')", (tank_id,))
    ret = c.fetchall()
    accesstime = [localtime_from_response(i[0]) for i in ret]
    username = [i[2] for i in ret]
    name = [i[1] for i in ret]
    access_type = [i[3] for i in ret]
    burner = [i[4] for i in ret]
    ret_dict = {'timestamp':accesstime, 'username':username, 'name':name, 'access_type':access_type, 'burner':burner}
    print ret_dict 
    return ret_dict


def plot_tank(user, period, target_id, q_range):
    #make a stacked histogram with number of access attempts over time?
    format_date = md.DateFormatter('%H:%M\n%d-%m')
    # Note that using plt.subplots below is equivalent to using
    # fig = plt.figure and then ax = fig.add_subplot(111)
    fig, ax = plt.subplots()
    if vers == 'water':
        data = 'water_volume'
        label = 'Volume (l)'
    if vers == 'batt':
        data = 'voltage'
        label = 'Battery Voltage'
    if type(user) is list:
        title_name = ''
        for i in user:
            d = query_via_user(i.user.name, period, q_range)
            ax.plot_date(d['timestamp'],d[data], i.line_colour, label=i.name, marker='o', markersize='5')
            title_name += ' '+i.name
            ax.set(xlabel='Datetime', ylabel=label, title='Tanks '+label)
        title_name += ' plot'
    else:
        d = query_via_user(user.name, period, q_range)
        if vers == 'bi_plot':
            title_name = 'Water Level and Voltage for '+user.name+' Tank'
            ax.plot_date(d['timestamp'],d['water_volume'], 'b', label='Water Volume (l)',  marker='o', markersize='5')
            ax.set_xlabel('Time')
            # Make the y-axis label, ticks and tick labels match the line color.
            ax.set_ylabel('Water Volume', color='b')
            ax.tick_params('y', colors='b')
            ax2 = ax.twinx()
            ax2.plot_date(d['timestamp'],d['voltage'], 'r', label='Voltage (V)', marker='p', markersize='5')
            ax2.set_ylabel('Voltage', color='r')
            ax2.tick_params('y', colors='r')
        else:
            title_name = user.name+' plot'
            ax.plot_date(d['timestamp'],d[data], user.line_colour, label=user.name, marker='o', markersize='5')
            ax.set(xlabel='Datetime', ylabel=label, title=user.name+' '+label)
            plt.axhspan(user.min_vol, user.calced_vol, facecolor='#2ca02c', alpha=0.3)
    ax.get_xaxis().set_major_formatter(format_date)
    #times = ax.get_xticklabels()
    #plt.setp(times, rotation=30)       
    plt.legend()
    ax.grid()
    plt.tight_layout()
    fig.savefig(keyserver.USER_LIST[0].pngpath+'net.png')
    plt.close()
    send_graph = bot.sendPhoto(target_id, open(keyserver.USER_LIST[0].pngpath+'net.png'))
