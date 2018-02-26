#!/bin/sh
ESPToolDir="$HOME/git/esptool"
FirmwareDir="$HOME/Downloads/esp8266_at_bin_v1.6.1/ESP8266_AT_Bin_V1.6.1"
cd "$FirmwareDir" 
port=/dev/ttyUSB0
if [ ! -c $port ]; then
   port=/dev/ttyUSB1
fi
if [ ! -c $port ]; then
   echo "No device appears to be plugged in.  Stopping."
fi

#"$ESPToolDir/esptool.py" --port $port erase_flash
 
"$ESPToolDir/esptool.py" --chip esp8266 --port $port \
   write_flash -fm qio -ff 20m -fs 4MB \
   0x0000 "$FirmwareDir/bin/boot_v1.7.bin" \
   0x01000 "$FirmwareDir/bin/at/512+512/user1.1024.new.2.bin" \
   0x3fc000 "$FirmwareDir/bin/esp_init_data_default_v05.bin"  \
   0x7e000 "$FirmwareDir/bin/blank.bin"  \
   0x3fe000 "$FirmwareDir/bin/blank.bin"
 
