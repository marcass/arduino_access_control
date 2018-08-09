#include <WiFi.h>
//#include <WiFi101.h>
//don't use arduin http client, use this: https://github.com/espressif/arduino-esp32/blob/51a4432ca8e71be202358ceb068f3047bb8ad762/libraries/HTTPClient/src/HTTPClient.h
// with instructions: https://techtutorialsx.com/2017/05/19/esp32-http-get-requests/
#include <HTTPClient.h>
#include "secrets.h"
#include <OneWire.h>
#include <DallasTemperature.h>
#include <ArduinoJson.h>

#define test
//#define testssl
//#define prod

// Data wire is plugged into pin 2 on the Arduino
#define ONE_WIRE_BUS 15
OneWire oneWire(ONE_WIRE_BUS);
// Pass our oneWire reference to Dallas Temperature.
DallasTemperature sensors(&oneWire);
const char sensorID[] = "hall";
String curToken;
float thisTemp;
String Token;

///////please enter your sensitive data in the Secret tab/secrets.h
/////// Wifi Settings ///////
const char* ssid = MYSSID;
const char* password = PASS;

// CA details for https:
// https://techtutorialsx.com/2017/11/18/esp32-arduino-https-get-request/
const char* root_ca= \
"-----BEGIN CERTIFICATE-----\n" \
"MIIDSjCCAjKgAwIBAgIQRK+wgNajJ7qJMDmGLvhAazANBgkqhkiG9w0BAQUFADA/\n" \
"MSQwIgYDVQQKExtEaWdpdGFsIFNpZ25hdHVyZSBUcnVzdCBDby4xFzAVBgNVBAMT\n" \
"DkRTVCBSb290IENBIFgzMB4XDTAwMDkzMDIxMTIxOVoXDTIxMDkzMDE0MDExNVow\n" \
"PzEkMCIGA1UEChMbRGlnaXRhbCBTaWduYXR1cmUgVHJ1c3QgQ28uMRcwFQYDVQQD\n" \
"Ew5EU1QgUm9vdCBDQSBYMzCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEB\n" \
"AN+v6ZdQCINXtMxiZfaQguzH0yxrMMpb7NnDfcdAwRgUi+DoM3ZJKuM/IUmTrE4O\n" \
"rz5Iy2Xu/NMhD2XSKtkyj4zl93ewEnu1lcCJo6m67XMuegwGMoOifooUMM0RoOEq\n" \
"OLl5CjH9UL2AZd+3UWODyOKIYepLYYHsUmu5ouJLGiifSKOeDNoJjj4XLh7dIN9b\n" \
"xiqKqy69cK3FCxolkHRyxXtqqzTWMIn/5WgTe1QLyNau7Fqckh49ZLOMxt+/yUFw\n" \
"7BZy1SbsOFU5Q9D8/RhcQPGX69Wam40dutolucbY38EVAjqr2m7xPi71XAicPNaD\n" \
"aeQQmxkqtilX4+U9m5/wAl0CAwEAAaNCMEAwDwYDVR0TAQH/BAUwAwEB/zAOBgNV\n" \
"HQ8BAf8EBAMCAQYwHQYDVR0OBBYEFMSnsaR7LHH62+FLkHX/xBVghYkQMA0GCSqG\n" \
"SIb3DQEBBQUAA4IBAQCjGiybFwBcqR7uKGY3Or+Dxz9LwwmglSBd49lZRNI+DT69\n" \
"ikugdB/OEIKcdBodfpga3csTS7MgROSR6cz8faXbauX+5v3gTt23ADq1cEmv8uXr\n" \
"AvHRAosZy5Q6XkjEGB5YGV8eAlrwDPGxrancWYaLbumR9YbK+rlmM6pZW87ipxZz\n" \
"R8srzJmwN0jP41ZL9c8PDHIyh8bwRLtTcm1D9SZImlJnt1ir/md2cXjbDaJWFBM5\n" \
"JDGFoqgCWjBH4d1QB7wCCZAA62RjYJsWvIjJEubSfZGL+T0yjWW06XyxV3bqxbYo\n" \
"Ob8VZRzI9neWagqNdwvYkQsEjgfbKbYK7p2CNTUQ\n" \
"-----END CERTIFICATE-----\n";

