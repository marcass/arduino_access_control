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
#define ONE_WIRE_BUS 2
OneWire oneWire(ONE_WIRE_BUS);
// Pass our oneWire reference to Dallas Temperature.
DallasTemperature sensors(&oneWire);
const char sensorID[] = "hall";
String curToken;
float thisTemp;
const char* Token;

///////please enter your sensitive data in the Secret tab/secrets.h
/////// Wifi Settings ///////
const char* ssid = MYSSID;
const char* password = PASS;
//char serverAddress[] = API_SERVER;  // server address
//int port = API_PORT;

// CA details for https:
// https://techtutorialsx.com/2017/11/18/esp32-arduino-https-get-request/
//const char* root_ca= \
//  "-----BEGIN CERTIFICATE-----\n" \
//  "MIIGDTCCBPWgAwIBAgISAxEJX04zgM2AaLHfb/GzCXpaMA0GCSqGSIb3DQEBCwUA\n" \
//  "MEoxCzAJBgNVBAYTAlVTMRYwFAYDVQQKEw1MZXQncyBFbmNyeXB0MSMwIQYDVQQD\n" \
//  "ExpMZXQncyBFbmNyeXB0IEF1dGhvcml0eSBYMzAeFw0xODA3MDQxOTQ1MjVaFw0x\n" \
//  "ODEwMDIxOTQ1MjVaMBwxGjAYBgNVBAMTEXNraWJvLmR1Y2tkbnMub3JnMIIBIjAN\n" \
//  "BgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEAvK9zUIdWI6QABIwsuGd3H+8Vjg9O\n" \
//  "s09x852RvANt1ZTkQfZcNp+Wl3cj49MYWpKZem8SbDsbUg1wdVXutEnMuvPqH+5k\n" \
//  "koD6FGPF+bk1uQU2RgOe3JlIoxe3IvJZB44i78dLoAzQVLc3uH+38Qe2hgheW1o/\n" \
//  "nfJuXjedSAu9yh84bf0P6fERl0xiPEprjWIjvhwOIoWf+fl+W6uWxAH9PiKoNIxO\n" \
//  "jgtj6Gz4Wq1dSx9VrZYOeJgaU5zk6eK+VNQ7XEyYYgPn3896IDt0gT3Grr5IJ9my\n" \
//  "azlnn6TUd2iiWbdVszkQM+RER3pwEge/Kl0McX8vuHk/giG91rMNBPCJ3wIDAQAB\n" \
//  "o4IDGTCCAxUwDgYDVR0PAQH/BAQDAgWgMB0GA1UdJQQWMBQGCCsGAQUFBwMBBggr\n" \
//  "BgEFBQcDAjAMBgNVHRMBAf8EAjAAMB0GA1UdDgQWBBQlUje6dtyzfkjMLlZtsCec\n" \
//  "ST/VjTAfBgNVHSMEGDAWgBSoSmpjBH3duubRObemRWXv86jsoTBvBggrBgEFBQcB\n" \
//  "AQRjMGEwLgYIKwYBBQUHMAGGImh0dHA6Ly9vY3NwLmludC14My5sZXRzZW5jcnlw\n" \
//  "dC5vcmcwLwYIKwYBBQUHMAKGI2h0dHA6Ly9jZXJ0LmludC14My5sZXRzZW5jcnlw\n" \
//  "dC5vcmcvMBwGA1UdEQQVMBOCEXNraWJvLmR1Y2tkbnMub3JnMIH+BgNVHSAEgfYw\n" \
//  "gfMwCAYGZ4EMAQIBMIHmBgsrBgEEAYLfEwEBATCB1jAmBggrBgEFBQcCARYaaHR0\n" \
//  "cDovL2Nwcy5sZXRzZW5jcnlwdC5vcmcwgasGCCsGAQUFBwICMIGeDIGbVGhpcyBD\n" \
//  "ZXJ0aWZpY2F0ZSBtYXkgb25seSBiZSByZWxpZWQgdXBvbiBieSBSZWx5aW5nIFBh\n" \
//  "cnRpZXMgYW5kIG9ubHkgaW4gYWNjb3JkYW5jZSB3aXRoIHRoZSBDZXJ0aWZpY2F0\n" \
//  "ZSBQb2xpY3kgZm91bmQgYXQgaHR0cHM6Ly9sZXRzZW5jcnlwdC5vcmcvcmVwb3Np\n" \
//  "dG9yeS8wggEEBgorBgEEAdZ5AgQCBIH1BIHyAPAAdgDbdK/uyynssf7KPnFtLOW5\n" \
//  "qrs294Rxg8ddnU83th+/ZAAAAWRnCugLAAAEAwBHMEUCIE3zA7vs05ofLR/Rk61D\n" \
//  "D/tL0UsD68RaIGksvUl/CMXsAiEAhEV52zlTfhJKp2HfojwJIKyfkbjQ+t6SVNWC\n" \
//  "nOQRfVgAdgApPFGWVMg5ZbqqUPxYB9S3b79Yeily3KTDDPTlRUf0eAAAAWRnCugb\n" \
//  "AAAEAwBHMEUCIQCoNpRTM5fprKtUEPABNMDfXdVDgudNovjSY2meOr+DAgIgVcpE\n" \
//  "rDL9wReqjFrSn9re4WNVkp2oGL1XFxeZTzmGWTQwDQYJKoZIhvcNAQELBQADggEB\n" \
//  "ADIOSyELErhxe5wihLiQkORg3DgrBmN/TdpbcsJGpv7XiXe+fTZkikTmaIPyrXpV\n" \
//  "C7BdubS/23RdEKhM40NTn11mV3E/KEC3eAuZpe8X/GhH5dW1zEp32qAHCvUI9PBG\n" \
//  "hDwgdbE9YmUNLgxECHtm7kFDnne5VpN9KAYGvP9i+GA/jEGqpdIZJsvO5Ig75dO/\n" \
//  "rsgC95AuXyspf9ElxIvaNKVCpTQvcxp0fiuLymbAHE82rSIcWbnezpLlvTtqHgU8\n" \
//  "plSEFaR3VgpHudhiM0JkCK6zPJilKHLwQk88H0etfOh9r7McmNo5wtKpQvLXTxM/\n" \
//  "UwgVeTkHC9trs74WsJ5fsrk=\n" \
//  "-----END CERTIFICATE-----";

