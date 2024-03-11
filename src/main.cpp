#include <string.h>
#include <TimerOne.h>
#include <strlcpy.h>
#include <LiquidCrystal.h>
#include <MobaTools.h>
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

// error and warnings
enum ErrorStates : byte {
  OK,
  HIT_HOME,
  HIT_MAX,
  HIT_YARN1,
  HIT_YARN2,
  ERR_OVRL,
  MISS_FOOT
};

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

// different main screens to show during APP_NORMAL_MODE
enum Screens : byte
{
  STEPS,
  ROWS,
  ROWS_WITH_HEADER
};

// Rotary encoder params
long oldEncPosition  = 0;
long newEncPosition = 0;
short encDir = STEP_STOP;

// Stepper pins params
long stepperMaxPos = 0;     // max endstop
long stepperMidPos = 0;     // mid position after homing
byte stepperAttached = 0;
int operationRPM = 0;

long posFromEnc = 0;  // calculated stepper position from encoder

byte updLcd = 1; // update LCD after encoder value changed

// error relevant variables
volatile byte errorState = OK;  // endstops/sensors hit? (0=ok,1=max endstop,2=home endstop,3=yarnsensor)
volatile short errPreservedDir = STEP_STOP;   // preserve old direction in case of an error

// temporary button request, later changed with menu
const char EmptyStr[] = "";
char strbuf[LCD_COLS + 1]; // one line of lcd display

// knitting related
unsigned int currentRowCount = 0;
unsigned int oldRowCount = 0;
unsigned int rowsKnit = 0;
volatile byte knitRow = 0;         // switch to knit 1 row
volatile byte knitContinuous = 0;  // switch to control continuous knitting
byte fpHit = 0;           // footpedal hit switch

// error and display related
unsigned long menuStartTime;
byte arrowShown = 0;    // arrow on display?
byte startupDisplayed = 0;  // is showing startup display
byte errorDisplayed = 0;  // controls blinking of error message
byte errorSymbolDisplayed = 0; // show continue screen after error

MoToStepper myStepper( stepsPerRev, STEPDIR );
LiquidCrystal lcd(rsPin, enablePin, d0Pin, d1Pin, d2Pin, d3Pin);
AppModeValues currentAppMode = APP_PRE_CHECK;
AppModeValues nextAppMode = APP_PRE_CHECK;
MenuManager Menu1(knittingMenu_Root, menuCount(knittingMenu_Root));
Config currentConfig;
Screens screenToShow = ROWS_WITH_HEADER;
Bounce2::Button footswitchBtn = Bounce2::Button();
Bounce2::Button homeBtn = Bounce2::Button();
Bounce2::Button maxBtn = Bounce2::Button();
Bounce2::Button yarnMainSens = Bounce2::Button();
Bounce2::Button yarnSecSens = Bounce2::Button();
Bounce2::Button overloadSens= Bounce2::Button();

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
#ifdef DEBUG_POSITION
void debugReportSteps();
#endif

// ------------------------------------------------------------------------
void doEncoderA() {

  if (digitalRead(rotaA) != digitalRead(rotaB)) {
    newEncPosition--;
  } else {
    newEncPosition++;
  }
}

// ------------------------------------------------------------------------
void doEncoderB() {
  if (digitalRead(rotaA) == digitalRead(rotaB)) {
    newEncPosition--;
  } else {
    newEncPosition++;
  }
}

