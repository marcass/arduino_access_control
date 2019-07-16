#include <SPI.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <Keypad.h>
#include <Adafruit_NeoPixel.h>
#include "secrets.h"

#define debug

// content in secrets.h in this format: #define VAR "var"
char DOOR[] = thisDOOR;
char ssid[] = MYSSID;            // your network SSID (name)
char pass[] = PASS;            // your network password
const char mqttServer[] = BROKER;
const char DOOR_PUB[] = D_PUB;
const char DOOR_SUB[] = D_SUB;
const char DOOR_STATE[] = D_STATE;
const int mqttPort = 1883;

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
//byte                  colPins[ROWS] = {15,16,17,18}; //connect to the row pinouts of the keypad
//byte                  rowPins[COLS] = {19,23,21,22}; //connect to the column pinouts of the keypad
byte                  colPins[ROWS] = {19,21,22,23}; //connect to the row pinouts of the keypad
byte                  rowPins[COLS] = {15,16,17,18}; //connect to the column pinouts of the keypad
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
unsigned long         LED_CHANGE_TIME = 2000;
unsigned long         led_change_time;
const byte            STATE_IDLE = 1;
const byte            STATE_TRIGGER = 2;
byte                  state = STATE_IDLE;
const int             RELAY = 33;
#define               LED 32
#define               SW_OPEN 25
#define               SW_CLOSED 26
// How many NeoPixels are attached to the Arduino?
#define               NUMPIXELS      24
//char                  DOOR_PUB[] = D_PUB;
//char                  DOOR_SUB[] = D_SUB;
//char                  DOOR_STATE[] = D_STATE;
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
byte                  led_state = NOT_CONN;
byte                  new_led_state;
byte                  prev_led_state;
byte                  disp_pix = 0;
byte                  prev_disp_pix;

//initialize an instance of class NewKeypad
Keypad customKeypad = Keypad( makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);

// When we setup the NeoPixel library, we tell it how many pixels, and which pin to use to send signals.
// Note that for older NeoPixel strips you might need to change the third parameter--see the strandtest
// example for more information on possible values.
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, LED, NEO_GRB + NEO_KHZ800);

WiFiClient wifi;
int status = WL_IDLE_STATUS;     // the Wifi radio's status
PubSubClient client(wifi);

unsigned long lastMillis = 0;

//connection functions
//wifi
void connect_WIFI(){
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(MYSSID);
  WiFi.begin(MYSSID, PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  randomSeed(micros());
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

//mqtt
void reconnect_MQTT() {
  // Check wifi:
  if (WiFi.status() != WL_CONNECTED) {
    connect_WIFI();
  }
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a client ID
    String clientId = (String)DOOR;
    // Attempt to connect
//    if (client.connect(clientId.c_str())) {
    if (client.connect(clientId.c_str(),MQUSER,MQPASS)) {
      Serial.println("connected to mqtt broker");
      //Once connected, publish an announcement...
//      client.publish("/icircuit/presence/ESP32/", "hello world");
      // ... and resubscribe
      client.subscribe(D_SUB);
      new_led_state = state;
      //led_state = state;
      led_change_time = 0;
      manage_led();
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void callback(char* topic, byte *payload, unsigned int length) {
  Serial.println("-------new message from broker-----");
  Serial.print("channel:");
  Serial.println(topic);
  Serial.print("data:");  
  Serial.write(payload, length);
  Serial.println();
  payload[length] = '\0';
  String s = String((char*)payload);
  if (s == "1"){
    state = STATE_TRIGGER;
    led_state = state;
  }else{
    led_state = DENIED;
    #ifdef debug
      Serial.println("no, open it yourself");
    #endif
  }
}

void setup() {
  // initialize serial for debugging
  Serial.begin(115200);
  //setup digital pins
  pinMode(RELAY, OUTPUT);
  digitalWrite(RELAY, LOW);
  pinMode(LED, OUTPUT);
  digitalWrite(LED, LOW);
  pinMode(SW_OPEN, INPUT_PULLUP);
  pinMode(SW_CLOSED, INPUT_PULLUP);

  connect_WIFI();
  client.setServer(mqttServer, mqttPort);
  client.setCallback(callback);
  reconnect_MQTT();
  pixels.begin();
}

void check_state(){
  //if SW_OPEN is LOW (and SW_CLOSED is HIGH) door is open and vice versa. Unkown if not in either of these
  int open_reed = digitalRead(SW_OPEN);
  int closed_reed = digitalRead(SW_CLOSED);
  if ((open_reed == LOW) && (closed_reed == HIGH)){
    door_state = STATE_OPEN;
  }
  else if ((open_reed == HIGH) && (closed_reed == LOW)){
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
//    https://pubsubclient.knolleary.net/api.html#publish2
    if (!client.connected()) {
      reconnect_MQTT();
    }
    client.publish(DOOR_STATE, doorStates[door_state], true);
    prev_door_state = door_state;
    //manage_led();
  }
}

void send_pin(String pin){
  //  have to convert String to char array for mqtt lib
  int str_len = pin.length() + 1;
  // Prepare the character array (the buffer) 
  char char_array[str_len];
  // Copy it over 
  pin.toCharArray(char_array, str_len);
  //boolean publish(const String &topic, const String &payload, bool retained, int qos);
  if (client.publish(DOOR_PUB, char_array, false)){
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
      break;
    case STATE_IDLE:
      set_led(NUMPIXELS, 0, 0, 0);
      set_led(2, 20, 0, 0);
      break;
    case KEY_IN:
      set_led(NUMPIXELS, 0, 0, 0);
      set_led(disp_pix, 0, 50, 0);
      prev_disp_pix = disp_pix;
      break;
    case STATE_TRIGGER:
      set_led(NUMPIXELS, 0, 50, 50);
      new_led_state = STATE_IDLE;
//      led_state = STATE_IDLE;
      led_change_time = 0;
      break;
    case DENIED:
      set_led(NUMPIXELS, 50, 0, 50);
      break;
  }
  pixels.show(); // This sends the updated pixel color to the hardware.
  prev_led_state = led_state;
}

void loop() {
//  Serial.print("Led_state is ");
//  Serial.println(led_state);
  client.loop();
  //delay(100);
  //Serial.println(state);
  if (!client.connected()) {
    led_state = NOT_CONN;
    reconnect_MQTT();
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
    //Serial.println("I'm managing hte LED");
    manage_led();
  }
  if (led_state == DENIED) {
    if (led_time == 0) {
      led_time = millis();
    }
    if (millis() - led_time > RELAY_TRIG) {
      new_led_state = STATE_IDLE;
//      led_state = STATE_IDLE;
      led_time = 0;
    }
  }
  //manage delay for changing led state
  if (new_led_state) {
    if (led_change_time == 0) {
      led_change_time = millis();
    }
    if (millis() - led_change_time > LED_CHANGE_TIME) {
      led_state = new_led_state;
      new_led_state = NULL;
    }
  }
  //check_state();
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
