/*!`
 * \file Config.h
 *
 * This file is part of Open Knitting Motor Project OKMP.
 *
 *    OKMP is free software: you can redistribute it and/or modify
 *    it under the terms of the CC BY-NC-SA license
 *    as published by Creative Commons, either version 4.0 of the License, or
 *    (at your option) any later version.
 *
 *    OKMP is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    Creative Commons License for more details.
 *
 *    You should have received a copy of the Creative Commons License
 *    along with OKMP.
 *    If not, see <https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode.en/>.
 *
 *    Original Work Copyright 2024 Holger Pandel
 *    http://pages.open-mind.space/knittingmotor
 */

#ifndef CONFIG_H_
#define CONFIG_H_

#include <Arduino.h>

const char NameAndVersion[] = "Knitting Motor Configuration V1.9";

extern long addToVal(short delta, long timeval, long minval = 1, long maxval = 1);

struct cfgparams
{
  char appNameAndVersion[sizeof(NameAndVersion)];
  unsigned int rowCount;      // setup row counts
  long leftBoundary;          // knitting boundaries
  long rightBoundary;         // knitting boundaries
  byte alarmTune;             // sing no.
  byte buttonBeep;            // true/false
  byte displayBrightness;     // 1=33%, 2=66%, 3=100%
  byte yarnsensorMainEnable;  // true/false
  byte yarnsensorSecEnable;   // true/false
  byte overloadsensorEnable;  // true/false
  byte opMode;                // operation mode true=row count down (auto), false=row count up (manual)
  byte footMode;              // foot pedal mode: single / continuous
  byte navMode;               // navigation with left/right buttons: max or boundary
  byte arrowMode;             // show arrow after knitting true/false
  unsigned int carriageSpeed; // see myStepper.setSpeed( 800 );  // 80 Rev/Min ( if stepsPerRev is set correctly )
  uint16_t buttonValues[5];   // analog values for buttons, must be in ascending order
  bool buttonsInitialized;    // true/false
};

class Config
{
public:
  /// <configValues>

  /// If adding/removing/changing config items,
  /// then increment version in NameAndVersion string above.
  /// Must also modify body of setDefaults() and getFormattedStr() methods of this class.
  /// It may also be necessary to modify processMenuCommand() method in LcdCountDownTimer file.

  cfgparams cfg;

  /// </configValues>

  char *getFormattedStr(byte cmdId); // Returns formatted config value associated with menu command id.
  void save();                       // Saves config to EEPROM.
  void load();                       // Loads config from EEPROM.
  void setDefaults();                // Sets config to default values.
                                     // void copyTo(Config *dest);          // Copies current instance to destination instance.
};

#endif