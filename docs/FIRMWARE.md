# Firmware

The project is being developed under Visual Studio Code with the PlatformIO plugin. If you use the same IDE, you should not have any problems compiling and flashing the firmware to the Arduino, as all necessary external libraries will be configured via `platformio.ini`.

If you want to enable some more (serial) DEBUG options, you can setup some additional `build_flags` in platformio.ini:

- -D DEBUG: will enable general debug output, has to be defined always for debuggin
- -D DEBUG_APPMODE: will generate debug messages regarding the firmware operation itself
- -D DEBUG_POSITION: will generate additional information regarding the stepper and encoder operation

<u>**BEWARE: The project is NOT compatible with the newer Arduino UNO R4**</u>


