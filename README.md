# arduino_access_control
Use arduino to control access to electrically actuated devices
Plus put boiler interface stuff in there as an afterthought

## Components
* Electronically actuated device (garage door/door/or anything you want to access)
* WiFi in range
* Arduino as node/client (communicates to network using ESP8266 and [WIFIESP](https://github.com/bportaluri/WiFiEsp) arduino lib)
* Arduino runs this [MQTT lib](https://github.com/256dpi/arduino-mqtt)
* A master/server that runs mosquitto and a webserver

## How it works
1. Pincode gets sent to topic for door requests (either by pinpad, mqttclient, web interface, rfid device etc)
2. Pincode checked against database of allowed users
3. Response is sent to the node (allow or deny)
4. Relay is actuated and DB is updated

## User managment
Users added, modified and deleted via webpage in webserver (https://<servername/URL>)

## Maintenace
### Initial setup
* Update doorsetup.py to reflect doors you want to control
* run `python views.py <admin username> <admin password>` You get to choose what they are!
* Stop script then run via systemd (or whaterever process manager you use)
* Populate db via web interface (login usng credentials supplied above above)

### ssl certs
Letsencrypt cert needs to be renewed periodically:
1. Open port 80 on router to webserver (need to enable ipv6 for fritzbox to allow this then disable as ISP falls apart with IPv6 used)
2. run `sudo certbot renew`
3. Disable access via port 80

## Troubleshooting
### My uwsgi won't work (upstream timeout errors in /var/log/nginx/error.log)
* Changing /tmp/api.sock ownership to www-data:www-data then restarting door service and nginx service did the trick
