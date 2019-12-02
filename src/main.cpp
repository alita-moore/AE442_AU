/* main.cpp for AE442
The purpose of this file is to perform all main function calls to achieve the following:

Note: files are included as they are for easier debugging
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
    lp* pitch;
    lp* roll;
    lp* gx;
    lp* gy;

////// pd controller
    #include "avionics\control\pd.h" // performs pd control with global values

////// EEPROM save
    #include "Data\save.h" // the save functions used in main loop

////// modes
    #include "Util\modes.h" // lists of which loop items are performed for each mode

////// Servos
    #include <PWMServo.h>
    #include <output\servo.h> // contains the function to initialize servos
    PWMServo S_1;
    PWMServo S_2;

extern int loops;
extern float alt_zero;

void setup(){
  // zero/define all globals
  setup_globals();
  ignition = false;
  save = false;
  control = true;
  out_ignite = false;
  out_servo = false;
  
  // Define debug settings (redefined for clarity)
  bug.imu = false; // output imu debug info?
  bug.imu_AHRS = true; // if output imu debug, output verbose info?
  bug.low_pass = false; // output low_pass debug?
  bug.low_pass_spam = true; // output all data from low_pass?
  bug.loop = false; // output loop progress?
  bug.control = true; // output control steps?
  bug.ang = false;  // output the calculated angle?
  bug.alt = false;

  // low pass setup
  init_lp(pitch, 0.05f, 0);
  init_lp(gx, 0.05f, 2);

  // Setup sensors
  setup_IMU();
  setup_alt();
  setup_servos((int)3, (int)4); // (pin S_1, pin S_2) -> type int
}

void loop(){
  if(bug.loop){
    Serial.println("loop started");
  }
  loops += 1;

  /////////////// All mode loops
  // update myIMU
  update_IMU(); 
  alt = myPressure.readAltitudeFt() - alt_zero;

  if(bug.loop && loops % 1000 == 0){
    Serial.print("updated IMU, updated alt :: loop ");
    Serial.println(loops);
  }

  if(bug.alt){
    Serial.print("Relative Altitude(ft): ");
    Serial.println(alt, 2);
  }
  
  //////////////////////
  // control
  //////////////////////
  if(control){
    // Check if IMU values have updated, if so then perform actions
    if(!(myIMU.pitch == theta[0])){
      // Perform clean
      // low_pass(pitch, myIMU.pitch); // clean pitch
      low_pass(gx, myIMU.gx);
      
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
  }

  if(out_servo){
    S_1.write(s_ang[0]);
    S_2.write(s_ang[1]);
  }

  /////////////
  // Save to EEPROM
  /////////////
  if(save){
    if(loops % 1000 == 0){
      EEPROM_writeAnything(addr, alt);
      EEPROM_writeAnything(addr, myIMU.pitch);
      EEPROM_writeAnything(addr, myIMU.roll);
      EEPROM_writeAnything(addr, myIMU.gx);
      EEPROM_writeAnything(addr, myIMU.gy);
      EEPROM_writeAnything(addr, s_ang);
      EEPROM_writeAnything(addr, false);
    }
  }
}