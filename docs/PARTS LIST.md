# Open Knitting Motor - Parts list

This document hopefully lists all the needed parts to build the open knitting motor.

## 3D printed parts

Print all of the parts you find in the `Kitting machine motor.3mf`. I used 6 walls, with 35% infill at 0.2mm layer height and tree support where needed. If you have a Bambulab X1C printer, you should be able to use the 3MF file directly without any further modifications.

Regarding the housing, you have to choose between TB6600 and DM542T because of the stepper driver.

## V-Slot Extrusions and accessories

- 2020:
  - 1700mm (1x)
  - 1160mm (1x)

- 2040:
  - 250mm (2x)
  - 200mm (2x)

- corner brackets (8 minimum, more as you like)
- T-nuts (35 minimum)
- M5x8 T-nut pan head screws
- end caps for extrusions, if you like

## Electronics

- Arduino UNO R3 (R4 support since 0.4.6+78 in separate fw branch)
- DFRobot LCD Keypad shield
- LM2596 Step-down DC-DC Buck-Converter modul for 4-40V to 3,3-24V
- Passive buzzer module
- Optical rotary A/B encoder E38S6G5-600B-G24N
- DC power supply 24V 10A
- IEC320 / C14 power jack
- Nema 23 stepper motor type 23HS2430B [like this](https://www.sys-motor.com/show.aspx?id=165)
- Stepper driver alternatives (configured for 1600 steps):
    1. TB6600 stepper driver (works, but not recommended)
    2. DM542T (MUCH better, quieter, more precise, etc. etc.)
- 12V 2-pin Fan (40x40x10)
- (2x) Lerdge endstops [like this](https://shop.lerdge.com/products/mechanical-endstop-horizontal)
- (2x) small endstop switches (normally closed, footswitch+overload)
- (4x) reed contact 10mm (normally closed, yarn sensing)
- (2x) GX16 4-pin male+female plug connector (for stepper motor and encoder)
- (3x-4x) Stereo jack connector socket panel mount (for endstops, footswitch, (optional) overload and yarn sensor)
- (3x-4x) 3.5mm stereo TRS plug
- something to connect all GND and +5V lines together - I used Wago splicing connectors with levers, but everthing else with which you can connect multiple wires into one should work :D

## Mechanics

- (1x) Openbuild V-slot gantry for 2020 with 4 wheels, [like this](https://openbuildspartstore.com/v-slot-gantry-kit-20mm/)
- (1x) GT2 pulley 30T 8mm bore
- (1x) GT2 idler pulley with 5mm bore
- (5m) GT2 belt 6mm width
- (1x) (heatbed) compression spring (len 20 mm/0.79 inch, od 7.5 mm/0.3 inch, id 5 mm/0.2 inch, wire 1.2 mm)
- a whole bunch of M3, M4, M5 and M6 screws in different lengths
- about 500mm 0.8 piano wire
