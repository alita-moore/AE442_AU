// LED breakdown:
     // standby via inline LED
     // armed via inline LED
     // ignition via inline LED
          // this means that it's not necessary to code LED signalling for all these systems

// returns the status of standby for ignition (true = standby, false = not standby)
bool check_standby(){
     if(false){ // if the switch is ON = true
          return true;
     } else {
          return false;
     }
}

// returns the status of armed state of ignition (true = armed, false = not armed)
bool check_armed(){
     if(false){ // checks the release mechanism status (connected = false)
          return true;
     } else{
          return false;
     }
}

// return the status of whether or not the (true = ignition, false = no ignition)
bool check_ignition(){
     if(true){ // check the midway voltage
          return true;
     } else {
          return false;
     }
}

// this function runs through each necessary loop, sets variables, times, and ignites
void ignition_protocol(){
     if(bug.ignite_checks){
          Serial.print("Standby: ");
          Serial.print(check_standby());
          Serial.print(" Armed: ");
          Serial.print(check_armed());
          Serial.print(" Ignition: ");
          Serial.println(check_ignition());
     }

     
     if(check_standby()){
          if(check_armed()){
               if(!release){
                    ignite_timer = millis();
                    release = true;

                    if(bug.ignite_checks){
                         Serial.print("RELEASE! Ignition timer has started at loop ");
                         Serial.print(loops);
                         Serial.print(", timer: ");
                         Serial.print(ignite_timer);
                         Serial.println(" (ms)");
                    }
               }
               
               if(abs(ignite_timer - millis())>ignite_time){
                    FIRE();
                    ignition = true;
               }
          }
     }
}

void FIRE(){
     // set the variables to true to fire
}