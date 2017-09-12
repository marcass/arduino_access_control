import pytz
import sys
import time
import datetime
import sqlite3
import door_setup

users_db = '/home/mw/git/arduino_access_control/python/door_database.db'
tz = 'Pacific/Auckland'

def localtime_from_response(resp):
    ts = datetime.datetime.strptime(resp, "%Y-%m-%d %H:%M:%S.%f")
    ts = ts.replace(tzinfo=pytz.UTC)
    return ts.astimezone(pytz.timezone(tz))

def get_db():
    conn = sqlite3.connect(users_db)
    c = conn.cursor()
    return conn, c

def setup_db():
    # Create table
    conn, c = get_db()
    #type is of 'mqtt', 'keycode' or 'rfid'
    c.execute('''CREATE TABLE IF NOT EXISTS doorID
                    (door_id TEXT)''')
    c.execute('''CREATE TABLE IF NOT EXISTS actionLog
                    (timestamp TIMESTAMP, message TEXT, type TEXT)''')
    c.execute('''CREATE TABLE IF NOT EXISTS doorUsers
                    (username TEXT UNIQUE, keycode TEXT UNIQUE, enabled INTEGER, timeStart TIMESTAMP, timeEnd TIMESTAMP )''' )
    c.execute('''CREATE TABLE IF NOT EXISTS doorStates
                    (timestamp TIMESTAMP, door TEXT, state TEXT )''')
    c.execute('''CREATE TABLE IF NOT EXISTS canOpen
                    (door TEXT, userallowed TEXT, FOREIGN KEY(door) REFERENCES doorID(door_id), FOREIGN KEY(userallowed) REFERENCES doorUsers(username) ON DELETE CASCADE)''')
    conn.commit() # Save (commit) the changes


def setup_doors():
    conn, c = get_db()
    c.execute("SELECT * FROM doorID")
    if len(c.fetchall()) > 0:
        return
    else:
        for i in door_setup.doors:
            c.execute("INSERT INTO doorID VALUES (?)", (i,) )
        conn.commit()

#######  Get data #############################
def get_allowed():
    conn, c = get_db()
    utcnow = datetime.datetime.utcnow()
    c.execute("SELECT * FROM canOpen WHERE userallowed IN (SELECT username FROM doorUsers WHERE enabled=? AND ? BETWEEN timeStart AND timeEnd)", (1, utcnow))
    ret = c.fetchall()
    doors = [i[0] for i in ret]
    users = [i[1] for i in ret]
    door_list = get_all_doors()
    ret_dict = {'doors':doors, 'users':users}
    ret_list = []
    for i in door_list:
        user_list = []
        for c, value in enumerate(ret_dict['doors']):
            res_dict = {}
            if value == i:
                user_list.append(ret_dict['users'][c])
        res_dict = ({'door':i, 'users':user_list})
        ret_list.append(res_dict)
    return ret_list

def get_doorUser_col(column):
    conn, c = get_db()
    c.execute("SELECT %s FROM doorUsers" %(column))
    ret = [i[0] for i in c.fetchall()]
    print ret
    return ret

def get_all_doors():
    conn, c = get_db()
    c.execute("SELECT * FROM doorID ")
    ret =  [i[0] for i in c.fetchall()]
    return ret

def fetch_user_data(user):
    conn, c = get_db()
    c.execute("SELECT * FROM doorUsers WHERE username=?", (user,))
    res = c.fetchall()[0]
    c.execute("SELECT door FROM canOpen WHERE userallowed=?", (user,))
    doors =  [i[0] for i in c.fetchall()]
    ret = ({'username': res[0], 'keycode': res[1], 'enabled': res[2], 'times' : {'start':res[3][:-3].replace(' ','T')+'Z','end':res[4][:-3].replace(' ','T')+'Z'}, 'doors':doors})
    return ret

def get_all_users():
    conn, c = get_db()
    ret = []
    for username in get_doorUser_col('username'):
        ret.append(fetch_user_data(username))
    return ret

def get_doorstatus():
    conn, c = get_db()
    c.execute("SELECT * FROM doorStates GROUP BY door")
    ret = c.fetchall()
    doors = [i[1] for i in ret]
    time_altered = [i[0] for i in ret]
    status = [i[2] for i in ret]
    door_list = get_all_doors()
    ret_dict = {'doors':doors, 'time':time_altered, 'status':status}
    status_list = []
    for i in door_list:
        for c, value in enumerate(ret_dict['doors']):
            if value == i:
                status_dict = {'door':i, 'status':ret_dict['status'][c], 'time':ret_dict['time'][c]}
                status_list.append(status_dict)
    return status_list

