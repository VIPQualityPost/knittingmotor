#ifndef HWCONFIG_H_
#define HWCONFIG_H_

#include <Arduino.h>

// LiquidCrystal display init pins
const byte d0Pin = 4;
const byte d1Pin = 5;
const byte d2Pin = 6;
const byte d3Pin = 7;
const byte rsPin = 8;
const byte enablePin = 9;

// Encoder pins and parameters
const byte rotaA = 2;                     // encoder pin A
const byte rotaB = 3;                     // encoder pin B
const int encRes = 2400;                  // encoder resolution

// Stepper pins and parameters
const byte stepPin = 11;                  // stepper PUL(SE)
const byte dirPin  = 12;                  // stepper DIR(ECTION)
const byte enaPin  = 13;                  // stepper ENA(BLE)
const byte enaLevel = HIGH;               // level to enable stepper
const byte enaDelay = 200;                // DM542T (v4.0) - enable time at least 200ms according manual
const int stepsPerRev = 1600;             // Steps per Revolution ( example with 1/4 microsteps )
const int rampLen = 200;                  // stepper acceleration ramp
const int homeSpeed = 600;                // constant homing speed, regardless of user speed setting
const int fastSpeedSteps = 20000;         // steps per 10sec fast
const int slowSpeedSteps = 1000;          // steps per 10sec slow
const int endstopOffset = 80;             // endstop clearance offset after homing

// Endstop pins and parameters
const byte homePin = A5;                   // home endstop
const byte maxPin = A4;                   // max endstop
const byte atRefpoint = LOW;              // endstop active on ... level

// Signal pins and parameters 
const byte footswitchPin = A3;            // footswitch
const byte yarnMainPin = A2;              // main yarn sensor pin (SET TO UNUSED PIN NO. IF NOT USED)
const byte yarnSecPin = A1;               // second yarn sensor pin (SET TO UNUSED PIN NO. IF NOT USED)
const byte overloadPin = 1;               // carrier overload sensor pin (SET TO UNUSED PIN NO. IF NOT USED)
const byte alarmPin = 0;                  // buzzer pin
const byte signalLevel = HIGH;            // switch level

const float encoderMultiplier = ((float)stepsPerRev / (float)encRes); // encoder to stepper steps transpose value

// const unsigned long swDebounceTime = 20;  // switch debounce time - NOT USED, see ISR (PCINT1_vect){...}

#endif
