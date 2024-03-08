# Open Knitting Motor - Wiring

## How to wire all parts?

I try to describe how I wired everything together the best I can. Sorry, no schematic available, but it should not be too complicated.

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

### GND connections rail

    Arduino / LCD Shield
    all switches
    rotary encoder
    passive buzzer

### VIN (+5V) connections rail

    rotary encoder
    TB6600 Ena+
    TB6600 Dir+
    TB6600 Pul+
    passive buzzer

## Power supply

    24V volt to TB6600
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

    D13: TB6600 Ena-
    D12: TB6600 Dir-
    D11: TB6600 Pul-
    D3:  Rotary Encoder B
    D2:  Rotary Encoder A
    D1:  overload sensor (normally closed)
    D0:  passive buzzer
    A5:  endstop zero (normally open)
    A4:  endstop max  (normally open)
    A3:  footswitch   (normally closed)
    A2:  yarn sensor main (normally closed)
    A1:  yarn sensor secondary (normally closed)

## DM542T / TB6600

Use a GX16 connector.

(Only outgoing connections described, see VIN rail and LCD Keypad Shield for the rest of the connections)

A+ / A- / B+ / B-: to stepper motor via GX16 connector

*Hint:<br>
To find out, which cables belong to which phase, simply hold two wires together and try to turn the motor by hand. If you feel quite a lot of resistance while the cables are held together, you found a phase ;-)*

After you hooked up the motor, you will have to check, if your wiring leads to the correct turning direction of the stepper. As a hint, if you mount the GT2 pulley to the motor and look right at it, the motor has to turn CCW when going to the zero position and CW to the max.

If the motor turns the opposite way, then simply rewire the cables on motor connector on the TB6600 the other way around.

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

## Endstops and sensor pin

Use stereo connectors.

See description under **Endstop connectors** on top of the document. There is nothing more to it.
