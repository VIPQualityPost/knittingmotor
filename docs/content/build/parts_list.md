---
title: "Parts list"
date:  "2024-03-18"
#menu:
#  main:
#      parent: Build your own!
weight: 10
TableOfContents: true
---

This document hopefully lists all the needed parts to build the open knitting motor.

## 3D printed parts

If you have a Bambulab X1C printer and  use BambuStudio as your preferred slicer, print all of the parts you find in the `Open knitting motor.3mf`without any further modifications. I positioned every item the same way I printed them myself. 

If you have to use the single STL files, then, as a rule of thumb, 

- use 6 walls
- with 35% infill at 0.2mm layer height
- tree support where needed.

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

- [Openbuild V-slot gantry for 2020 with 4 wheels,](https://openbuildspartstore.com/v-slot-gantry-kit-20mm/)
- [corner brackets](https://openbuildspartstore.com/cast-corner-bracket/) (8 minimum, more as you like)
- T-nuts M5 (for frame connection, about 36, but it is handy two a some more)
- M5x8 T-nut pan head screws for the T-nuts
- M3 hammer nuts (for fixing endstops, etc.)
- end caps for extrusions (for esthetical reasons, if you like)

I cannot give you any advice where to buy your parts from, but for the extrusion, see the [Openbuilds Partstore](https://openbuildspartstore.com/) for reference.

## Electronics

I try to provide links to every part as an example, so that you can see what is meant exactly.

- [Arduino UNO R4 Wifi](https://store.arduino.cc/products/uno-r4-wifi) (DON'T use UNO R3, firmware does not support it anymore)
- [D1 Robot LCD Keypad shield](https://www.aliexpress.us/item/3256805923237105.html)<br>
  There are other  shields made by DFRobot, but they are slightly different and bigger. They won't fit into the keypad bezzle and also may react differently to the buttons attached, so try to find the exact same one made by D1 Robot.
- [LM2596 Step-down DC-DC Buck-Converter modul for 4-40V to 3,3-24V](https://www.amazon.de/gp/product/B08T1ZR6XG)
- [Passive(!) Arduino buzzer module](https://www.amazon.de/gp/product/B096ZWCG7F)
- [Optical rotary A/B encoder 600P/R rotary encoder](https://www.amazon.com/Signswise-Incremental-Optical-Encoder-Quadrature/dp/B085ZLCYS1)<br>
  Its technical no. is sometimes *LPD3806-600BM-G5-24C* or *E38S6G5-600B-G24N*.
- [DC power supply 24V 10A](https://www.amazon.de/gp/product/B0896XTJ14)
- [IEC320 / C14 power jack (pre-wired at best)](https://www.amazon.com/button-Adapter-Connector-Socket-MXRS/dp/B082ZFRV1B)
- [Nema 23 stepper motor type 23HS2430B](https://www.sys-motor.com/show.aspx?id=165)
- Stepper driver alternatives (configured for 1600 steps):
    1. [TB6600 stepper driver](https://www.amazon.com/Stepper-DC9-42V-Subdivision-Controller-MicroStepping/dp/B08SG7L54W) (works, but not recommended)
    2. [DM542T stepper driver](https://www.omc-stepperonline.com/search?search=dmt542t&description=true) (MUCH MUCH better, quieter, more precise, etc. etc.)
- [Noctua 12V 3-pin fan (40x40x10)](https://www.amazon.com/Noctua-Cooling-Blades-Bearing-NF-A4x10/dp/B009NQLT0M)
- [ZHIYU 1X3L fan controller](https://www.ebay.com/itm/404706261069)
- (2x) [Lerdge endstops ](https://shop.lerdge.com/products/mechanical-endstop-horizontal)
- (2x) [small endstop switches](https://www.amazon.de/gp/product/B07P85749W) (normally closed, footswitch+overload)
- (2x) [GPS-11B reed contact 10mm](https://www.ebay.com/itm/284819532789) (normally closed, yarn knot sensor)
- (2x) [GPS-14A reed contact 14mm](https://www.ebay.com/itm/284819532789) (normally open, yarn run out sensor)
- (4x) [neodymium magnet 6mm x 3mm](https://www.ebay.com/itm/305313785899) (for reed contacts used in yarn sensor)
- (2x) [GX16 4-pin male+female plug connector](https://www.ebay.com/itm/335215956016) (for stepper motor and encoder)
- (3x-5x) [Stereo jack connector socket panel mount](https://www.ebay.com/itm/385445764175) (two endstops, footswitch, (optional) overload and yarn sensor)
- (3x-5x) [3.5mm stereo TRS plug](https://www.amazon.com/Fancasee-Replacement-Connector-Headphone-Earphone/dp/B07Y8KGJKL)
- something to connect all GND and +5V lines together - I used Wago splicing connectors with levers, but everthing else with which you can connect multiple wires into one should work :D
- A bunch of wires
    1. 24 AWG (multi-color): Arduino, rotary encoder, switches, reed contacts
    2. 18-20 AWG: power supply to step down, stepper+driver phase connections (better use 4 core cable with shielding)

## Additional mechanical parts

- **(2x) [original ribber clamps for KR830/850](https://theknittingcloset.com/products/knitting-machine-ribber-parts), you cannot use the whole setup without them**
- (1x) [GT2 pulley 30T 8mm bore](https://www.amazon.com/Saipe-Synchronous-Teeth-Bore-8mm/dp/B09JS7CZNM)
- (1x) [GT2 idler pulley 5mm bore](https://www.amazon.com/3Dman-Toothless-Aluminum-Timing-Printer/dp/B07RV2T54M)
- (5m) [GT2 belt 6mm width](https://www.amazon.com/Timing-Printer-FYSETC-Fiberglass-Reinforced/dp/B07BRKZGMS)
- (1x)  compression spring (len 20 mm/0.79 inch, od 7.5 mm/0.3 inch, id 5 mm/0.2 inch, wire 1.2 mm) - i. e. for 3D printer heatbed
- a whole bunch of M3, M4, M5 and M6 screws and (nylon lock) nuts in different lengths
- about 500mm 0.8 piano wire
