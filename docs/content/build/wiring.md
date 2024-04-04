---
title: "Wiring"
date:  "2024-03-18"
#menu:
#  main:
#      parent: Build your own!
TableOfContents: true
weight: 9
index: false
---

## How to wire all parts?

I try to describe how I wired everything together the best I can. Sorry, no schematic available, but it should not be too complicated.

## PIN defaults

See `HardwareConfig.h` for the pin assignment defaults.

The minimum sensor (aka switch) pins you have to use are:
- home endstop
- max endstop
- foot switch

*Regarding yarn sensor / overload sensor pins:
You HAVE to assign valid pin numbers, even though you do not have to use the sensors as of the current firmware implementation.*

## External connectors

    GX16 4pin                   : stepper motor connection
    GX16 4pin                   : rotary encoder
    Stereo plug (GND + Tip)     : endstop max (left side of needle bed)
    Stereo plug (GND + Tip)     : endstop zero (right side of needle bed)
    Stereo plug (GND + Tip)     : foot switch
    Stereo plug (GND + Tip)     : overload sensor
    Stereo plug (GND, Ring, Tip): yarn sensor (main+secondary)

### PIN assignment
If you want to change the pin assignment to the Arduino for any reason, you will have to change the associated values in `include\HardwareConfig.h` and recompile the firmware.

## Common connection rails

### GND connection rail

    Arduino / LCD Shield
    all switches
    rotary encoder
    passive buzzer

### VIN (+5V) connection rail

    rotary encoder
    TB6600/DM542T Ena+
    TB6600/DM542T Dir+
    TB6600/DM542T Pul+
    passive buzzer

## Power supply

    24V volt to TB6600/DM542T
    24V volt to LM2596 Buck (In)

**Set Buck converter to 9 Volt.**


## Buck converter

    GND (Out): Arduino/LCD Shield GND
    GND (Out): Fan
    7V+ (Out): Arduino/LCD Shield VIN
    7V+ (Out): Fan

## Arduino + LCD Keypad Shield

Simply plug them together ;-)

Because of the design of the upper housing, you will have to solder wire connections and plugs to all available connectors on the LCD Keypad Shield, which are (D0-D3,D11-D13,A1-A5).

## LCD Keypad Shield

    D0:  passive buzzer
    D1:  overload sensor (normally closed)
    D2:  Rotary Encoder A
    D3:  Rotary Encoder B
    D11: TB6600/DM542T Pul-
    D12: TB6600/DM542T Dir-
    D13: TB6600/DM542T Ena-

    A1:  yarn sensor secondary (normally closed)
    A2:  yarn sensor main (normally closed)
    A3:  footswitch   (normally closed)
    A4:  endstop max  (normally closed)
    A5:  endstop zero (normally closed)

## TB6600 / DM542T

Use a GX16 connector.

(Only outgoing connections described, see VIN rail and LCD Keypad Shield for the rest of the connections.)

A+ / A- / B+ / B-: to stepper motor via GX16 connector

*Hint:
To find out, which cables belong to which phase, simply hold two wires together and try to turn the motor by hand. If you feel quite a lot of resistance while the cables are held together, you found a phase ;-)*

After you hooked up the motor, you will have to check, if your wiring leads to the correct turning direction of the stepper. As a hint, if you mount the GT2 pulley to the motor and look right at it, the motor has to turn CCW when moving the carrier to the zero (right) position and CW to the max (left).

If the motor turns the opposite way, then simply rewire the cables on motor connector on the driver the other way around.

See this as example for rewiring the direction:

    A+: white    -> becomes   A+: green
    A-: blue     -> becomes   A-: brown
    B+: brown    -> becomes   B+: blue
    B-: green    -> becomes   B-: white

Regarding DM542T: make sure that the control voltage switch on top is set to 5V, or nothing will happen - don't ask how I know...

## Rotary enoder

Use a GX16 connector.

    GND  : Arduino GND **(!IMPORTANT!)**
    5V   : Arduino 5V (you have to make sure, that your Arduino
           DOES deliver real 5V, not 4.98V or something **(!IMPORTANT!)**
    GREEN: D2
    WHITE: D3

## Endstops and sensor pins

Use the stereo connectors.

See description under **LCD Keypad Shield** to find out where every switch has to be connected to. They are all tied to GND.

Only the yarn sensor will have all three contacts involved, as one cable will transport both signals for main and secondary yarn sensor. After everything is connected you may have to check if you used the right connections for main and secondary. In case they are flipped you simply have to swap the two signal cables accordingly.

