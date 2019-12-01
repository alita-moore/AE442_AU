extern float alt_zero;
extern debug bug;

float update_ALT(){
  //float altitude = myPressure.readAltitude();
  //Serial.print("Altitude(m):");
  //Serial.print(altitude, 2);

  float alt = myPressure.readAltitudeFt() - alt_zero;
  if(bug.alt){
    Serial.print("Relative Altitude(ft): ");
    Serial.println(alt, 2);
  }
  return alt;
}