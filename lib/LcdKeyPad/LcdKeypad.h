/*!`
 * \file LcdKeypad.h
 *
 * This file is part of Open Knitting Motor Project OKMP
 * and based on the fantastic work of Kashif Baif for his
 * Arduino LCD Menu Library.
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
 *    Original Work Copyright Kashif Baig
 *    https://www.cohesivecomputing.co.uk/hackatronics/arduino-lcd-menu-library/
 */

#ifndef LCDKEYPAD_H_
#define LCDKEYPAD_H_

#define LCD_KEYPAD_1_0

#include "Arduino.h"
#include <stdarg.h>
#include <LiquidCrystal.h>

#define LCD_ROWS 2
#define LCD_COLS 16

#define BUTTON_PIN 0
#define BACKLIGHT_PIN 10

// button state indicators
#define BUTTON_PRESSED_IND (0 << 6)
#define BUTTON_SHORT_RELEASE_IND (1 << 6)
#define BUTTON_LONG_PRESSED_IND (2 << 6)
#define BUTTON_LONG_RELEASE_IND (3 << 6)

// Button values must be assigned in ascending order as the analog values.
// The analog values must be declared in ascending order in buttonValues[] in LcdKeypad.cpp.

#define BUTTON_RIGHT_PRESSED (1 | BUTTON_PRESSED_IND)
#define BUTTON_RIGHT_SHORT_RELEASE (1 | BUTTON_SHORT_RELEASE_IND)
#define BUTTON_RIGHT_LONG_PRESSED (1 | BUTTON_LONG_PRESSED_IND)
#define BUTTON_RIGHT_LONG_RELEASE (1 | BUTTON_LONG_RELEASE_IND)
#define BUTTON_RIGHT_ANALOG_VALUE 60 // 60

#define BUTTON_UP_PRESSED (2 | BUTTON_PRESSED_IND)
#define BUTTON_UP_SHORT_RELEASE (2 | BUTTON_SHORT_RELEASE_IND)
#define BUTTON_UP_LONG_PRESSED (2 | BUTTON_LONG_PRESSED_IND)
#define BUTTON_UP_LONG_RELEASE (2 | BUTTON_LONG_RELEASE_IND)
#define BUTTON_UP_ANALOG_VALUE 250 // 250

#define BUTTON_DOWN_PRESSED (3 | BUTTON_PRESSED_IND)
#define BUTTON_DOWN_SHORT_RELEASE (3 | BUTTON_SHORT_RELEASE_IND)
#define BUTTON_DOWN_LONG_PRESSED (3 | BUTTON_LONG_PRESSED_IND)
#define BUTTON_DOWN_LONG_RELEASE (3 | BUTTON_LONG_RELEASE_IND)
#define BUTTON_DOWN_ANALOG_VALUE 400 // 485

#define BUTTON_LEFT_PRESSED (4 | BUTTON_PRESSED_IND)
#define BUTTON_LEFT_SHORT_RELEASE (4 | BUTTON_SHORT_RELEASE_IND)
#define BUTTON_LEFT_LONG_PRESSED (4 | BUTTON_LONG_PRESSED_IND)
#define BUTTON_LEFT_LONG_RELEASE (4 | BUTTON_LONG_RELEASE_IND)
#define BUTTON_LEFT_ANALOG_VALUE 650 // 650

#define BUTTON_SELECT_PRESSED (5 | BUTTON_PRESSED_IND)
#define BUTTON_SELECT_SHORT_RELEASE (5 | BUTTON_SHORT_RELEASE_IND)
#define BUTTON_SELECT_LONG_PRESSED (5 | BUTTON_LONG_PRESSED_IND)
#define BUTTON_SELECT_LONG_RELEASE (5 | BUTTON_LONG_RELEASE_IND)
#define BUTTON_SELECT_ANALOG_VALUE 800 // 800

extern void queueButton(byte button);
extern byte getButton();
extern void findButtonValues(LiquidCrystal &LC, uint16_t *&newButtonValues);
extern void loadButtonValues(uint16_t newButtonValues[5]);

extern void buttonHandlerCycle(); // Can call in ISR, but will need to modify body of queueButton() method (see method body).

extern void backLightOn();
extern void backLightOff();
extern void setBacklightBrightness(byte brightness);

// Convert integer to string.
extern char *inttostr(char *dest, short integer);
// Apply left padding to string.
extern char *lpad(char *dest, const char *str, char chr = ' ', unsigned char width = LCD_COLS);
// Apply right padding to string.
extern char *rpad(char *dest, const char *str, char chr = ' ', unsigned char width = LCD_COLS);
// Apply string concatenation. argc = number of string arguments to follow.
extern char *fmt(char *dest, unsigned char argc, ...);

#endif
