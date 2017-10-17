import sql


def use_key(key, door):
    d = sql.validate_key(key, door)
    if d is None:
        x = sql.insert_actionLog('Pinpad', door, key)
        print x
        return False
    else:
        if d == 'burner':
            print 'user tested true for burner'
            sql.remove_disable_key(d)
        print 'username = '+str(d)+' for '+door
        y = sql.insert_actionLog('Pinpad', door, key, d)
        print y
        return True
