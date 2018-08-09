#include <WiFi.h>
//don't use arduin http client, use this: https://github.com/espressif/arduino-esp32/blob/51a4432ca8e71be202358ceb068f3047bb8ad762/libraries/HTTPClient/src/HTTPClient.h
// with instructions: https://techtutorialsx.com/2017/05/19/esp32-http-get-requests/
#include <HTTPClient.h>
#include "secrets.h"


///////please enter your sensitive data in the Secret tab/secrets.h
/////// Wifi Settings ///////
const char* ssid = MYSSID;
const char* password = PASS;

const char* root_ca= \
"-----BEGIN CERTIFICATE-----\n" \
"MIICiTCCAg+gAwIBAgIQH0evqmIAcFBUTAGem2OZKjAKBggqhkjOPQQDAzCBhTEL\n" \
"MAkGA1UEBhMCR0IxGzAZBgNVBAgTEkdyZWF0ZXIgTWFuY2hlc3RlcjEQMA4GA1UE\n" \
"BxMHU2FsZm9yZDEaMBgGA1UEChMRQ09NT0RPIENBIExpbWl0ZWQxKzApBgNVBAMT\n" \
"IkNPTU9ETyBFQ0MgQ2VydGlmaWNhdGlvbiBBdXRob3JpdHkwHhcNMDgwMzA2MDAw\n" \
"MDAwWhcNMzgwMTE4MjM1OTU5WjCBhTELMAkGA1UEBhMCR0IxGzAZBgNVBAgTEkdy\n" \
"ZWF0ZXIgTWFuY2hlc3RlcjEQMA4GA1UEBxMHU2FsZm9yZDEaMBgGA1UEChMRQ09N\n" \
"T0RPIENBIExpbWl0ZWQxKzApBgNVBAMTIkNPTU9ETyBFQ0MgQ2VydGlmaWNhdGlv\n" \
"biBBdXRob3JpdHkwdjAQBgcqhkjOPQIBBgUrgQQAIgNiAAQDR3svdcmCFYX7deSR\n" \
"FtSrYpn1PlILBs5BAH+X4QokPB0BBO490o0JlwzgdeT6+3eKKvUDYEs2ixYjFq0J\n" \
"cfRK9ChQtP6IHG4/bC8vCVlbpVsLM5niwz2J+Wos77LTBumjQjBAMB0GA1UdDgQW\n" \
"BBR1cacZSBm8nZ3qQUfflMRId5nTeTAOBgNVHQ8BAf8EBAMCAQYwDwYDVR0TAQH/\n" \
"BAUwAwEB/zAKBggqhkjOPQQDAwNoADBlAjEA7wNbeqy3eApyt4jf/7VGFAkK+qDm\n" \
"fQjGGoe9GKhzvSbKYAydzpmfz1wPMOG+FDHqAjAU9JM8SaczepBGR7NjfRObTrdv\n" \
"GDeAU/7dIOA1mjbRxwG55tzd8/8dLDoWV9mSOdY=\n" \
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
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());
  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);
}

void loop() {
  if ((WiFi.status() == WL_CONNECTED)) { //Check the current connection status
 
    HTTPClient http;
 
    http.begin("https://jsonplaceholder.typicode.com/posts?userId=1", root_ca); //Specify the URL and certificate
    int httpCode = http.GET();                                                  //Make the request
 
    if (httpCode > 0) { //Check for the returning code
 
        String payload = http.getString();
        Serial.println(httpCode);
        Serial.println(payload);
      }
 
    else {
      Serial.println("Error on HTTP request");
    }
 
    http.end(); //Free the resources
  }
 
  delay(5000);
}
