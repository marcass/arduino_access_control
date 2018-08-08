#include <WiFi.h>
//#include <WiFi101.h>
//don't use arduin http client, use this: https://github.com/espressif/arduino-esp32/blob/51a4432ca8e71be202358ceb068f3047bb8ad762/libraries/HTTPClient/src/HTTPClient.h
// with instructions: https://techtutorialsx.com/2017/05/19/esp32-http-get-requests/
#include <HttpClient.h>
#include "secrets.h"
#include <OneWire.h>
#include <DallasTemperature.h>
#include <ArduinoJson.h>

#define test
//#define testssl
//#define prod

// Data wire is plugged into pin 2 on the Arduino
#define ONE_WIRE_BUS 2
OneWire oneWire(ONE_WIRE_BUS);
// Pass our oneWire reference to Dallas Temperature.
DallasTemperature sensors(&oneWire);
String sensorID = "hall";
char token[] = "blah";
float thisTemp;
const char auth_start[] = "Authorization: Bearer ";
char auth_header[200];

///////please enter your sensitive data in the Secret tab/secrets.h
/////// Wifi Settings ///////
const char* ssid = SECRET_SSID;
const char* password = SECRET_PASS;
//char serverAddress[] = API_SERVER;  // server address
//int port = API_PORT;
#define SERVER_80 "http://houseslave"
#define SERVER_443 "https://skibo.duckdns.org"

