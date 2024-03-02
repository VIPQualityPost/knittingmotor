#include <LiquidCrystal.h>
#include <MobaTools.h>
#include <Encoder.h>
#include <anyrtttl.h>
#include <Pitches.h>
#include <Bounce2.h>
#include "LcdKeypad.h"
#include "MenuData.h"
#include "Config.h"
#include "HardwareConfig.h"
#include "RTTTLTunes.h"
#include "Version.h"
#include "Lang.h"

// make real string from preprocessor text
// #define _STRINGIZE(x) #x
// #define STRINGIZE(x) _STRINGIZE(x)

// bit manipulation macros
#define clear_bit(a,z) (a &= ~_BV(z))
#define set_bit(a,z) (a |= _BV(z))

#if !defined(VERSION)
  #define VERSION 0.0.1-dbg
#endif // VERSION
#if !defined(DBG)
  #define DBG 0            // DEBUGGING motorcontrol
#endif // DBG

// define macros for debug output
#ifdef DEBUG
  #define DBG_PRINT(...)    Serial.print(__VA_ARGS__)     //DBGPRINT is a macro, debug print
  #define DBG_PRINTLN(...)  Serial.println(__VA_ARGS__)   //DBGPRINTLN is a macro, debug print with new line
  char dbgbuf[100];
#else
  #define DBG_PRINT(...)     //now defines a blank line
  #define DBG_PRINTLN(...)   //now defines a blank line
#endif

// further possible debug macros
// #define DEBUG_APPMODE
// #define DEBUG_POSITION

#define STEP_CW 1
#define STEP_CCW -1
#define STEP_STOP 0

// Rotary encoder params
long oldEncPosition  = 0;
long newEncPosition = 0;
short encDir = STEP_STOP;
Encoder myEnc(rotaA, rotaB);

// Stepper pins params
long stepperMaxPos = 0; // max endstop
long stepperMidPos = 0; // mid position after homing
byte stepperAttached = 0;
const int rampLen = 200;
const int fastSpeedSteps = 20000;
const int slowSpeedSteps = 1000;
int operationRPM = 0;
MoToStepper myStepper( stepsPerRev, STEPDIR );

// encoder to stepper steps transpose value
const float encoderMultiplier = ((float)encRes / (float)stepsPerRev);
float posFromEnc = 0;  // calculated encoder position from stepper steps

// error and warnings
enum ErrorStates : byte {
  OK, 
  HIT_HOME, 
  HIT_MAX, 
  HIT_YARN1,
  HIT_YARN2,
  MISS_OVRL,
  MISS_FOOT
};

volatile byte errorState = OK;  // endstops/sensors hit? (0=ok,1=max endstop,2=home endstop,3=yarnsensor)

byte updLcd = 1; // update LCD after encoder value changed

// ISR relevant variables
volatile byte knitRow = 0; // has to be defined here because of PCI ISR
volatile byte knitContinuous = 0;
volatile byte fpHit = 0;
volatile short errPreservedDir = STEP_STOP;   // preserve old direction in case of an error


#ifdef DEBUG_POSITION
void debugReportSteps();
#endif

// temporary button request, later changed with menu
LiquidCrystal lcd(rsPin, enablePin, d0Pin, d1Pin, d2Pin, d3Pin);
const char EmptyStr[] = "";
char strbuf[LCD_COLS + 1]; // one line of lcd display

// app states
// APP_PGMSTART - program just started, not homed and uninitialized values on LCD screen
// APP_DISP_UPD - repeated homing done, but display needs update
// APP_PRE_CHECK - basic checks, if all necessary sensors are attached
enum AppModeValues : byte 
{ 
    APP_PRE_CHECK,          // 0
    APP_PGMSTART,           // 1
    APP_DISP_UPD,           // 2
    APP_NORMAL_MODE,        // 3
    APP_CARRIAGE_RUNNING,   // 4
    APP_ALARM,              // 5
    APP_MENU_MODE,          // 6
    APP_PROCESS_MENU_CMD    // 7
};

AppModeValues currentAppMode = APP_PRE_CHECK;
AppModeValues nextAppMode = APP_PRE_CHECK;

MenuManager Menu1(knittingMenu_Root, menuCount(knittingMenu_Root));

unsigned int currentRowCount = 0;
unsigned int oldRowCount = 0;
unsigned int rowsKnit = 0;
// unsigned long startMillis;
unsigned long menuStartTime;


Config currentConfig;

// different main screens to show during APP_NORMAL_MODE
enum Screens : byte
{
  STEPS,
  ROWS,
  ROWS_WITH_HEADER
};

Screens screenToShow = ROWS_WITH_HEADER;

byte arrowShown = 0;    // arrow on display?
byte startupDisplayed = 0;  // is showing startup display
byte errorDisplayed = 0;  // controls blinking of error message
byte errorSymbolDisplayed = 0; // show continue screen after error

Bounce2::Button footswitchBtn = Bounce2::Button();
byte btn;  // menu button

// timed actions always like this (!)
const unsigned long REFRESH_INTERVAL = 600; // display refresh when blinking in ms
unsigned long lastRefreshTime = 0;
/*
    ....
    if(millis() - lastRefreshTime >= REFRESH_INTERVAL){
      lastRefreshTime += REFRESH_INTERVAL;
      // DO SOMETHING HERE
    }
    ....
*/

void printRowCount(unsigned int rowCount, bool withTopic);
void refreshMenuDisplay (byte refreshMode);
byte getNavAction();
void enablePort_PCI();
void disablePort_PCI();
void enablePort_Pins();
void disablePort_Pins();
void startBacklightPWM();
void stepperAttach();
void stepperDetach();
void toRefPoint();
void toMaxPoint();
void readEncoder();
byte processMenuCommand(byte cmdId);
void homing();
void lcdClear();
void printVersion();
void show2secMessage(const __FlashStringHelper *line1, const __FlashStringHelper *line2, const AppModeValues appM);
void show2secMessage(const char *line1, const char *line2, const AppModeValues appM);


// ------------------------------------------------------------------------

/*
------------------------------------------------------------------------
Regarding PIN CHANGE INTERRUPT
READ THIS!!!!: https://dronebotworkshop.com/interrupts/

  - enable pin change interrupts for Port C (A0-A5) for endstops and interrupt pins
   Enable PCIE0 Bit0 = 1 (Port B - D8-D13)
   Enable PCIE1 Bit1 = 1 (Port C - A0-A5)
   Enable PCIE2 Bit2 = 1 (Port D - D0-D7)

  - use defined macro: set_bit(PCICR, PCIE1); // aka  PCICR |= B00000010;
 
  - Pin change mask PCMSK: select pins to use
   PCMSK0 (Port B), PCMSK1 (Port C), PCMSK2 (Port D)
   Select PCINT13 Bit5 = 1 (Pin A5)
   Select PCINT12 Bit4 = 1 (Pin A4)

  - use defined macro: set_bit(PCMSK1, PCINT13); // aka PCMSK1 |= B00100000;

  - PCI ISR pre-defined function names
   ISR (PCINT0_vect) - Port B
   ISR (PCINT1_vect) - Port C
   ISR (PCINT2_vect) - Port D

  Beware:
   if you want to explicitly access a pin via digitalRead that is under
   surveillance of PCI, temporarily disable PCI for that port
------------------------------------------------------------------------
*/

// ------------------------------------------------------------------------
// Enable PCI for Port C
void enablePort_PCI() 
{
  set_bit(PCICR, PCIE1);
  if (currentConfig.overloadsensorEnable == true) 
  {
    set_bit(PCICR, PCIE2);
  }
}

// ------------------------------------------------------------------------
// Disable PCI for Port C
void disablePort_PCI() 
{
  clear_bit(PCICR, PCIE1);
  if (currentConfig.overloadsensorEnable == true) 
  {
    clear_bit(PCICR, PCIE2);
  }
}

