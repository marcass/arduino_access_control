#include <SPI.h>
#include "WiFiEsp.h"
#include <Keypad.h>
#include <MQTTClient.h>


#define debug

char DOOR[] = "topgarage";
char ssid[] = "";            // your network SSID (name)
char pass[] = "";            // your network password
char HOST[] = "houseslave";

const char USER[] = "esp";
const char MOSQ_PASS[] = "";

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
byte colPins[ROWS] = {8,9,10,11}; //connect to the row pinouts of the keypad
byte rowPins[COLS] = {2,3,4,5}; //connect to the column pinouts of the keypad
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
#define LED 13
#define SW_OPEN A0
#define SW_CLOSED A1
char DOOR_PUB[] = "doors/request/topgarage";
char DOOR_SUB[] = "doors/response/topgarage";
char DOOR_STATE[] = "doors/status/topgarage";
const int STATE_OPEN = 0;
const int STATE_CLOSED = 1;
const int STATE_UNKNOWN = 2;
int door_state = STATE_UNKNOWN;
int prev_door_state;

//initialize an instance of class NewKeypad
Keypad customKeypad = Keypad( makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);

// Emulate Serial1 on pins 6/7 if not present
#ifndef HAVE_HWSERIAL1
#include "SoftwareSerial.h"
SoftwareSerial Serial1(6, 7); // RX, TX
#endif

int status = WL_IDLE_STATUS;     // the Wifi radio's status

// Initialize the Ethernet client object
WiFiEspClient net;
MQTTClient client;

unsigned long lastMillis = 0;

void setup() {
  // initialize serial for debugging
  Serial.begin(115200);
  // initialize serial for ESP module
  Serial1.begin(9600);
  //setup digital pins
  pinMode(RELAY, OUTPUT);
  digitalWrite(RELAY, LOW);
  pinMode(LED, OUTPUT);
  digitalWrite(LED, LOW);
  pinMode(SW_OPEN, INPUT);
  pinMode(SW_CLOSED, INPUT);
  digitalWrite(SW_OPEN, INPUT_PULLUP);
  digitalWrite(SW_CLOSED, INPUT_PULLUP);
  // initialize ESP module
  WiFi.init(&Serial1);

  // MQTT brokers usually use port 8883 for secure connections.
  client.begin(HOST, net);
  client.onMessage(messageReceived);

  connect();
}

void connect(){
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
    Serial.print("Attempting to connect to WPA SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network
    status = WiFi.begin(ssid, pass);
  }
  #ifdef debug
    // you're connected now, so print out the data
    Serial.println("You're connected to the network");
    printWifiStatus();
  #endif

  Serial.print("\nconnecting...");
  while (!client.connect(DOOR, USER, MOSQ_PASS)) {
  //while (!client.connect(DOOR, USER, MOSQ_PASS)) {
    Serial.print(".");
    delay(1000);
  }

  //boolean connect(const char clientId[], const char username[], const char password[]);
  //client.connect(DOOR, USER, MOSQ_PASS);
  //boolean subscribe(const String &topic, int qos);
  client.subscribe(DOOR_SUB);
  //client.subscribe(DOOR_SUB, 2);
}

void send_pin(String pin){
  //boolean publish(const String &topic, const String &payload, bool retained, int qos);
  if (client.publish(DOOR_PUB, pin)){
  //if (client.publish(DOOR_PUB, pin, false, 2)){
    #ifdef debug
      Serial.println("Send successful");
    #endif
  }else{
    #ifdef debug
      Serial.println("Send failed");
    #endif
  }
}

void keypadListen(){
  //timeout for keys not pressed
  if (millis() - pin_start > KEY_THRESH){
    if (key_str != ""){
      key_str = "";
    }
  }
  char key = customKeypad.getKey();
  if (key){
    //restet timeout timer
    pin_start = millis();
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
    send_pin(key_str);
    key_str = "";
    sendKey = false;
  }else{
    //do nothing and come back again
  }
}

void open_door(){
  #ifdef debug
    Serial.println("Opening door as requested");
  #endif
  if (relay_time == 0){
    relay_time = millis();
  }
  if (millis() - relay_time > RELAY_TRIG){
    digitalWrite(RELAY, LOW);
  }else{
    digitalWrite(RELAY, HIGH);
  }
  if (status_time == 0){
    status_time = millis();
  }
  if (millis() - status_time > STATUS_TIME){
    Serial.print("Do LED stuff for here");
    relay_time = 0;
    status_time = 0;
    state = STATE_IDLE;
  }else{
    Serial.print("Do LED stuff for here");
  }
}

void check_state(){
  //if SW_OPEN is LOW (and SW_CLOSED is HIGH) door is open and vice versa. Unkown if not in either of these
  int open_reed = digitalRead(SW_OPEN);
  int closed_reed = digitalRead(SW_CLOSED);
  if ((open_reed == LOW) && (closed_reed == HIGH)){
    door_state = STATE_OPEN;
  }
  else if((open_reed == HIGH) && (closed_reed == LOW)){
    door_state = STATE_CLOSED;
  }
  else{
    door_state = STATE_UNKNOWN;
  }
  if (door_state != prev_door_state){
    #ifdef debug
      Serial.print("Publishing state change to: ");
      Serial.println(door_state);
    #endif
    if (client.publish(DOOR_STATE, (String)door_state)){
    //if (client.publish(DOOR_STATE, (String)door_state, true, 2)){
    #ifdef debug
      Serial.println("Status send successful");
    #endif
  }else{
    #ifdef debug
      Serial.println("Status send failed");
    #endif
  }
  prev_door_state = door_state;
  }
}

void loop() {
  client.loop();

  if (!client.connected()) {
    connect();
  }
  switch (state){
    case STATE_IDLE:
      keypadListen();
      check_state();
      break;
    case STATE_TRIGGER:
      open_door();
      break;
  }
}

void messageReceived(String &topic, String &payload) {
  Serial.println("incoming: " + topic + " - " + payload);
  if (payload == "1"){
    state = STATE_TRIGGER;
  }else{
    #ifdef debug
      Serial.println("get fucked, open it yourself");
    #endif
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
