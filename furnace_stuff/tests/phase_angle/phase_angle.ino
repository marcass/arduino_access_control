
// AC Control V1.1
//
// This Arduino sketch is for use with the heater 
// control circuit board which includes a zero 
// crossing detect function and an opto-isolated TRIAC.
//
// AC Phase control is accomplished using the internal 
// hardware timer1 in the Arduino
//
// Timing Sequence
// * timer is set up but disabled
// * zero crossing detected on pin 2
// * timer starts counting from zero
// * comparator set to "delay to on" value
// * counter reaches comparator value
// * comparator ISR turns on TRIAC gate
// * counter set to overflow - pulse width
// * counter reaches overflow
// * overflow ISR turns off TRIAC gate
// * TRIAC stops conducting at next zero cross


// The hardware timer runs at 16MHz. Using a
// divide by 256 on the counter each count is 
// 16 microseconds.  1/2 wave of a 60Hz AC signal
// is about 520 counts (8,333 microseconds).

//#define test
#define fan
#define pump

#include <avr/io.h>
#include <avr/interrupt.h>
#include <math.h>

int flame_val;
const int LIGHT = 2;
const int PUMP = 7;
#define DETECT 2  //zero cross detect
#define GATE 9    //TRIAC gate
#define PULSE 4   //trigger pulse width (counts)
int power= 8;
int on_wait;
char rx_byte = 0;
String inString = "";    // string to hold input
int proportion;
float divisor;
int crosses;
int counts;

void setup(){

  // set up pins
  pinMode(DETECT, INPUT);     //zero cross detect
  digitalWrite(DETECT, HIGH); //enable pull-up resistor
  pinMode(GATE, OUTPUT);      //TRIAC gate control
  digitalWrite(GATE, LOW);

  // set up Timer1 
  //(see ATMEGA 328 data sheet pg 134 for more details)
  OCR1A = 100;      //initialize the comparator
  TIMSK1 = 0x03;    //enable comparator A and overflow interrupts
  TCCR1A = 0x00;    //timer control registers set for
  TCCR1B = 0x00;    //normal operation, timer disabled


  // set up zero crossing interrupt
  attachInterrupt(0,zeroCrossingInterrupt, RISING);    
    //IRQ0 is pin 2. Call zeroCrossingInterrupt 
    //on rising signal
  #ifdef pump
    pinMode(PUMP, OUTPUT);
    digitalWrite(PUMP, HIGH);
  #endif
  Serial.begin(115200);
}  

//Interrupt Service Routines

void zeroCrossingInterrupt(){ //zero cross detect   
  TCCR1B=0x04; //start timer with divide by 256 input
  TCNT1 = 0;   //reset timer - count from zero
  crosses++;
}


#ifdef fan
  ISR(TIMER1_COMPA_vect){ //comparator match
    digitalWrite(GATE,HIGH);  //set TRIAC gate to high
    TCNT1 = 65536-PULSE;      //trigger pulse width
  }
  
  ISR(TIMER1_OVF_vect){ //timer1 overflow
    digitalWrite(GATE,LOW); //turn off TRIAC gate
    TCCR1B = 0x00;          //disable timer stopd unintended triggers
  }
#endif

void run_fan(int x) { 

  if (x == 10) { //no phase angle control needed if you want balls out fan speed
    digitalWrite(GATE,HIGH);
  }else {
    //do magic phase angle stuff here
    /* x is the int as a percentage of fan power
     * OCR1A is the comparator for the phase angle cut-off
     * - when TCNT1 > OCR1A, ISR(TIMER1_OVF_vect) is called tellng optocoupler to power down
     * - 520 counts (16000000 cycles scaled to 256) per half AC sine wave
     * - a figure of 480 is a large proportion of 520 adn avoids latching the triac over the next zero cross
     * - 65 is the lower limit to avoid firing the triac too close to teh zero cross
     * - The smaller the value of OCR1A the more power we have
     */
    TIMSK1 = 0x03;    //enable comparator A and overflow interrupts
    //set up interrupt
    attachInterrupt(0,zeroCrossingInterrupt, RISING);  // inturrupt 0 on digital pin 2
    //set a value that is a proportion of 520 for power
    divisor = (float)x / 10;
    proportion = divisor * 520;
    on_wait = 520 - proportion;
    //on_wait = (520 - (x / 10 * 520));
//    Serial.print("on_wait = ");
//    Serial.println(on_wait);
    //a value of 65 gives close to full power (overflow counter triggered early in wave turing triac on
    //a value of 480 gives close to fuck all power (don't want to be too close to zero cross 
    // when turning optocoupler off or latch will spill over to next half wave leaving it on
    if ( on_wait < 104) {
      OCR1A = 104;
    }
    if ( on_wait > 364) {
      OCR1A = 364;
    }else {
      OCR1A = on_wait;
    }
    #ifdef test
    Serial.print("   Number of counts until fire = ");
    Serial.println(OCR1A);
    #endif
  }
  
}

void loop(){ // sample code to exercise the circuit

//i--;
//OCR1A = i;     //set the compare register brightness desired.
//if (i<65){i=483;}                      
//delay(15);      
// if (Serial.available() > 0) {    // is a character available?
//    rx_byte = Serial.read();       // get the character
//  
//    // check if a number was received
//    if ((rx_byte >= '0') && (rx_byte <= '9')) {
//      power = rx_byte;
//      Serial.print("Number received: ");
//      Serial.println(rx_byte);
//      
//    }
//    else {
//      Serial.println("Not a number.");
//    }
//  } // end: if (Serial.available() > 0)  
  #ifdef fan
    // Read serial input:
    while (Serial.available() > 0) {
      int inChar = Serial.read();
      if (isDigit(inChar)) {
        // convert the incoming byte to a char
        // and add it to the string:
        inString += (char)inChar;
      }
      // if you get a newline, print the string,
      // then the string's value:
      if (inChar == '\n') {
        Serial.print("Value:");
        Serial.println(inString.toInt());
        Serial.print("String: ");
        Serial.println(inString);
        //set power variable
        power = inString.toInt();
        // clear the string for new input:
        inString = "";
      }
    }
    flame_val = analogRead(LIGHT);
    Serial.print("Light = ");
    Serial.print(flame_val);
    Serial.print("  Power = ");
    Serial.println(power); 
    
    run_fan(power); 
  #endif
  #ifdef test
    if (crosses > 60) {
      crosses = 0;
      counts++;
      Serial.print("Number of counts = ");
      Serial.println(counts);
    }
  #endif
  //delay(1000);  

}
