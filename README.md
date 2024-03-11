# OKM - Open Knitting Motor project
===================================

This project intends to provide a knitting motor for the common Brother flat bed knitting machines. It is originally build for a Brother KH 830 knitting machine, but the parts can be customized easily, so that it can be used with different machines.

It provides currently the following features:

*User features:*
- manual knitting with simple row counter
- automatic knitting a pre-defined no. of rows
- foot pedal control (single row / continuous knitting)
- knitting of the whole bed or a subsection
- setup and row data saved between power on/off
- alarm song after pre-defined no. of rows done
- user definable speed setting

*Machine operation and safety features:*
- high resolution stepper motor drive
- electronic endstops
- mechanic (spring driven) coupler for the carriage
- (optional) yarn and knot sensor

Building this device is not for the faint-hearted, because you will have to solder, crimp connectors, even deal with mains connections (! PLEASE BE CAREFUL ! I AM NOT RESPONSIBLE IF SOMETHING GOES WRONG HERE !), building and uploading the firmware to the Arduino, and in the end, debug the whole thing if something does not work as expected.

So, if you are not familiar with doing things like this, please ask a friend with the appropriate knowledge for help.