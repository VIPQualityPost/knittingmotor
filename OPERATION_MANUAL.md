OKM - Open Knitting Motor operation manual
==========================================

1.) Configuration
-----------------

Open include/HardwareConfig.h and set up necessary Arduino pins. Please only change any of the other hardware parameters, like stepper and encoder resolution, if you exactly know what you are doing.

2.) Main operation
------------------

After power on, you will have to home the knitting carriage once. So please make sure that no needles are in working position or, alternatively, that both PART buttons on the carriage are pressed. After homing, the carriage will return to the starting point on the right and the motor is ready to knit.

Just hit the foot pedal and the motor should start knitting one complete row. If you hit the pedal again, the carriage should knit in the opposite direction.

If you want the motor to continuously knit a decent amount of rows, you have to setup the control box in "auto" mode and, set a row count and start continuous knitting. See further details on how to do this.

If you simply want the motor to continuously knit, regardless of the row count, you set the control box into "manual" mode. Then you can start continuous knitting.

                              Control unit - button naming convention

                                                 UP
                                                 |
                                SELECT -- LEFT --|-- RIGHT -- REBOOT
                                                 |
                                                DOWN


3.) Firmware configuration parameters
-------------------------------------

  General menu handling:
  Press and hold the UP button to open the configuration menu. If you do nothing inside the menu for 5 seconds, it will close automatically.

    - Scroll through the menu with `UP` and `DOWN` buttons
    - If submenus are available (small arrow on the right), change menu depth with `RIGHT` (descend) or `SELECT` (ascend)
    - Select value to edit with `SELECT` -> a small arrow in front of the value will appear
    - Change values with `UP` or `DOWN` button
    - Finish editing a value with `SELECT` again
  You can use the `Back` menu options to change the submenu and the `Exit` menu command to exit the menu itself, or you can simply wait 5 seconds, and the main will close.

- Set row count (saved and reloaded)
  Setup a number of rows to knit while continuous knitting is active.

- Boundaries (Left / Right boundary)
  If you don't always want to knit the whole needle bed from left to right but rather a subsection in the middle, you can set a left and right boundary for the carriage.

  If you enter one of these options the carriage will behave as follow:
   a) go to the middle of the needle bed, if no boundary has been set before
   b) return to the last saved position
     
  You can then move the carriage left and right with the corresponding buttons on the control box and the new position will be saved.
  Beware, that you can only set the left boundary further to the left of the middle of the bed, and it is the opposite with the right boundary. They can not be cross-set aka left boundary on the right and right boundary on the left.

                                              needle bed
    |----------------------------------------------|----------------------------------------------|
  max position               |              mid position                    |               zero position
                          left boundary                               right boundary

  The boundaries don't have to be symmetrically alined, but have to always around the 0 needle position.

  Carriage behaviour when you start knitting with boundaries:
  If the carriage is on the right side of the middle of the bed, it will go to the left boundary. Is the carriage on the left side, it will knit to the right side of the bed.

  Normally, boundary values are saved. If for some reasons there are any problems with the carriage endstop positions, the boundary values will be set to 0 after power on.

  BEWARE: Make sure no needles are engaged while setting the boundaries!

- Operation mode (`auto` by default)
  `auto`:   Automatic knitting according to the given row count, machine will stop when all rows are done.
          Continuous knitting can only be activated, if a positive row count is set.
          Knitting single rows is always possible via foot switch or buttons.
  `manual`: The machine will only count the rows knitted and let you continuously knit as long as you want.

  Changing this configuration parameter will automatically clear any row count values and set them to 0.

- Foot pedal mode (`single` by default)
  `single`:     The foot switch will initiated knitting one row.
  `continuous`: The foot switch will start continuous knitting.

- Carriage speed (`80` by default)
  You can raise and lower the carriage speed with a certain range. Please be careful and only raise the value in small steps and try again until it works as desired.

- Yarn sensor (`off` by default)
  Activate or deactivate if the control box should check for messages from the yarn sensor. If you don't use one, set to `off`.

- Overload sensor (`off` by default)
  Activate or deactivate if the control box should check for messages from the overload sensor. If you don't use one, set it to `off` (default).

- Direction arrow (`on` by default)
  During knitting, besides the row count, you will be presented with a small combination of symbols on the lower right side of the display, showing you the knitting direction, if continuous knitting is active and if boundaries are set.
  If you set this option to `show`, the symbol combination will be kept on screen even after the carriage has stopped.
  If you set this to `hide`, the symbols will only be visible as long as the carriage is running.

- Button beep (`on` by default)
  If your control unit is equipped with a buzzer, you can turn its usage on or off here.

- Alarm duration (2 sec default)
  If you are in auto mode and have a buzzer installed, an alarm will turn on after the last row has been knitted. Adjust the length of the alarm to your liking.

- LCD backlight (100% default)
  If your LCD display is too bright, you can adjust the brightness. 

4.) Basic usage
---------------

Main screen button combinations:

|----------|----------------|--------------------------------------------------------------------------------------------------- |
|  Button  | Press duration | Function                                                                                           |
|----------|----------------|--------------------------------------------------------------------------------------------------- |
| `SELECT` | short press    | after power on, start homing                                                                       |
|----------|----------------|--------------------------------------------------------------------------------------------------- |
| `SELECT` | press and hold | start continuous knitting                                                                          |
|----------|----------------|--------------------------------------------------------------------------------------------------- |
| `LEFT`   | press and hold | move carriage to its max position (ignores boundaries)                                             |
|----------|----------------|--------------------------------------------------------------------------------------------------- |
| `UP`     | press and hold | open menu                                                                                          |
|----------|----------------|--------------------------------------------------------------------------------------------------- |
| `DOWN`   | short press    | stop continuous knitting while carriage is runnning                                                |
|          |                | (Due to the underlying system, it *can* sometimes                                                  |
|          |                | last up to two rows of knitting until the carriage stops.)                                         |
|----------|----------------|--------------------------------------------------------------------------------------------------- |
| `DOWN`   | press and hold | zero "Rows to knit" / "Knitted rows"                                                               |
|----------|----------------|--------------------------------------------------------------------------------------------------- |
| `RIGHT`  | press and hold | move carriage to its zero position (ignores boundaries)                                            |
|----------|----------------|--------------------------------------------------------------------------------------------------- |

Special menu items:

You find some additional options with which you can further customize the machine in the `Settings` menu.

- Boundaries / Clear boundaries
  Reset boundary values to 0

- Tools / Home carriage / Move to (start/middle/end)
  Home carriage starts the self-calibration routine again you are faced with when you power on the machine.
  This can be necessary if there was a failure with the stepper motor or after the coupler between the carriage mover and the carriage itself has been disconnect because of a blockage of the carriage.

  The `Move to...` options are only there for convenience and will behave exactly the same as the display buttons.

- Info
  Will show the current firmware version.

- Reset all
  Resets all firmware parameters to default. Please restart unit afterwards.