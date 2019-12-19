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
          false;
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

void FIRE(){
     // some code that signals fire mode this should be a permanent loop and not repeat if already true
     if(check_standby()){
          if(check_armed()){
               
          }
     }
}