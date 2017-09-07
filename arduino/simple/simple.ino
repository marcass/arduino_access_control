// Sample Arduino Json Web Client
// Downloads and parse http://jsonplaceholder.typicode.com/users/1
//consdier this: https://pubsubclient.knolleary.net/api.html mqtt in arduino

#include <WiFiEspServer.h>
#include <WiFiEspClient.h>
#include <WiFiEspUdp.h>
#include <WiFiEsp.h>
#include <Keyboard.h>
#include <ArduinoJson.h>
#include <SPI.h>

// Emulate Serial1 on pins 6/7 if not present
#ifndef HAVE_HWSERIAL1
  #include "SoftwareSerial.h"
  SoftwareSerial Serial1(6, 7); // RX, TX
#endif

#define debug

//wifi setup
char ssid[] = "Twim";            // your network SSID (name)
char pass[] = "12345678";        // your network password
int status = WL_IDLE_STATUS;     // the Wifi radio's status
char server[] = "houseslave";

// Initialize the Ethernet client object
WiFiEspClient client;

const byte rows = 4; //four rows
const byte cols = 4; //three columns
char keys[rows][cols] = {
  {'1','2','3', 'A'},
  {'4','5','6', 'B'},
  {'7','8','9', 'C'},
  {'#','0','*', 'D'}
};
byte rowPins[rows] = {5, 4, 3, 2}; //connect to the row pinouts of the keypad
byte colPins[cols] = {8, 7, 6}; //connect to the column pinouts of the keypad
String key_str = "";
unsigned long code_timer;
const unsigned long CODE_TIMER_THRESH = 20000; //20sec
Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, rows, cols );

const char* resource = "/keys/1";                    // http resource
const unsigned long BAUD_RATE = 9600;                 // serial connection speed
const unsigned long HTTP_TIMEOUT = 10000;  // max respone time from server
const size_t MAX_CONTENT_SIZE = 512;       // max size of the HTTP response
char actuate[30] = "false";

// SETUP SERIAL COMM
String inputString = "";         // a string to hold incoming data
bool stringComplete = false;  // whether the string is complete

#define RED_LED 10
#define GREEN_LED 11
#define CLOSED 12
#define OPEN 11
#define RELAY 5
#define LEDPIN 13
int door_state;
int open_reed;
int closed_reed;
int prev_state = 2;

int state;
unsigned long time_since_trigger;

//trigger states
const int WAITING = 1;
const int PRELIMINARY_TRIGGER = 2;
const int TRIGGER = 3;
const int TOO_LONG_TRIGGER = 4;
bool triggered;

//door states
const int DOOR_OPEN = 0;
const int DOOR_CLOSED = 1;
const int DOOR_UNKNOWN = 3;

const int MIN_TRIG_TIME = 300;

//timers
unsigned long check_timer;
const unsigned long CHECK_THRESH = 1000;

void setup() {
  inputString.reserve(200); // reserve mem for received message on serial port
  // initialize serial for debugging
  Serial.begin(115200);
  // initialize serial for ESP module
  Serial1.begin(9600);
  // initialize ESP module
  WiFi.init(&Serial1);
  Serial.println("Serial ready");
  pinMode(RED_LED, OUTPUT);
  digitalWrite(RED_LED, LOW);
  pinMode(GREEN_LED, OUTPUT);
  digitalWrite(GREEN_LED, LOW);
  pinMode(OPEN, INPUT_PULLUP);
  pinMode(CLOSED, INPUT_PULLUP);
  pinMode(RELAY, OUTPUT); 
  pinMode(LEDPIN, OUTPUT);
  digitalWrite(RELAY, HIGH);
  digitalWrite(LEDPIN, LOW);
  state = WAITING;

  // check for the presence of the shield
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present");
    // don't continue
    while (true); 
  }
  // attempt to connect to WiFi network
  while ( status != WL_CONNECTED) {
    Serial.print("Attempting to connect to WPA SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network
    status = WiFi.begin(ssid, pass); 
  }
  printWifiStatus();
  Serial.println();
//  Serial.println("Starting connection to server...");
//  // if you get a connection, report back via serial
//  if (client.connect(server, 80)) {
//    Serial.println("Connected to server");
//  }else{
//    Serial.println("Connection to server FAILED");
//  }
  check_timer = millis();
  triggered = false;
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

void check_door() {
  //prev_state = door_state;
  open_reed = digitalRead(OPEN);
  closed_reed = digitalRead(CLOSED);
  if ((open_reed == LOW) && (closed_reed == HIGH)) {
    door_state = DOOR_OPEN;
  }else if ((open_reed == HIGH) && (closed_reed == LOW)) {
    door_state = DOOR_CLOSED;
  } else {
    door_state = DOOR_UNKNOWN;
  }
  //publish if changed
  if (door_state != prev_state) {
    switch (door_state) {
      case DOOR_CLOSED:
        Serial.println("m:publish(\"door/state\",\"Closed\",0,1, function(conn) end )");//fix to reflect variable
        break;
      case DOOR_OPEN:
        Serial.println("m:publish(\"door/state\",\"Open\",0,1, function(conn) end )"); //fix to reflect variable
        break;
      case DOOR_UNKNOWN:
        Serial.println("m:publish(\"door/state\",\"Unknown\",0,1, function(conn) end )");//fix to reflect variable
        break;
    }
    prev_state = door_state;
  }
}

void waiting(){
  if (stringComplete) {
    #ifdef debug
      Serial.println("stuff from serial port");
    #endif
    if (inputString.startsWith("Kick door")) {
      #ifdef debug
        Serial.println("door kicked");
      #endif
      inputString = "";
      stringComplete = false;
      state = TRIGGER;
      time_since_trigger = millis();
    }
  }
}

bool test_input(String in_key, int type){
  //format post to server and await response
  if (client.connect(server, 80)) {
    #ifdef debug
      Serial.println("still connected to server");
    #endif
    // Make the HTTP POST request:
    //format the json string
    String content = "{"type+":" + in_key + "}";
    //get the post header stuff ready
    client.println("POST "+resource+" HTTP/1.1");
    client.println("Host: "+server);
    client.println("Accept: */*");
    client.println("Content-Length: " + String(content.length()));
    client.println("Content-Type: application/json");
    client.println();
    //dump the json data
    client.println(content);
  }
  //end of post

  //await response do we need to put this in loop? or ahve a pause?
  if (skipResponseHeaders()) {
      actuate userData;
      if (readReponseContent(&userData)) {
        printactuate(&userData);
      }
    }
  client.stop();
  //can we find the key name value for found key??
  if (actuate == "true")){
    return true
    #ifdef debug
      Serial.println("key found, tests true");
    #endif
  }else{
    return false
    #ifdef debug
      Serial.println("key NOT found, tests false");
    #endif
  }
}