WiFiClient wifi;
HTTPClient http;
int status = WL_IDLE_STATUS;
String response;
int statusCode = 0;

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
  Serial.begin(115200);
  delay(2000);
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
  Token = getAuth();
}

String getAuth() {
  String payload;
  //Ensure token fits in here
  StaticJsonBuffer<200> jsonBuffer;
  JsonObject& creds = jsonBuffer.createObject();
  creds["username"] = API_user;
  creds["password"] = API_pass;
  creds.printTo(Serial);
  http.begin(SERVER_443_auth, root_ca);
  http.addHeader("Content-Type", "application/json");
  String input;
  creds.printTo(input);
  int httpCode = http.POST(input);
  // httpCode will be negative on error
  if(httpCode > 0) {
    // HTTP header has been send and Server response header has been handled
    Serial.print("HTTP code = ");
    Serial.println(httpCode);
    // file found at server
    if(httpCode == HTTP_CODE_OK) {
        payload = http.getString();
        Serial.println(payload);
    }
  }else{
    Serial.printf("[HTTP] POST... failed, error: %s\n", http.errorToString(httpCode).c_str());
  }
  http.end();;
//  parse jwt here
//https://github.com/bblanchon/ArduinoJson
  int len = payload.length();
  char json[len];
  payload.toCharArray(json, len);
  JsonObject& root = jsonBuffer.parseObject(json);
  if (!root.success()) {
    Serial.println("parseObject() failed");
  }
  const char* jwt_token = root["access_token"];
  Serial.println(jwt_token);
//  return String(jwt_token);
  return "Bearer "+ String(jwt_token);
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

void updateAPI() {
  //Ensure token fits in here
  StaticJsonBuffer<500> jsonBuffer;
  //build json object
  Serial.print("SensorID is ");
  Serial.println(sensorID);
  Serial.print("temp is ");
  Serial.println(thisTemp);
  JsonObject& root = jsonBuffer.createObject();
  root["type"] = "temp";
  root["group"] = "julian";
  root["value"] = thisTemp;
  root["sensor"] = sensorID;
  root.printTo(Serial);
  Serial.println();
  Serial.println("making POST request");
  http.begin(SERVER_443_data, root_ca);
  http.addHeader("Authorization", Token);
  http.addHeader("Content-Type", "application/json");
  String input;
  root.printTo(input);
  int httpCode = http.POST(input);
  // httpCode will be negative on error
  if(httpCode > 0) {
    // HTTP header has been send and Server response header has been handled
    Serial.print("HTTP code = ");
    Serial.println(httpCode);
    if(httpCode == 401){
    //token expired so get a new one
      Token = getAuth();
    }
    // file found at server
    if(httpCode == HTTP_CODE_OK) {
        String payload = http.getString();
        Serial.println(payload);
    }
  }else{
    Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
  }
  http.end();
}

void loop() {
  #ifdef test
    thisTemp = temp();
    Serial.println(thisTemp);
    updateAPI();
//    http.begin(SERVER_80);
//    Serial.println(Token);
//    http.addHeader("Authorization", Token);
//    int httpCode = http.GET();
//    if (httpCode > 0) { //Check for the returning code 
//        String payload = http.getString();
//        Serial.println(httpCode);
//        Serial.println(payload);
//      }else {
//      Serial.println("Error on HTTP request");
//    }
//    http.end(); //Free the resources
//    thisTemp = thisTemp + 0.50;
//    Serial.print(thisTemp);
    delay(10000);
  #endif
  #ifdef testssl
    http.begin(SERVER_443, root_ca); //Specify the URL and certificate
    int httpCode = http.GET();
    if (httpCode > 0) { //Check for the returning code 
        String payload = http.getString();
        Serial.println(httpCode);
        Serial.println(payload);
      }else {
      Serial.println("Error on HTTP request");
    } 
    http.end(); //Free the resources
    delay(10000);
  #endif
//  getAuth();
//  float thisTemp = temp();
//  Serial.print("Temperature for Device 1 is: ");
//  Serial.println(thisTemp);
//  int resp = updateAPI();
//  if (resp == 200) { //Consider testing for 401
//    Serial.println("Data successfully posted");
//  }else if (resp == 401){
//    getAuth();
//    resp = updateAPI();
//  }
//  delay(5000);
}
