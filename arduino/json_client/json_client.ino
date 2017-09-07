// Sample Arduino Json Web Client
// Downloads and parse http://jsonplaceholder.typicode.com/users/1


#include <Keyboard.h>
#include <ArduinoJson.h>
#include <SPI.h>

#define debug

const byte rows = 4; //four rows
const byte cols = 4; //three columns
char keys[rows][cols] = {
  {'1','2','3', 'A'},
  {'4','5','6', 'B'},
  {'7','8','9', 'C'},
  {'#','0','*', 'D'}
};
byte rowPins[rows] = {5, 4, 3, 2}; //connect to the row pinouts of the keypad
byte colPins[cols] = {8, 7, 6}; //connect to the column pinouts of the keypad
String key_str = "";
unsigned long code_timer;
const unsigned long CODE_TIMER_THRESH = 20000; //20sec
Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, rows, cols );

const char* server = "houseslave";  // server's address
const char* resource = "/users/1";                    // http resource
const unsigned long BAUD_RATE = 9600;                 // serial connection speed
const unsigned long HTTP_TIMEOUT = 10000;  // max respone time from server
const size_t MAX_CONTENT_SIZE = 512;       // max size of the HTTP response

#define RED_LED 10
#define GREEN_LED 11
//types of access inputs
const int KEYPAD = 1;
const int RFID   = 2;
const int MQTT   = 3;

// The type of data that we want to extract from the page
struct UserData {
  char name[32];
  char company[32];
};

// ARDUINO entry point #1: runs once when you press reset or power the board
void setup() {
  Serial.begin(BAUD_RATE);
  Serial.println("Serial ready");
  pinMode(RED_LED, OUTPUT);
  digitalWrite(RED_LED, LOW);
  pinMode(GREEN_LED, OUTPUT);
  digitalWrite(GREEN_LED, LOW);
}

void actuate(){
  //
}

