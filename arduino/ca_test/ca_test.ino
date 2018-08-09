#include <WiFi.h>
//#include <WiFi101.h>
//don't use arduin http client, use this: https://github.com/espressif/arduino-esp32/blob/51a4432ca8e71be202358ceb068f3047bb8ad762/libraries/HTTPClient/src/HTTPClient.h
// with instructions: https://techtutorialsx.com/2017/05/19/esp32-http-get-requests/
#include <HTTPClient.h>


///////please enter your sensitive data in the Secret tab/secrets.h
/////// Wifi Settings ///////
const char* ssid = "insecure";
const char* password = "casualusers";
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
