# Open Knitting Motor - Firmware

The project is being developed under Visual Studio Code with the PlatformIO plugin. If you use the same IDE, you should not have any problems compiling and flashing the firmware to the Arduino, as all necessary external libraries will be configured via `platformio.ini`.

The firmware comes in two different languages at the moment, German or English. You can change this in `platform.io`via the `build_flags` parameter:

- -D LANG_DE (for German) <br>
or <br>
- -D LANG_EN (for English)

I did not build a live language option into the firmware, as it won't happen too often that a user wants to change the language while using the machine. So, the language will be compiled directly into the firmware.

If you want to enable some more (serial) DEBUG options, you can setup some additional `build_flags` in platformio.ini:

- -D DEBUG: will enable general debug output, has to be defined always for debuggin
- -D DEBUG_APPMODE: will generate debug messages regarding the firmware operation itself
- -D DEBUG_POSITION: will generate additional information regarding the stepper and encoder operation

<u>**BEWARE: The project is NOT compatible with the newer Arduino UNO R4**</u>