void loop() {
  switch (state) {
    case WAITING:
      waiting();
      break;
    case TRIGGER:
      triggered = true;
      break;
  }
  if (triggered){
    if (trig_time == 0){
      trig_time = millis();
      digitalWrite(RELAY, HIGH);
    }
    if (millis() - trig_time > MIN_TRIG_TIME){
      digitalWrite(RELAY, LOW);
      triggered = false;
      state = WAITING;
    }
  if (stringComplete){
    inputString = "";
    stringComplete = false;
  }
  if (millis() - check_timer > CHECK_THRESH){
    check_door();
    check_timer = millis();
  }
  //************************ KEYPAD ENTRY  ******************************
  //timeout for key presses
  if (millis() - code_timer > CODE_TIMER_THRESH){
    key_str = "";
    digitalWrite(RED_LED, HIGH);
    //light red led
  }
  char key = keypad.getKey();
  if(key == '#'){
    //test for valid key_str via json request
    if (test_input(key_str, KEYPAD)){
      //open door or blink red light if wrong
      kick_door();
    }else{
      //opened = false;
    }
    key_str = "";
  }
  if(key){
    code_timer = millis();
    key_str =+ key;
    #ifdef debug
      Serial.print("Key string is: ");
      Serial.println(key_str);
    #endif
  }

  //******************** MQTT ENTRY  ****************************
  

  //*******************  RFID ENTRY  *****************************

}

// Skip HTTP headers so that we are at the beginning of the response's body
bool skipResponseHeaders() {
  // HTTP headers end with an empty line
  char endOfHeaders[] = "\r\n\r\n";

  client.setTimeout(HTTP_TIMEOUT);
  bool ok = client.find(endOfHeaders);

  if (!ok) {
    Serial.println("No response or invalid response!");
  }

  return ok;
}

// Parse the JSON from the input string and extract the interesting values
// Here is the JSON we need to parse
// {
//   "id": 1,
//   "name": "Leanne Graham",
//   "username": "Bret",
//   "email": "Sincere@april.biz",
//   "address": {
//     "street": "Kulas Light",
//     "suite": "Apt. 556",
//     "city": "Gwenborough",
//     "zipcode": "92998-3874",
//     "geo": {
//       "lat": "-37.3159",
//       "lng": "81.1496"
//     }
//   },
//   "phone": "1-770-736-8031 x56442",
//   "website": "hildegard.org",
//   "company": {
//     "name": "Romaguera-Crona",
//     "catchPhrase": "Multi-layered client-server neural-net",
//     "bs": "harness real-time e-markets"
//   }
// }

bool readReponseContent(char actuate* userData) {
  // Compute optimal size of the JSON buffer according to what we need to parse.
  // See https://bblanchon.github.io/ArduinoJson/assistant/
  const size_t bufferSize = JSON_OBJECT_SIZE(1) + 20;
  // Allocate a temporary memory pool
  DynamicJsonBuffer jsonBuffer(bufferSize);
  //const char* json = "{\"open\":\"true\"}";
  JsonObject& root = jsonBuffer.parseObject(client);
  
  if (!root.success()) {
    Serial.println("JSON parsing failed!");
    return false;
  }

  // Here were copy the strings we're interested in
  strcpy(actuate, root["open"]); // "true" or "false"

  return true;
}

//my json
// Parse the JSON from the input string and extract the interesting values
// Here is the JSON we need to parse
// {
//   "id": 1,
//   "name": "Joe Bloggs",
//   "username": "Joe",
//   "enabled": "0",              //enabled(1)/disabled(0) user
//   "KEYPAD": {
//     "key_str": "1234A",
//     "allowed": "1"             //enable(1)/disable(0) this method
//   },
//   "RFID": {
//     "id": "123456789",
//     "allowed": "0"
//   },
//   "MQTT": {
//     "client_id": "mqtt_client_id_set_in_device",
//     "allowed": "1"
//   }
// }