//const char* root_ca = "-----BEGIN CERTIFICATE-----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-----END CERTIFICATE-----";

const char* root_ca = \
"-----BEGIN CERTIFICATE-----\n" \
"MIIMCjCCC6+gAwIBAgIRAJ+6YOfwhW79vw2rgFt/AFwwCgYIKoZIzj0EAwIwgZIx\n" \
"CzAJBgNVBAYTAkdCMRswGQYDVQQIExJHcmVhdGVyIE1hbmNoZXN0ZXIxEDAOBgNV\n" \
"BAcTB1NhbGZvcmQxGjAYBgNVBAoTEUNPTU9ETyBDQSBMaW1pdGVkMTgwNgYDVQQD\n" \
"Ey9DT01PRE8gRUNDIERvbWFpbiBWYWxpZGF0aW9uIFNlY3VyZSBTZXJ2ZXIgQ0Eg\n" \
"MjAeFw0xODA4MDEwMDAwMDBaFw0xOTAyMDcyMzU5NTlaMGwxITAfBgNVBAsTGERv\n" \
"bWFpbiBDb250cm9sIFZhbGlkYXRlZDEhMB8GA1UECxMYUG9zaXRpdmVTU0wgTXVs\n" \
"dGktRG9tYWluMSQwIgYDVQQDExtzbmkyMzM0MjUuY2xvdWRmbGFyZXNzbC5jb20w\n" \
"WTATBgcqhkjOPQIBBggqhkjOPQMBBwNCAASwROMTb+hGoPnEie/EH/+J6OOxjyuk\n" \
"zTX5gk82mxAg4i74Eo6IwErscW9cNo+W/Vjoutj+g2XwLdw6BansTqvBo4IKCTCC\n" \
"CgUwHwYDVR0jBBgwFoAUQAlhZ/C8g3FP3hIILG/U1Ct2PZYwHQYDVR0OBBYEFMN3\n" \
"DCHL2Q9XR2SfdgFKWMwFoTX/MA4GA1UdDwEB/wQEAwIHgDAMBgNVHRMBAf8EAjAA\n" \
"MB0GA1UdJQQWMBQGCCsGAQUFBwMBBggrBgEFBQcDAjBPBgNVHSAESDBGMDoGCysG\n" \
"AQQBsjEBAgIHMCswKQYIKwYBBQUHAgEWHWh0dHBzOi8vc2VjdXJlLmNvbW9kby5j\n" \
"b20vQ1BTMAgGBmeBDAECATBWBgNVHR8ETzBNMEugSaBHhkVodHRwOi8vY3JsLmNv\n" \
"bW9kb2NhNC5jb20vQ09NT0RPRUNDRG9tYWluVmFsaWRhdGlvblNlY3VyZVNlcnZl\n" \
"ckNBMi5jcmwwgYgGCCsGAQUFBwEBBHwwejBRBggrBgEFBQcwAoZFaHR0cDovL2Ny\n" \
"dC5jb21vZG9jYTQuY29tL0NPTU9ET0VDQ0RvbWFpblZhbGlkYXRpb25TZWN1cmVT\n" \
"ZXJ2ZXJDQTIuY3J0MCUGCCsGAQUFBzABhhlodHRwOi8vb2NzcC5jb21vZG9jYTQu\n" \
"Y29tMIIHSAYDVR0RBIIHPzCCBzuCG3NuaTIzMzQyNS5jbG91ZGZsYXJlc3NsLmNv\n" \
"bYIJKi5hYmVsLmFtggoqLmFxdmFwLmNmghgqLmFzdHJvc3RlYW1mYW5zdG9yZS5j\n" \
"b22CDyouYXR0aXJlaHV0LmNvbYIOKi5iaXphcmVhZHMuZ2GCGSouYmxhY2tmb3Jl\n" \
"c3RkeW5hbWljcy5jb22CGyouYmxhY2tmb3Jlc3RtZXRhbHdvcmtzLmNvbYIOKi5i\n" \
"dXlhY3JneHouZ3GCDSouY2h3aXNnaXAudGuCFiouY29yZWUtZ2FsaXNhbi5ldS5v\n" \
"cmeCECouY3Jvd2RjYXNlcy5vcmeCDiouZGlubHVtdXN1LmdhghYqLmRpc2NvcmQt\n" \
"cmVsb2FkZWQuY29tghQqLmRpc2NvdmVyeWJsb2NrLmNvbYIRKi5kcmFtYXNway5v\n" \
"bmxpbmWCDSouZWZyZWlkb2MuZnKCEyouZWluYXJtYXR0c3NvaG4uZ3GCEyouZXZp\n" \
"ZGVhcmRkaXRpdmEudGuCGyouZ2lhbnRzZmFuc2FwcGFyZWxzaG9wLmNvbYIaKi5o\n" \
"YWNrd2Fsa2luZ3dhcnJvYm90cy53aW6CDSouaG90dmRpbmcuY2aCDSouaG90dmRp\n" \
"bmcuZ3GCEiouaHlyYS1zcGFubmllbi5nYYITKi5pbXBvcnRlcmhzcXVhaC5tbIIO\n" \
"Ki5pc3RlZG9jc2kubWyCFSouanNvbnBsYWNlaG9sZGVyLmNvbYIXKi5rYW1pMmhh\n" \
"Y2tlZG9ubGluZS53aW6CDyoua3VsbGhhZ3Fhcy5ncYIQKi5tYWxpbm5vZXRpby5n\n" \
"YYIOKi5tZXJmaW1hc2kuZ2GCECoubWhhemVvZnRpbWUudGuCDyoubnFvdGJvY2tl\n" \
"ci5jZoIRKi5vbGl2ZWlyYTI5LndvcmuCGyoucGFja2Vyc2NvbGxlY3Rpb25zaG9w\n" \
"LmNvbYIWKi5waGlsbGllc2VkZ2VzaG9wLmNvbYINKi5wbGF1ZGl1by50a4IPKi5w\n" \
"b3Juby12aXAubmV0ghAqLnByaWNldmZxZHdyLm1sghYqLnJhdmVuc2xvY2tlcnJv\n" \
"b20uY29tghYqLnJheXN0ZWFtZmFuc3RvcmUuY29tghAqLnJpbnBpcml0dXJzLmNm\n" \
"ggwqLnJvY29kZS5uZXSCESouc2FuZ2F0bXVyYWgubmV0gg8qLnNvbGp1Y29ucmUu\n" \
"bWyCGCoudGl0YW5zZmFuc2dlYXJzaG9wLmNvbYINKi50b3Ata2FsLmNvbYIRKi50\n" \
"cmFjZGVyY2lsZWEuY2aCDioudHlwaWNvZGUuY29tghEqLnYtdm5ucmV2aWV3cy5j\n" \
"ZoIHYWJlbC5hbYIIYXF2YXAuY2aCFmFzdHJvc3RlYW1mYW5zdG9yZS5jb22CDWF0\n" \
"dGlyZWh1dC5jb22CDGJpemFyZWFkcy5nYYIXYmxhY2tmb3Jlc3RkeW5hbWljcy5j\n" \
"b22CGWJsYWNrZm9yZXN0bWV0YWx3b3Jrcy5jb22CDGJ1eWFjcmd4ei5ncYILY2h3\n" \
"aXNnaXAudGuCFGNvcmVlLWdhbGlzYW4uZXUub3Jngg5jcm93ZGNhc2VzLm9yZ4IM\n" \
"ZGlubHVtdXN1LmdhghRkaXNjb3JkLXJlbG9hZGVkLmNvbYISZGlzY292ZXJ5Ymxv\n" \
"Y2suY29tgg9kcmFtYXNway5vbmxpbmWCC2VmcmVpZG9jLmZyghFlaW5hcm1hdHRz\n" \
"c29obi5ncYIRZXZpZGVhcmRkaXRpdmEudGuCGWdpYW50c2ZhbnNhcHBhcmVsc2hv\n" \
"cC5jb22CGGhhY2t3YWxraW5nd2Fycm9ib3RzLndpboILaG90dmRpbmcuY2aCC2hv\n" \
"dHZkaW5nLmdxghBoeXJhLXNwYW5uaWVuLmdhghFpbXBvcnRlcmhzcXVhaC5tbIIM\n" \
"aXN0ZWRvY3NpLm1sghNqc29ucGxhY2Vob2xkZXIuY29tghVrYW1pMmhhY2tlZG9u\n" \
"bGluZS53aW6CDWt1bGxoYWdxYXMuZ3GCDm1hbGlubm9ldGlvLmdhggxtZXJmaW1h\n" \
"c2kuZ2GCDm1oYXplb2Z0aW1lLnRrgg1ucW90Ym9ja2VyLmNmgg9vbGl2ZWlyYTI5\n" \
"LndvcmuCGXBhY2tlcnNjb2xsZWN0aW9uc2hvcC5jb22CFHBoaWxsaWVzZWRnZXNo\n" \
"b3AuY29tggtwbGF1ZGl1by50a4INcG9ybm8tdmlwLm5ldIIOcHJpY2V2ZnFkd3Iu\n" \
"bWyCFHJhdmVuc2xvY2tlcnJvb20uY29tghRyYXlzdGVhbWZhbnN0b3JlLmNvbYIO\n" \
"cmlucGlyaXR1cnMuY2aCCnJvY29kZS5uZXSCD3NhbmdhdG11cmFoLm5ldIINc29s\n" \
"anVjb25yZS5tbIIWdGl0YW5zZmFuc2dlYXJzaG9wLmNvbYILdG9wLWthbC5jb22C\n" \
"D3RyYWNkZXJjaWxlYS5jZoIMdHlwaWNvZGUuY29tgg92LXZubnJldmlld3MuY2Yw\n" \
"ggEEBgorBgEEAdZ5AgQCBIH1BIHyAPAAdQDuS723dc5guuFCaR+r4Z5mow9+X7By\n" \
"2IMAxHuJeqj9ywAAAWTzPsHxAAAEAwBGMEQCIBBCtXgu0D5pRY9gJTh5OSZv00AJ\n" \
"xWE8avRm0MkHtTqdAiALxK1D8GbypYhIGkRbKeZfv7Sxv1qVRgONMdHUs1qQVAB3\n" \
"AHR+2oMxrTMQkSGcziVPQnDCv/1eQiAIxjc1eeYQe8xWAAABZPM+wlMAAAQDAEgw\n" \
"RgIhAOgonE5bvA7q+g332va0IbXim4kye+dXzF3MW+UCJX8hAiEAhO+lenIvGwMr\n" \
"QV8oD9CP5UHQkG5rao2PY0w+FXs6bL4wCgYIKoZIzj0EAwIDSQAwRgIhANkA0PXC\n" \
"V5v88GCJAp0Wiz6I83B7vgIs6Zm5vbNgVV+ZAiEAnfW8GRnaevZ8HwqBawL2zfmx\n" \
"O3pqqb4YaND2f2+or9E=" \
"-----END CERTIFICATE-----";

