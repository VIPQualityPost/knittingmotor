---
title: "Firmware"
date:  "2024-04-04"
#menu:
#  main:
#      parent: Build your own!
weight: 8
TableOfContents: true
index: false
---

The project is being developed under Visual Studio Code with the PlatformIO plugin. If you use the same IDE, you should not have any problems compiling and flashing the firmware to the Arduino, as all necessary external libraries will be configured via `platformio.ini`.

## 1. Choosing the right firmware branch

The firmware is (theoretically) available in two different versions:
- Arduino R3 support (`master` branch) - NOT SUPPORTED ANYMORE - ONLY FOR REFERENCE - DONT USE!
- Arduino R4 support (`r4wifiv2` branch)
  - [Get latest release here](https://gitlab.open-mind.space/pages/knittingmotor/-/releases/permalink/latest)

Choose the firmware branch according the Arduino UNO type you are using. The branches are not cross-compatible, so you cannot simply compile the `master` branch and use it wth the Arduino R4. This simply won't work.

## 2. Firmware pre-build configuration

Under normal circumstances you should use my pre-defined pin assignments, but in case you want to change something, you will have to adopt your changes before compiling the firmware. 
Open include/HardwareConfig.h and make all necessary changes.

<u>*Please only change any of the hardware parameters, like stepper and encoder resolution, if you exactly know what you are doing.*</u>

## 3. Language configuration

The firmware initially comes in two different languages at the moment, German or English. You can change this in `platformio.ini` via the `build_flags` parameter:

- -D LANG_DE (for German) 
or
- -D LANG_EN (for English)

I did not build a live language option into the firmware, as it won't happen too often that a user wants to change the language while using the machine. So, the language will be compiled directly into the firmware.

If you want to add another language, see `Lang.h` for how it is done.

## 4. Serial debug options

If you want to enable some more (serial) DEBUG options, you can setup some additional `build_flags` in platformio.ini:

- -D DEBUG: will enable general debug output, has to be defined always for debuggin
- -D DEBUG_APPMODE: will generate debug messages regarding the firmware operation itself
- -D DEBUG_POSITION: will generate additional information regarding the stepper and encoder operation