// CA details for https:
// https://techtutorialsx.com/2017/11/18/esp32-arduino-https-get-request/
const char* test_root_ca= \
  "-----BEGIN CERTIFICATE-----\n" \
  "MIIGDTCCBPWgAwIBAgISAxEJX04zgM2AaLHfb/GzCXpaMA0GCSqGSIb3DQEBCwUA\n" \
  "MEoxCzAJBgNVBAYTAlVTMRYwFAYDVQQKEw1MZXQncyBFbmNyeXB0MSMwIQYDVQQD\n" \
  "ExpMZXQncyBFbmNyeXB0IEF1dGhvcml0eSBYMzAeFw0xODA3MDQxOTQ1MjVaFw0x\n" \
  "ODEwMDIxOTQ1MjVaMBwxGjAYBgNVBAMTEXNraWJvLmR1Y2tkbnMub3JnMIIBIjAN\n" \
  "BgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEAvK9zUIdWI6QABIwsuGd3H+8Vjg9O\n" \
  "s09x852RvANt1ZTkQfZcNp+Wl3cj49MYWpKZem8SbDsbUg1wdVXutEnMuvPqH+5k\n" \
  "koD6FGPF+bk1uQU2RgOe3JlIoxe3IvJZB44i78dLoAzQVLc3uH+38Qe2hgheW1o/\n" \
  "nfJuXjedSAu9yh84bf0P6fERl0xiPEprjWIjvhwOIoWf+fl+W6uWxAH9PiKoNIxO\n" \
  "jgtj6Gz4Wq1dSx9VrZYOeJgaU5zk6eK+VNQ7XEyYYgPn3896IDt0gT3Grr5IJ9my\n" \
  "azlnn6TUd2iiWbdVszkQM+RER3pwEge/Kl0McX8vuHk/giG91rMNBPCJ3wIDAQAB\n" \
  "o4IDGTCCAxUwDgYDVR0PAQH/BAQDAgWgMB0GA1UdJQQWMBQGCCsGAQUFBwMBBggr\n" \
  "BgEFBQcDAjAMBgNVHRMBAf8EAjAAMB0GA1UdDgQWBBQlUje6dtyzfkjMLlZtsCec\n" \
  "ST/VjTAfBgNVHSMEGDAWgBSoSmpjBH3duubRObemRWXv86jsoTBvBggrBgEFBQcB\n" \
  "AQRjMGEwLgYIKwYBBQUHMAGGImh0dHA6Ly9vY3NwLmludC14My5sZXRzZW5jcnlw\n" \
  "dC5vcmcwLwYIKwYBBQUHMAKGI2h0dHA6Ly9jZXJ0LmludC14My5sZXRzZW5jcnlw\n" \
  "dC5vcmcvMBwGA1UdEQQVMBOCEXNraWJvLmR1Y2tkbnMub3JnMIH+BgNVHSAEgfYw\n" \
  "gfMwCAYGZ4EMAQIBMIHmBgsrBgEEAYLfEwEBATCB1jAmBggrBgEFBQcCARYaaHR0\n" \
  "cDovL2Nwcy5sZXRzZW5jcnlwdC5vcmcwgasGCCsGAQUFBwICMIGeDIGbVGhpcyBD\n" \
  "ZXJ0aWZpY2F0ZSBtYXkgb25seSBiZSByZWxpZWQgdXBvbiBieSBSZWx5aW5nIFBh\n" \
  "cnRpZXMgYW5kIG9ubHkgaW4gYWNjb3JkYW5jZSB3aXRoIHRoZSBDZXJ0aWZpY2F0\n" \
  "ZSBQb2xpY3kgZm91bmQgYXQgaHR0cHM6Ly9sZXRzZW5jcnlwdC5vcmcvcmVwb3Np\n" \
  "dG9yeS8wggEEBgorBgEEAdZ5AgQCBIH1BIHyAPAAdgDbdK/uyynssf7KPnFtLOW5\n" \
  "qrs294Rxg8ddnU83th+/ZAAAAWRnCugLAAAEAwBHMEUCIE3zA7vs05ofLR/Rk61D\n" \
  "D/tL0UsD68RaIGksvUl/CMXsAiEAhEV52zlTfhJKp2HfojwJIKyfkbjQ+t6SVNWC\n" \
  "nOQRfVgAdgApPFGWVMg5ZbqqUPxYB9S3b79Yeily3KTDDPTlRUf0eAAAAWRnCugb\n" \
  "AAAEAwBHMEUCIQCoNpRTM5fprKtUEPABNMDfXdVDgudNovjSY2meOr+DAgIgVcpE\n" \
  "rDL9wReqjFrSn9re4WNVkp2oGL1XFxeZTzmGWTQwDQYJKoZIhvcNAQELBQADggEB\n" \
  "ADIOSyELErhxe5wihLiQkORg3DgrBmN/TdpbcsJGpv7XiXe+fTZkikTmaIPyrXpV\n" \
  "C7BdubS/23RdEKhM40NTn11mV3E/KEC3eAuZpe8X/GhH5dW1zEp32qAHCvUI9PBG\n" \
  "hDwgdbE9YmUNLgxECHtm7kFDnne5VpN9KAYGvP9i+GA/jEGqpdIZJsvO5Ig75dO/\n" \
  "rsgC95AuXyspf9ElxIvaNKVCpTQvcxp0fiuLymbAHE82rSIcWbnezpLlvTtqHgU8\n" \
  "plSEFaR3VgpHudhiM0JkCK6zPJilKHLwQk88H0etfOh9r7McmNo5wtKpQvLXTxM/\n" \
  "UwgVeTkHC9trs74WsJ5fsrk=\n" \
  "-----END CERTIFICATE-----";


WiFiClient wifi;
HttpClient http;
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
}

void getAuth() {
  client.beginRequest();
  client.post("/auth/login");
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

void updateAPI() {
  //build json object
  JsonObject& root = jsonBuffer.createObject();
  root["type"] = "temp";
  root["group"] = "julian";
  root["sensor"] = sensorID;
  root["value"] = thisTemp;
  root.printTo(Serial);
  Serial.println();
  Serial.println("making POST request");
  http.begin(SERVER_443_data);
  sprintf(auth_header, "%s = %s, auth_start, token);
  Serial.println(auth_header);
  http.setAuthorization(auth_header);
  root.prettyPrintTo(http);
  int httpCode = http.POST();
  // httpCode will be negative on error
  if(httpCode > 0) {
      // HTTP header has been send and Server response header has been handled
      Serial.println("[HTTP] GET... code: %d\n", httpCode);
      // file found at server
      if(httpCode == HTTP_CODE_OK) {
          String payload = http.getString();
          Serial.println(payload);
      }
  } else {
      Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
  }

  http.end();

}

void loop() {
  #ifdef test
    thisTemp = 19.13;
    http.begin(SERVER_80);
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
