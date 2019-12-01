/* main.cpp for AE442
The purpose of this file is to perform all main function calls to achieve the following:

Note: files are included as they are for easy debugging
*/
#include "Arduino.h"
#include "quaternionFilters.h"
#include "MPU9250.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

////// global variables and typedefs
    #include "Data\global.h"

////// debug
    #include "util\debug.h"
      // All debug options are set at ::() initial value
      // Available values:
      // [].imu -> general serial output from IMU::(false)
      //    [].imu_AHRS -> provide verbose debug info::(true), provided [].imu is true
    debug bug;

////// IMU sub_files required constants
    // #define MPU9250_ADDRESS MPU9250_ADDRESS_AD0   
    // Use either this line or the next to select which I2C address your device is using
    #define MPU9250_ADDRESS MPU9250_ADDRESS_AD0
    #define I2Cclock 400000
    #define I2Cport Wire
    MPU9250 myIMU(MPU9250_ADDRESS, I2Cport, I2Cclock);
    // Pin definitions
    int intPin = 12;  // These can be changed, 2 and 3 are the Arduinos ext int pins
    int myLed  = 13;  // Set up pin 13 led for toggling
    // Other constants
    uint32_t timer = 0;
    uint32_t counter = 0;
    // include header files
    #include "Util\initailize\init_imu.h" // setup_IMU() -> intializes and zeros IMU
    #include "avionics\sensors\IMU\IMU.h" // update_IMU() -> automatically updates myIMU

////// low_pass cleaner
    #include "avionics\clean\low_pass.h" // lp structure, low_pass(lp, value)

////// pd controller
    #include "avionics\control\pd.h"

int loops = 0;
int j = 0;
extern float lim_s1[2];
extern float lim_s2[2];

void setup(){
  // Define debug settings (redefined for clarity)
  bug.imu = false; // output imu debug info?
  bug.imu_AHRS = true; // if output imu debug, output verbose info?
  bug.low_pass = true; // output low_pass debug?
  bug.low_pass_spam = true; // output all data from low_pass?
  bug.loop = false;

  // Setup sensors
  setup_IMU();

}

void loop(){
  if(bug.loop){
    Serial.println("loop started");
  }
  
  update_IMU(); // update myIMU
  
  if(bug.loop){
    Serial.println("updated IMU");
    loops += 1;
    Serial.println(loops);
  }



}