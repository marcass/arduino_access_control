import sql

class User:
    sql.setup_db()
    def __init__(self, name, username, keycode, keycode_en, mqtt_id, mqtt_id_en, burner, enabled):
        self.name = self.add_item('name', name)
        self.username = self.add_item('username', username)
        self.keycode = self.add_item('keycode', keycode)
        self.keycode_en = self.add_item('keycode_en', keycode_en)
        self.mqtt_id = self.add_item('mqtt_id', mqtt_id)
        self.mqtt_id_en = self.add_item('mqtt_id_en', mqtt_id_en)
        self.burner = self.add_item('burner', burner)
        self.enabled = self.add_item('enabled', enabled)
        sql.add_user(self.name, self.username, self.keycode, self.keycode_en, self.mqtt_id, self.mqtt_id_en, self.burner, self.enabled)
        
    def add_item(self, var_name, item):
        d = sql.build_user_dict_query()
        error = 0
        if (var_name == 'keycode') or (var_name == 'mqtt_id') or (var_name == 'name') or (var_name == 'username'):
            #print type(item)
            if type(item) is not str:
                print 'invalid data type (not str) '+var_name
                error = 1
            try:
                print d[var_name]
                if item in d[var_name]:
                    print 'entry already exists'
                    error = 1
                else:
                    print 'not found in dict, returning item'
                    return item
            except:
                print 'exception, returning item'
                return item
        if (var_name == 'burner') or (var_name == 'mqtt_id_en') or (var_name == 'keycode_en') or (var_name == 'enabled'):
            print 'booleans'
            if type(item) is not int:
                print 'invalid data type (not int)'
                error = 1
            else:
                print 'returning '+var_name+' = '+str(item)
                return item
        if error == 1:
            raise Exception

    #USER_LIST.append(self)
        
    #def exception(self, item, value):
        #print item+' is not a unique '+value+'. Please try again'
        

#ALWAYS_ALLOWED_KEYS_LIST = ['1234','5123']
#BURN_KEYS_LIST = ['0000',]
#USER_LIST = []
#username_list = [i.name for i in USER_LIST]
#mqtt_id_list = [i.mqtt_id for i in USER_LIST]
#username_by_keycode = {i.keycode : i.username for in USER_LIST}

p = User('Pellets', 'pell', str('111111'), 1, str('phone'), 1, 1, 1)
