---
title: "Parts list"
date:  "2024-03-18"
menu:
  main:
      parent: Build your own!
weight: 6
TableOfContents: true
---

This document hopefully lists all the needed parts to build the open knitting motor.

## 3D printed parts

Print all of the parts you find in the `Kitting machine motor.3mf`. I used 6 walls, with 35% infill at 0.2mm layer height and tree support where needed. If you have a Bambulab X1C printer, you should be able to use the 3MF file directly without any further modifications.

Regarding the housing, you have to choose between TB6600 and DM542T because of the stepper driver.

For the foot switch, get the following part from its designer *flummer* directly via Printables:
Foot switch: https://www.printables.com/de/model/723123-foot-pedal-switch


## V-Slot Extrusions and accessories

- 2020:
  - 1700mm (1x, run bar)
  - 1160mm (1x, front rest bar)

- 2040:
  - 250mm (2x, mount post)
  - 200mm (2x, foot)

- corner brackets (8 minimum, more as you like)
- T-nuts (for frame connection, about 36, but it is handy two a some more)
- M5x8 T-nut pan head screws for the T-nuts
- M3 hammer nuts (for fixing endstops, etc.)
- end caps for extrusions (optional, if you like)

## Electronics

- Arduino UNO R3 (R4 support beginning with FW v0.4.6+138)
- DFRobot LCD Keypad shield
- LM2596 Step-down DC-DC Buck-Converter modul for 4-40V to 3,3-24V
- Passive(!) Arduino buzzer module
- Optical rotary A/B encoder E38S6G5-600B-G24N
- DC power supply 24V 10A
- IEC320 / C14 power jack (pre-wired at best)
- Nema 23 stepper motor type 23HS2430B [like this](https://www.sys-motor.com/show.aspx?id=165)
- Stepper driver alternatives (configured for 1600 steps):
    1. TB6600 stepper driver (works, but not recommended)
    2. DM542T (MUCH MUCH better, quieter, more precise, etc. etc.)
- 12V 3-pin fan (40x40x10), I used a Noctua fan, super quiet
- ZHIYU 1X3L fan controller [like this](https://www.ebay.com/itm/404706261069)
- (2x) Lerdge endstops [like this](https://shop.lerdge.com/products/mechanical-endstop-horizontal)
- (2x) small endstop switches (normally closed, footswitch+overload)
- (2x) reed contact 10mm (normally closed, yarn knot sensor)
- (2x) reed contact 14mm (normally open, yarn run out sensor)
- (4x) neodym magnet 6mm x 3mm (for reed contacts used in yarn sensor)
- (2x) GX16 4-pin male+female plug connector (for stepper motor and encoder)
- (3x-5x) Stereo jack connector socket panel mount (two endstops, footswitch, (optional) overload and yarn sensor)
- (3x-5x) 3.5mm stereo TRS plug
- something to connect all GND and +5V lines together - I used Wago splicing connectors with levers, but everthing else with which you can connect multiple wires into one should work :D
- A bunch of wires
    1. 24 AWG (multi-color): Arduino, rotary encoder, switches, reed contacts
    2. 18-20 AWG: power supply to step down, stepper+driver phase connections (better use 4 core cable with shielding)

## Additional mechanical parts

- **(2x) ribber clamps for your machine, you cannot use the whole setup without them**
- (1x) Openbuild V-slot gantry for 2020 with 4 wheels, [like this](https://openbuildspartstore.com/v-slot-gantry-kit-20mm/)
- (1x) GT2 pulley 30T 8mm bore
- (1x) GT2 idler pulley with 5mm bore
- (5m) GT2 belt 6mm width
- (1x) (heatbed) compression spring (len 20 mm/0.79 inch, od 7.5 mm/0.3 inch, id 5 mm/0.2 inch, wire 1.2 mm)
- a whole bunch of M3, M4, M5 and M6 screws and (nylon lock) nuts in different lengths
- about 500mm 0.8 piano wire