// ------------------------------------------------------------------------
// Enable Pins A4 and A5 for PCI on Port C
void enablePort_Pins() 
{
  if (currentConfig.overloadsensorEnable == true) 
  {
    set_bit(PCMSK2, PCINT17); // D1
  }
  set_bit(PCMSK1, PCINT13); // A5
  set_bit(PCMSK1, PCINT12); // A4
  // set_bit(PCMSK1, PCINT11); // A3
  if (currentConfig.yarnsensorMainEnable == true) 
  {
    set_bit(PCMSK1, PCINT10); // A2
  }
  if (currentConfig.yarnsensorSecEnable == true) 
  {
    set_bit(PCMSK1, PCINT9); // A1
  }
}

// ------------------------------------------------------------------------
// Disable Pins A4 and A5 for PCI on Port C
void disablePort_Pins() 
{
  if (currentConfig.overloadsensorEnable == true) 
  {
    clear_bit(PCMSK2, PCINT17); // D1
  }
  clear_bit(PCMSK1, PCINT13); // A5
  clear_bit(PCMSK1, PCINT12); // A4
  // clear_bit(PCMSK1, PCINT11); // A3
  if (currentConfig.yarnsensorMainEnable == true) 
  {
    clear_bit(PCMSK1, PCINT10); // A2
  }
  if (currentConfig.yarnsensorSecEnable == true) 
  {
    clear_bit(PCMSK1, PCINT9);  // A1
  }
}

// ------------------------------------------------------------------------
// Use soft PWM for backlight, as hardware PWM must be avoided for some LCD shields.
// piggy back on to timer0, which is already set to approx 1khz.
void startBacklightPWM() 
{
  OCR0A = 0xAF;
  TIMSK0 |= _BV(OCIE0A);
}

// ------------------------------------------------------------------------
void stepperAttach() 
{
  myStepper.attach( stepPin, dirPin );
  //myStepper.attachEnable( enaPin, 50, HIGH );        // Enable Pin aktivieren ( HIGH=aktiv )
  myStepper.setSpeed( 600 );
  myStepper.setRampLen( 200 );                       // Rampenlänge 100 Steps bei 20U/min
  digitalWrite(enaPin, enaLevel);
  delay(50);
  stepperAttached = 1;
}

// ------------------------------------------------------------------------
void stepperDetach() 
{
  myStepper.detach();
  digitalWrite(enaPin, LOW);
  stepperAttached = 0;
}

// ------------------------------------------------------------------------
// Move stepper to home position, backup and return
void toRefPoint() 
{
  disablePort_PCI();   // disable pin change interrupts for exclusive pin access
  // Run stepper to home endpoint and zero position
  DBG_PRINT(F("Homing"));

  // move fast to endpoint...
  if ( digitalRead( refPin ) != atRefpoint ) 
  {
    // ... but only if stepper is not already there
    myStepper.setSpeedSteps( fastSpeedSteps, rampLen );
    myStepper.rotate(STEP_CCW);
    while ( digitalRead( refPin ) != atRefpoint );
  }

  // home endstop reached, stop
  myStepper.stop();
  // myStepper.rotate(STEP_STOP);
  while ( myStepper.moving() );     // wait for deceleration ramp;
  
  // slow backup until endstop releases
  myStepper.setSpeedSteps( slowSpeedSteps );
  myStepper.setRampLen(0);
  myStepper.rotate(STEP_CW);
  while ( digitalRead( refPin ) == atRefpoint );
  
  DBG_PRINTLN(F("Home endpoint reached"));
  myStepper.rotate(0);
  while (myStepper.moving() );
  myStepper.setZero();        // zero stepper pos
  myStepper.setSpeed( operationRPM );
  myStepper.setRampLen((uint16_t)rampLen);        // acceleration ramp 100 steps at 20rpm
  DBG_PRINTLN(F("Homeing end"));
  enablePort_PCI();
}

// ------------------------------------------------------------------------
// Move stepper to max end of pathway
void toMaxPoint() 
{
  disablePort_PCI();   // disable pin change interrupts for exclusive pin access
  // Run stepper to home endpoint and zero position
  DBG_PRINTLN(F("Going to max endstop"));
  // move fast to endpoint...
  if ( digitalRead( maxPin ) != atRefpoint ) 
  {
    // ... but only if stepper is not already there
    myStepper.setSpeedSteps( fastSpeedSteps, rampLen );
    myStepper.rotate(STEP_CW);
    while ( digitalRead( maxPin ) != atRefpoint );
  }

  // home endstop reached, stop
  myStepper.stop();
  // myStepper.rotate(STEP_STOP);
  while ( myStepper.moving() );     // wait for deceleration ramp;
  
  // slow backup until endstop releases
  myStepper.setSpeedSteps( slowSpeedSteps );
  myStepper.setRampLen(0);
  myStepper.rotate(STEP_CCW);
  while ( digitalRead( maxPin ) == atRefpoint );
  
  DBG_PRINT(F("Max endpoint reached at: "));
  DBG_PRINTLN(myStepper.currentPosition());
  myStepper.rotate(0);
  while (myStepper.moving() );
  myStepper.setSpeed( operationRPM );
  myStepper.setRampLen((uint16_t)rampLen);
  stepperMaxPos = myStepper.currentPosition();
  DBG_PRINTLN(F("Maxing finished"));
  enablePort_PCI();
}

// ------------------------------------------------------------------------
// Read values from rotary encoder
void readEncoder() 
{   
  newEncPosition = myEnc.read();

  if (newEncPosition != oldEncPosition) 
  {
    if (oldEncPosition < newEncPosition) 
    {
      encDir = STEP_CCW;
    } 
    else 
    {
      encDir = STEP_CW;
    }
    oldEncPosition = newEncPosition;
    updLcd = 1;
  }
}

#ifdef DEBUG_POSITION
// ------------------------------------------------------------------------
// report steps debug
void debugReportSteps() 
{
  uint32_t interval = 250;
  static uint32_t last = - interval;

  if(millis() - last < interval) return;
  last += interval;
  dbgbuf[0] = 0;
  snprintf(debugbuf,sizeof(debugbuf),"stepper: %ld, newEncPosition: %ld, posFromEncoder: %ld, posDiff: %ld", myStepper.currentPosition(), newEncPosition, (long)posFromEnc, (newEncPosition-(long)posFromEnc));
  DBG_PRINTLN(debugbuf);
}
#endif

//----------------------------------------------------------------------
// Callback to convert button press to navigation action.
byte getNavAction()
{
  byte navAction = 0;
  byte currentItemHasChildren = Menu1.currentItemHasChildren();
  
  if (btn == BUTTON_UP_PRESSED || btn == BUTTON_UP_LONG_PRESSED) navAction = MENU_ITEM_PREV;
  else if (btn == BUTTON_DOWN_PRESSED || btn == BUTTON_DOWN_LONG_PRESSED) navAction = MENU_ITEM_NEXT;
  else if (btn == BUTTON_SELECT_PRESSED || (btn == BUTTON_RIGHT_PRESSED && currentItemHasChildren)) navAction = MENU_ITEM_SELECT;
  //else if (btn == BUTTON_LEFT_PRESSED) navAction = MENU_BACK;
  return navAction;
}