bool test_input(String in_key, int type){
  //get keys
    if (connect(server)) {
    if (sendRequest(server, resource) && skipResponseHeaders()) {
      UserData userData;
      if (readReponseContent(&userData, type)) {
        printUserData(&userData);
      }
    }
  }
  disconnect();
  if(
}

void build_response(int type, String data, bool opened){
  DynamicJsonBuffer jsonBuffer;
  JsonArray& root = jsonBuffer.createArray();
  JsonObject& action = root.createNestedObject().createNestedObject("action");
  action["type"] = type;
  action["data"] = data;
  action["opened"] = opened;
  root.prettyPrintTo(Serial);
}

// ARDUINO entry point #2: runs over and over again forever
void loop() {
  //************************ KEYPAD ENTRY  ******************************
  //timeout for key presses
  if (millis() - code_timer > CODE_TIMER_THERESH){
    key_str = "";
    digitalWrite(RED_LED, HIGH);
    //light red led
  }
  char key = keypad.getKey();
  if(key == '#'){
    //test for valid key_str via json request
    if (test_input(key_str, KEYPAD)){
      //open door or blink red light if wrong
      //actuate();
      //return json string to server for stats and burner code handling
      opened = true;
    }else{
      opened = false;
    }
    build_respose(KEYPAD, key_str, opened);
    key_str = "";
  }
  if(key){
    code_timer = millis();
    key_str =+ key;
    #ifdef debug
      Serial.print("Key string is: ");
      Serial.println(key_str);
    #endif
  }

  //******************** MQTT ENTRY  ****************************
  

  //*******************  RFID ENTRY  *****************************

}

// Open connection to the HTTP server
bool connect(const char* hostName) {
  Serial.print("Connect to ");
  Serial.println(hostName);

  bool ok = client.connect(hostName, 80);

  Serial.println(ok ? "Connected" : "Connection Failed!");
  return ok;
}

// Send the HTTP GET request to the server
bool sendRequest(const char* host, const char* resource) {
  Serial.print("GET ");
  Serial.println(resource);

  client.print("GET ");
  client.print(resource);
  client.println(" HTTP/1.0");
  client.print("Host: ");
  client.println(host);
  client.println("Connection: close");
  client.println();

  return true;
}

// Skip HTTP headers so that we are at the beginning of the response's body
bool skipResponseHeaders() {
  // HTTP headers end with an empty line
  char endOfHeaders[] = "\r\n\r\n";

  client.setTimeout(HTTP_TIMEOUT);
  bool ok = client.find(endOfHeaders);

  if (!ok) {
    Serial.println("No response or invalid response!");
  }

  return ok;
}

// Parse the JSON from the input string and extract the interesting values
// Here is the JSON we need to parse
// {
//   "id": 1,
//   "name": "Leanne Graham",
//   "username": "Bret",
//   "email": "Sincere@april.biz",
//   "address": {
//     "street": "Kulas Light",
//     "suite": "Apt. 556",
//     "city": "Gwenborough",
//     "zipcode": "92998-3874",
//     "geo": {
//       "lat": "-37.3159",
//       "lng": "81.1496"
//     }
//   },
//   "phone": "1-770-736-8031 x56442",
//   "website": "hildegard.org",
//   "company": {
//     "name": "Romaguera-Crona",
//     "catchPhrase": "Multi-layered client-server neural-net",
//     "bs": "harness real-time e-markets"
//   }
// }

bool readReponseContent(struct UserData* userData) {
  // Compute optimal size of the JSON buffer according to what we need to parse.
  // See https://bblanchon.github.io/ArduinoJson/assistant/
  const size_t BUFFER_SIZE =
      JSON_OBJECT_SIZE(8)    // the root object has 8 elements
      + JSON_OBJECT_SIZE(5)  // the "address" object has 5 elements
      + JSON_OBJECT_SIZE(2)  // the "geo" object has 2 elements
      + JSON_OBJECT_SIZE(3)  // the "company" object has 3 elements
      + MAX_CONTENT_SIZE;    // additional space for strings

  // Allocate a temporary memory pool
  DynamicJsonBuffer jsonBuffer(BUFFER_SIZE);

  JsonObject& root = jsonBuffer.parseObject(client);

  if (!root.success()) {
    Serial.println("JSON parsing failed!");
    return false;
  }

  // Here were copy the strings we're interested in
  strcpy(userData->name, root["name"]);
  strcpy(userData->company, root["company"]["name"]);
  // It's not mandatory to make a copy, you could just use the pointers
  // Since, they are pointing inside the "content" buffer, so you need to make
  // sure it's still in memory when you read the string

  return true;
}

//my json
// Parse the JSON from the input string and extract the interesting values
// Here is the JSON we need to parse
// {
//   "id": 1,
//   "name": "Joe Bloggs",
//   "username": "Joe",
//   "enabled": "0",              //enabled(1)/disabled(0) user
//   "keycode": {
//     "key_str": "1234A",
//     "allowed": "1"             //enable(1)/disable(0) this method
//   },
//   "rfid": {
//     "id": "123456789",
//     "allowed": "0"
//   },
//   "mqtt": {
//     "client_id": "mqtt_client_id_set_in_device",
//     "allowed": "1"
//   }
// }
bool readReponseContent(struct UserData* userData, int type) {
  //generate from here: https://bblanchon.github.io/ArduinoJson/assistant/
//  const size_t bufferSize = 3*JSON_OBJECT_SIZE(2) + JSON_OBJECT_SIZE(7) + 180;
//  DynamicJsonBuffer jsonBuffer(bufferSize);
//  
//  const char* json = "{\"id\":1,\"name\":\"Joe Bloggs\",\"username\":\"Joe\",\"enabled\":\"0\",\"keycode\":{\"key_str\":\"1234A\",\"allowed\":\"1\"},\"rfid\":{\"id\":\"123456789\",\"allowed\":\"0\"},\"mqtt\":{\"client_id\":\"mqtt_client_id_set_in_device\",\"allowed\":\"1\"}}";
//  
//  JsonObject& root = jsonBuffer.parseObject(json);
//  
//  int id = root["id"]; // 1
//  const char* name = root["name"]; // "Joe Bloggs"
//  const char* username = root["username"]; // "Joe"
//  const char* enabled = root["enabled"]; // "0"
//  
//  const char* keycode_key_str = root["keycode"]["key_str"]; // "1234A"
//  const char* keycode_allowed = root["keycode"]["allowed"]; // "1"
//  
//  const char* rfid_id = root["rfid"]["id"]; // "123456789"
//  const char* rfid_allowed = root["rfid"]["allowed"]; // "0"
//  
//  const char* mqtt_client_id = root["mqtt"]["client_id"]; // "mqtt_client_id_set_in_device"
//  const char* mqtt_allowed = root["mqtt"]["allowed"]; // "1"
  const size_t BUFFER_SIZE = 3*JSON_OBJECT_SIZE(2) + JSON_OBJECT_SIZE(7) + 180;
  // Allocate a temporary memory pool
  DynamicJsonBuffer jsonBuffer(bufferSize);
  JsonObject& root = jsonBuffer.parseObject(client);
  if (!root.success()) {
    Serial.println("JSON parsing failed!");
    return false;
  }
    
  //find appropriate key
  switch (type) {
    case KEYPAD:
      // see if key_str json string
      
      strcpy(userData->name, root["name"]);
    case RFID:
    case MQTT:
    default:
      return false
    break;
  }
}


// Print the data extracted from the JSON
void printUserData(const struct UserData* userData) {
  Serial.print("Name = ");
  Serial.println(userData->name);
  Serial.print("Company = ");
  Serial.println(userData->company);
}

// Close the connection with the HTTP server
void disconnect() {
  Serial.println("Disconnect");
  client.stop();
}
