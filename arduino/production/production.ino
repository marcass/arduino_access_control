#include <SPI.h>
#include "WiFiEsp.h"
#include <WebSocketClient.h>
#include <ArduinoJson.h>
#include <Keypad.h>

//#define debug

const char door[] = "topgarage";

const byte ROWS = 4; //four rows
const byte COLS = 4; //four columns
//define the symbols on the buttons of the keypads
char hexaKeys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};

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
bool sendKey = false;

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
    #ifdef debug
      Serial.println("WiFi shield not present");
    #endif
    // don't continue
    while (true);
  }

  // attempt to connect to WiFi network
  while ( status != WL_CONNECTED) {
    char ssid[] = "skibo";            // your network SSID (name)
    char pass[] = "r4bb1tshurtlegs";        // your network password
    #ifdef debug
      Serial.print("Attempting to connect to WPA SSID: ");
      Serial.println(ssid);
    #endif
    // Connect to WPA/WPA2 network
    status = WiFi.begin(ssid, pass);
  }
  #ifdef debug
    // you're connected now, so print out the data
    Serial.println("You're connected to the network");
    printWifiStatus();
  #endif
  websocketCon();
}

void websocketCon() {
  char server[] = "skibo.duckdns.org";
  int port = 443;
  char serverPath[] = "/websocket/usekey";
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

bool send_pin(String pin){
  if (client.connected()) {
    //set buffer for json
    StaticJsonBuffer<30> jsonBuffer;
  
    //build json object tree in memmory
    JsonObject& root = jsonBuffer.createObject();
    //input key
    root["door"] = door;
    root["pincode"] = key_str;
    //measure lenght of array
    int len = root.measureLength();
    //add 1 to len (as it comes up short)
    len = len + 1;
    char json[len];
    root.printTo(json, sizeof(json));
    webSocketClient.sendData(json);
    return true;
  }else{
    #ifdef debug
      Serial.println("Client disconnected.");
    #endif
    //so connect and report error
    websocketCon();
    //while (1) {
     // Hang on disconnect.
    return false;  //so try again after waiting for connect
  }
}

void loop() {
  //put timer in here for dumping key_str after certain time ahs elapsed
  //receive response
  if (client.connected()) {
    String data;
    webSocketClient.getData(data);
  
    if (data.length() > 0) {
      #ifdef debug
        Serial.print("Received data: ");
        Serial.println(data);
      #endif
      //parse json for permissions here {"door":"topgarage","action":"allowed"/"denied"}
      StaticJsonBuffer<30> jsonBuffer;
      JsonObject& root = jsonBuffer.parseObject(data);
      if (root["door"] == door){
        if (root["status"] == "allowed"){
          Serial.println("Open door numbnuts!");
        }
        if (root["door"] == "denied"){
          Serial.println("Won't open door numbnuts!");
        }else{
          Serial.println("Door vcerification error");
        }
      }else{
        Serial.println("Wrong door");
      }
    }
  }
  char key = customKeypad.getKey(); 
  if (key){
    if(key == '#'){
      sendKey = true;
    }else{
      key_str += key;
      Serial.print("Pressed key is: ");
      Serial.println(key);
    }
  }
  if (sendKey){
    Serial.println(key_str);
    if (client.connected()) {
      if (send_pin(key_str)){
        key_str = "";
        sendKey = false;
      }else{
        //do nothing and come back again
      }
    }else{
      //connect
      websocketCon();
    }
  }
}


void printWifiStatus()
{
  // print the SSID of the network you're attached to
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength
  long rssi = WiFi.RSSI();
  Serial.print("Signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}
