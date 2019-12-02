float alt_zero = 0.0;

void setup_alt(){
    Wire.begin();        // Join i2c bus
    //Serial.begin(9600);  // Start serial for output

    myPressure.begin(); // Get sensor online

    //Configure the sensor
    myPressure.setModeAltimeter(); // Measure altitude above sea level in meters
    //myPressure.setModeBarometer(); // Measure pressure in Pascals from 20 to 110 kPa

    myPressure.setOversampleRate(7); // Set Oversample to the recommended 128
    myPressure.enableEventFlags(); // Enable all three pressure and temp event flags 
    
    Serial.print("altitude zero at ");
    Serial.print(alt_zero);
    Serial.println("ft");
} 

void zero_alt(){
    for(int i = 0; i<2000; i++){
        alt_zero += myPressure.readAltitudeFt()/((float) 2000);
        delay(1);
    }
}