// Callback to refresh display during menu navigation, using parameter of type enum DisplayRefreshMode.
void refreshMenuDisplay (byte refreshMode)
{
  char nameBuf[LCD_COLS+1];

/*
  if (refreshMode == REFRESH_DESCEND || refreshMode == REFRESH_ASCEND)
  {
    byte menuCount = Menu1.getMenuItemCount();
    
    // uncomment below code to output menus to serial monitor
    if (Menu1.currentMenuHasParent())
    {
      Serial.print("Parent menu: ");
      Serial.println(Menu1.getParentItemName(nameBuf));
    }
    else
    {
      Serial.println("Main menu:");
    }
    
    for (int i=0; i<menuCount; i++)
    {
      Serial.print(Menu1.getItemName(nameBuf, i));

      if (Menu1.itemHasChildren(i))
      {
        Serial.println("->");
      }
      else
      {
        Serial.println();
      }
    }
  }
*/

  lcd.setCursor(0, 0);
  if (Menu1.currentItemHasChildren())
  {
    rpad(strbuf, Menu1.getCurrentItemName(nameBuf));
    strbuf[LCD_COLS-1] = 0b01111110;            // Display forward arrow if this menu item has children.
    lcd.print(strbuf);
    lcd.setCursor(0, 1);
    lcd.print(rpad(strbuf, EmptyStr));          // Clear config value in display
  }
  else
  {
    byte cmdId;
    rpad(strbuf, Menu1.getCurrentItemName(nameBuf));
    
    if ((cmdId = Menu1.getCurrentItemCmdId()) == 0)
    {
      strbuf[LCD_COLS-1] = 0b01111111;          // Display back arrow if this menu item ascends to parent.
      lcd.print(strbuf);
      lcd.setCursor(0, 1);
      lcd.print(rpad(strbuf, EmptyStr));        // Clear config value in display.
    }
    else
    {
      lcd.print(strbuf);
      lcd.setCursor(0, 1);
      lcd.print(" ");
      lcd.print(rpad(strbuf, currentConfig.getFormattedStr(cmdId))); // Display config value.
    }
  }
}

//----------------------------------------------------------------------
void printRowCount(unsigned int rowCount, bool withTopic) 
{
  char intbuf[3];
  // char tmpbuf[17];
  byte bdPad = 0; 

//  if (updLcd == 1 && currentAppMode == APP_DISP_UPD) 
  if (updLcd == 1) 
  {

    if (withTopic == true)
    {
      lcd.clear();
      lcd.setCursor(0,0);
      if (currentConfig.opMode) {
        lcd.print(F(MAIN_rtk));
      } 
      else
      {
        lcd.print(F(MAIN_rk));
      }
    }

    lcd.setCursor(0,1);

    inttostr(intbuf, rowCount);
    char tmpbuf[17];

    if (nextAppMode == APP_ALARM) {
      strncpy_P(tmpbuf, (const char*)F(MAIN_done), 16);
      rpad(strbuf, tmpbuf);
    } 
    else 
    {
      if (rowCount == 1)
      {
        strncpy_P(tmpbuf, (const char*)F(MAIN_row), 16);
        fmt(strbuf, 2, intbuf, tmpbuf);
      } 
      else 
      {
        strncpy_P(tmpbuf, (const char*)F(MAIN_rows), 16);
        fmt(strbuf, 2, intbuf, tmpbuf);
      }
    }

    // add small arrows to indicate direction
    // if (myStepper.moving()) 
    // {

      rpad (strbuf, strbuf); // if strbuf is not padded, one cannot put an arrow at the end

      // show [C->] if boundaries are defined
      if (currentConfig.leftBoundary != 0 || currentConfig.rightBoundary != 0)
      {
        bdPad = 1;
        if (knitContinuous == 1) 
        {
          strbuf[LCD_COLS-4] = 0b01011011;
        } 
        else 
        {
          strbuf[LCD_COLS-3] = 0b01011011;
        }
        strbuf[LCD_COLS-1] = 0b01011101;
      }

      if (knitContinuous == 1) 
      {
        strbuf[LCD_COLS-2-bdPad] = 0b01000011; // for continuous
      }

      if (errorSymbolDisplayed == 1)
      {
        errorSymbolDisplayed = 0;
        strbuf[LCD_COLS-1-bdPad] = 0b01000101; // show E for continue after error
      }
      else
      {
        if (encDir == STEP_CW) 
        {
          strbuf[LCD_COLS-1-bdPad] = 0b01111110; // forward array
        } 
        else if (encDir == STEP_CCW) 
        {
          strbuf[LCD_COLS-1-bdPad] = 0b01111111;    // back arrow
        }
      }

      arrowShown = 1;
    // }

    lcd.print(strbuf);
  }
}

