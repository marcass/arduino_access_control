#include <SPI.h>
#include "WiFiEsp.h"
#include <Keypad.h>
#include <MQTTClient.h>
#include <Adafruit_NeoPixel.h>


#define debug

char DOOR[] = "topgarage";
char ssid[] = "Garage";            // your network SSID (name)
char pass[] = "";            // your network password
char HOST[] = "192.168.0.3";

const char USER[] = "";
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
byte                  colPins[ROWS] = {8,9,10,11}; //connect to the row pinouts of the keypad
byte                  rowPins[COLS] = {2,3,4,5}; //connect to the column pinouts of the keypad
String                key_str = "";
bool                  sendKey = false;
unsigned long         pin_start = 0;
const unsigned long   KEY_THRESH = 30000; //30sec to put pin in
unsigned long         relay_time = 0;
const unsigned long   RELAY_TRIG = 300;
unsigned long         led_time = 0;
const unsigned long   LED_TIME = 3000; //3sec
unsigned long         INTERVAL = 50;  // the time we need to wait for led update
unsigned long         previousMillis = 0;
const byte            STATE_IDLE = 1;
const byte            STATE_TRIGGER = 2;
byte                  state = STATE_IDLE;
const int             RELAY = A2;
#define               LED 12
#define               SW_OPEN A0
#define               SW_CLOSED A1
// How many NeoPixels are attached to the Arduino?
#define               NUMPIXELS      24
char                  DOOR_PUB[] = "doors/request/topgarage";
char                  DOOR_SUB[] = "doors/response/topgarage";
char                  DOOR_STATE[] = "doors/status/topgarage";
//door states
const int             STATE_OPEN = 0;
const int             STATE_CLOSED = 1;
const int             STATE_UNKNOWN = 2;
int                   door_state = STATE_UNKNOWN;
int                   prev_door_state;
//led states
const byte            NOT_CONN = 3;
const byte            KEY_IN = 4;
const byte            DENIED = 5;
byte                  led_state = NULL;
byte                  prev_led_state;
byte                  disp_pix = 0;
byte                  prev_disp_pix;

//initialize an instance of class NewKeypad
Keypad customKeypad = Keypad( makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);

// When we setup the NeoPixel library, we tell it how many pixels, and which pin to use to send signals.
// Note that for older NeoPixel strips you might need to change the third parameter--see the strandtest
// example for more information on possible values.
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, LED, NEO_GRB + NEO_KHZ800);

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
  pinMode(SW_OPEN, INPUT_PULLUP);
  pinMode(SW_CLOSED, INPUT_PULLUP);
  // initialize ESP module
  WiFi.init(&Serial1);

  // MQTT brokers usually use port 8883 for secure connections.
  client.begin(HOST, net);
  client.onMessage(messageReceived);

  connect();
  pixels.begin();
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
    Serial.print(".");
    delay(1000);
  }
  client.setOptions(60, true, 1000);
  led_state = state;

  //boolean connect(const char clientId[], const char username[], const char password[]);
  //client.connect(DOOR, USER, MOSQ_PASS);
  //client.subscribe(DOOR_SUB, 1);
  while (!client.subscribe("doors/response/topgarage", 1)) {
    Serial.print("*");
    delay(1000);
  }
  led_state = state;
}

void check_state(){
  //if SW_OPEN is LOW (and SW_CLOSED is HIGH) door is open and vice versa. Unkown if not in either of these
  int open_reed = digitalRead(SW_OPEN);
  int closed_reed = digitalRead(SW_CLOSED);
//  Serial.print("open reed ");
//  Serial.print(open_reed);
//  Serial.print(": closed reed ");
//  Serial.println(closed_reed);
  if ((open_reed == LOW) && (closed_reed == HIGH)){
//  if (open_reed == LOW) {
    door_state = STATE_OPEN;
  }
  else if ((open_reed == HIGH) && (closed_reed == LOW)){
//  if (closed_reed == LOW){
    door_state = STATE_CLOSED;
  }
  else{
    door_state = STATE_UNKNOWN;
  }
  if (door_state != prev_door_state){
    char* doorStates[] = {"open", "closed", "unknown"};
    #ifdef debug
      Serial.print("Publishing state change to: ");
      Serial.println(doorStates[door_state]);
    #endif
    client.publish(DOOR_STATE, (String)doorStates[door_state]);
//    client.publish(DOOR_STATE, (String)door_state);
//    delay(1000);
    prev_door_state = door_state;
  }
}

void send_pin(String pin){
  //boolean publish(const String &topic, const String &payload, bool retained, int qos);
  if (client.publish(DOOR_PUB, pin, false, 2)){
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
      disp_pix = 0;
    }
  }
  char key = customKeypad.getKey();
  if (key){
    #ifdef debug
//      Serial.print("Currnet state for LED is "+(String)led_state);
      Serial.println("Changing to key in LED state");
    #endif
    led_state = KEY_IN;
    //reset timeout timer
    pin_start = millis();
    if(key == '#'){
      sendKey = true;
      disp_pix = 0;
      led_time = millis();
    }else{
      key_str += key;
      #ifdef debug
        Serial.print("Pressed key is: ");
        Serial.println(key);
      #endif
      disp_pix = disp_pix + 3;      
    }
  }
  if (sendKey){
    #ifdef debug
      Serial.println(key_str);
    #endif
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
    relay_time = 0;
    state = STATE_IDLE;
    led_state = state;
  }else{
    digitalWrite(RELAY, HIGH);
  }
}


void set_led(int in_pix, int red, int green, int blue){
  for (int i = 0; i < in_pix; i++){
    pixels.setPixelColor(i, pixels.Color(red,green,blue));
  }
}

void manage_led(){
  switch (led_state){
    case NOT_CONN:
      set_led(NUMPIXELS, 20, 20, 20);
      prev_led_state = led_state;
      break;
    case STATE_IDLE:
      set_led(2, 20, 0, 0);
      prev_led_state = led_state;
      break;
    case KEY_IN:
      set_led(NUMPIXELS, 0, 0, 0);
      set_led(disp_pix, 0, 50, 0);
      prev_disp_pix = disp_pix;
      prev_led_state = led_state;
      break;
    case STATE_TRIGGER:
      set_led(NUMPIXELS, 0, 50, 50);
      led_state = STATE_IDLE;
      break;
    case DENIED:
      set_led(NUMPIXELS, 50, 0, 50);
      prev_led_state = led_state;
      break;
  }
  pixels.show(); // This sends the updated pixel color to the hardware.
}

void loop() {
  client.loop();
  //Serial.println(state);
  if (!client.connected()) {
    led_state = NOT_CONN;
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
  //run led stuff
  if ((disp_pix != prev_disp_pix) or (led_state != prev_led_state)){
    manage_led();
  }
  if (led_state == DENIED) {
    if (led_time == 0) {
      led_time = millis();
    }
    if (millis() - led_time > RELAY_TRIG) {
      led_state = STATE_IDLE;
    }
  }
  //check_state();
}

void messageReceived(String &topic, String &payload) {
  #ifdef debug
    Serial.println("incoming: " + topic + " - " + payload);
  #endif
  if (payload == "1"){
    state = STATE_TRIGGER;
    led_state = state;
  }else{
    led_state = DENIED;
    #ifdef debug
      Serial.println("no, open it yourself");
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
