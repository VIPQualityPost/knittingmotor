#ifndef HWCONFIG_H_
#define HWCONFIG_H_

#include <Arduino.h>

// Display pins
const byte rsPin = 8;
const byte enablePin = 9;
const byte d0Pin = 4;
const byte d1Pin = 5;
const byte d2Pin = 6;
const byte d3Pin = 7;

// Encoder pins
const byte rotaA = 2;     // encoder pin A
const byte rotaB = 3;     // encoder pin B
const int encRes = 2400;  // encoder resolution

// Stepper pins
const byte stepPin = 11;
const byte dirPin  = 12;
const byte enaPin  = 13;
const byte enaLevel = HIGH;     // level to enable stepper
const int stepsPerRev = 1600;   // Steps per Revolution ( example with 1/4 microsteps )

// Endstop pins
const byte refPin = A5;         // home endstop
const byte maxPin = A4;         // max endstop
const byte atRefpoint = LOW;    // endstop active on ... level

// Signal pins
const byte footswitchPin = A3;  // footswitch
const byte yarnMainPin = A2;   // main yarn sensor pin
const byte yarnSecPin = A1;   // second yarn sensor pin
const byte overloadPin = 1;   // carrier overload sensor pin
const byte alarmPin = 0;       // buzzer pin
const byte signalLevel = HIGH;  // switch level
// const byte hx711SCKPin = A2;    // HX711 serial clock input
// const byte hx711DTPin = A1;     // HX711 DataOut
const unsigned long swDebounceTime = 10;  // switch debounce time

#endif
