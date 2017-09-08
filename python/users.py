import pickle

#need to pickle data for future retrieval in case of restart

class User:
    global ALWAYS_ALLOWED_KEYS_LIST, BURN_KEYS_LIST, USER_LIST, username_list, mqtt_id_list, username_by_keycode
    def __init__(self, name, username, keycode, mqtt_id, burner):
        self.name = name
        self.username = username
        if self.username in username_list:
            raise exception(self.username)
        self.keycode = keycode
        if (self.keycode in ALWAYS_ALLOWED_KEYS_LIST) or(self.keycode in BURN_KEYS_LIST) :
            raise exception(self.keycode)        
        self.mqtt_id = mqtt_id
        if self.mqtt_id in mqtt_id_list:
            raise exception(self.mqtt_id)
        self.burner = burner
        
    USER_LIST.append(self)
        
    if burner = True:
        BURN_KEYS_LIST.append(self.keycode)
        
    if burner = False:
        ALWAYS_ALLOWED_KEYS_LIST.append(self.keycode)
        
    def exception(item):
        print item+' is not unique. Please try again'

ALWAYS_ALLOWED_KEYS_LIST = ['1234','5123']
BURN_KEYS_LIST = ['0000',]
USER_LIST = []
username_list = [i.name for i in USER_LIST]
mqtt_id_list = [i.mqtt_id for i in USER_LIST]
username_by_keycode = {i.keycode : i.username for in USER_LIST}

m = User('Marcus Wells', 'marcass', '1234A', 'mqttdash-mi4c', False)
