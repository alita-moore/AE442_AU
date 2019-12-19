#include <EEPROM.h>
#include <SD.h>

// I did not make these, they were from the following arduino post: https://forum.arduino.cc/index.php?topic=41497.0
template <class T> int EEPROM_writeAnything(int ee, const T& value)
{
   const byte* p = (const byte*)(const void*)&value;
   int i;
   for (i = 0; i < sizeof(value); i++)
       EEPROM.write(ee++, *p++);
   return i;
}

template <class T> int EEPROM_readAnything(int ee, T& value)
{
   byte* p = (byte*)(void*)&value;
   int i;
   for (i = 0; i < sizeof(value); i++)
       *p++ = EEPROM.read(ee++);
   return i;
}
File dataFile;
void SD_setup()
{
  //UNCOMMENT THESE TWO LINES FOR TEENSY AUDIO BOARD:
  //SPI.setMOSI(7);  // Audio shield has MOSI on pin 7
  //SPI.setSCK(14);  // Audio shield has SCK on pin 14

 // Open serial communications and wait for port to open
  Serial.print("Initializing SD card...");

  // see if the card is present and can be initialized:
  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    // don't do anything more:
    return;
  }

  Serial.println("card initialized.");
  dataFile = SD.open("datalog.txt", FILE_WRITE);
//   if(SD_open_cond){
//     dataFile = SD.open("datalog.txt", FILE_WRITE);
//     SD_open_cond = false;
//   }

  // if the file is available, write to it:
  String header = "time [ms],loops,alt [relative ft],pitch [deg],roll [deg],gx [deg/s],gy [deg/s], torq_x [N/m], torq_y [N/m]";
  if (dataFile) {
    dataFile.println(header);
    dataFile.close();
  }
  else {
    Serial.println("error opening datalog.txt");
  }

}

// make a string for assembling the data to log:
void SD_save(){ 
  String dataStream = "";

  dataStream += String(millis());
  dataStream += ",";
  dataStream += String(loops);
  dataStream += ",";
  dataStream += String(alt);
  dataStream += ",";
  dataStream += String(pitch->avg);
  dataStream += ",";
  dataStream += String(roll->avg);
  dataStream += ",";
  dataStream += String(gx->avg);
  dataStream += ",";
  dataStream += String(gy->avg);
  dataStream += ",";
  dataStream += String(torque[0]);
  dataStream += ",";
  dataStream += String(torque[1]);

  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
    dataFile = SD.open("datalog.txt", FILE_WRITE);
//   if(SD_open_cond){
//     dataFile = SD.open("datalog.txt", FILE_WRITE);
//     SD_open_cond = false;
//   }

  // if the file is available, write to it:
  if (dataFile) {
    dataFile.println(dataStream);
    dataFile.close();
    // if(loops % 80 == 0){
    //     dataFile.close();
    //     SD_open_cond = true;
    // }
    // print to the serial port too:
    if(bug.save_debug){
        Serial.println(dataStream);
    }
  }  
  // if the file isn't open, pop up an error:
  else {
    Serial.println("error opening datalog.txt");
  }
}