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
#include <Wire.h>
#include "SparkFunMPL3115A2.h"

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

////// Altimeter
    // required stuff...
    MPL3115A2 myPressure;
    #include "Util\initailize\init_alt.h"
    #include "avionics\sensors\altimeter\Altimeter.h"

////// low_pass cleaner
    #include "avionics\clean\low_pass.h" // lp structure, low_pass(lp, value)

////// pd controller
    #include "avionics\control\pd.h"

int loops = 0;
int j = 0;
extern float lim_s1[2];
extern float lim_s2[2];
float alt;

void setup(){
  // zero/define all globals
  setup_globals();
  
  // Define debug settings (redefined for clarity)
  bug.imu = false; // output imu debug info?
  bug.imu_AHRS = true; // if output imu debug, output verbose info?
  bug.low_pass = false; // output low_pass debug?
  bug.low_pass_spam = false; // output all data from low_pass?
  bug.loop = false; // output loop progress?
  bug.control = false; // output control steps?
  bug.ang = false;

  // Setup sensors
  setup_IMU();
  setup_alt();

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
  
  //////
  // control
  //////

  // Check if IMU values have updated, if so then perform actions
  if(!(myIMU.pitch == theta[0])){
    // Perform clean
    // low_pass(*pitch, myIMU.pitch); // clean pitch
    // low_pass(*gx, myIMU.gx); // clean gx
    
    // Perform control
    theta[0] = myIMU.pitch;
    omega[0] = myIMU.gx;
    torque[0] = pd(theta[0], omega[0]);
    if(bug.loop){
      Serial.print("pitch and gx updated and torque found on loop ");
      Serial.println(loops);
    }
  } 
  
  if(!(myIMU.roll == theta[1])){
    // perform clean
    // low_pass(*roll, myIMU.roll); // clean roll
    // low_pass(*gy, myIMU.gy); // clean gy

    // perform control
    theta[1] = myIMU.roll;
    omega[1] = myIMU.gy;
    torque[1] = pd(theta[1], omega[1]);
    if(bug.loop){
      Serial.print("roll and gy updated and torque found on loop ");
      Serial.println(loops);
    }
  }

  // find desired servo angle (needs updating)
  get_ang(torque, s_ang);
  if(bug.loop){
    Serial.print("Angle 2 found on loop ");
    Serial.println(loops);
  }

  // get altitude reading
  alt = myPressure.readAltitudeFt() - alt_zero;
  if(bug.alt){
    Serial.print("Relative Altitude(ft): ");
    Serial.println(alt, 2);
  }

}