//----------------------------------------------------------------------
// Addition or removal of menu items in MenuData.h will require this method
// to be modified accordingly. 
byte processMenuCommand(byte cmdId)
{
  byte complete = false;  // set to true when menu command processing complete.
  byte configChanged = false;
  int oldTune;
  char sngbuf[420];


  if (btn == BUTTON_SELECT_PRESSED)
  {
    if (cmdId != mnuCmdResetToDefaults && 
        cmdId != mnuCmdInfo && \
        cmdId != mnuCmdHomeing && \
        cmdId != mnuCmdMoveToStart && \
        cmdId != mnuCmdMoveToMid && \
        cmdId != mnuCmdMoveToEnd && \
        cmdId != mnuCmdClearBnd
        ) complete = true;
  }

  oldTune = currentConfig.alarmTune;

  switch (cmdId)
  {
    case mnuCmdLeftBnd:
      configChanged = true;
      if (btn == BUTTON_LEFT_PRESSED || btn == BUTTON_LEFT_LONG_PRESSED)
      {
        if (btn == BUTTON_LEFT_PRESSED)
        {
          currentConfig.leftBoundary = addToVal(120, currentConfig.leftBoundary,stepperMidPos+2,stepperMaxPos);
          myStepper.moveTo(currentConfig.leftBoundary);
        }
        if (btn == BUTTON_LEFT_LONG_PRESSED)
        {
          currentConfig.leftBoundary = addToVal(600, currentConfig.leftBoundary,stepperMidPos+2,stepperMaxPos);
          myStepper.moveTo(currentConfig.leftBoundary);
        }
      }
      else if (btn == BUTTON_RIGHT_PRESSED || btn == BUTTON_RIGHT_LONG_PRESSED)
      {
        if (btn == BUTTON_RIGHT_PRESSED)
        {
          currentConfig.leftBoundary = addToVal(-120, currentConfig.leftBoundary,stepperMidPos+2,stepperMaxPos);
          myStepper.moveTo(currentConfig.leftBoundary);
        }
        if (btn == BUTTON_RIGHT_LONG_PRESSED)
        {
          currentConfig.leftBoundary = addToVal(-600, currentConfig.leftBoundary,stepperMidPos+2,stepperMaxPos);
          myStepper.moveTo(currentConfig.leftBoundary);
        }
      }
      else
      {
        configChanged = false;
      }

      myStepper.setSpeed(1000);
      if (currentConfig.leftBoundary == 0)
      {
        myStepper.moveTo(stepperMidPos+1);
        currentConfig.leftBoundary = stepperMidPos+1;
        configChanged = true;
        DBG_PRINTLN(F("Moving to stepperMidPos+1"));
      } else {
        myStepper.moveTo(currentConfig.leftBoundary);
        DBG_PRINTLN(F("Moving to leftBoundary"));
      }
      while ( myStepper.moving() );

      break;
    case mnuCmdRightBnd:
      configChanged = true;
      if (btn == BUTTON_LEFT_PRESSED || btn == BUTTON_LEFT_LONG_PRESSED)
      {
        if (btn == BUTTON_LEFT_PRESSED)
        {
          currentConfig.rightBoundary = addToVal(120, currentConfig.rightBoundary,0,stepperMidPos-2);
          myStepper.moveTo(currentConfig.rightBoundary);
        }
        if (btn == BUTTON_LEFT_LONG_PRESSED)
        {
          currentConfig.rightBoundary = addToVal(600, currentConfig.rightBoundary,0,stepperMidPos-2);
          myStepper.moveTo(currentConfig.rightBoundary);
        }
      }
      else if (btn == BUTTON_RIGHT_PRESSED || btn == BUTTON_RIGHT_LONG_PRESSED)
      {
        if (btn == BUTTON_RIGHT_PRESSED)
        {
          currentConfig.rightBoundary = addToVal(-120, currentConfig.rightBoundary,0,stepperMidPos-2);
          myStepper.moveTo(currentConfig.rightBoundary);
        }
        if (btn == BUTTON_RIGHT_LONG_PRESSED)
        {
          currentConfig.rightBoundary = addToVal(-600, currentConfig.rightBoundary,0,stepperMidPos-2);
          myStepper.moveTo(currentConfig.rightBoundary);
        }
      }
      else
      {
        configChanged = false;
      }

      myStepper.setSpeed(1000);
      if (currentConfig.rightBoundary == 0)
      {
        myStepper.moveTo(stepperMidPos-1);
        currentConfig.rightBoundary = stepperMidPos-1;
        configChanged = true;
        DBG_PRINTLN(F("Moving to stepperMidPos-1"));
      } 
      else 
      {
        myStepper.moveTo(currentConfig.rightBoundary);
        DBG_PRINTLN(F("Moving to rightBoundary"));
      }
      while ( myStepper.moving() );

      break;
    case mnuCmdClearBnd:
      if (btn == BUTTON_SELECT_LONG_PRESSED)
      {
        lcdClear();
        currentConfig.leftBoundary = 0;
        currentConfig.rightBoundary = 0;
        lcd.setCursor(0, 0);
        lcd.print(F(MAIN_boundaries));
        lcd.setCursor(0, 1);
        lcd.print(F(MAIN_cleared));
      } else if (/*btn == BUTTON_SELECT_SHORT_RELEASE ||*/ btn == BUTTON_SELECT_LONG_RELEASE)
      {
        refreshMenuDisplay(REFRESH_ASCEND);
        complete = true;
      }
      break;
    case mnuCmdHomeing:
      if (btn == BUTTON_SELECT_LONG_PRESSED)
      {
        homing();
      } else if (/*btn == BUTTON_SELECT_SHORT_RELEASE ||*/ btn == BUTTON_SELECT_LONG_RELEASE)
      {
        refreshMenuDisplay(REFRESH_ASCEND);
        complete = true;
      }
      break;
    case mnuCmdMoveToStart:
      if (btn == BUTTON_SELECT_LONG_PRESSED)
      {
        myStepper.moveTo(0);
      } else if (/*btn == BUTTON_SELECT_SHORT_RELEASE ||*/ btn == BUTTON_SELECT_LONG_RELEASE)
      {
        refreshMenuDisplay(REFRESH_ASCEND);
        complete = true;
      }
      break;
    case mnuCmdMoveToMid:
      if (btn == BUTTON_SELECT_LONG_PRESSED)
      {
        myStepper.moveTo(stepperMidPos);
      } else if (/*btn == BUTTON_SELECT_SHORT_RELEASE ||*/ btn == BUTTON_SELECT_LONG_RELEASE)
      {
        refreshMenuDisplay(REFRESH_ASCEND);
        complete = true;
      }
      break;
    case mnuCmdMoveToEnd:
      if (btn == BUTTON_SELECT_LONG_PRESSED)
      {
        myStepper.moveTo(stepperMaxPos);
      } else if (/*btn == BUTTON_SELECT_SHORT_RELEASE ||*/ btn == BUTTON_SELECT_LONG_RELEASE)
      {
        refreshMenuDisplay(REFRESH_ASCEND);
        complete = true;
      }
      break;
    case mnuCmdSetRowCount :
      lcd.blink();
      configChanged = true;
      if (btn == BUTTON_UP_PRESSED || btn == BUTTON_UP_LONG_PRESSED)
      {
        if (btn == BUTTON_UP_PRESSED)
        {
          currentConfig.rowCount = addToVal(1, currentConfig.rowCount,1,999);
        }
        if (btn == BUTTON_UP_LONG_PRESSED)
        {
          currentConfig.rowCount = addToVal(10, currentConfig.rowCount,1,999);
        }
      }
      else if (btn == BUTTON_DOWN_PRESSED || btn == BUTTON_DOWN_LONG_PRESSED)
      {
        if (btn == BUTTON_DOWN_PRESSED)
        {
          currentConfig.rowCount = addToVal(-1, currentConfig.rowCount,1,999);
        }
        if (btn == BUTTON_DOWN_LONG_PRESSED)
        {
          currentConfig.rowCount = addToVal(-10, currentConfig.rowCount,1,999);
        }
      }
      else
      {
        configChanged = false;
      }
      lcd.noBlink();
      break;
    case mnuCmdCarSpeed :
      lcd.blink();
      configChanged = true;
      if (btn == BUTTON_UP_PRESSED || btn == BUTTON_UP_LONG_PRESSED)
      {
        if (btn == BUTTON_UP_PRESSED)
        {
          currentConfig.carriageSpeed = addToVal(1, currentConfig.carriageSpeed,60,200);
        }
        if (btn == BUTTON_UP_LONG_PRESSED)
        {
          currentConfig.carriageSpeed = addToVal(10, currentConfig.carriageSpeed,60,200);
        }
      }
      else if (btn == BUTTON_DOWN_PRESSED || btn == BUTTON_DOWN_LONG_PRESSED)
      {
        if (btn == BUTTON_DOWN_PRESSED)
        {
          currentConfig.carriageSpeed = addToVal(-1, currentConfig.carriageSpeed,60,200);
        }
        if (btn == BUTTON_DOWN_LONG_PRESSED)
        {
          currentConfig.carriageSpeed = addToVal(-10, currentConfig.carriageSpeed,60,200);
        }
      }
      else
      {
        configChanged = false;
      }
      operationRPM = currentConfig.carriageSpeed * 10;
      myStepper.setSpeed(operationRPM);
      lcd.noBlink();
      break;
    case mnuCmdOpMode:
      configChanged = true;
      if (btn == BUTTON_UP_PRESSED || btn == BUTTON_UP_LONG_PRESSED)
      {
        currentConfig.opMode = true;
        currentConfig.rowCount = 0;
        currentRowCount = 0;
        oldRowCount = currentRowCount;
      }
      else if (btn == BUTTON_DOWN_PRESSED || btn == BUTTON_DOWN_LONG_PRESSED)
      {
        currentConfig.opMode = false;
        currentConfig.rowCount = 0;
        currentRowCount = 0;
        oldRowCount = currentRowCount;
      }
      else
      {
        configChanged = false;
      }
      break;
    case mnuCmdFootMode:
      configChanged = true;
      if (btn == BUTTON_UP_PRESSED || btn == BUTTON_UP_LONG_PRESSED)
      {
        currentConfig.footMode = true;
      }
      else if (btn == BUTTON_DOWN_PRESSED || btn == BUTTON_DOWN_LONG_PRESSED)
      {
        currentConfig.footMode = false;
      }
      else
      {
        configChanged = false;
      }
      break;
    case mnuCmdArrowMode:
      configChanged = true;
      if (btn == BUTTON_UP_PRESSED || btn == BUTTON_UP_LONG_PRESSED)
      {
        currentConfig.arrowMode = true;
      }
      else if (btn == BUTTON_DOWN_PRESSED || btn == BUTTON_DOWN_LONG_PRESSED)
      {
        currentConfig.arrowMode = false;
      }
      else
      {
        configChanged = false;
      }
      break;
    case mnuCmdButtonBeep:
      configChanged = true;
      if (btn == BUTTON_UP_PRESSED || btn == BUTTON_UP_LONG_PRESSED)
      {
        currentConfig.buttonBeep = true;
      }
      else if (btn == BUTTON_DOWN_PRESSED || btn == BUTTON_DOWN_LONG_PRESSED)
      {
        currentConfig.buttonBeep = false;
      }
      else
      {
        configChanged = false;
      }
      break;
    case mnuCmdYarnMain:
      configChanged = true;
      if (btn == BUTTON_UP_PRESSED || btn == BUTTON_UP_LONG_PRESSED)
      {
        currentConfig.yarnsensorMainEnable = true;
      }
      else if (btn == BUTTON_DOWN_PRESSED || btn == BUTTON_DOWN_LONG_PRESSED)
      {
        currentConfig.yarnsensorMainEnable = false;
      }
      else
      {
        configChanged = false;
      }
      if (configChanged == true) {
        cli();
        disablePort_Pins();
        disablePort_PCI();
        enablePort_PCI();
        enablePort_Pins();
        sei();
      }
      break;
    case mnuCmdYarnSec:
      configChanged = true;
      if (btn == BUTTON_UP_PRESSED || btn == BUTTON_UP_LONG_PRESSED)
      {
        currentConfig.yarnsensorSecEnable = true;
      }
      else if (btn == BUTTON_DOWN_PRESSED || btn == BUTTON_DOWN_LONG_PRESSED)
      {
        currentConfig.yarnsensorSecEnable = false;
      }
      else
      {
        configChanged = false;
      }
      if (configChanged == true) {
        cli();
        disablePort_Pins();
        disablePort_PCI();
        enablePort_PCI();
        enablePort_Pins();
        sei();
      }
      break;
    case mnuCmdOverloadsensor:
      configChanged = true;
      if (btn == BUTTON_UP_PRESSED || btn == BUTTON_UP_LONG_PRESSED)
      {
        currentConfig.overloadsensorEnable = true;
      }
      else if (btn == BUTTON_DOWN_PRESSED || btn == BUTTON_DOWN_LONG_PRESSED)
      {
        currentConfig.overloadsensorEnable = false;
      }
      else
      {
        configChanged = false;
      }
      if (configChanged == true) {
        cli();
        disablePort_Pins();
        disablePort_PCI();
        enablePort_PCI();
        enablePort_Pins();
        sei();
      }
      break;
    case mnuCmdDisplayBrightness :
      configChanged = true;
      if (btn == BUTTON_UP_PRESSED || btn == BUTTON_UP_LONG_PRESSED)
      {
        currentConfig.displayBrightness++;
        currentConfig.displayBrightness = constrain(currentConfig.displayBrightness, 1, 3);
        setBacklightBrightness(currentConfig.displayBrightness);
      }
      else if (btn == BUTTON_DOWN_PRESSED || btn == BUTTON_DOWN_LONG_PRESSED)
      {
        currentConfig.displayBrightness--;
        currentConfig.displayBrightness = constrain(currentConfig.displayBrightness, 1, 3);
        setBacklightBrightness(currentConfig.displayBrightness);
      }
      else
      {
        configChanged = false;
      }
      break;
    case mnuCmdAlarmTune :
      configChanged = true;
      if (btn == BUTTON_UP_PRESSED || btn == BUTTON_UP_LONG_PRESSED)
      {
        currentConfig.alarmTune++;
        currentConfig.alarmTune = constrain(currentConfig.alarmTune, 0, songCount-1);
      }
      else if (btn == BUTTON_DOWN_PRESSED || btn == BUTTON_DOWN_LONG_PRESSED)
      {
        currentConfig.alarmTune--;
        currentConfig.alarmTune = constrain(currentConfig.alarmTune, 0, songCount-1);
      }
      else
      {
        configChanged = false;
      }

      // stop any current playing tune
      if (anyrtttl::nonblocking::isPlaying())
      {
        if (oldTune != currentConfig.alarmTune)
        {
          anyrtttl::nonblocking::stop();
        }
      }
      // start playing current selected tune
      if ( !anyrtttl::nonblocking::isPlaying() )
      {
        strcpy_P(sngbuf, (char*)pgm_read_dword(&(Melodies[currentConfig.alarmTune])));
        anyrtttl::nonblocking::begin(alarmPin, sngbuf);
      }
      else {
        anyrtttl::nonblocking::play();
      }
      break;
    case mnuCmdInfo:
      if (btn == BUTTON_SELECT_LONG_PRESSED)
      {
        lcd.clear();
        lcd.print(F("Knitting Motor"));
        lcd.setCursor(0,1);
        printVersion();
      }
      else if (/*btn == BUTTON_SELECT_SHORT_RELEASE ||*/ btn == BUTTON_SELECT_LONG_RELEASE)
      {
        refreshMenuDisplay(REFRESH_ASCEND);
        complete = true;
      }
      break;
    case mnuCmdResetToDefaults:
      if (btn == BUTTON_SELECT_LONG_PRESSED)
      {
        currentConfig.setDefaults();
        setBacklightBrightness(currentConfig.displayBrightness);
        lcd.setCursor(1, 1);
        lcd.print(F(MAIN_defload));
      }
      else if (/*btn == BUTTON_SELECT_SHORT_RELEASE ||*/ btn == BUTTON_SELECT_LONG_RELEASE)
      {
        refreshMenuDisplay(REFRESH_ASCEND);
        complete = true;
      }
      break;    // TODO Process menu commands here:
      default:
    break;
  }

 if (configChanged && (cmdId != mnuCmdResetToDefaults && cmdId != mnuCmdClearBnd)) {
    lcd.setCursor(1, 1);
    lcd.print(rpad(strbuf, currentConfig.getFormattedStr(cmdId))); // Display config value.
  }
  if (complete) {
    currentConfig.save();
    currentRowCount = currentConfig.rowCount;
  }
  return complete;
}

