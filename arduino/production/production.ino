#include <SPI.h>
#include "WiFiEsp.h"
#include <WebSocketClient.h>
#include <ArduinoJson.h>
#include <Keypad.h>

const byte ROWS = 4; //four rows
const byte COLS = 4; //four columns
//define the symbols on the buttons of the keypads
char hexaKeys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};

const char door[] = "topgarage";
/*           Row pin
 * 1 2 3 A | 6
 * 4 5 6 B | 7
 * 7 8 9 C | 8
 * * 0 # D | 9
 * ---------
 * 2 3 4 5 <- Col pin
 * Pins numbered form left to right when looking at keys on keypad
 * Need D6 and D7 for software serial
 * 
 * byte rowPins[ROWS] = {6, 7, 8, 9}; //connect to the row pinouts of the keypad
 * byte colPins[COLS] = {2, 3, 4, 5}; //connect to the column pinouts of the keypad
 */
byte rowPins[ROWS] = {8,9,10,11}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {2,3,4,5}; //connect to the column pinouts of the keypad
String key_str = "";
//initialize an instance of class NewKeypad
Keypad customKeypad = Keypad( makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);

// Here we define a maximum framelength to 64 bytes. Default is 256.
//#define MAX_FRAME_LENGTH 64

// Define how many callback functions you have. Default is 1.
#define CALLBACK_FUNCTIONS 1

// Emulate Serial1 on pins 6/7 if not present
#ifndef HAVE_HWSERIAL1
#include "SoftwareSerial.h"
SoftwareSerial Serial1(6, 7); // RX, TX
#endif

int status = WL_IDLE_STATUS;     // the Wifi radio's status

// Initialize the Ethernet client object
WiFiEspClient client;

WebSocketClient webSocketClient;

void setup() {
  // initialize serial for debugging
  Serial.begin(115200);
  // initialize serial for ESP module
  Serial1.begin(9600);
  // initialize ESP module
  WiFi.init(&Serial1);

  // check for the presence of the shield
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present");
    // don't continue
    while (true);
  }

  // attempt to connect to WiFi network
  while ( status != WL_CONNECTED) {
    const char ssid[] = "skibo";            // your network SSID (name)
    const char pass[] = "r4bb1tshurtlegs";        // your network password
    Serial.print("Attempting to connect to WPA SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network
    status = WiFi.begin(ssid, pass);
  }

  // you're connected now, so print out the data
  Serial.println("You're connected to the network");
  
  //printWifiStatus();
  //websocketCon();
  const char server[] = "skibo.duckdns.org";
  const byte port = 443;
  const char serverPath[] = "/websocket/usekey";
  // Connect to the websocket server
  if (client.connectSSL(server, port)) {
    Serial.println("Connected");
  } else {
    Serial.println("Connection failed.");
    //while(1) {
      // Hang on failure
    //}
  }

  // Handshake with the server
  webSocketClient.path = serverPath;
  webSocketClient.host = server;

  if (webSocketClient.handshake(client)) {
    Serial.println("Handshake successful");
  } else {
    Serial.println("Handshake failed.");
//    while(1) {
//      // Hang on failure
//    }
  }
}

void websocketCon() {
  const char server[] = "skibo.duckdns.org";
  const byte port = 443;
  const char serverPath[] = "/websocket/usekey";
  // Connect to the websocket server
  if (client.connectSSL(server, port)) {
    Serial.println("Connected");
  } else {
    Serial.println("Connection failed.");
    //while(1) {
      // Hang on failure
    //}
  }

  // Handshake with the server
  webSocketClient.path = serverPath;
  webSocketClient.host = server;

  if (webSocketClient.handshake(client)) {
    Serial.println("Handshake successful");
  } else {
    Serial.println("Handshake failed.");
//    while(1) {
//      // Hang on failure
//    }
  }
}

String send_pin(String pin){
  if (client.connected()) {
    //set buffer for json
    StaticJsonBuffer<30> jsonBuffer;
  
    //build json object tree in memmory
    JsonObject& root = jsonBuffer.createObject();
    //input key
    root["door"] = door;
    root["pincode"] = key_str;
  

    int len = root.measureLength();
    //add 1 to len (as it comes up short)
    len = len + 1;
    char json[len];
    root.printTo(json, sizeof(json));
    webSocketClient.sendData(json);

    //receive response
    String data;
    webSocketClient.getData(data);

    if (data.length() > 0) {
      Serial.print("Received data: ");
      Serial.println(data);
      return data;
    }
  }else{
   Serial.println("Client disconnected.");
   //so connect and report error
   websocketCon();
   //while (1) {
     // Hang on disconnect.
    return "disconnected";  //so try again after waiting for connect
  }
}

void loop() {
  char key = customKeypad.getKey(); 
  if (key){
    if(key == '#'){
      Serial.println(key_str);
      String result = send_pin(key_str);
      if (result == "allowed"){
        Serial.println("do magic open stuff");
      }else{
        Serial.print("Result = ");
        Serial.println(result);
      }    
      key_str = "";
    }else{
      key_str += key;
      Serial.print("Pressed key is: ");
      Serial.println(key);
    }
  }
}


//void printWifiStatus()
//{
//  // print the SSID of the network you're attached to
//  Serial.print("SSID: ");
//  Serial.println(WiFi.SSID());
//
//  // print your WiFi shield's IP address
//  IPAddress ip = WiFi.localIP();
//  Serial.print("IP Address: ");
//  Serial.println(ip);
//
//  // print the received signal strength
//  long rssi = WiFi.RSSI();
//  Serial.print("Signal strength (RSSI):");
//  Serial.print(rssi);
//  Serial.println(" dBm");
//}