def get_doorlog(days):
    conn, c = get_db()
    conn1, c1 = get_db()
    days = int(days)
    print days
    c.execute("SELECT * FROM actionLog WHERE timestamp BETWEEN datetime('now', '-%i days') AND datetime('now','localtime')" % (days))
    ret = c.fetchall()
    timestamp_action = [localtime_from_response(i[0]) for i in ret]
    message = [i[1] for i in ret]
    actionType = [i[2] for i in ret]
    dump = []
    for a in ret:
        dump = dump+[a[0],a[1],a[2]]
    c1.execute("SELECT * FROM doorStates WHERE timestamp BETWEEN datetime('now', '-%i days') AND datetime('now','localtime')" % (days))
    got = c1.fetchall()
    timestamp_open = [localtime_from_response(i[0]) for i in got]
    state = [i[2] for i in got]
    dump1 = []
    for x in got:
        dump1 = dump1 + [x[1], x[2], x[0]]
    ret_dict = {"actions":dump, "states":dump1}
    return ret_dict

############  Write data ########################
def update_doorUsers(user, column, value):
    conn, c = get_db()
    c.execute("UPDATE doorUsers SET %s=? WHERE username=?" %(column), (value, user))
    conn.commit()

def remove_disable_key(username):
    conn, c = get_db()
    #remove burnkey row
    c.execute("DELETE FROM canOpen WHERE userallowed=?", ('burner',))
    #disable in dorrUsers
    c.execute("UPDATE doorUsers SET enabled=? WHERE username=?", (0, username))
    conn.commit()

def write_userdata(resp):
    utcnow = datetime.datetime.utcnow()
    conn, c = get_db()
    #ret = {}
    print resp['timeStart']
    if resp['timeStart'] == 0:
        timeStart = utcnow
        print timeStart
    else:
        timeStart = resp['timeStart']
    if resp['timeEnd'] == 0:
        from dateutil.relativedelta import relativedelta
        timeEnd = utcnow + relativedelta(years=+20)
    else:
        timeEnd = resp['timeEnd']
    print resp['username']
    users_in = get_doorUser_col('username')
    print users_in
    if resp['username'] not in users_in:
        try:
            c.execute("INSERT INTO doorUsers VALUES (?,?,?,?,?)",(resp['username'], resp['keycode'], resp['enabled'], timeStart, timeEnd))
        except:
            return {'status':'Failed as non_unique new user'}
    else:
        c.execute("UPDATE doorUsers SET keycode=?, enabled=?, timeStart=?, timeEnd=? WHERE username=?", (resp['keycode'], resp['enabled'], timeStart, timeEnd, resp['username']))
    conn.commit()
    update_canOpen(resp['username'], resp['doorlist'])
    return {'status':'Success'}

def update_canOpen(user, doors):
    conn, c = get_db()
    c.execute("DELETE FROM canOpen WHERE userallowed=?", (user,))
    # if len(c.fetchall()) > 0:
    #     #flush detail from canOpen then rewrite it
    #     c.execute("DELETE FROM canOpen WHERE userallowed=?", (user,))
    if len(doors) > 0:
        for i in doors:
            c.execute("INSERT INTO canOpen VALUES (?,?)", (i, user))
    conn.commit()

#special sql query. Look at that trickiness!
#c.execute("SELECT * FROM canOpen WHERE userallowed IN (SELECT username FROM doorUsers WHERE enabled=1 AND ? BETWEEN timeStart AND timeEnd)", (utcnow,))
        #ret = c.fetchall()

def delete_user(user):
    conn, c = get_db()
    #cascade delete in canOpen table (set in table initialisation)
    c.execute("DELETE FROM doorUsers WHERE username=?", (user,))
    conn.commit()

def update_doorstatus(status, door):
    conn, c = get_db()
    utcnow = datetime.datetime.utcnow()
    c.execute("INSERT INTO doorStates VALUES (?,?,?)", (utcnow,door,status) )
    conn.commit()

def insert_actionLog(access_type, door, pin=None, username='Someone'):
    if username is not 'Someone':
        pin = '****'
    if pin is None:
        pin = 'mqtt'
    message = '{} attempted access at {} with {}'.format(username, door, pin)
    conn, c = get_db()
    c.execute("INSERT INTO actionLog VALUES (?,?,?)",\
        (datetime.datetime.utcnow(), message, access_type) )
    conn.commit()
    return message

def build_user_dict_query():
    conn, c = get_db()
    c.execute("SELECT * FROM doorUsers")
    ret = c.fetchall()
    username = [i[0] for i in ret]
    keycode = [i[1] for i in ret]
    ret_dict = {'username':username, 'keycode':keycode}
    print ret_dict
    return ret_dict

def validate_key(key, door):
    utcnow = datetime.datetime.utcnow()
    conn, c = get_db()
    if key != 0:
        c.execute("SELECT username FROM doorUsers WHERE enabled=? AND keycode=? AND ? BETWEEN timeStart AND timeEnd", (1, key, utcnow,))
        ret = c.fetchall()
        print ret
        try:
            print key
            print door
            in_user = ret[0][0]
            print in_user
            c.execute("SELECT userallowed FROM canOpen WHERE door=? AND userallowed=?", (door, in_user))
            #c.execute("SELECT * FROM canOpen")
            user = c.fetchall()[0][0]
            print user
            #user = ret["userallowed"]
            return user
        except:
            return None

setup_db()
setup_doors()