// ------------------------------------------------------------------------
// Remove anything left on LCD screen
void lcdClear() 
{
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(rpad(strbuf, EmptyStr));
  lcd.setCursor(0,1);
  lcd.print(rpad(strbuf, EmptyStr));
  lcd.setCursor(0,0);
}

// ------------------------------------------------------------------------
// Run complete homing and calibration run
void homing() 
{
  char tmpbuf[17];

  if (stepperAttached == 0) { stepperAttach(); }

  lcdClear();
  strncpy_P(tmpbuf, (const char*)F(MAIN_home1),16);
  lcd.print(rpad(strbuf, tmpbuf));
  lcd.setCursor(0,1);
  lcd.print(F(MAIN_home2));
  toRefPoint();
  myEnc.write(0);  // zero encoder pos, too

  DBG_PRINT(F("Position after homeing: "));
  DBG_PRINTLN(myStepper.currentPosition());

  lcdClear();
  strncpy_P(tmpbuf, (const char*)F(MAIN_home3),16);
  lcd.print(rpad(strbuf, tmpbuf));
  lcd.setCursor(0,1);
  lcd.print(F(MAIN_home4));
  toMaxPoint();
  
  DBG_PRINT(F("Max position: "));
  DBG_PRINTLN(stepperMaxPos);
  
  stepperMidPos = (long)(stepperMaxPos/2);

  DBG_PRINT(F("Mid position: "));
  DBG_PRINTLN(stepperMidPos);

  // if somehow boundaries are not correct in relation
  // to mid point, zero them out
  if (currentConfig.leftBoundary < stepperMidPos && currentConfig.leftBoundary != 0) {
    currentConfig.leftBoundary = 0;
    currentConfig.rightBoundary = 0;
    currentConfig.save();
    DBG_PRINTLN(F("Zeroing boundaries, left out of bounds"));
  }

  if (currentConfig.rightBoundary > stepperMidPos && currentConfig.rightBoundary != 0) {
    currentConfig.leftBoundary = 0;
    currentConfig.rightBoundary = 0;
    currentConfig.save();
    DBG_PRINTLN(F("Zeroing boundaries, right out of bounds"));
  }

  DBG_PRINT(F("Left boundary: "));
  DBG_PRINTLN(currentConfig.leftBoundary);
  DBG_PRINT(F("Right boundary: "));
  DBG_PRINTLN(currentConfig.rightBoundary);

  lcdClear();
  strncpy_P(tmpbuf, (const char*)F(MAIN_home5),16);
  lcd.print(rpad(strbuf, tmpbuf));
  lcd.setCursor(0,1);
  lcd.print(F(MAIN_home6));
  delay(1000);
  lcd.setCursor(0,1);
  lcd.print(F(MAIN_home7));
  delay(1000);
  lcd.setCursor(0,1);
  lcd.print(F(MAIN_home8));
  delay(1000);
  lcdClear();
  lcd.print(F(MAIN_home9));

  myStepper.moveTo(0);
  while ( myStepper.moving() );     // wait for deceleration ramp;
}