// ------------------------------------------------------------------------
void stepperAttach() 
{
  myStepper.attach( stepPin, dirPin );
  // myStepper.attachEnable( enaPin, enaDelay, signalLevel );        // Enable Pin aktivieren ( signalLevel=aktiv )
  myStepper.setSpeed( homeSpeed );
  myStepper.setRampLen( rampLen );                       // Rampenlänge 100 Steps bei 20U/min
  digitalWrite(enaPin, enaLevel);
  delay(enaDelay);
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
  // Run stepper to home endpoint and zero position
  DBG_PRINT(F("Homing"));

  // move fast to endpoint...
  if ( digitalRead( homePin ) != signalLevel ) 
  {
    // ... but only if stepper is not already there
    myStepper.setSpeedSteps( fastSpeedSteps, rampLen );
    myStepper.rotate(STEP_CCW);
    while ( digitalRead( homePin ) != signalLevel );
  }

  // home endstop reached, stop
  myStepper.stop();
  // myStepper.rotate(STEP_STOP);
  while ( myStepper.moving() );     // wait for deceleration ramp;
  
  // slow backup until endstop releases
  myStepper.setSpeedSteps( slowSpeedSteps );
  myStepper.setRampLen(0);
  myStepper.rotate(STEP_CW);
  while ( digitalRead( homePin ) == signalLevel );
  
  DBG_PRINTLN(F("Home endpoint reached"));
  myStepper.rotate(0);
  while (myStepper.moving() );
  myStepper.setZero();        // zero stepper pos
  myStepper.setSpeed( operationRPM );
  myStepper.setRampLen((uint16_t)rampLen);        // acceleration ramp 100 steps at 20rpm
  DBG_PRINTLN(F("Homeing end"));
}

// ------------------------------------------------------------------------
// Move stepper to max end of pathway
void toMaxPoint() 
{
  // Run stepper to home endpoint and zero position
  DBG_PRINTLN(F("Going to max endstop"));
  // move fast to endpoint...
  if ( digitalRead( maxPin ) != signalLevel ) 
  {
    // ... but only if stepper is not already there
    myStepper.setSpeedSteps( fastSpeedSteps, rampLen );
    myStepper.rotate(STEP_CW);
    while ( digitalRead( maxPin ) != signalLevel );
  }

  // home endstop reached, stop
  myStepper.stop();
  // myStepper.rotate(STEP_STOP);
  while ( myStepper.moving() );     // wait for deceleration ramp;
  
  // slow backup until endstop releases
  myStepper.setSpeedSteps( slowSpeedSteps );
  myStepper.setRampLen(0);
  myStepper.rotate(STEP_CCW);
  while ( digitalRead( maxPin ) == signalLevel );
  
  DBG_PRINT(F("Max endpoint reached at: "));
  DBG_PRINTLN(myStepper.currentPosition());
  myStepper.rotate(0);
  while (myStepper.moving() );
  myStepper.setSpeed( operationRPM );
  myStepper.setRampLen((uint16_t)rampLen);
  stepperMaxPos = myStepper.currentPosition();
  DBG_PRINTLN(F("Maxing finished"));
}

