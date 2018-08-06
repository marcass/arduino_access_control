#include <WiFi.h>
//#include <WiFi101.h>
#include <ArduinoHttpClient.h>
#include "secrets.h"
#include <OneWire.h>
#include <DallasTemperature.h>
#include <ArduinoJson.h>

// Data wire is plugged into pin 2 on the Arduino
#define ONE_WIRE_BUS 2
OneWire oneWire(ONE_WIRE_BUS);
// Pass our oneWire reference to Dallas Temperature.
DallasTemperature sensors(&oneWire);
String sensorID = "hall";
String token;

///////please enter your sensitive data in the Secret tab/secrets.h
/////// Wifi Settings ///////
const char* ssid = SECRET_SSID;
const char* password = SECRET_PASS;
char serverAddress[] = API_SERVER;  // server address
int port = API_PORT;

WiFiClient wifi;
HttpClient client = HttpClient(wifi, serverAddress, port);
int status = WL_IDLE_STATUS;
String response;
int statusCode = 0;
//Ensure token fits in here
StaticJsonBuffer<200> jsonBuffer;

void connectWifi(){
  WiFi.begin (ssid, password);
  Serial.print("Attempting to connect to Network named: ");
  Serial.println(ssid);
  WiFi.mode(WIFI_STA);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
}

void setup() {
  Serial.begin(9600);
  connectWifi();
  // Start up the temperature measurement library
  sensors.begin();
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
//https://github.com/bblanchon/ArduinoJson
  int len = response.length();
  char json[len];
  response.toCharArray(json, len);
  JsonObject& root = jsonBuffer.parseObject(json);
  if (!root.success()) {
    Serial.println("parseObject() failed");
    return;
  }
  const char* jwt_token = root["access_token"];
  token = String(jwt_token);
}

float temp() {
  Serial.print("Measuing temp...");
  sensors.requestTemperatures(); // Send the command to get temperatures
  Serial.println("DONE");
  // Why "byIndex"? 
  // You can have more than one IC on the same bus. 
  // 0 refers to the first IC on the wire
//  float intemp = sensors.getTempCByIndex(0);
//  // Convert to string
//  char buffer[12];
//  return itoa(intemp,buffer,10); //(integer, yourBuffer, base)
  return sensors.getTempCByIndex(0);
}

int updateAPI() {
  Serial.println("making GET request with HTTP basic authentication");
//  build json
  char PostData[] = "{\"group\": \"julian\", \"sensor\": sensorID, \"temp\": temp}";
  String postData = String(PostData);
//  String postData = "{group: julian, sensor: "+sensorID+", temp:" +temp+"}";
//  String contentType = "application/x-www-form-urlencoded";
  client.beginRequest();
  client.post("/data");
//  client.sendHeader(HTTP_HEADER_CONTENT_TYPE, "application/x-www-form-urlencoded");
  client.sendHeader(HTTP_HEADER_CONTENT_TYPE, "application/json");
  client.sendHeader(HTTP_HEADER_CONTENT_LENGTH, postData.length());
//  client.sendHeader(HTTP_HEADER_CONTENT_LENGTH, sizeof(postData));
  client.sendHeader("X-CUSTOM-HEADER", "Authorization: Bearer "+token);
//  https://github.com/arduino-libraries/ArduinoHttpClient/blob/master/examples/CustomHeader/CustomHeader.ino up to here
//  Creds in secrets.h
//  client.sendBasicAuth(API_user, API_pass); // send the username and password for authentication
  client.endRequest();
  client.write((const byte*)postData.c_str(), postData.length());
//  client.write((const byte*)postData.c_str(), sizeof(postData));
  // read the status code and body of the response
  statusCode = client.responseStatusCode();
  response = client.responseBody();
  Serial.print("Status code: ");
  Serial.println(statusCode);
  Serial.print("Response: ");
  Serial.println(response);
  Serial.println("Wait five seconds");
  return statusCode;
}

void loop() {
  float thisTemp = temp();
  Serial.print("Temperature for Device 1 is: ");
  Serial.print(thisTemp);
  int resp = updateAPI();
  if (resp == 200) { //Consider testing for 401
    Serial.println("Data successfully posted");
  }else if (resp == 401){
    getAuth();
    resp = updateAPI();
  }
  delay(5000);
}
