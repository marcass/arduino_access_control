#include <SPI.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include "secrets.h"
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

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


unsigned long         relay_time = 0;
const unsigned long   RELAY_TRIG = 300;
const byte            STATE_IDLE = 1;
const byte            STATE_TRIGGER = 2;
byte                  state = STATE_IDLE;
#define               RELAY 26
#define               SW_OPEN 25
#define               SW_CLOSED 16

//door states
const int             STATE_OPEN = 0;
const int             STATE_CLOSED = 1;
const int             STATE_UNKNOWN = 2;
const int             STATE_ERROR = 3;
int                   door_state = STATE_UNKNOWN;
int                   prev_door_state;

//*********************** Setup dislplay ************
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET -1  // GPIO0

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins) adn no reset pin
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

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
    if (client.connect(clientId.c_str(),willTopic, 0, true, willMessage)) {
      Serial.println("connected to mqtt broker");
      //Once connected, publish an empty message to offline topic...
      client.publish(willTopic, "online", true);
      // ... and resubscribe
      client.subscribe(D_SUB);
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
  }else{
    #ifdef debug
      Serial.println("no, open it yourself");
    #endif
  }
}

void setup() {
  // initialize serial for debugging
  Serial.begin(115200);
  //Initialise display
  // Start I2C Communication SDA = 5 and SCL = 4 on Wemos Lolin32 ESP32 with built-in SSD1306 OLED
  Wire.begin(5, 4);
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C, false, false)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  delay(2000); // Pause for 2 seconds
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  // Display static text
  display.print("Setting up ");
  display.println(thisDOOR);
  display.display(); 
  delay(2000);
  //setup digital pins
  pinMode(RELAY, OUTPUT);
  digitalWrite(RELAY, LOW);
  pinMode(SW_OPEN, INPUT_PULLUP);
  pinMode(SW_CLOSED, INPUT_PULLUP);

  connect_WIFI();
  client.setServer(mqttServer, mqttPort);
  client.setCallback(callback);
  reconnect_MQTT();
}

void check_state(){
  //if SW_OPEN is LOW (and SW_CLOSED is HIGH) door is open and vice versa. Unkown if not in either of these
  int open_reed = digitalRead(SW_OPEN);
  int closed_reed = digitalRead(SW_CLOSED);
  if ((open_reed == LOW) && (closed_reed == HIGH)){
//  if (open_reed == LOW){
    door_state = STATE_OPEN;
  }
  else if ((open_reed == HIGH) && (closed_reed == LOW)){
//  else if (closed_reed == LOW){
    door_state = STATE_CLOSED;
  }
  else if ((open_reed == HIGH) && (closed_reed == HIGH)){
    door_state = STATE_UNKNOWN;
  }else {
    door_state- STATE_ERROR;
  }
  if (door_state != prev_door_state){
    char* doorStates[] = {"open", "closed", "unknown", "error"};
    #ifdef debug
      Serial.print("Publishing state change to: ");
      Serial.println(doorStates[door_state]);
      display.clearDisplay();
      display.setTextSize(1);
      display.setTextColor(WHITE);
      display.setCursor(0, 0);
      display.print("State is ");
      display.println(doorStates[door_state]);
      display.setCursor(0, 20);
      display.print("Open sensor = ");
      display.println(open_reed);
      display.setCursor(0, 40);
      display.print("Closed sensor = ");
      display.println(closed_reed);
      display.display();
    #endif
//    https://pubsubclient.knolleary.net/api.html#publish2
    if (!client.connected()) {
      reconnect_MQTT();
    }
    client.publish(DOOR_STATE, doorStates[door_state], true);
    prev_door_state = door_state;
    delay(1000);
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
  }else{
    digitalWrite(RELAY, HIGH);
  }
}

void loop() {
  client.loop();
  //delay(100);
  //Serial.println(state);
  if (!client.connected()) {
    reconnect_MQTT();
  }
  switch (state){
    case STATE_IDLE:
      check_state();
      break;
    case STATE_TRIGGER:
      open_door();
      break;
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
