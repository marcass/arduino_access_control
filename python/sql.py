import pytz
import sys
import time
import sqlite3
import door_setup
from dateutil import parser
from passlib.hash import pbkdf2_sha256
import datetime
from datetime import timedelta

users_db = '/home/marcus/git/arduino_access_control/python/door_database.db'
tz = 'Pacific/Auckland'

def localtime_from_response(resp):
    ts = datetime.datetime.strptime(resp, "%Y-%m-%d %H:%M:%S.%f")
    ts = ts.replace(tzinfo=pytz.UTC)
    return ts.astimezone(pytz.timezone(tz))

def utc_from_string(payload):
    local = pytz.timezone(tz)
    try:
        naive = datetime.datetime.strptime(payload, "%Y-%m-%dT%H:%M:%S.%fZ")
    except:
        # print 'not first format'
        pass
        try:
            naive = datetime.datetime.strptime(payload, "%a, %b %d %Y, %H:%M")
            # print 'successful convertion'
        except:
            # print 'problem with time string format'
            pass
            try:
                naive = datetime.datetime.strptime(payload, "%Y-%m-%d %H:%M:%S.%f")
            except:
                return 'failed'
    local_dt = local.localize(naive, is_dst=None)
    utc_dt = local_dt.astimezone(pytz.utc)
    return utc_dt


def get_db():
    conn = sqlite3.connect(users_db)
    c = conn.cursor()
    return conn, c

def setup_db():
    # Create table
    conn, c = get_db()
    #type is of 'mqtt', 'keycode' or 'rfid'
    c.execute('''CREATE TABLE IF NOT EXISTS userAuth
                    (username TEXT UNIQUE, password TEXT, role TEXT)''')
    c.execute('''CREATE TABLE IF NOT EXISTS doorID
                    (door_id TEXT)''')
    c.execute('''CREATE TABLE IF NOT EXISTS actionLog
                    (timestamp TIMESTAMP, message TEXT, type TEXT, door TEXT, FOREIGN KEY(door) REFERENCES doorID(door_iD))''')
    c.execute('''CREATE TABLE IF NOT EXISTS doorUsers
                    (user TEXT, keycode TEXT UNIQUE, enabled INTEGER, timeStart TIMESTAMP, timeEnd TIMESTAMP, FOREIGN KEY(user) REFERENCES userAuth(username) ON DELETE CASCADE)''' )
    c.execute('''CREATE TABLE IF NOT EXISTS doorStates
                    (timestamp TIMESTAMP, door TEXT, state TEXT )''')
    c.execute('''CREATE TABLE IF NOT EXISTS canOpen
                    (door TEXT, userallowed TEXT, FOREIGN KEY(door) REFERENCES doorID(door_id), FOREIGN KEY(userallowed) REFERENCES userAuth(username) ON DELETE CASCADE)''')
    conn.commit() # Save (commit) the changes


def setup_door(door):
    print 'door in setup is '+door
    conn, c = get_db()
    c.execute("SELECT * FROM doorID")
    print c.fetchall()
    if door in c.fetchall():
        message = 'Door already exists'
        status = 'Error'
    else:
        c.execute("INSERT INTO doorID VALUES (?)", (door,) )
        conn.commit()
        message = door + ' added'
        status = 'Success'
    # if len(c.fetchall()) > 0:
    #     return
    # else:
    #     for i in door_setup.doors:
    #         c.execute("INSERT INTO doorID VALUES (?)", (i,) )
    #     conn.commit()
    return {'Status': status, 'Message': message}

def setup_admin_user(user, passw):
    conn, c = get_db()
    c.execute("SELECT * FROM userAuth")
    if len(c.fetchall()) > 0:
        return
    else:
        pw_hash = pbkdf2_sha256.hash(passw)
        c.execute("INSERT INTO userAuth VALUES (?,?,?)", (user, pw_hash, 'admin'))
        conn.commit()

#######  Get data #############################

def auth_user(thisuser, passw):
    conn, c = get_db()
    try:
        c.execute("SELECT * FROM userAuth WHERE username=?", (thisuser,))
        ret = c.fetchall()
        pw_hash = ret[0][1]
        role = ret[0][2]
        if (pbkdf2_sha256.verify(passw, pw_hash)):
            status = 'passed'
        else:
            status = 'failed'
        ret_dict = {'status': status, 'role': role}
    except:
        ret_dict = {'status': 'exception', 'role': 'undefined'}
    return ret_dict

def get_user_role(thisuser):
    conn, c = get_db()
    try:
        c.execute("SELECT role FROM userAuth WHERE username=?", (thisuser,))
        ret = c.fetchall()
    except:
        pass
        # print 'Problem getting role'
    return {'username':thisuser, 'role':role}