// ------------------------------------------------------------------------
// Read values from rotary encoder
void readEncoder() 
{ 
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
  char dbgbuf[250];
  uint32_t interval = 250;
  static uint32_t last = - interval;

  if(millis() - last < interval) return;
  last += interval;
  dbgbuf[0] = 0;
  snprintf(dbgbuf,sizeof(dbgbuf),"stepper: %ld, newEncPosition: %ld, posFromEncoder: %ld, posDiff: %ld", myStepper.currentPosition(), newEncPosition, (long)posFromEnc, (myStepper.currentPosition()-posFromEnc));
  DBG_PRINTLN(dbgbuf);
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
  char intbuf[4];
  // char tmpbuf[17];
  byte bdPad = 0; 

//  if (updLcd == 1 && currentAppMode == APP_DISP_UPD) 
  if (updLcd == 1) 
  {

    if (withTopic == true)
    {
      lcd.clear();
      lcd.setCursor(0,0);
      if (currentConfig.cfg.opMode) {
        lcd.print(F(MAIN_rtk));
      } 
      else
      {
        lcd.print(F(MAIN_rk));
      }
    }

    lcd.setCursor(0,1);

    inttostr(intbuf, rowCount);
    char tmpbuf[LCD_COLS + 1];

    if (nextAppMode == APP_ALARM) {
      strlcpy(tmpbuf, PSTR(MAIN_done), sizeof(tmpbuf));
      rpad(strbuf, tmpbuf);
    } 
    else 
    {
      if (rowCount == 1)
      {
        strlcpy(tmpbuf, PSTR(MAIN_row), sizeof(tmpbuf));
        fmt(strbuf, 2, intbuf, tmpbuf);
      } 
      else 
      {
        strlcpy(tmpbuf, PSTR(MAIN_rows), sizeof(tmpbuf));
        fmt(strbuf, 2, intbuf, tmpbuf);
      }
    }

    // add small arrows to indicate direction
    // if (myStepper.moving()) 
    // {

      rpad (strbuf, strbuf); // if strbuf is not padded, one cannot put an arrow at the end

      // show [C->] if boundaries are defined
      if (currentConfig.cfg.leftBoundary != 0 || currentConfig.cfg.rightBoundary != 0)
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
  char sngbuf[longest_tune+1];


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

  oldTune = currentConfig.cfg.alarmTune;

  switch (cmdId)
  {
    case mnuCmdLeftBnd:
      configChanged = true;
      if (btn == BUTTON_LEFT_PRESSED || btn == BUTTON_LEFT_LONG_PRESSED)
      {
        if (btn == BUTTON_LEFT_PRESSED)
        {
          currentConfig.cfg.leftBoundary = addToVal(120, currentConfig.cfg.leftBoundary,stepperMidPos+2,stepperMaxPos);
          myStepper.moveTo(currentConfig.cfg.leftBoundary);
        }
        if (btn == BUTTON_LEFT_LONG_PRESSED)
        {
          currentConfig.cfg.leftBoundary = addToVal(600, currentConfig.cfg.leftBoundary,stepperMidPos+2,stepperMaxPos);
          myStepper.moveTo(currentConfig.cfg.leftBoundary);
        }
      }
      else if (btn == BUTTON_RIGHT_PRESSED || btn == BUTTON_RIGHT_LONG_PRESSED)
      {
        if (btn == BUTTON_RIGHT_PRESSED)
        {
          currentConfig.cfg.leftBoundary = addToVal(-120, currentConfig.cfg.leftBoundary,stepperMidPos+2,stepperMaxPos);
          myStepper.moveTo(currentConfig.cfg.leftBoundary);
        }
        if (btn == BUTTON_RIGHT_LONG_PRESSED)
        {
          currentConfig.cfg.leftBoundary = addToVal(-600, currentConfig.cfg.leftBoundary,stepperMidPos+2,stepperMaxPos);
          myStepper.moveTo(currentConfig.cfg.leftBoundary);
        }
      }
      else
      {
        configChanged = false;
      }

      myStepper.setSpeed(1000);
      if (currentConfig.cfg.leftBoundary == 0)
      {
        myStepper.moveTo(stepperMidPos+1);
        currentConfig.cfg.leftBoundary = stepperMidPos+1;
        configChanged = true;
        DBG_PRINTLN(F("Moving to stepperMidPos+1"));
      } else {
        myStepper.moveTo(currentConfig.cfg.leftBoundary);
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
          currentConfig.cfg.rightBoundary = addToVal(120, currentConfig.cfg.rightBoundary,0,stepperMidPos-2);
          myStepper.moveTo(currentConfig.cfg.rightBoundary);
        }
        if (btn == BUTTON_LEFT_LONG_PRESSED)
        {
          currentConfig.cfg.rightBoundary = addToVal(600, currentConfig.cfg.rightBoundary,0,stepperMidPos-2);
          myStepper.moveTo(currentConfig.cfg.rightBoundary);
        }
      }
      else if (btn == BUTTON_RIGHT_PRESSED || btn == BUTTON_RIGHT_LONG_PRESSED)
      {
        if (btn == BUTTON_RIGHT_PRESSED)
        {
          currentConfig.cfg.rightBoundary = addToVal(-120, currentConfig.cfg.rightBoundary,0,stepperMidPos-2);
          myStepper.moveTo(currentConfig.cfg.rightBoundary);
        }
        if (btn == BUTTON_RIGHT_LONG_PRESSED)
        {
          currentConfig.cfg.rightBoundary = addToVal(-600, currentConfig.cfg.rightBoundary,0,stepperMidPos-2);
          myStepper.moveTo(currentConfig.cfg.rightBoundary);
        }
      }
      else
      {
        configChanged = false;
      }

      myStepper.setSpeed(1000);
      if (currentConfig.cfg.rightBoundary == 0)
      {
        myStepper.moveTo(stepperMidPos-1);
        currentConfig.cfg.rightBoundary = stepperMidPos-1;
        configChanged = true;
        DBG_PRINTLN(F("Moving to stepperMidPos-1"));
      } 
      else 
      {
        myStepper.moveTo(currentConfig.cfg.rightBoundary);
        DBG_PRINTLN(F("Moving to rightBoundary"));
      }
      while ( myStepper.moving() );

      break;
    case mnuCmdClearBnd:
      if (btn == BUTTON_SELECT_LONG_PRESSED)
      {
        lcdClear();
        currentConfig.cfg.leftBoundary = 0;
        currentConfig.cfg.rightBoundary = 0;
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
        myStepper.moveTo(0+endstopOffset);
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
        myStepper.moveTo(stepperMaxPos-endstopOffset);
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
          currentConfig.cfg.rowCount = addToVal(1, currentConfig.cfg.rowCount,1,999);
        }
        if (btn == BUTTON_UP_LONG_PRESSED)
        {
          currentConfig.cfg.rowCount = addToVal(10, currentConfig.cfg.rowCount,1,999);
        }
      }
      else if (btn == BUTTON_DOWN_PRESSED || btn == BUTTON_DOWN_LONG_PRESSED)
      {
        if (btn == BUTTON_DOWN_PRESSED)
        {
          currentConfig.cfg.rowCount = addToVal(-1, currentConfig.cfg.rowCount,1,999);
        }
        if (btn == BUTTON_DOWN_LONG_PRESSED)
        {
          currentConfig.cfg.rowCount = addToVal(-10, currentConfig.cfg.rowCount,1,999);
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
          currentConfig.cfg.carriageSpeed = addToVal(1, currentConfig.cfg.carriageSpeed,60,200);
        }
        if (btn == BUTTON_UP_LONG_PRESSED)
        {
          currentConfig.cfg.carriageSpeed = addToVal(10, currentConfig.cfg.carriageSpeed,60,200);
        }
      }
      else if (btn == BUTTON_DOWN_PRESSED || btn == BUTTON_DOWN_LONG_PRESSED)
      {
        if (btn == BUTTON_DOWN_PRESSED)
        {
          currentConfig.cfg.carriageSpeed = addToVal(-1, currentConfig.cfg.carriageSpeed,60,200);
        }
        if (btn == BUTTON_DOWN_LONG_PRESSED)
        {
          currentConfig.cfg.carriageSpeed = addToVal(-10, currentConfig.cfg.carriageSpeed,60,200);
        }
      }
      else
      {
        configChanged = false;
      }
      operationRPM = currentConfig.cfg.carriageSpeed * 10;
      myStepper.setSpeed(operationRPM);
      lcd.noBlink();
      break;
    case mnuCmdOpMode:
      configChanged = true;
      if (btn == BUTTON_UP_PRESSED || btn == BUTTON_UP_LONG_PRESSED)
      {
        currentConfig.cfg.opMode = true;
        currentConfig.cfg.rowCount = 0;
        currentRowCount = 0;
        oldRowCount = currentRowCount;
      }
      else if (btn == BUTTON_DOWN_PRESSED || btn == BUTTON_DOWN_LONG_PRESSED)
      {
        currentConfig.cfg.opMode = false;
        currentConfig.cfg.rowCount = 0;
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
        currentConfig.cfg.footMode = true;
      }
      else if (btn == BUTTON_DOWN_PRESSED || btn == BUTTON_DOWN_LONG_PRESSED)
      {
        currentConfig.cfg.footMode = false;
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
        currentConfig.cfg.arrowMode = true;
      }
      else if (btn == BUTTON_DOWN_PRESSED || btn == BUTTON_DOWN_LONG_PRESSED)
      {
        currentConfig.cfg.arrowMode = false;
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
        currentConfig.cfg.buttonBeep = true;
      }
      else if (btn == BUTTON_DOWN_PRESSED || btn == BUTTON_DOWN_LONG_PRESSED)
      {
        currentConfig.cfg.buttonBeep = false;
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
        currentConfig.cfg.yarnsensorMainEnable = true;
      }
      else if (btn == BUTTON_DOWN_PRESSED || btn == BUTTON_DOWN_LONG_PRESSED)
      {
        currentConfig.cfg.yarnsensorMainEnable = false;
      }
      else
      {
        configChanged = false;
      }
      break;
    case mnuCmdYarnSec:
      configChanged = true;
      if (btn == BUTTON_UP_PRESSED || btn == BUTTON_UP_LONG_PRESSED)
      {
        currentConfig.cfg.yarnsensorSecEnable = true;
      }
      else if (btn == BUTTON_DOWN_PRESSED || btn == BUTTON_DOWN_LONG_PRESSED)
      {
        currentConfig.cfg.yarnsensorSecEnable = false;
      }
      else
      {
        configChanged = false;
      }
      break;
    case mnuCmdOverloadsensor:
      configChanged = true;
      if (btn == BUTTON_UP_PRESSED || btn == BUTTON_UP_LONG_PRESSED)
      {
        currentConfig.cfg.overloadsensorEnable = true;
      }
      else if (btn == BUTTON_DOWN_PRESSED || btn == BUTTON_DOWN_LONG_PRESSED)
      {
        currentConfig.cfg.overloadsensorEnable = false;
      }
      else
      {
        configChanged = false;
      }
      break;
    case mnuCmdDisplayBrightness :
      configChanged = true;
      if (btn == BUTTON_UP_PRESSED || btn == BUTTON_UP_LONG_PRESSED)
      {
        currentConfig.cfg.displayBrightness++;
        currentConfig.cfg.displayBrightness = constrain(currentConfig.cfg.displayBrightness, 1, 3);
        setBacklightBrightness(currentConfig.cfg.displayBrightness);
      }
      else if (btn == BUTTON_DOWN_PRESSED || btn == BUTTON_DOWN_LONG_PRESSED)
      {
        currentConfig.cfg.displayBrightness--;
        currentConfig.cfg.displayBrightness = constrain(currentConfig.cfg.displayBrightness, 1, 3);
        setBacklightBrightness(currentConfig.cfg.displayBrightness);
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
        currentConfig.cfg.alarmTune++;
        currentConfig.cfg.alarmTune = constrain(currentConfig.cfg.alarmTune, 0, songCount-1);
      }
      else if (btn == BUTTON_DOWN_PRESSED || btn == BUTTON_DOWN_LONG_PRESSED)
      {
        currentConfig.cfg.alarmTune--;
        currentConfig.cfg.alarmTune = constrain(currentConfig.cfg.alarmTune, 0, songCount-1);
      }
      else
      {
        configChanged = false;
      }

      // stop any current playing tune
      if (anyrtttl::nonblocking::isPlaying())
      {
        if (oldTune != currentConfig.cfg.alarmTune)
        {
          anyrtttl::nonblocking::stop();
        }
      }
      // start playing current selected tune
      if ( !anyrtttl::nonblocking::isPlaying() )
      {
        strcpy_P(sngbuf, (char*)pgm_read_dword(&(Melodies[currentConfig.cfg.alarmTune])));
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
        setBacklightBrightness(currentConfig.cfg.displayBrightness);
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
    currentRowCount = currentConfig.cfg.rowCount;
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
  char tmpbuf[LCD_COLS + 1];

  if (stepperAttached == 0) { stepperAttach(); }

  lcdClear();
  strlcpy(tmpbuf, PSTR(MAIN_home1), sizeof(tmpbuf));
  lcd.print(rpad(strbuf, tmpbuf));
  lcd.setCursor(0,1);
  lcd.print(F(MAIN_home2));
  toRefPoint();
  newEncPosition = 0;  // zero encoder pos, too

  DBG_PRINT(F("Position after homeing: "));
  DBG_PRINTLN(myStepper.currentPosition());

  lcdClear();
  strlcpy(tmpbuf, PSTR(MAIN_home3), sizeof(tmpbuf));
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
  if (currentConfig.cfg.leftBoundary < stepperMidPos && currentConfig.cfg.leftBoundary != 0) {
    currentConfig.cfg.leftBoundary = 0;
    currentConfig.cfg.rightBoundary = 0;
    currentConfig.save();
    DBG_PRINTLN(F("Zeroing boundaries, left out of bounds"));
  }

  if (currentConfig.cfg.rightBoundary > stepperMidPos && currentConfig.cfg.rightBoundary != 0) {
    currentConfig.cfg.leftBoundary = 0;
    currentConfig.cfg.rightBoundary = 0;
    currentConfig.save();
    DBG_PRINTLN(F("Zeroing boundaries, right out of bounds"));
  }

  DBG_PRINT(F("Left boundary: "));
  DBG_PRINTLN(currentConfig.cfg.leftBoundary);
  DBG_PRINT(F("Right boundary: "));
  DBG_PRINTLN(currentConfig.cfg.rightBoundary);

  lcdClear();
  strlcpy(tmpbuf, PSTR(MAIN_home5), sizeof(tmpbuf));
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

  myStepper.moveTo(0+endstopOffset);

  while ( myStepper.moving() );     // wait for deceleration ramp;
}

// ------------------------------------------------------------------------
// show Stepper/Encoder/calculated steps on screen
void displaySteps() 
{
  char intbuf[7];   // buffer for encoder and steps (5 characters + \0)
  char intbuf2[7];  // buffer for encoder and steps

  // first display line
  if (updLcd == 1) 
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
    snprintf(intbuf2,sizeof(intbuf2),"%ld",posFromEnc);
    // inttostr(intbuf, newEncPosition);
    fmt(strbuf, 4, "E", intbuf,"/C",intbuf2);
    rpad(strbuf,strbuf);
    lcd.print(strbuf);
    updLcd = 0;
  }

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
            lcd.print(MAIN_err1);
            break;
          case HIT_HOME:
            lcd.print(MAIN_err2);
            break;
          case HIT_YARN1:
            lcd.print(MAIN_err3);
            errorSymbolDisplayed = 1;
            break;
          case HIT_YARN2:
            lcd.print(MAIN_err4);
            errorSymbolDisplayed = 1;
            break;
          case MISS_FOOT:
            lcd.print(MAIN_err5);
            break;
          case ERR_OVRL:
            lcd.print(MAIN_err6);
            break;
          default:
            break;
        }
        lcd.setCursor(0, 1);
        lcd.print(MAIN_err_sel);
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
      if (currentAppMode == APP_PRE_CHECK && digitalRead(footswitchPin) != signalLevel) // foot switch installed at start
      { 
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
      errorState = OK;
      knitContinuous = 0;
      knitRow = 0;
    }
  }
}

