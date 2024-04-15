---
title: "Operation manual"
date: "2024-03-18"
menu: "main"
weight: 4
TableOfContents: true
---

# Open Knitting Motor Project - Operation manual

## 1. System startup

After powering on the control box, you will have to reset (aka homing) the knitting carriage once by pressing the `SELECT` button.

*Please make sure that during homing:*

- *no needles are in working position or*
- *that both PART buttons on the carriage are pressed and needles are in E position*
- *simply disengage the carriage from the mover plate. After homing, the carriage will return to the zero position on the right and the motor is ready to knit.*

**(It can happen, that the carriage activates the zero position endstop when it returned right after homing. Simply ignore this error message and clear it by pressing the `SELECT` button.)**

To start knitting a single row right away, all you have to do is to just hit the foot pedal and the motor should start transporting the carriage one complete row. After the row has finished the carriage will knit in the opposite direction, if you hit the pedal again.

*You find knitting single rows to be too tedious? Me too...*
No problem. If you want the motor to continuously knit a decent amount of rows, you simply setup the control box in `auto` mode (which is by default), enter a defined no. of rows and start the continuous knitting process via the buttons on the control box, or with the foot pedal, if it is configured for continuous knitting. See part 3. of the manual on how to do this.

***
*There is variant regarding continuous knitting:*

If you simply want the motor to continuously knit, regardless of the row count, you can set the control box into `manual` mode. Then you can start continuous knitting and the control box will only count the rows knitted so far, but will not stop on its own.
***

If, for whatever reason, you encounter any problems (some unknown error, menu not responding, whatever) and want to start over, simply press the `RESET`button. The button will restart the control box and you are good to go. You will not loose the row count, if your box is in `auto` mode.(I.e. menu not responding *can* happen after one of the safety sensors was engaged.)

## 2. Daily usage, the menu system and important parameters

**Needle bed position naming convention**

			                                       needle bed
			      100 ---------------------------------0---------------------------------100
			       |                  |                |                 |                |
			    max position          |                |                 |          zero position
			                          |           mid position           |
			                    left boundary                        right boundary


### General menu handling

Press and hold the `UP` button to open the menu of the control box.

- Scroll through the menu with `UP` and `DOWN` buttons
- If submenus are available, a small arrow on the left or right of the menu item will appear. Now change the menu depth with `RIGHT` (descend) or `SELECT` (ascend)
- To edit a configuration value press `SELECT` -> a small arrow in front of the value will appear
- Change values with `UP` or `DOWN` button
- Finish editing a value by pressing `SELECT` again
You can use the `Back` menu options to change the submenu one level up and the `Exit` menu command to exit the menu completely. Alternatively, you can simply wait 5 seconds, and the main menu will close. Hold in mind that the menu won't close as long as you are editing a parameter, like row count, motor speed, etc..

### Main daily usage parameters and handling

- Set row count (will be saved and reloaded)<br>
  Can be set between 1 and 999 rows.<br>The meaning of "row count" is different between operation modes:<br>
	
	- `auto` mode: Setup a number of rows to knit
  - `manua` mode: Correct the no. of rows knitted here to reflect any manual interactions which the control box can not account for on its own
  
- Boundaries (Left / Right boundary)<br>
  If you don't want to always knit the whole needle bed from left to right but rather a subsection in the middle to save time, you can set a left and right boundary for the carriage. 
  
  
  If you press `SELECT`to edit one of the boundary options the carriage will behave as follow:

  1. go to the middle of the needle bed, if no boundary has been set before
  2. return to the last saved position
  
	You can then move the carriage left and right with the corresponding `LEFT` or `RIGHT` button on the control box and the new position will be saved. A short press will move the carriage a very short amount, press and hold the button for a bigger step.

  Beware, that you can only set the left boundary further to the left of the middle of the bed, and it is the opposite with the right boundary. They can not be cross-set aka left boundary on the right and right boundary on the left.
  
  The boundaries don't have to be symmetrically aligned, but always have to be placed around the 0 needle position (middle of the bed).

**Carriage behaviour when you start knitting with boundaries:**

If the carriage is on the zero position, it will knit to the left boundary. Is the carriage on the max position, it will knit to the right boundary. Every further row will then be knitted between these two boundaries.

#### Manual carriage movement

If for some reason you want to bring the carriage over to the max or zero position regardless of the boundaries set, press and hold the `LEFT` or `RIGHT` button and the carriage will first start to move to the assigned boundary. If you do exactly the same again after the carriage has reached the boundary it will move again in the same direction until the endpoint is reached.

Normally, boundary values are saved. If there are any problems with the carriage endstop positions when you turn the power on and after the first calibration routine (homing), the boundary values will be automatically set to 0 and you have to dial them in again.

<u>*BEWARE: Make sure no needles are engaged while setting the boundaries!*</u>

#### The yarn sensor and its activation

In case your system is equipped with a yarn sensor, the control box is able to detect, if the yarn runs out or if there is a knot in the yarn, for main and secondary color. You can activate the sensor for both colors independently, but runout and knot sensing will always be activated together per side.

So, if you activate main yarn sensor, it will automatically watch for yarn runout *and* knots in the yarn at the same time.

If the yarn sensor is activated during knitting, the following will happen:
- you get a message on screen, which you have to acknowledge by pressing `SELECT`
- the display will show an extra symbol `E` (see below for symbol description)
- the row counter will not be decreased (`auto` mode) or increased (`manual` mode) if you begin to knit again
- if you restart knitting, single row or continuous, the current row direction will be continued (!)

## 3. Configuration settings