def get_allowed():
    conn, c = get_db()
    utcnow = datetime.datetime.utcnow()
    c.execute("SELECT * FROM canOpen WHERE userallowed IN (SELECT user FROM doorUsers WHERE enabled=? AND ? BETWEEN timeStart AND timeEnd)", (1, utcnow))
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
    return ret

def get_all_doors():
    conn, c = get_db()
    c.execute("SELECT * FROM doorID ")
    ret =  [i[0] for i in c.fetchall()]
    return ret

def fetch_user_data(user_in):
    conn, c = get_db()
    c.execute("SELECT * FROM doorUsers WHERE user=?", (user_in,))
    res = c.fetchall()[0]
    c.execute("SELECT door FROM canOpen WHERE userallowed=?", (user_in,))
    doors =  [i[0] for i in c.fetchall()]
    ret = ({'username': res[0], 'keycode': res[1], 'enabled': res[2], 'times' : {'start':res[3][:-3].replace(' ','T')+'Z','end':res[4][:-3].replace(' ','T')+'Z'}, 'doors':doors})
    return ret

def get_all_users():
    conn, c = get_db()
    ret = []
    for user_in in get_doorUser_col('user'):
        ret.append(fetch_user_data(user_in))
    return ret

def get_doorstatus():
    conn, c = get_db()
    c.execute("SELECT * FROM doorStates GROUP BY door")
    ret = c.fetchall()
    doors = [i[1] for i in ret]
    time_altered = [localtime_from_response(i[0]) for i in ret]
    status = [i[2] for i in ret]
    door_list = get_all_doors()
    ret_dict = {'doors':doors, 'time':time_altered, 'status': status}
    status_list = []
    for i in door_list:
        for c, value in enumerate(ret_dict['doors']):
            if value == i:
                status_dict = {'door':i, 'status':ret_dict['status'][c], 'time':ret_dict['time'][c]}
                # status_dict = {'status':ret_dict['status'][c], 'time':ret_dict['time'][c]}
                status_list.append(status_dict)
    #print status_list
    return status_list
    #print ret_dict
    #return ret_dict

def get_adoorstatus(door):
    conn, c = get_db()
    c.execute("SELECT * FROM doorStates WHERE door=?", (door,))
    ret = c.fetchall()
    # doors = [i[1] for i in ret]
    try:
        # print ret[-1]
        time_altered = localtime_from_response(ret[-1][0])
        status = ret[-1][2]
        door = ret[-1][1]
    except:
        time_altered = None
        status = None
        door = None
    ret_dict = {'door':door, 'time':time_altered, 'status': status}
    # print ret_dict
    # status_list = []
    return ret_dict

def get_doorlog(door, resp):
    conn, c = get_db()
    # conn1, c1 = get_db()
    start = resp['timeStart']
    end = resp['timeEnd']
    if (start is not None) and (len(start) > 0):
        timeStart = utc_from_string(start)
    else:
        timeStart = datetime.datetime.utcnow() - timedelta(days=7)
    if (end is not None) and (len(end) > 0):
        timeEnd = utc_from_string(end)
    else:
        timeEnd = datetime.datetime.utcnow()
    c.execute("SELECT * FROM actionLog WHERE door=? AND timestamp BETWEEN datetime(?) AND datetime(?)", (door, timeStart, timeEnd))
    ret = c.fetchall()
    message = [i[1] for i in ret]
    actionType = [i[2] for i in ret]
    # door = [i[3] for i in ret]
    dump = []
    for a in ret:
        dump = dump+[localtime_from_response(a[0]),a[1],a[2]]
    c.execute("SELECT * FROM doorStates WHERE door=? AND timestamp BETWEEN datetime(?) AND datetime(?)",  (door, timeStart, timeEnd))
    print dump
    got = c.fetchall()
    state = [i[2] for i in got]
    dump1 = []
    print 'making states dump'
    for x in got:
        dump1 = dump1 + [x[1], x[2], localtime_from_response(x[0])]
    ret_dict = {"actions":dump, "states":dump1}
    print ret_dict
    return ret_dict

############  Write data ########################
def setup_user(user_in, passw, role=0):
    conn, c = get_db()
    try:
        if user_in == 'burner':
            role = 'burner'
        if role == 0:
            role = 'user'
        pw_hash = pbkdf2_sha256.hash(passw)
        c.execute("INSERT INTO userAuth VALUES (?,?,?)", (user_in, pw_hash, role))
        conn.commit()
        return True
    except:
        return False

