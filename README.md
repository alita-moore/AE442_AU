# AE442_AU
code for the arduino onboard controllers

## Notes about the vsCode - Teensy environment
*  The ".vscode/c_cpp_properties.json" ".vscode/tasks.json" ".vsteensy/vsteensy.json" and makefile will likely be different for each computer because they include specific file paths for finding libraries, compilers, loaders, and the like.
*  The best thing to do is to use visualteensy yourself after you've cloned the repo.
*  By default, visualteensy chooses the TyCommander uploader and we will use the PRJC uploader as it comes with teensyduino. Assuming you already have teensyduino installed simply add the file path the teensy.exe to the "PRJC Uploader" option in setting and clear the information in the TyCommander option. It should be the same file path that was autofilled under TyCommander. Mine is "C:\Program Files (x86)\Arduino\hardware\tools" For reference.
*  Lastly, the visualteensy does not generate c_cpp_properties.json properly. Atleast not for me on windows with version 0.9.7.2. I had to make the following change: 
`"compilerPath": "C:/PROGRA~2/Arduino/hardware/tools/arm/bin/arm-none-eabi-gcc.exe",`
to
`"compilerPath": "C:/Program Files (x86)/Arduino/hardware/tools/arm/bin/arm-none-eabi-gcc.exe",`
