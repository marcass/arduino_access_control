#include <ArduinoJson.h>


void setup() {
  // put your setup code here, to run once:
  // initialize serial for debugging
  Serial.begin(115200);


}

void loop() {
  //set buffer for json
  StaticJsonBuffer<200> jsonBuffer;
  JsonObject& root = jsonBuffer.createObject();
  const char door[] = "topgarage";
  // put your main code here, to run repeatedly:
  root["door"] = door;
  root["pincode"] = "1234";
  int len = root.measureLength();
  len = len + 1;
  Serial.print("len is ");
  Serial.print(len);
  char json[len];
  root.printTo(json, sizeof(json));
  Serial.println(json);
  delay(3000);
}
