#include <EEPROM.h>

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

// void save_eeprom(float val, int size){
//   /***
//     Write the value to the appropriate byte of the EEPROM.
//     these values will remain there when the board is
//     turned off.
//   ***/

//   EEPROM.write(addr, val);

//   /***
//     Advance to the next address, when at the end restart at the beginning.
//   ***/

//   addr = addr + size;
//   if(addr == EEPROM.length())
//     addr = 0;
// }