WiFiClient wifi;
HTTPClient http;
int status = WL_IDLE_STATUS;
String response;
int statusCode = 0;
//Ensure token fits in here
StaticJsonBuffer<500> jsonBuffer;

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

const char* getAuth() {
  String payload;
  JsonObject& creds = jsonBuffer.createObject();
  creds["username"] = API_user;
  creds["password"] = API_pass;
  creds.printTo(Serial);
  http.begin(SERVER_443_auth);
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
  return jwt_token;
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
  http.begin(SERVER_443_data);
//  sprintf(auth_header, "%s = %s", auth_start, curToken);
//  Serial.println(auth_header);
  http.addHeader("Content-Type", "application/json", false, false);
  String bits = "bearer "+curToken;
  http.addHeader("Authorization", bits, true, false);
//  http.setUserAgent("bearer");
//  http.setAuthorization(Token);
  String input;
  root.printTo(input);
  int len = input.length();
  Serial.println(len);
  int httpCode = http.POST(input);
  // httpCode will be negative on error
  if(httpCode > 0) {
    // HTTP header has been send and Server response header has been handled
    Serial.print("HTT code = ");
    Serial.println(httpCode);
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
    thisTemp = 19.13;
    updateAPI();
//    http.begin(SERVER_80);
//    int httpCode = http.GET();
//    if (httpCode > 0) { //Check for the returning code 
//        String payload = http.getString();
//        Serial.println(httpCode);
//        Serial.println(payload);
//      }else {
//      Serial.println("Error on HTTP request");
//    }
//    http.end(); //Free the resources
    thisTemp = thisTemp + 0.50;
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
