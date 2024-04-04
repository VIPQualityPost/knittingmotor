# OKM - Open Knitting Motor project
===================================

THIS PROJECT AND ITS DOCUMENTATION IS WORK IN PROGRESS - EVEN THOUGH THE ACTUAL MACHINE WORKS, THERE ARE STILL SOME DOCUMENTATIONAL PARTS MISSING, SO IT MIGHT NOT BE AS EASY TO FOLLOW EVERYTHING AS IT SHOULD!


This project intends to provide a knitting motor for the common Brother flat bed knitting machines. It is originally build for a Brother KH 830 knitting machine, but the parts can be customized easily, so that it can be used with different machines.

It provides currently the following features:

*Features:*
- manual knitting with simple row counter
- automatic knitting a pre-defined no. of rows
- foot pedal control (single row / continuous knitting)
- knitting of the whole bed or a subsection
- setup and row data saved between power on/off
- alarm song after pre-defined no. of rows done
- user definable speed setting

*Machine operation and safety features:*
- high resolution stepper motor driver
- electronic endstops and carriage transport overload protection
- mechanic (spring driven) carriage coupler (additionally to overload protection)
- yarn and knot sensor (optional)

Current documentation: https://pages.open-mind.space/knittingmotor

Building this device is not for the faint-hearted, because you will have to solder, crimp connectors, even deal with mains connections (! PLEASE BE CAREFUL ! I AM NOT RESPONSIBLE IF SOMETHING GOES WRONG HERE !), building and uploading the firmware to the Arduino, and in the end, debug the whole thing if something does not work as expected.

So, if you are not familiar with doing things like this, please ask a friend with appropriate skills for help.
