---
title: "Roadmap"
date:  "2024-04-19"
#menu:
#  main:
#      parent: Build your own!
weight: 60
TableOfContents: true
---

## Things could get better over time...

Although I am quite happy with how the system is working today, things can always be improved.

### Control box

- Optimize display performance of control box in case of an error<br>
	In case one of the sensors (yarn, overload, endstop) got hit it can happen, that the display will begin to flicker or at least need some time until the error message is clearly blinking on the screen. It does not happen all the time and I am not quite sure atm why it does that. Might have to do with how the main firmware loop is working. Has to be investigated further.<br><br>
- Poweroff stepper motor while the carriage is not running<br>
  At the moment, when the carriage is not running, the stepper motor is still powered by the driver, so that the control box does not loose track of its position in relation to the knitting bed. This has the sideeffect that you cannot move the carriage by hand while it is being coupled to the carriage mover. As the system already has a rotary encoder attached to it, it should be possible to poweroff the stepper motor and correct its internal position from any manually introduced positional deviance based on encoder position calculations. This would make moving the carriage by hand possible without disconnecting the coupling bolt and also save some power.<br><br>
  Technical background:<br>The stepper needs 1600 steps/rev and the rotary encoder delivers 2400 pulses/rev, so we can say that moving the stepper to any direction manually changes the current positional step value by *[0.6&#773; x no. pulses = **Δ** steps]*.<br>I already made some tests regarding the positional error between the stepper motor steps, which is controled by the driver, and the pulses the rotary encoder returns while the motor is turning: I get about 20 pulses difference while the motor is running, and about 5 pulses when it is not moving. As the rotary encoder returns 2400 pulses/rev I think its preciseness should be enough and we can ignore the pulse errors here. Otherwise we would have to introdice some kind of error map based correction which can get reeeaallyyy tricky!<br>

### 3D printed parts

- ~~The sensor mount for the overload sensor is currently mounted towards the front end of the spring lever near the spring. This is not the best for real fine adjustment of the sensor as the lever's movements naturally are much bigger at its front end than on its back end. I plan to reposition it to the back of the lever near the bolt so that smaller adjustments of the sensor mounting plate are necessary to cover a wider (and so logically more precise) adjustment range.~~ <br>Done. See `mover_plate_v2.stl` and additional chapter in the assembly documentation.<br><br>
- Due to feedback of the community (Thanks, Klaus!) I might think of offering the possibility to replace the endstop switches with reed switches or even hall effect sensors for contact-less switching. Reed switches could even be placed inside the groove of the aluminium extrusion which would literally make them disappear from the outside frame structure, what I'd prefer here. Have to think about it...