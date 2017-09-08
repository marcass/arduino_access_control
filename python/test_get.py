import sql
from flask import Flask, request, jsonify

def build_things():
    #{'username': [u'marcass'], 'name': [u'Marcus Wells'], 'keycode_en': [1], 'mqtt_id': [u'mqttdash-mi4c'], 'mqtt_id_en': [1], 'burner': [0], 'createtime': [datetime.datetime(2017, 9, 9, 0, 19, 4, 703059, tzinfo=<DstTzInfo 'Pacific/Auckland' NZST+12:00:00 STD>)], 'enabled': [1], 'keycode': [u'1234A']}
    enabled_username_keycodes = []
    for b in d['keycode_en']:
        if b == 1:
            enabled_username_keycodes.append(d['username'][b])
    print enabled_username_keycodes
    keys_by_user = {a : d['username'] for a in d['keycode']}
    print keys_by_user

def get_allowed_keys():
    global d
    d = sql.build_allowed_keys()
    #print d
    #build_things()
    
    #add rfid if works
    
    #build list of allowed users
    #user_list = []
    #for i in d['enabled']:
        #if i == 1:
            #user_list.append(i['user_name'])
            ##test kecode enabled
        
    #print user_list
    allowed_keys_list = []
    
    #global ALWAYS_ALLOWED_KEYS_LIST, BURN_KEYS_LIST
    #return ALWAYS_ALLOWED_KEYS_LIST+BURN_KEYS_LIST
    return d

get_allowed_keys()
