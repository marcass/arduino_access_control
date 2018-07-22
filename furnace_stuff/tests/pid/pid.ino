
#include <math.h>
#include <PID_v1.h>


//PID setup
//double Setpoint, Input, Output;
double TEMP_SET_POINT = 50; //PID SETPOINT
double power; //variable for percentage power we want fan to run at works from 30 (min) to 80 (max)
double water_temp;
double feed_pause_percent;
double feed_percent;
long feed_pause = 2000;
long feed_time;
long start_feed_time;
long start_feed_pause;
const long FEED_PAUSE = 60000;
const long FEED_TIME = 30000;
bool feeding = true;
double Kp=10, Ki=0.001, Kd=1;

//Specify the links and initial tuning parameters
//PID fanPID(&Input, &Output, &Setpoint,2,5,1, DIRECT);
//PID fanPID(&water_temp, &power, &TEMP_SET_POINT,2,5,1, DIRECT); //need more fan power to get hotter so DIRECT
//PID pelletsPID(&water_temp, &feed_pause_percent, &TEMP_SET_POINT,2,5,1, REVERSE); //need shorter feed time to get to hotter so REVERSE
PID fanPID(&water_temp, &power, &TEMP_SET_POINT,Kp, Ki, Kd, DIRECT); //need more fan power to get hotter so DIRECT
PID pelletsPID(&water_temp, &feed_pause_percent, &TEMP_SET_POINT,Kp, Ki, Kd, REVERSE); //need shorter feed time to get to hotter so REVERSE
PID feedPID(&water_temp, &feed_percent, &TEMP_SET_POINT,Kp, Ki, Kd, DIRECT);

//Inputs
const int WATER_TEMP = 1; //analogue pin 3

//Analogue reading maths for temp
double Thermistor(int RawADC) {
 double Temp;
 Temp = log(10000.0*((1024.0/RawADC-1)));
 //         =log(10000.0/(1024.0/RawADC-1)) // for pull-up configuration
 Temp = 1 / (0.001129148 + (0.000234125 + (0.0000000876741 * Temp * Temp ))* Temp );
 Temp = Temp - 273.15;            // Convert Kelvin to Celcius
 //Temp = (Temp * 9.0)/ 5.0 + 32.0; // Convert Celcius to Fahrenheit
 return Temp;
}


void setup() {
  // initialize serial communication:
  Serial.begin(115200);
  //initialize the PID variables we're linked to
  fanPID.SetOutputLimits(30, 80); //percentage of fan power
  fanPID.SetSampleTime(200); //SAMPLES EVERY 0.5s
  feedPID.SetSampleTime(200);
  feedPID.SetOutputLimits(60,100);
  pelletsPID.SetOutputLimits(60,100); //percentage of feed time check to see that burning all of load
  pelletsPID.SetSampleTime(200);
  //turn the PID on
  fanPID.SetMode(AUTOMATIC);
  pelletsPID.SetMode(AUTOMATIC);
  feedPID.SetMode(AUTOMATIC);
}

void loop() {
  water_temp = int(Thermistor(analogRead(WATER_TEMP)));
  fanPID.Compute();
  pelletsPID.Compute();
  feedPID.Compute();
  feed_pause = (feed_pause_percent / 100) * 20000; //caluclate actual pause from PID derived value
  feed_time =  (feed_percent / 100)       * FEED_TIME;
  
  feed_pause = (feed_pause_percent / 100) * FEED_PAUSE; //caluclate actual pause from PID derived value
  feed_time =  (feed_percent / 100)       * FEED_TIME;
  
  if (feeding) {
    if (start_feed_time == 0) {
      start_feed_time = millis();
    }
    //test to see if feed been on for long enough
    if (millis() - start_feed_time > (long)feed_time) {
      //stop feeding and start pausing
      feeding = false;
      start_feed_time = 0;
      //start_feed_pause = millis();
    }
  } 
  if (!feeding) { 
    if (start_feed_pause == 0 ) {
      start_feed_pause = millis();
    }
    if (millis() - start_feed_pause > (long)feed_pause) {
      //stop pausing, start feeding
      
      start_feed_pause = 0;
      feeding = true;
    }
  }
  
  Serial.print("Temp = ");
  Serial.print((int)water_temp);
  Serial.print("  Fan power = ");
  Serial.print(power);
  Serial.print("%");
  Serial.print("  Pellets pause = ");
  Serial.print(feed_pause_percent);
  Serial.print("%");
  Serial.print("  Feed percent:  ");
  Serial.print(feed_percent);
  Serial.print("%");
  Serial.print("  Pellets feed pause time = ");
  Serial.print((int)feed_pause);
  Serial.print("ms");
  Serial.print("  Pellets feed time = ");
  Serial.print((int)feed_time);
  Serial.print("ms");
  Serial.print(" Time left to feed = ");
  Serial.print((long)feed_time - (millis()-start_feed_time));
  Serial.print("  Time left in pause = ");
  Serial.println((long)feed_pause - (millis()-start_feed_pause));
  delay(200);

}