To change any of the following parameters, open the control box menu and go to `Settings`.

- Operation mode (default: `auto`)<br>
  - `auto`: Automatic knitting according to the given row count, machine will stop when all rows are done. Continuous knitting can only be started, if a positive row count is set.
  Knitting single rows is always possible via foot switch or `LEFT` and `RIGHT` buttons.
  - `manual`: The machine will only count the rows knitted.
  *Changing this configuration parameter will automatically clear the row count value and set it to 0!*<br><br>
- Foot pedal mode (default: `single`)<br>
  - `single`:     The foot switch will start knitting one row.
  - `continuous`: The foot switch will start continuous knitting. Hit the foot switch again to stop continuous knitting after the current row is done.<br><br>
- Carriage speed (default: `80`)<br>
  You can raise and lower the carriage speed with a certain range. Please be careful and only raise the value in small steps and try again until it works as desired.<br><br>
- Yarn sensor main (default: `off`)<br>
  Activate or deactivate, if the control box should check for messages from the main (color) yarn sensor.
  If you don't use one, set to `off`.<br>
  ***Changing this parameter requires a manual `REBOOT` of the control box.***<br><br>
- Yarn sensor sec (default: `off`)<br>
  Activate or deactivate, if the control box should check for messages from the secondary (color) yarn sensor.
  If you don't use one, set to `off`.<br>
  ***Changing this parameter requires a manual `REBOOT` of the control box.***<br><br>
- Overload sensor (default: `off`)<br>
  Activate or deactivate, if the control box should check for messages from the overload sensor.
  If you don't use one, set it to `off`.<br>
  ***Changing this parameter requires a manual `REBOOT` of the control box.***<br><br>
- Direction arrow (default: `on`)<br>
  During knitting you will be presented with a small combination of symbols on the lower right side of the display right next to the current row count, showing you the knitting direction, if continuous knitting is active and if boundaries are set.
  If you set this option to `show`, the symbol combination will be kept on screen even after the carriage has stopped.
  If you set this to `hide`, the symbols will only be visible as long as the carriage is running.

  *Possible symbols:*

      <-  -> : running direction of the carriage
      C      : continuous knitting active
      E      : shown, if the yarn sensor was activated in current row
      [    ] : boundaries set

  Examples:
  - `[C->]`: last row was knitted to the right between boundaries and continuous knitting is on
  - `<-`   : last row was knitted to the left
  - `[E]`  : during last row between boundaries, the yarn sensor was activated<br><br>
- Button beep (default: `on`)<br>
  If your control box is equipped with a buzzer, you can turn beeps on button press on or off.<br><br>
- Alarm tune (default: `Entertainer`)<br>
  If you are in `auto` mode and have a buzzer installed, an alarm song will be played to show that everything finished.
  Choose the one you like the most.<br><br>
- LCD backlight (default: `100%`)<br>
  If your LCD display is too bright for you, you can adjust the brightness between 33%, 66% and 100%.

## 4. Button usage on main screen

**Control box - button naming convention**

                                      UP
                                      |
                     SELECT -- LEFT --|-- RIGHT -- REBOOT
                                      |
                                      DOWN

| Button   | Press duration | Function                                                            |
| :------- | :----:         | :------------------------------------------------------------------ |
| `SELECT` | short press    | after power on, start homing                                        |
| `SELECT` | press and hold | start continuous knitting                                           |
| `LEFT`   | short press    | move carriage a short way (about two needles) to the left           |
| `LEFT`   | press and hold | move carriage to left boundary and/or max position              |
| `UP`     | press and hold | open menu                                                           |
| `DOWN`   | short press    | stop continuous knitting after current row<br><br>*(Due to the underlying system, it *can* sometimes last up to two rows <br>of knitting until the carriage stops.<br>But, normally it stops after the current row has finished.)*                                               |
| `DOWN`   | press and hold | zero values "Rows to knit" / "Knitted rows"                         |
| `RIGHT`  | short press    | move carriage a short way (about two needles) to the right          |
| `RIGHT`  | press and hold | move carriage to right boundary and/or zero position             |

## 5. Special menu items

You find some additional, less important options in the menu:

- Boundaries / Clear boundaries<br>
  Reset boundary values to 0<br><br>
- Tools / Home carriage<br>
  Tools / Move to (start/middle/end)<br>
  Home carriage starts the self-calibration routine again you are faced with when you power on the control box.
  This can be necessary if there was a failure with the stepper motor, or after the coupler between the carriage mover and the carriage itself has been disconnected because of a blockage of the carriage.
  The `Move to...` options are only there for convenience and will behave exactly the same as the display buttons, although you can only move the carriage to the middle of the bed from here.<br><br>
- Info<br>
  Will show the current firmware version.<br><br>
- Reset all<br>
  Resets all firmware parameters to default. Please restart control box afterwards via the `REBOOT` button.

## 6. Error messages

| Error message | Description                                 | Solution                                    |
| :----         | :-----                                      | :-----                                      |
| HIT 1 - MAX   | Endstop in maximum position was activated   | press `SELECT` to clear, re-home carriage   |
| HIT 2 - HOME  | Endstop in zero position was activated      | press `SELECT` to clear, re-home carriage   |
| YARN 3 - MAIN | Yarn sensor main color activated            | check, if knot in yarn or run out           |
| YARN 4 - SEC  | Yarn sensor secondary color activated       | check, if knot in yarn or run out           |
| ERR 1 - FOOT  | No foot switch attached                     | attach foot switch                          |
| ERR 2 - OVRL  | Overload sensor activated                   | clear blockage or carrier, re-home carriage |