// ------------------------------------------------------------------------
// show Stepper/Encoder/calculated steps on screen
void displaySteps() 
{
  char intbuf[7];   // buffer for encoder and steps (5 characters + \0)
  char intbuf2[7];  // buffer for encoder and steps

  // first display line
  if (updLcd == 1 && currentAppMode == APP_DISP_UPD) 
  {
    lcd.print(rpad(strbuf, EmptyStr));
    lcd.setCursor(0, 0);
    snprintf(intbuf,sizeof(intbuf),"%ld",myStepper.currentPosition());  // (use "%ld" for longs)
    fmt(strbuf, 2, "Steps ", intbuf);
    rpad (strbuf, strbuf); // if strbuf is not padded, one cannot put an arrow at the end
    // add small arrows to indicate direction
    if (myStepper.moving()) 
    {
      if (knitContinuous == 1) 
      {
        strbuf[LCD_COLS-2] = 0b01000011; // for continuous
      }
      if (encDir == STEP_CW) 
      {
        strbuf[LCD_COLS-1] = 0b01111110; // formward array
      } 
      else if (encDir == STEP_CCW) 
      {
        strbuf[LCD_COLS-1] = 0b01111111;    // back arrow
      }
    }
    lcd.print(strbuf);

  // second display line
    lcd.print(rpad(strbuf, EmptyStr));
    lcd.setCursor(0, 1);
    snprintf(intbuf,sizeof(intbuf),"%ld",newEncPosition);  // (use "%ld" for longs)
    snprintf(intbuf2,sizeof(intbuf2),"%ld",(long)posFromEnc);
    // inttostr(intbuf, newEncPosition);
    fmt(strbuf, 4, "E", intbuf,"/C",intbuf2);
    rpad(strbuf,strbuf);
    lcd.print(strbuf);
    updLcd = 0;
  }

  #ifdef DEBUG_POSITION
  debugReportSteps();
  #endif

}

// ------------------------------------------------------------------------
// display startup screen with homeing hint
void startupScreen() 
{
  if (startupDisplayed == 0) 
  {
    lcdClear();
    lcd.print(F(MAIN_start1));
    lcd.setCursor(0, 1);
    lcd.print(F(MAIN_start2));
    startupDisplayed = 1;
  }
}

// ------------------------------------------------------------------------
// handler errorState events (endstops, yarnsensor)
void errorStateHandling() 
{
  byte nok = 1;

  while (errorState != OK) 
  {
    if(millis() - lastRefreshTime >= REFRESH_INTERVAL)
    {
      lastRefreshTime += REFRESH_INTERVAL;
      errorDisplayed = 1 - errorDisplayed;
      if (errorDisplayed == 1) 
      {
        #ifdef DEBUG_APPMODE
        DBG_PRINTLN("Errorscreen LCD on");
        #endif
        lcdClear();

        switch (errorState) 
        {
          case HIT_MAX:
            lcd.print(F(MAIN_err1));
          break;
          case HIT_HOME:
            lcd.print(F(MAIN_err2));
          break;
          case HIT_YARN1:
            lcd.print(F(MAIN_err3));
            errorSymbolDisplayed = 1;
          break;
          case HIT_YARN2:
            lcd.print(F(MAIN_err4));
            errorSymbolDisplayed = 1;
          break;
          case MISS_FOOT:
            lcd.print(F(MAIN_err5));
          break;
          case MISS_OVRL:
            lcd.print(F(MAIN_err6));
          break;
          default:
          break;
        }
        lcd.setCursor(0, 1);
        lcd.print(F(MAIN_err_sel));
      } 
      else 
      {
        #ifdef DEBUG_APPMODE
        DBG_PRINTLN("Errorscreen LCD off");
        #endif

        lcdClear();
      }
    }

    btn = getButton();

    if (btn==BUTTON_SELECT_SHORT_RELEASE) 
    {
      if (currentAppMode == APP_PRE_CHECK && digitalRead(footswitchPin) != signalLevel)
      { // foot switch installed
        currentAppMode = APP_PGMSTART;
        nok = 0;
      } 
      
      if (currentAppMode != APP_PRE_CHECK) 
      {
        // only clear error after overload sensor reset
        if (digitalRead(overloadPin) != signalLevel) 
        {
          // if (currentAppMode == APP_CARRIAGE_RUNNING) 
          // { 
            nextAppMode = APP_NORMAL_MODE;
          // } 
          // else 
          // {
          //  nextAppMode = currentAppMode;
          // }
          currentAppMode = APP_DISP_UPD;
          screenToShow = ROWS_WITH_HEADER;
          nok = 0;
        } 
      }
    }

    if (nok == 0) 
    {
      cli();
      errorState = OK;
      knitContinuous = 0;
      knitRow = 0;
      sei();
    }
  }
}

// ------------------------------------------------------------------------
void printVersion() {
  char version[15];
  snprintf(version, 15, "%i.%i.%i+%i", VERSION_MAJOR, VERSION_MINOR, VERSION_PATCH, VERSION_BUILD);
  lcd.print(version);
}

// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
// ------------------------------------------------------------------------

// ------------------------------------------------------------------------
void setup() 
{ 
  #ifdef DEBUG
  Serial.begin(115200);
  #endif
  backLightOn();
  lcd.begin(LCD_COLS, LCD_ROWS);

  pinMode(alarmPin, OUTPUT);

  currentConfig.load();
  if (!currentConfig.opMode)
  {
    currentConfig.rowCount = 0;
    currentConfig.save();
  }
  currentRowCount = currentConfig.rowCount;
  // oldRowCount = currentRowCount;
  operationRPM = currentConfig.carriageSpeed * 10;

  startBacklightPWM();
  setBacklightBrightness(currentConfig.displayBrightness);

  enablePort_PCI();
  enablePort_Pins();
  
  DBG_PRINTLN(F("Knitting motor test started..."));
  DBG_PRINT(F("Encoder multiplier: "));
  DBG_PRINTLN(encoderMultiplier);

  pinMode(enaPin, OUTPUT);
  digitalWrite(enaPin, LOW);
  
  pinMode(refPin, INPUT_PULLUP );       // NO
  pinMode(maxPin, INPUT_PULLUP );       // NO
  pinMode(yarnMainPin, INPUT_PULLUP );  // NO
  pinMode(yarnSecPin, INPUT_PULLUP );   // NO
  pinMode(overloadPin, INPUT_PULLUP );  // NC
  // pinMode(footswitchPin, INPUT_PULLUP); // NC

  footswitchBtn.attach ( footswitchPin , INPUT_PULLUP );
  footswitchBtn.interval( 5 );
  footswitchBtn.setPressedState( HIGH ); 

  lcd.clear();
  lcd.print(F("Knitting Motor"));
  lcd.setCursor(0,1);
  printVersion();
  delay(2000);

}

void show2secMessage(const __FlashStringHelper *line1, const __FlashStringHelper *line2, const AppModeValues appM) {
  lcdClear();
  lcd.print(line1);
  lcd.setCursor(0, 1);
  lcd.print(line2);
  delay(2000);
  currentAppMode = appM;
}

void show2secMessage(const char *line1, const char *line2, const AppModeValues appM) {
  lcdClear();
  lcd.print(line1);
  lcd.setCursor(0, 1);
  lcd.print(line2);
  delay(2000);
  currentAppMode = appM;
}

void startCarriage() {
  if (currentConfig.opMode){
    if (currentRowCount > 0)
    {
      cli();
      knitContinuous = 1 - knitContinuous;
      sei();
      currentAppMode = APP_CARRIAGE_RUNNING;
    }
    else
    {
      show2secMessage(F(MAIN_rowcount1), F(MAIN_rowcount2), APP_DISP_UPD);
    }
  } 
  else
  {
      cli();
      knitContinuous = 1 - knitContinuous;
      sei();
      currentAppMode = APP_CARRIAGE_RUNNING;
  }
}

// ------------------------------------------------------------------------
void knitLeft() {
  DBG_PRINT(F("-> knitting to the left pos: "));
  if (currentConfig.leftBoundary != 0) 
  {
    myStepper.moveTo(currentConfig.leftBoundary);
    DBG_PRINTLN(currentConfig.leftBoundary);
  } 
  else 
  {
    myStepper.moveTo(stepperMaxPos);
    DBG_PRINTLN(stepperMaxPos);
  }
}

// ------------------------------------------------------------------------
void knitRight() {
    DBG_PRINT(F("-> knitting to the right pos: "));
    if (currentConfig.rightBoundary != 0) {
      myStepper.moveTo(currentConfig.rightBoundary);
      DBG_PRINTLN(currentConfig.rightBoundary);
    } 
    else 
    {
      myStepper.moveTo(0);
      DBG_PRINTLN(0);
    }
}