def update_doorUsers(user, column, value):
    conn, c = get_db()
    if 'time' in column:
        #parse timestting
        value = utc_from_string(value).strftime('%Y-%m-%dT%H:%M:%S.%fZ')
    c.execute("UPDATE doorUsers SET %s=? WHERE user=?" %(column), (value, user))
    conn.commit()

def remove_disable_key(username):
    conn, c = get_db()
    #remove burnkey row
    c.execute("DELETE FROM canOpen WHERE userallowed=?", ('burner',))
    #disable in dorrUsers
    c.execute("UPDATE doorUsers SET enabled=? WHERE user=?", (0, username))
    conn.commit()

def write_userdata(resp):
    utcnow = datetime.datetime.utcnow()
    conn, c = get_db()
    start = resp['timeStart']
    end = resp['timeEnd']
    if (start == '') or (start == None):
        timeStart = utcnow
    else:
        timeStart = utc_from_string(start).strftime('%Y-%m-%dT%H:%M:%S.%fZ')
    #2017-11-05T23:17:22.303Z
    if (end == '') or (end == None):
        from dateutil.relativedelta import relativedelta
        timeEnd = utcnow + relativedelta(years=+20)
    else:
        timeEnd = utc_from_string(end).strftime('%Y-%m-%dT%H:%M:%S.%fZ')
    users_in = get_doorUser_col('user')
    if resp['username'] not in users_in:
        try:
            if (setup_user(resp['username'], resp['password'], resp['role'])):
                c.execute("INSERT INTO doorUsers VALUES (?,?,?,?,?)",(resp['username'], resp['keycode'], resp['enabled'], timeStart, timeEnd))
                conn.commit()
            else:
                return {'status':'Failed to setup user'}
        except:
            return {'status':'Failed as non-unique new user'}
    else:
        c.execute("UPDATE doorUsers SET keycode=?, enabled=?, timeStart=?, timeEnd=? WHERE user=?", (resp['keycode'], resp['enabled'], timeStart, timeEnd, resp['username']))
        conn.commit()
    update_canOpen(resp['username'], resp['doorlist'])
    return {'status':'Success'}

def update_canOpen(user, doors):
    conn, c = get_db()
    try:
        c.execute("DELETE FROM canOpen WHERE userallowed=?", (user,))
        # print 'deleting new user from any existing door access'
    except:
        # print 'this user is not in canOpen so passing'
        pass
    if len(doors) > 0:
        for i in doors:
            c.execute("INSERT INTO canOpen VALUES (?,?)", (i, user))
    conn.commit()

#special sql query. Look at that trickiness!
#c.execute("SELECT * FROM canOpen WHERE userallowed IN (SELECT user FROM doorUsers WHERE enabled=1 AND ? BETWEEN timeStart AND timeEnd)", (utcnow,))
        #ret = c.fetchall()

def delete_user(user):
    conn, c = get_db()
    #cascade delete in canOpen and doorUsers table (set in table initialisation)
    c.execute("DELETE FROM doorUsers WHERE user=?", (user,))
    c.execute("DELETE FROM userAuth WHERE username=?", (user,))
    conn.commit()

def update_doorstatus(status, door):
    conn, c = get_db()
    utcnow = datetime.datetime.utcnow()
    c.execute("INSERT INTO doorStates VALUES (?,?,?)", (utcnow,door,status) )
    conn.commit()

def insert_actionLog(access_type, door, pin, username='Someone'):
    if username is not 'Someone':
        pin = '****'
    message = '{} attempted access with {}'.format(username, pin)
    conn, c = get_db()
    c.execute("INSERT INTO actionLog VALUES (?,?,?,?)",\
        (datetime.datetime.utcnow(), message, access_type, door) )
    conn.commit()
    ret = {'Message':message, 'Door': door}
    return ret

def build_user_dict_query():
    conn, c = get_db()
    c.execute("SELECT * FROM doorUsers")
    ret = c.fetchall()
    username = [i[0] for i in ret]
    keycode = [i[1] for i in ret]
    ret_dict = {'username':username, 'keycode':keycode}
    return ret_dict

def validate_key(key, door):
    utcnow = datetime.datetime.utcnow()
    conn, c = get_db()
    if key != 0:
        c.execute("SELECT user FROM doorUsers WHERE enabled=? AND keycode=? AND ? BETWEEN timeStart AND timeEnd", (1, key, utcnow,))
        ret = c.fetchall()
        try:
            in_user = ret[0][0]
            c.execute("SELECT userallowed FROM canOpen WHERE door=? AND userallowed=?", (door, in_user))
            user = c.fetchall()[0][0]
            return user
        except:
            return None

setup_db()
# setup_doors()
