/* main.cpp for AE442
The purpose of this file is to perform all main function calls to achieve the following:

Note: files are included as they are due to an oversight of coding header files
      as a result, the code is poorly defined, and works with great dependence on this file's construction.
      In other words, if you try to use this in a more robust way or try to change the code, prepare to redo
      all the header files such that they are not order-dependent.
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
    lp* pitch = (lp*)malloc(sizeof(lp));
    lp* gx = (lp*)malloc(sizeof(lp));
    lp* roll = (lp*)malloc(sizeof(lp));
    lp* gy = (lp*)malloc(sizeof(lp));
    lp* lp_alt = (lp*)malloc(sizeof(lp));

////// pd controller
    #include "avionics\control\pd.h" // performs pd control with global values

////// EEPROM save
    const int chipSelect = 10; // for SD_setup()
    #include "Data\save.h" // the save functions used in main loop

////// Servos
    #include <PWMServo.h>
    #include <output\servo.h> // contains the function to initialize servos
    PWMServo S_1;
    PWMServo S_2;

////// ignition
    #include <output\ignite.h>

void setup(){
  Serial.begin(38600);
  delay(2000);
  //while(!Serial){};
  
  // zero/define all globals
  setup_globals();
  ignition = false;
  save = true;
  control = true;
  out_ignite = false;
  out_servo = true;
  
  // Define debug settings (redefined for clarity)
  bug.imu = false; // output imu debug info?
  bug.imu_AHRS = true; // if output imu debug, output verbose info?
  bug.low_pass = false; // output low_pass debug?
  bug.low_pass_spam = false; // output all data from low_pass?
  bug.loop = false; // output loop progress?
  bug.control = false; // output control steps?
  bug.ang = true;  // output the calculated angle?
  bug.alt = false;  // output currently read relative altitude?
  bug.zero = false;  // output zeroing steps/values?
  bug.save_debug = false;  // output datastream?
  bug.servo = false; // output the inputted servo angles?
  bug.ignite_checks = true; // output the current read outputs from the checks?
  bug.ignite_spam = true; // output every current ignition parameter every loop?

  // low pass setup
  init_lp(pitch, 0.05f, 0);
  init_lp(gx, 0.05f, 2);
  init_lp(roll, 0.05f, 1);
  init_lp(gy, 0.05, 3);
  init_lp(lp_alt, 0.05, 4);

  // Setup sensors
  setup_IMU();
  setup_alt();
  setup_servos((int)8, (int)9); // (pin S_1, pin S_2) -> type int
  SD_setup();

  // Define control parameters
  Kp = 5*(0.003);
  Kd = 0.003;

  // Define ignition parameters
  ignite_time = 5000; // MUST BE IN MILLISECONDS
  ignite_timer = millis(); // for the debug option, simplifies things
}

void loop(){
  if(bug.loop){
    Serial.println("loop started");
  }
  loops += 1;

  /////////////// All mode loops
  // update myIMU
  update_IMU(); 
  if(!zero){
    alt = low_pass(lp_alt, myPressure.readAltitudeFt());
  } else{
    alt = low_pass(lp_alt, myPressure.readAltitudeFt() - alt_zero);
  }

  if(bug.loop && loops % 1000 == 0){
    Serial.print("updated IMU, updated alt :: loop ");
    Serial.println(loops);
  }

  if(bug.alt){
    Serial.print("Relative Altitude(ft): ");
    Serial.println(alt, 2);
  }
  
  //////////////////////
  // zeroing
  //////////////////////
  if(loops<200){
    pitch_zero = pitch->avg;
    roll_zero = roll->avg;
    gx_zero = gx->avg;
    gy_zero = gy->avg;
    alt_zero = lp_alt->avg;

    if(bug.zero){
      Serial.println('zero----');
      Serial.print("pitch_zero = ");
      Serial.print(pitch_zero);
      Serial.print(" roll_zero = ");
      Serial.print(roll_zero);
      Serial.print(" gx_zero = ");
      Serial.print(gx_zero);
      Serial.print(" gy_zero = ");
      Serial.print(gy_zero);
      Serial.print(" loops = ");
      Serial.println(loops);
    }
  } else {
    zero = true;
  }

  if(bug.zero){
      Serial.print("pitch avg = ");
      Serial.print(pitch->avg);
      Serial.print(" roll avg = ");
      Serial.print(roll-> avg);
      Serial.print(" gx avg = ");
      Serial.print(gx->avg);
      Serial.print(" gy avg = ");
      Serial.println(gy->avg);
  }

  //////////////////////
  // control
  //////////////////////
  if(control){
    // Check if IMU values have updated, if so then perform actions
    if(!(myIMU.pitch == theta[0])){
      // Perform clean
      if(!zero){
      theta[0] = low_pass(pitch, myIMU.pitch);
      omega[0] = low_pass(gx, myIMU.gx);
      } else {
      theta[0] = low_pass(pitch, myIMU.pitch - pitch_zero);
      omega[0] = low_pass(gx, myIMU.gx - gx_zero);
      }
      
      // Perform control
      torque[0] = pd(theta[0], omega[0]);
      if(bug.loop){
        Serial.print("pitch and gx updated and torque found on loop ");
        Serial.println(loops);
      }
    } 
    
    if(!(myIMU.roll == theta[1])){
      // perform clean
      if(!zero){
      theta[1] = low_pass(roll, myIMU.roll);
      omega[1] = low_pass(gy, myIMU.gy);
      } else {
      theta[1] = low_pass(roll, myIMU.roll-roll_zero);
      omega[1] = low_pass(gy, myIMU.gy-gy_zero);
      }

      // perform control
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
    if(bug.servo){
      Serial.print("S_1 = ");
      Serial.print(s_ang[0]);
      Serial.print(" --- S_2 = ");
      Serial.println(s_ang[1]);
    }

    S_1.write(s_ang[0]);
    S_2.write(s_ang[1]);
  }

  /////////////
  // ignition
  /////////////
  if(out_ignite){
    if(!(ignition)){
      ignition_protocol();
    }

    if(bug.ignite_checks){
      Serial.print("Standby: ");
      Serial.print(check_standby()); // this depends on the switch status
      Serial.print(" Release: ");
      Serial.print(release); // this means that the motor is now armed/the timer has started
      Serial.print(" Armed: ");
      Serial.print(check_armed()); // this is based on the teensy reader after bridge
      Serial.print(" Ignition: ");
      Serial.print(check_ignition()); // depends on the value on the side of the trigger
      Serial.print(" timer-time:");
      Serial.println(abs(ignite_timer-millis()));
    }
  }

  /////////////
  // Save to SD
  /////////////
  if(save && loops % 15 == 0){
    SD_save();
  }
  
  ////////////
  // LED stuff
  ////////////
  
  // LED for each status boolean
    // initialize
    // control
    // out_servo
    // out_ignite
    // 
  
}