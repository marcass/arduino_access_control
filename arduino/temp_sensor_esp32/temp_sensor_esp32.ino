//#include <WiFi.h>
#include <WiFi101.h>
#include <ArduinoHttpClient.h>
#include "secrets.h"
#include <OneWire.h>
#include <DallasTemperature.h>

// Data wire is plugged into pin 2 on the Arduino
#define ONE_WIRE_BUS 2
// Pass our oneWire reference to Dallas Temperature.
DallasTemperature sensors(&oneWire);
float temp;
String sensorID = "hall";
String jwtHeader;
String 

///////please enter your sensitive data in the Secret tab/secrets.h
/////// Wifi Settings ///////
char ssid[] = SECRET_SSID;
char pass[] = SECRET_PASS;
char serverAddress[] = "192.168.0.3";  // server address
int port = 8080;

WiFiClient wifi;
HttpClient client = HttpClient(wifi, serverAddress, port);
int status = WL_IDLE_STATUS;
String response;
int statusCode = 0;

void setup() {
  Serial.begin(9600);
  while ( status != WL_CONNECTED) {
    Serial.print("Attempting to connect to Network named: ");
    Serial.println(ssid);                   // print the network name (SSID);

    // Connect to WPA/WPA2 network:
    status = WiFi.begin(ssid, pass);
    // Start up the temperature measurement library
    sensors.begin();
  }

  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);
}

void getAuth() {
  client.beginRequest();
  client.post("/auth/login");
  String postData = "{username: "+API_user+", password : "+API_pass+"}";
  client.sendHeader(HTTP_HEADER_CONTENT_TYPE, "application/x-www-form-urlencoded");
  client.sendHeader(HTTP_HEADER_CONTENT_LENGTH, postData.length());
  client.endRequest();
  client.write((const byte*)postData.c_str(), postData.length());
  // read the status code and body of the response
  statusCode = client.responseStatusCode();
  response = client.responseBody();

  Serial.print("POST Status code: ");
  Serial.println(statusCode);
  Serial.print("POST Response: ");
  Serial.println(response);
//  parse jwt here
}

void loop() {
  Serial.print("Measuing temp...");
  sensors.requestTemperatures(); // Send the command to get temperatures
  Serial.println("DONE");
  temp = sensors.getTempCByIndex(0)
  Serial.print("Temperature for Device 1 is: ");
  Serial.print(temp); // Why "byIndex"? 
    // You can have more than one IC on the same bus. 
    // 0 refers to the first IC on the wire
  Serial.println("making GET request with HTTP basic authentication");
//  build json
//  char PostData[] = "{\"group\": \"julian\", \"sensor\": sensorID, \"temp\": temp}";
  String postData = "{group: julian, sensor: "+sensorID+", temp:" +temp+"}";
  String contentType = "application/x-www-form-urlencoded";
  client.beginRequest();
  client.post("/data  ");
//  https://github.com/arduino-libraries/ArduinoHttpClient/blob/master/examples/CustomHeader/CustomHeader.ino up to here
//  Creds in secrets.h
  client.sendBasicAuth(API_user, API_pass); // send the username and password for authentication
  client.endRequest();

  // read the status code and body of the response
  statusCode = client.responseStatusCode();
  response = client.responseBody();

  Serial.print("Status code: ");
  Serial.println(statusCode);
  Serial.print("Response: ");
  Serial.println(response);
  Serial.println("Wait five seconds");
  delay(5000);
}
