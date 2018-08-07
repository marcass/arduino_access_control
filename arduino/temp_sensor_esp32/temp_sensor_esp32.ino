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
String token = "blah";
float thisTemp;

///////please enter your sensitive data in the Secret tab/secrets.h
/////// Wifi Settings ///////
const char* ssid = SECRET_SSID;
const char* password = SECRET_PASS;
char serverAddress[] = API_SERVER;  // server address
int port = API_PORT;

// CA details for https:
// https://techtutorialsx.com/2017/11/18/esp32-arduino-https-get-request/
//const char* root_ca = \
//"-----BEGIN CERTIFICATE-----
//MIIGDTCCBPWgAwIBAgISAxEJX04zgM2AaLHfb/GzCXpaMA0GCSqGSIb3DQEBCwUA
//MEoxCzAJBgNVBAYTAlVTMRYwFAYDVQQKEw1MZXQncyBFbmNyeXB0MSMwIQYDVQQD
//ExpMZXQncyBFbmNyeXB0IEF1dGhvcml0eSBYMzAeFw0xODA3MDQxOTQ1MjVaFw0x
//ODEwMDIxOTQ1MjVaMBwxGjAYBgNVBAMTEXNraWJvLmR1Y2tkbnMub3JnMIIBIjAN
//BgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEAvK9zUIdWI6QABIwsuGd3H+8Vjg9O
//s09x852RvANt1ZTkQfZcNp+Wl3cj49MYWpKZem8SbDsbUg1wdVXutEnMuvPqH+5k
//koD6FGPF+bk1uQU2RgOe3JlIoxe3IvJZB44i78dLoAzQVLc3uH+38Qe2hgheW1o/
//nfJuXjedSAu9yh84bf0P6fERl0xiPEprjWIjvhwOIoWf+fl+W6uWxAH9PiKoNIxO
//jgtj6Gz4Wq1dSx9VrZYOeJgaU5zk6eK+VNQ7XEyYYgPn3896IDt0gT3Grr5IJ9my
//azlnn6TUd2iiWbdVszkQM+RER3pwEge/Kl0McX8vuHk/giG91rMNBPCJ3wIDAQAB
//o4IDGTCCAxUwDgYDVR0PAQH/BAQDAgWgMB0GA1UdJQQWMBQGCCsGAQUFBwMBBggr
//BgEFBQcDAjAMBgNVHRMBAf8EAjAAMB0GA1UdDgQWBBQlUje6dtyzfkjMLlZtsCec
//ST/VjTAfBgNVHSMEGDAWgBSoSmpjBH3duubRObemRWXv86jsoTBvBggrBgEFBQcB
//AQRjMGEwLgYIKwYBBQUHMAGGImh0dHA6Ly9vY3NwLmludC14My5sZXRzZW5jcnlw
//dC5vcmcwLwYIKwYBBQUHMAKGI2h0dHA6Ly9jZXJ0LmludC14My5sZXRzZW5jcnlw
//dC5vcmcvMBwGA1UdEQQVMBOCEXNraWJvLmR1Y2tkbnMub3JnMIH+BgNVHSAEgfYw
//gfMwCAYGZ4EMAQIBMIHmBgsrBgEEAYLfEwEBATCB1jAmBggrBgEFBQcCARYaaHR0
//cDovL2Nwcy5sZXRzZW5jcnlwdC5vcmcwgasGCCsGAQUFBwICMIGeDIGbVGhpcyBD
//ZXJ0aWZpY2F0ZSBtYXkgb25seSBiZSByZWxpZWQgdXBvbiBieSBSZWx5aW5nIFBh
//cnRpZXMgYW5kIG9ubHkgaW4gYWNjb3JkYW5jZSB3aXRoIHRoZSBDZXJ0aWZpY2F0
//ZSBQb2xpY3kgZm91bmQgYXQgaHR0cHM6Ly9sZXRzZW5jcnlwdC5vcmcvcmVwb3Np
//dG9yeS8wggEEBgorBgEEAdZ5AgQCBIH1BIHyAPAAdgDbdK/uyynssf7KPnFtLOW5
//qrs294Rxg8ddnU83th+/ZAAAAWRnCugLAAAEAwBHMEUCIE3zA7vs05ofLR/Rk61D
//D/tL0UsD68RaIGksvUl/CMXsAiEAhEV52zlTfhJKp2HfojwJIKyfkbjQ+t6SVNWC
//nOQRfVgAdgApPFGWVMg5ZbqqUPxYB9S3b79Yeily3KTDDPTlRUf0eAAAAWRnCugb
//AAAEAwBHMEUCIQCoNpRTM5fprKtUEPABNMDfXdVDgudNovjSY2meOr+DAgIgVcpE
//rDL9wReqjFrSn9re4WNVkp2oGL1XFxeZTzmGWTQwDQYJKoZIhvcNAQELBQADggEB
//ADIOSyELErhxe5wihLiQkORg3DgrBmN/TdpbcsJGpv7XiXe+fTZkikTmaIPyrXpV
//C7BdubS/23RdEKhM40NTn11mV3E/KEC3eAuZpe8X/GhH5dW1zEp32qAHCvUI9PBG
//hDwgdbE9YmUNLgxECHtm7kFDnne5VpN9KAYGvP9i+GA/jEGqpdIZJsvO5Ig75dO/
//rsgC95AuXyspf9ElxIvaNKVCpTQvcxp0fiuLymbAHE82rSIcWbnezpLlvTtqHgU8
//plSEFaR3VgpHudhiM0JkCK6zPJilKHLwQk88H0etfOh9r7McmNo5wtKpQvLXTxM/
//UwgVeTkHC9trs74WsJ5fsrk=
//-----END CERTIFICATE-----";

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
  Serial.begin(115200);
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
//  String postData = "{\"username\": \"+API_user+\"+, password : "+API_pass+"}";
  //build json object
  JsonObject& creds = jsonBuffer.createObject();
  creds["username"] = API_user;
  creds["password"] = API_pass;
  creds.printTo(Serial);
