//#include <SPI.h>
#include "WiFiEsp.h"
#include <WebSocketClient.h>
#include <Keypad.h>
#include <ArduinoJson.h>

//#define debug

const char door[] = "topgarage";
char ssid[] = "ssid";            // your network SSID (name)
char pass[] = "pass";        // your network password

const byte ROWS = 4; //four rows
const byte COLS = 4; //four columns
//define the symbols on the buttons of the keypads
char hexaKeys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};

byte rowPins[ROWS] = {8,9,10,11}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {2,3,4,5}; //connect to the column pinouts of the keypad
String key_str = "";
bool sendKey = false;
unsigned long pin_start = 0;
const unsigned long KEY_THRESH = 30000; //30sec to put pin in
unsigned long relay_time = 0;
const unsigned long RELAY_TRIG = 300;
unsigned long status_time = 0;
const unsigned long STATUS_TIME = 10000; //10sec
const byte STATE_IDLE = 1;
const byte STATE_TRIGGER = 2;
byte state = STATE_IDLE;
#define RELAY 12
#define RED_LED 13
#define GREEN_LED A0

//initialize an instance of class NewKeypad
Keypad customKeypad = Keypad( makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);

// Emulate Serial1 on pins 6/7 if not present
#ifndef HAVE_HWSERIAL1
#include "SoftwareSerial.h"
SoftwareSerial Serial1(6, 7); // RX, TX
#endif

int status = WL_IDLE_STATUS;     // the Wifi radio's status

// Initialize the Ethernet client object
WiFiEspClient client;
WebSocketClient webSocketClient;
// Define how many callback functions you have. Default is 1.
#define CALLBACK_FUNCTIONS 1

void setup() {
  // initialize serial for debugging
  Serial.begin(115200);
  // initialize serial for ESP module
  Serial1.begin(9600);
  // initialize ESP module
  WiFi.init(&Serial1);
  // check for the presence of the shield
  if (WiFi.status() == WL_NO_SHIELD) {
    //#ifdef debug
      //Serial.println("WiFi shield not present");
    //#endif
    // don't continue
    while (true);
  }
  // attempt to connect to WiFi network
  while ( status != WL_CONNECTED) {
    //#ifdef debug
      //Serial.print("Attempting to connect to WPA SSID: ");
      //Serial.println(ssid);
    //#endif
    // Connect to WPA/WPA2 network
    status = WiFi.begin(ssid, pass);
  }
  //#ifdef debug
    // you're connected now, so print out the data
    Serial.println("You're connected to the network");
  //#endif
  delay(3000);
  while (!WSconnect()){
    delay(5000);
    WSconnect();
  }
  
}

bool WSconnect(){
  char server[] = "skibo.duckdns.org";
  //char server[] = "houseslave";
  int port = 443;
  //int port = 8000;
  char serverPath[] = "/websocket/usekey";
  //char serverPath[] = "/";
  // Connect to the websocket server
  if (client.connectSSL(server, port)) {
  //if (client.connect(server, port)) {
    Serial.println("Connected");
  } else {
    Serial.println("Connection failed.");
    return false;
    //while(1) {
      // Hang on failure
    //}
  }

  // Handshake with the server
  webSocketClient.path = serverPath;
  webSocketClient.host = server;
  delay(100);
  if (webSocketClient.handshake(client)) {
    Serial.println("Handshake successful");
    return true;
  } else {
    Serial.println("Handshake failed.");
    return false;
//    while(1) {
//      // Hang on failure
//    }
  }
}

void send_pin(String pin){
  //build json-like object 
  String json = "{\"door\":\""+(String)door+"\",\"pincode\":\""+pin+"\"}";
  webSocketClient.sendData(json);
}

void keypadListen(){
  //timeout for keys not pressed
  bool pin_current;
  if (pin_current){
    if (millis() - pin_start > KEY_THRESH){
      //Serial.print("over key press thresh");
      pin_current = false;
    }
  }

  char key = customKeypad.getKey(); 
  if (key){
    pin_current = true;
    //restet timeout timer
    pin_start = millis();
    if(key == '#'){
      //Serial.println(key_str);
      send_pin(key_str);
      key_str = "";
    }else{
      key_str += key;
      Serial.print("Pressed key is: ");
      Serial.println(key);
    }
  }
}

void open_door(){
  Serial.println("opening door");
//  if (relay_time == 0){
//    relay_time = millis();
//  }
//  if (millis() - relay_time > RELAY_TRIG){
//    digitalWrite(RELAY, LOW);
//  }else{
//    digitalWrite(RELAY, HIGH);
//  }
//  if (status_time == 0){
//    status_time = millis();
//  }
//  if (millis() - status_time > STATUS_TIME){
//    digitalWrite(GREEN_LED, LOW);
//    digitalWrite(RED_LED, HIGH);
//    relay_time = 0;
//    status_time = 0;
//    state = STATE_IDLE;
//  }else{
//    digitalWrite(GREEN_LED, HIGH);
//    digitalWrite(RED_LED, LOW);
//  }
}

void loop() {
  //client management
  if (!client.connected()){
    delay(1000);
    Serial.print("I've been disconnected");
    //client,stop();
    if (!client.connected()){
      WSconnect(); 
    }
       
  }
  //state machine stuff
  switch (state){
    case STATE_IDLE:
      keypadListen();
      break;
    case STATE_TRIGGER:
      open_door();
      break;
  }
  String data;
  webSocketClient.getData(data);
  if (data.length() > 0) {
    //#ifdef debug
//      Serial.print("Received data: ");
     Serial.println(data);
    //#endif
    //parse json for permissions here {"door":"topgarage","action":"allowed"/"denied"}
    StaticJsonBuffer<70> jsonBuffer;
    JsonObject& root = jsonBuffer.parseObject(data);
    const char* in_door = root[String("door")];
    const char* in_status = root[String("status")];
//    Serial.println(in_door);
//    Serial.println(in_status);
    if (strcmp(in_door, door) == 0){
      if (strcmp(in_status, "allowed") == 0){
        Serial.println("Open door numbnuts!");
        return;
      }
      if (strcmp(in_status, "denied") == 0){
        Serial.println("Won't open door numbnuts!");
        return;
      }else{
        Serial.println("Door verification error");
      }
    }else{
      Serial.println("Wrong door");
    }
    
//    if (in_door == door){
//      if (in_status == "allowed"){
//        Serial.println("Open door numbnuts!");
//        state = STATE_TRIGGER;
//      }
//      if (in_status == "denied"){
//        Serial.println("Won't open door numbnuts!");
//      }else{
//        Serial.println("Door vcerification error");
//      }          
//    }else{
//      Serial.println("Wrong door");
//    }
  }
}

void printWifiStatus()
{
  IPAddress ip = WiFi.localIP();
  //#ifdef debug
    // print the SSID of the network you're attached to
    Serial.print("SSID: ");
    Serial.println(WiFi.SSID());
  
    // print your WiFi shield's IP address
    
    Serial.print("IP Address: ");
    Serial.println(ip);
  
    // print the received signal strength
    long rssi = WiFi.RSSI();
    Serial.print("Signal strength (RSSI):");
    Serial.print(rssi);
    Serial.println(" dBm");
  //#endif
}
