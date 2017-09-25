#include <SPI.h>
#include "WiFiEsp.h"
#include <WebSocketClient.h>
#include <ArduinoJson.h>

const char door[] = "topgarage";

// Here we define a maximum framelength to 64 bytes. Default is 256.
//#define MAX_FRAME_LENGTH 64

// Define how many callback functions you have. Default is 1.
#define CALLBACK_FUNCTIONS 1

// Emulate Serial1 on pins 6/7 if not present
#ifndef HAVE_HWSERIAL1
#include "SoftwareSerial.h"
SoftwareSerial Serial1(6, 7); // RX, TX
#endif

char ssid[] = "insecure";            // your network SSID (name)
char pass[] = "blah";        // your network password
int status = WL_IDLE_STATUS;     // the Wifi radio's status

char server[] = "skibo.duckdns.org";
int port = 443;
char serverPath[] = "/websocket";

unsigned long lastConnectionTime = 0;         // last time you connected to the server, in milliseconds
const unsigned long postingInterval = 10000L; // delay between updates, in milliseconds

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
    Serial.print("Attempting to connect to WPA SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network
    status = WiFi.begin(ssid, pass);
  }

  // you're connected now, so print out the data
  Serial.println("You're connected to the network");
  
  printWifiStatus();


  // Connect to the websocket server
  if (client.connectSSL(server, port)) {
    Serial.println("Connected");
  } else {
    Serial.println("Connection failed.");
    while(1) {
      // Hang on failure
    }
  }

  // Handshake with the server
  webSocketClient.path = serverPath;
  webSocketClient.host = server;

  if (webSocketClient.handshake(client)) {
    Serial.println("Handshake successful");
  } else {
    Serial.println("Handshake failed.");
    while(1) {
      // Hang on failure
    }
  }
}

void loop() {
  String data;
  //set buffer for json
  StaticJsonBuffer<100> jsonBuffer;

  //build json object tree in memmory
  JsonObject& root = jsonBuffer.createObject();
  //input key
  root["door"] = door;
  root["pincode"] = "1234";

  if (client.connected()) {
    int len = root.measureLength();
    //add 1 to len (as it comes up short)
    len = len + 1;
    char json[len];
    root.printTo(json, sizeof(json));
    webSocketClient.sendData(json);

    webSocketClient.getData(data);

    if (data.length() > 0) {
      Serial.print("Received data: ");
      Serial.println(data);
    }

  } else {

    Serial.println("Client disconnected.");
    while (1) {
      // Hang on disconnect.
    }
  }

  // wait to fully let the client disconnect
  delay(3000);
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
