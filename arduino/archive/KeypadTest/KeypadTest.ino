/* @file CustomKeypad.pde
|| @version 1.0
|| @author Alexander Brevig
|| @contact alexanderbrevig@gmail.com
||
|| @description
|| | Demonstrates changing the keypad size and key values.
|| #
*/
#include <Keypad.h>

const byte ROWS = 4; //four rows
const byte COLS = 4; //four columns
//define the cymbols on the buttons of the keypads
char hexaKeys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
/*           Row pin
 * 1 2 3 A | 6
 * 4 5 6 B | 7
 * 7 8 9 C | 8
 * * 0 # D | 9
 * ---------
 * 2 3 4 5 <- Col pin
 * Pins numbered form left to right when looking at keys on keypad
 * Need D6 and D7 for software serial
 * 
 * byte rowPins[ROWS] = {6, 7, 8, 9}; //connect to the row pinouts of the keypad
 * byte colPins[COLS] = {2, 3, 4, 5}; //connect to the column pinouts of the keypad
 */
byte colPins[ROWS] = {8,9,10,11}; //connect to the row pinouts of the keypad
byte rowPins[COLS] = {2,3,4,5}; //connect to the column pinouts of the keypad


String key_str = "";

//initialize an instance of class NewKeypad
Keypad customKeypad = Keypad( makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS); 

void setup(){
  Serial.begin(115200);
}
  
void loop(){
  char key = customKeypad.getKey(); 
  if (key){
    if(key == '#'){
      Serial.println(key_str);
      key_str = "";
    }else{
      key_str += key;
      Serial.print("Pressed key is: ");
      Serial.println(key);
    }
  }
}