// ------------------------------------------------------------------------
void loop() 
{
  char sngbuf[420];

  readEncoder();

  btn = getButton();

  // button beep
  if (btn && currentConfig.buttonBeep && currentAppMode != APP_ALARM)
  {
    byte btnFlags = btn & 192;

    if (btnFlags == BUTTON_PRESSED_IND)   // if any button pressed.
    {
      tone(alarmPin, NOTE_G5, 100);
    }
  }

  // footswitch installed?
  if (currentAppMode == APP_PRE_CHECK && digitalRead(footswitchPin) == signalLevel) 
  {
    cli();
    errorState = MISS_FOOT;
    sei();
  } 
  else if (currentAppMode == APP_PRE_CHECK) {
      cli();
      errorState = OK;
      sei();
      currentAppMode = APP_PGMSTART;
      nextAppMode = APP_PGMSTART;
  }
  
  // only proceed if errorState is OK
  if (errorState == OK) 
  {

      // reset direction arrow on display if not moving
    if (!currentConfig.arrowMode) {
      if (!myStepper.moving() && arrowShown ==1)
      {
        for (int i=0; i<5; i++) {
          lcd.setCursor(LCD_COLS-i,1);
          lcd.print(" ");
        }
        arrowShown = 0;
      }
    }
    
    // only query myStepper if attached
    if (stepperAttached == 1) 
    {
      // calculated encoder position from stepper steps
      posFromEnc = round((float)myStepper.currentPosition() * encoderMultiplier);
    }

    // menu branching
    switch (currentAppMode) 
    {

      // ----------------------------------
      // program start
      case APP_PGMSTART:
        #ifdef DEBUG_APPMODE
        DBG_PRINTLN("appMode: APP_PGMSTART");
        #endif

        if (stepperAttached == 0) 
        {
          startupScreen();
        }

        if (btn==BUTTON_SELECT_SHORT_RELEASE) 
        {
          currentAppMode = APP_DISP_UPD;
          nextAppMode = APP_NORMAL_MODE;
          screenToShow = ROWS_WITH_HEADER;
          homing();
        }

      // ----------------------------------
      // normal operation, menu closed
      case APP_NORMAL_MODE :
        #ifdef DEBUG_APPMODE
        DBG_PRINTLN("appMode: APP_NORMAL_MODE");
        #endif

        if (btn == BUTTON_SELECT_LONG_PRESSED) {
          startCarriage();
        }

        if (btn == BUTTON_UP_LONG_PRESSED)
        {
          lcd.clear();
          menuStartTime = millis();
          currentAppMode = APP_MENU_MODE;
          refreshMenuDisplay(REFRESH_DESCEND);
        }

        if (btn == BUTTON_DOWN_LONG_PRESSED)
        {
          currentConfig.rowCount = 0;
          currentConfig.save();
          currentRowCount = currentConfig.rowCount;
          currentAppMode = APP_DISP_UPD;
          screenToShow = ROWS_WITH_HEADER;
        }

        if (btn == BUTTON_RIGHT_LONG_PRESSED)
        {
          if (!myStepper.moving())
          {
            if (myStepper.currentPosition() != 0)
            {
              myStepper.moveTo(0);
              screenToShow = ROWS_WITH_HEADER;
              show2secMessage(F(MAIN_goingto), F(MAIN_zero), APP_DISP_UPD);
            }
          }
        }

        if (btn == BUTTON_LEFT_LONG_PRESSED)
        {
          if (!myStepper.moving())
          {
            if (myStepper.currentPosition() != stepperMaxPos)
            {
              myStepper.moveTo(stepperMaxPos);
              screenToShow = ROWS_WITH_HEADER;
              show2secMessage(F(MAIN_goingto), F(MAIN_max), APP_DISP_UPD);
            }
          }
        }

        if (btn == BUTTON_LEFT_SHORT_RELEASE)
        {
          if (!myStepper.moving())
          {
            if (myStepper.currentPosition() != stepperMaxPos)
            {
              myStepper.move(240);
            }
          }
        }

        if (btn == BUTTON_RIGHT_SHORT_RELEASE)
        {
          if (!myStepper.moving())
          {
            if (myStepper.currentPosition() != 0)
            {
              myStepper.move(-240);
            }
          }
        }

        footswitchBtn.update();
        if (footswitchBtn.pressed()) {
          fpHit = 1;
        }

        // foot pedal hit?
        if (fpHit == 1) {
          if (currentConfig.footMode) {
            cli();
            knitRow = 1;
            sei();
            currentAppMode = APP_CARRIAGE_RUNNING;
          } 
          else
          {
            startCarriage();
          }

          cli();
          fpHit = 0;
          sei();

        }
        break;

      // ----------------------------------
      // active whenever the carriage moves
      case APP_CARRIAGE_RUNNING:
        #ifdef DEBUG_APPMODE
        DBG_PRINTLN("appMode: APP_CARRIAGE_RUNNING");
        #endif

        // foot pedal hit
        if (knitRow == 1) 
        {
          if (!myStepper.moving()) 
          {
            DBG_PRINT(F("Request to knit 1 row: "));

            if (errPreservedDir != STEP_STOP) {   // stepper stopped on error
              switch(errPreservedDir)
              {
                case STEP_CW:
                  knitRight();
                  break;
                case STEP_CCW:
                  knitLeft();
                  break;
              }
            } 
            else // normal operation
            {
              // carriage below half => knit to the left
              if (posFromEnc < ((float)stepperMidPos * encoderMultiplier)) 
              {  
                knitLeft();
              }

              // carriage above half => knit to the right
              if (posFromEnc >= ((float)stepperMidPos * encoderMultiplier)) 
              { 
                knitRight();
              }
            }

            // only normal row counting if not stopped by sensor
            if (errPreservedDir == STEP_STOP)
            {
              // count rows up or down regarding modus
              if (currentConfig.opMode) {
                if (currentRowCount > 0) {
                  currentRowCount -= 1;
                  DBG_PRINT(F("Remaining rows: "));
                  DBG_PRINTLN(currentRowCount);
                }
              } else {
                currentRowCount += 1;
                DBG_PRINT(F("Rows already knit: "));
                DBG_PRINTLN(currentRowCount);
              }
            }

            // reset direction saver if error
            if (errPreservedDir != STEP_STOP)
            {
              cli();
              errPreservedDir = STEP_STOP;
              sei();
              if (oldRowCount > 0)
              { 
                oldRowCount -= 1;
              }
            }

            cli();
            knitRow = 0;
            sei();
          }
        }

        // deactivate continuous?
        if (knitContinuous == 1)
        {
          footswitchBtn.update();

          // deeactivate continuous with DOWN button
          if (btn == BUTTON_DOWN_SHORT_RELEASE)
          {
              cli();
              knitContinuous = 0;
              sei();
              screenToShow = ROWS_WITH_HEADER;
              show2secMessage(F(MAIN_stopknit1), F(MAIN_stopknit2), APP_DISP_UPD);
          }
          // deactivate continuous with foot switch
          if (footswitchBtn.pressed())
          {
              cli();
              knitContinuous = 0;
              fpHit = 0;
              sei();
              screenToShow = ROWS_WITH_HEADER;
              show2secMessage(F(MAIN_stopknit1), F(MAIN_stopknit2), APP_DISP_UPD);
          }
        }

        // only allow continuous knitting if rowCount is set
        if (knitContinuous == 1) 
        {
          // auto mode
          if (currentConfig.opMode)
          {
            if (currentRowCount > 0) 
            {
              cli();
              knitRow = 1;
              sei();
              nextAppMode = APP_CARRIAGE_RUNNING;
            } 
            else
            {
              cli();
              knitContinuous = 0;
              sei();
              nextAppMode = APP_NORMAL_MODE;
            }
          } 
          else // manual mode
          {
              cli();
              knitRow = 1;
              sei();
              nextAppMode = APP_CARRIAGE_RUNNING;
          }
        } // if (knitContinuous == 1) 
        else
        {  
          nextAppMode = APP_NORMAL_MODE;
        }

        // save current row count, update display
        if (oldRowCount != currentRowCount)
        {
          currentConfig.rowCount = currentRowCount;
          currentConfig.save();

          oldRowCount = currentRowCount;
          // currentAppMode = APP_DISP_UPD;
          screenToShow = ROWS;

          if (currentRowCount <= 0)
          {
            // currentConfig.rowCount = 0;
            // currentConfig.save();
            // currentRowCount = currentConfig.rowCount;
            nextAppMode = APP_ALARM;
            if ( !anyrtttl::nonblocking::isPlaying() )
            {
              strcpy_P(sngbuf, (char*)pgm_read_dword(&(Melodies[currentConfig.alarmTune])));
              anyrtttl::nonblocking::begin(alarmPin, sngbuf);
            }
          }
        } // if (oldRowCount != currentRowCount)
        // else if (oldRowCount == 0)
        // {
        currentAppMode = APP_DISP_UPD;
        // }
       
        // counter if carriage moved
        rowsKnit += 1;
        break;

      // ----------------------------------
      // menu opened
      case APP_MENU_MODE :
      {
        #ifdef DEBUG_APPMODE
        DBG_PRINTLN("appMode: APP_MENU_MODE");
        #endif

        // exit menu after 5 seconds
        if ((menuStartTime + 5 * 1000) <= millis())
        {
          currentAppMode = APP_DISP_UPD;
          nextAppMode = APP_NORMAL_MODE;
          screenToShow = ROWS_WITH_HEADER;
        } 
        else 
        {
          byte menuMode = Menu1.handleNavigation(getNavAction, refreshMenuDisplay);

          // reset 5 second countdown on keypress
          if (btn)
          {
            menuStartTime = millis();
          }

          if (menuMode == MENU_EXIT)
          {
            currentAppMode = APP_DISP_UPD;
            nextAppMode = APP_NORMAL_MODE;
            screenToShow = ROWS_WITH_HEADER;
          }
          else if (menuMode == MENU_INVOKE_ITEM)
          {
            currentAppMode = APP_PROCESS_MENU_CMD;

            // Indicate selected item.
            if (Menu1.getCurrentItemCmdId() != mnuCmdResetToDefaults && \
                Menu1.getCurrentItemCmdId() != mnuCmdInfo && \
                Menu1.getCurrentItemCmdId() != mnuCmdHomeing && \
                Menu1.getCurrentItemCmdId() != mnuCmdMoveToStart && \
                Menu1.getCurrentItemCmdId() != mnuCmdMoveToMid && \
                Menu1.getCurrentItemCmdId() != mnuCmdMoveToEnd && \
                Menu1.getCurrentItemCmdId() != mnuCmdClearBnd )
            {
              lcd.setCursor(0, 1);
              strbuf[0] = 0b01111110; // forward arrow representing input prompt.
              strbuf[1] = 0;
              lcd.print(strbuf);
            }
          }
        }
        break;
      }

      // ----------------------------------
      // currentRowCount = 0, ready
      case APP_ALARM:
        #ifdef DEBUG_APPMODE
        DBG_PRINTLN("appMode: APP_ALARM");
        #endif

        if (btn)
        {
          byte btnFlags = btn & 192;
          if (btnFlags == BUTTON_SHORT_RELEASE_IND || btnFlags == BUTTON_LONG_RELEASE_IND)
          {
            currentAppMode = APP_NORMAL_MODE;
          }
        }
        else if (!anyrtttl::nonblocking::isPlaying())
        {
          currentAppMode = APP_NORMAL_MODE;
        }

        if (currentAppMode == APP_NORMAL_MODE)
        {
          anyrtttl::nonblocking::stop();
        }
        else
        {
          anyrtttl::nonblocking::play();
        }
        break;

      // ----------------------------------
      // display handling
      case APP_DISP_UPD:
        #ifdef DEBUG_APPMODE
        DBG_PRINTLN("appMode: APP_DISP_UPD");
        #endif

        switch (screenToShow)
        {
        case STEPS:
          break;
        case ROWS:
          printRowCount(currentRowCount, false);
          break;
        case ROWS_WITH_HEADER:
          printRowCount(currentRowCount, true);
          break;
        }

        currentAppMode = nextAppMode;
        break;

      // ----------------------------------
      // mode is called after entering a menu item
      case APP_PROCESS_MENU_CMD:
        {
          #ifdef DEBUG_APPMODE
          DBG_PRINTLN("appMode: APP_PROCESS_MENU_CMD");
          #endif

          byte processingComplete = processMenuCommand(Menu1.getCurrentItemCmdId());

          if (processingComplete)
          {
            menuStartTime = millis();
            currentAppMode = APP_MENU_MODE;
            if ( anyrtttl::nonblocking::isPlaying() )
            {
              anyrtttl::nonblocking::stop();
            }
            // clear forward arrow
            lcd.setCursor(0, 1);
            strbuf[0] = ' '; // clear forward arrow
            strbuf[1] = 0;
            lcd.print(strbuf);
          }
          break;
        } // case APP_PROCESS_MENU_CMD:

        default:
          break;

    } // switch(appMode)
  } // if (errorState == OK) {}
  else 
  {
      errorStateHandling();
  }

}