// ------------------------------------------------------------------------
void printVersion() {
  char version[LCD_COLS];
  snprintf_P(version, LCD_COLS, PSTR("%i.%i.%i+%i"), VERSION_MAJOR, VERSION_MINOR, VERSION_PATCH, VERSION_BUILD);
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
  if (!currentConfig.cfg.opMode)
  {
    currentConfig.cfg.rowCount = 0;
    currentConfig.save();
  }
  currentRowCount = currentConfig.cfg.rowCount;
  // oldRowCount = currentRowCount;
  operationRPM = currentConfig.cfg.carriageSpeed * 10;

  Timer1.initialize(10000);
  Timer1.attachInterrupt(lcdBacklightISR);

  setBacklightBrightness(currentConfig.cfg.displayBrightness);

  DBG_PRINTLN(F("Knitting motor test started..."));
  DBG_PRINT(F("Encoder multiplier: "));
  DBG_PRINTLN(encoderMultiplier);

  pinMode(rotaA, INPUT_PULLUP);
  pinMode(rotaB, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(rotaA), doEncoderA, CHANGE);
  attachInterrupt(digitalPinToInterrupt(rotaB), doEncoderB, CHANGE);

  pinMode(enaPin, OUTPUT);
  digitalWrite(enaPin, LOW);
  
  homeBtn.attach ( homePin , INPUT_PULLUP );
  homeBtn.interval( 5 );
  homeBtn.setPressedState( signalLevel );

  maxBtn.attach ( maxPin , INPUT_PULLUP );
  maxBtn.interval( 5 );
  maxBtn.setPressedState( signalLevel );

  footswitchBtn.attach ( footswitchPin , INPUT_PULLUP );
  footswitchBtn.interval( 5 );
  footswitchBtn.setPressedState( signalLevel );

  yarnMainSens.attach ( yarnMainPin, INPUT_PULLUP );
  yarnMainSens.interval( 5 );
  yarnMainSens.setPressedState( signalLevel );

  yarnSecSens.attach ( yarnSecPin , INPUT_PULLUP );
  yarnSecSens.interval( 5 );
  yarnSecSens.setPressedState( signalLevel );

  overloadSens.attach ( overloadPin, INPUT_PULLUP );
  overloadSens.interval( 5 );
  overloadSens.setPressedState( signalLevel );

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
  if (currentConfig.cfg.opMode){
    if (currentRowCount > 0)
    {
      knitContinuous = 1 - knitContinuous;
      currentAppMode = APP_CARRIAGE_RUNNING;
    }
    else
    {
      show2secMessage(F(MAIN_rowcount1), F(MAIN_rowcount2), APP_DISP_UPD);
    }
  } 
  else
  {
      knitContinuous = 1 - knitContinuous;
      currentAppMode = APP_CARRIAGE_RUNNING;
  }
}