//  Serial.println(postData);
  client.sendHeader(HTTP_HEADER_CONTENT_TYPE, "application/json");
  client.sendHeader(HTTP_HEADER_CONTENT_LENGTH, 200);
  client.endRequest();
  creds.prettyPrintTo(client);
//  client.write((const byte*)postData.c_str(), postData.length());
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
  //build json object
  JsonObject& root = jsonBuffer.createObject();
  root["type"] = "temp";
  root["group"] = "julian";
  root["sensor"] = sensorID;
  root["value"] = thisTemp;
  root.printTo(Serial);
  Serial.println();
  Serial.println("making POST request");
  client.beginRequest();
  client.post("/api/data");
//  client.sendHeader(HTTP_HEADER_CONTENT_TYPE, "application/x-www-form-urlencoded");
  client.sendHeader(HTTP_HEADER_CONTENT_TYPE, "application/json");
  client.sendHeader(HTTP_HEADER_CONTENT_LENGTH, 200); //200 is length of json buffer
//  client.sendHeader(HTTP_HEADER_CONTENT_LENGTH, sizeof(postData));
  client.sendHeader("X-CUSTOM-HEADER", "Authorization: Bearer "+token);
//  https://github.com/arduino-libraries/ArduinoHttpClient/blob/master/examples/CustomHeader/CustomHeader.ino up to here
//  Creds in secrets.h
//  client.sendBasicAuth(API_user, API_pass); // send the username and password for authentication
  client.endRequest();
  //need to do this to get object to client: root.prettyPrintTo(client);
  root.prettyPrintTo(client);
//  client.write((const byte*)postData.c_str(), postData.length());
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
  thisTemp = 19.13;
  getAuth();
//  float thisTemp = temp();
  Serial.print("Temperature for Device 1 is: ");
  Serial.println(thisTemp);
//  int resp = updateAPI();
//  if (resp == 200) { //Consider testing for 401
//    Serial.println("Data successfully posted");
//  }else if (resp == 401){
//    getAuth();
//    resp = updateAPI();
//  }
  delay(5000);
}