// ------------------------------------------------------------------------
// ISR: Regulate LCD backlight intensity
// (TIMER0 compare match mode)
SIGNAL(TIMER0_COMPA_vect)
{
  lcdBacklightISR();
}

// ------------------------------------------------------------------------
// ISR: Immediately stop stepper if endstop or interrupt pins are hit
// (Pin change interrupt on Port C)
ISR (PCINT1_vect)
{
  // static unsigned long previousStateChangeMillis = 0;
  // static bool previousPinState = LOW;

  if (stepperAttached == 1) 
  {
    if (myStepper.moving()) 
    {
      if (digitalRead(refPin) == atRefpoint) 
      {
        myStepper.stop();
        errorState = HIT_HOME;
        knitContinuous = 0;
        knitRow = 0;
      }
      if (digitalRead(maxPin) == atRefpoint) 
      {
        myStepper.stop();
        errorState = HIT_MAX;
        knitContinuous = 0;
        knitRow = 0;
      }

      if (currentConfig.yarnsensorMainEnable == true) 
      {
        if (digitalRead(yarnMainPin) != atRefpoint) 
        {
          errPreservedDir = encDir;
          myStepper.stop();
          errorState = HIT_YARN1;
          knitContinuous = 0;
          knitRow = 0;
        }
      }
      if (currentConfig.yarnsensorSecEnable == true) 
      {
        if (digitalRead(yarnSecPin) != atRefpoint) 
        {
          errPreservedDir = encDir;
          myStepper.stop();
          errorState = HIT_YARN2;
          knitContinuous = 0;
          knitRow = 0;
        }
      }
    }
      /*
    // debounce foot pedal, see: https://arduino.stackexchange.com/a/45051
    byte pinState = digitalRead(footswitchPin);
    if (pinState != previousPinState) 
    { // ignore pin changes of pins other than SELECTOR_BTN
      if (pinState == signalLevel) {
        if ((millis() - previousStateChangeMillis) > swDebounceTime) 
        { // debounce
          fpHit = 1;
        }
      }
      previousPinState = pinState;
      previousStateChangeMillis = millis();
    }
    */
    // if (digitalRead(footswitchPin) == signalLevel)
    // {
    //  fpHit = 1;
    // }
  }
}

// ------------------------------------------------------------------------
// ISR: Immediately stop stepper if endstop or interrupt pins are hit
// (Pin change interrupt on Port D)
ISR (PCINT2_vect)
{
  if (stepperAttached == 1) 
  {
    if (myStepper.moving()) 
    {
      if (currentConfig.overloadsensorEnable == true)
      {
        if (digitalRead(overloadPin) != atRefpoint) 
        {
          myStepper.stop();
          errorState = MISS_OVRL;
          knitContinuous = 0;
          knitRow = 0;
        }
      }
    }
  }
}