// ------------------------------------------------------------------------
void knitLeft() {
  DBG_PRINT(F("-> knitting to the left pos: "));
  if (currentConfig.cfg.leftBoundary != 0) 
  {
    myStepper.moveTo(currentConfig.cfg.leftBoundary);
    DBG_PRINTLN(currentConfig.cfg.leftBoundary);
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
    if (currentConfig.cfg.rightBoundary != 0) {
      myStepper.moveTo(currentConfig.cfg.rightBoundary);
      DBG_PRINTLN(currentConfig.cfg.rightBoundary);
    } 
    else 
    {
      myStepper.moveTo(0+endstopOffset);
      DBG_PRINTLN(0);
    }
}

// ------------------------------------------------------------------------
void loop() 
{
  char sngbuf[longest_tune+1];

  readEncoder();

  btn = getButton();

  // button beep
  if (btn && currentConfig.cfg.buttonBeep && currentAppMode != APP_ALARM)
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
    errorState = MISS_FOOT;
  } 
  else if (currentAppMode == APP_PRE_CHECK) {
      errorState = OK;
      currentAppMode = APP_PGMSTART;
      nextAppMode = APP_PGMSTART;
  }

  // check endstops and sensors
  if (stepperAttached == 1) {
    if (myStepper.moving())
    {

      homeBtn.update();
      if (homeBtn.pressed())
      {
        myStepper.stop();
        errorState = HIT_HOME;
        knitContinuous = 0;
        knitRow = 0;
      }

      maxBtn.update();
      if (maxBtn.pressed())
      {
        myStepper.stop();
        errorState = HIT_MAX;
        knitContinuous = 0;
        knitRow = 0;
      }

      if (currentConfig.cfg.yarnsensorMainEnable == true) 
      {
        yarnMainSens.update();
        if (yarnMainSens.pressed()) 
        {
          errPreservedDir = encDir;
          myStepper.stop();
          errorState = HIT_YARN1;
          knitContinuous = 0;
          knitRow = 0;
        }
      }

      if (currentConfig.cfg.yarnsensorSecEnable == true) 
      {
        yarnSecSens.update();
        if (yarnSecSens.pressed()) 
        {
          errPreservedDir = encDir;
          myStepper.stop();
          errorState = HIT_YARN2;
          knitContinuous = 0;
          knitRow = 0;
        }
      }

      if (currentConfig.cfg.overloadsensorEnable == true)
      {
        overloadSens.update();
        if (overloadSens.pressed()) 
        {
          myStepper.stop();
          errorState = ERR_OVRL;
          knitContinuous = 0;
          knitRow = 0;
        }
      }
    }
  }

  // only proceed if errorState is OK
  if (errorState == OK) 
  {

      // reset direction arrow on display if not moving
    if (!currentConfig.cfg.arrowMode) {
      if (!myStepper.moving() && arrowShown ==1)
      {
        for (int i=0; i<5; i++) 
        {
          lcd.setCursor(LCD_COLS-i,1);
          lcd.print(" ");
        }
        arrowShown = 0;
      }
    }
    
    // position from encoder, but only query myStepper if attached
    if (stepperAttached == 1) 
    {
      // calculated encoder position from stepper steps
      posFromEnc = round((float)newEncPosition * encoderMultiplier);
      #ifdef DEBUG_POSITION
      debugReportSteps();
      #endif

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

        if (btn == BUTTON_SELECT_SHORT_RELEASE || btn == BUTTON_SELECT_LONG_PRESSED || btn == BUTTON_SELECT_LONG_RELEASE) 
        {
          homing();
          btn = 0;    // reset button state explicitely for program start after homing
          currentAppMode = APP_DISP_UPD;
          nextAppMode = APP_NORMAL_MODE;
          screenToShow = ROWS_WITH_HEADER;
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
          currentConfig.cfg.rowCount = 0;
          currentConfig.save();
          currentRowCount = currentConfig.cfg.rowCount;
          currentAppMode = APP_DISP_UPD;
          screenToShow = ROWS_WITH_HEADER;
        }

        if (btn == BUTTON_RIGHT_LONG_PRESSED)
        {
          if (!myStepper.moving())
          {
            if (myStepper.currentPosition() > (0+endstopOffset))
            {
              myStepper.moveTo(0+endstopOffset);
              screenToShow = ROWS_WITH_HEADER;
              show2secMessage(F(MAIN_goingto), F(MAIN_zero), APP_DISP_UPD);
            }
          }
        }

        if (btn == BUTTON_LEFT_LONG_PRESSED)
        {
          if (!myStepper.moving())
          {
            if (myStepper.currentPosition() < (stepperMaxPos-endstopOffset))
            {
              myStepper.moveTo(stepperMaxPos-endstopOffset);
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
          if (currentConfig.cfg.footMode) {
            knitRow = 1;
            currentAppMode = APP_CARRIAGE_RUNNING;
          } 
          else
          {
            startCarriage();
          }
          fpHit = 0;
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
              if (myStepper.currentPosition() < stepperMidPos) 
              {  
                knitLeft();
              }

              // carriage above half => knit to the right
              if (myStepper.currentPosition() >= stepperMidPos)
              { 
                knitRight();
              }
            }

            // only normal row counting if not stopped by sensor
            if (errPreservedDir == STEP_STOP)
            {
              // count rows up or down regarding modus
              if (currentConfig.cfg.opMode) {
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
              errPreservedDir = STEP_STOP;
              if (oldRowCount > 0)
              { 
                oldRowCount -= 1;
              }
            }

            knitRow = 0;
          }
        }

        // deactivate continuous?
        if (knitContinuous == 1)
        {
          footswitchBtn.update();

          // deeactivate continuous with DOWN button
          if (btn == BUTTON_DOWN_SHORT_RELEASE)
          {
              knitContinuous = 0;
              screenToShow = ROWS_WITH_HEADER;
              show2secMessage(F(MAIN_stopknit1), F(MAIN_stopknit2), APP_DISP_UPD);
          }
          // deactivate continuous with foot switch
          if (footswitchBtn.pressed())
          {
              knitContinuous = 0;
              fpHit = 0;
              screenToShow = ROWS_WITH_HEADER;
              show2secMessage(F(MAIN_stopknit1), F(MAIN_stopknit2), APP_DISP_UPD);
          }
        }

        // only allow continuous knitting if rowCount is set
        if (knitContinuous == 1) 
        {
          // auto mode
          if (currentConfig.cfg.opMode)
          {
            if (currentRowCount > 0) 
            {
              knitRow = 1;
              nextAppMode = APP_CARRIAGE_RUNNING;
            } 
            else
            {
              knitContinuous = 0;
              nextAppMode = APP_NORMAL_MODE;
            }
          } 
          else // manual mode
          {
              knitRow = 1;
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
          currentConfig.cfg.rowCount = currentRowCount;
          currentConfig.save();

          oldRowCount = currentRowCount;
          // currentAppMode = APP_DISP_UPD;
          screenToShow = ROWS;

          if (currentRowCount <= 0)
          {
            // currentConfig.cfg.rowCount = 0;
            // currentConfig.save();
            // currentRowCount = currentConfig.cfg.rowCount;
            nextAppMode = APP_ALARM;
            if ( !anyrtttl::nonblocking::isPlaying() )
            {
              strcpy_P(sngbuf, (char*)pgm_read_dword(&(Melodies[currentConfig.cfg.alarmTune])));
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
          displaySteps();
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
