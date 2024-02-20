#include <LiquidCrystal.h>
#include "LcdKeypad.h"
#include "MenuData.h"
#include "Config.h"

#define ALARM_PIN A1

enum AppModeValues
{
  APP_NORMAL_MODE,
  APP_CARRIAGE_RUNNING,
  APP_ALARM,
  APP_MENU_MODE,
  APP_PROCESS_MENU_CMD
};

byte appMode = APP_NORMAL_MODE;

MenuManager Menu1(knittingMenu_Root, menuCount(knittingMenu_Root));

char strbuf[LCD_COLS + 1]; // one line of lcd display
byte btn;

unsigned int currentRowCount;
unsigned int oldRowCount;
unsigned long alarmStartTime;
unsigned long startMillis;
unsigned long menuStartTime;
Config currentConfig;


// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

void printRowCount(unsigned int rowCount, bool withTopic);
void refreshMenuDisplay (byte refreshMode);
byte getNavAction();


void setup()
{
  // Serial.begin(9600);
  pinMode(ALARM_PIN, OUTPUT);
  digitalWrite(ALARM_PIN, LOW);
  backLightOn();
  // set up the LCD's number of columns and rows:
  lcd.begin(LCD_COLS, LCD_ROWS);
  currentConfig.load();
  currentRowCount = currentConfig.rowCount;
  oldRowCount = currentRowCount;

  // Use soft PWM for backlight, as hardware PWM must be avoided for some LCD shields.
  // piggy back on to timer0, which is already set to approx 1khz.
  OCR0A = 0xAF;
  TIMSK0 |= _BV(OCIE0A);
  
  setBacklightBrightness(currentConfig.displayBrightness);

  lcd.clear();
  lcd.print("Knitting Motor");
  lcd.setCursor(4,1);
  lcd.print("starting...");
  delay(2000);

  printRowCount(currentRowCount, true);

}

SIGNAL(TIMER0_COMPA_vect)
{
  lcdBacklightISR();
}

//----------------------------------------------------------------------
// Addition or removal of menu items in MenuData.h will require this method
// to be modified accordingly. 
byte processMenuCommand(byte cmdId)
{
  byte complete = false;  // set to true when menu command processing complete.
  byte configChanged = false;

  if (btn == BUTTON_SELECT_PRESSED)
  {
    if (cmdId != mnuCmdResetToDefaults) complete = true;
  }

  switch (cmdId)
  {
    case mnuCmdLeftBnd:
      configChanged = false;
      break;
    case mnuCmdRightBnd:
      configChanged = false;
      break;
    case mnuCmdClearBnd:
      configChanged = true;
      if (btn == BUTTON_SELECT_LONG_PRESSED)
      {
        currentConfig.leftBoundary = 0;
        currentConfig.rightBoundary = 0;
        lcd.setCursor(1, 1);
        lcd.print("Clear boundaries");
      } else
      {
        configChanged = false;
      }
    case mnuCmdSetRowCount :
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
      break;
    case mnuCmdAlarmDuration:
      configChanged = true;
      if (btn == BUTTON_UP_PRESSED || btn == BUTTON_UP_LONG_PRESSED)
      {
        currentConfig.alarmDuration = ++currentConfig.alarmDuration > 10 ? 10 : currentConfig.alarmDuration;
      }
      else if (btn == BUTTON_DOWN_PRESSED || btn == BUTTON_DOWN_LONG_PRESSED)
      {
        currentConfig.alarmDuration = --currentConfig.alarmDuration < 1 ? 1 : currentConfig.alarmDuration;
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
    case mnuCmdYarnsensor:
      configChanged = true;
      if (btn == BUTTON_UP_PRESSED || btn == BUTTON_UP_LONG_PRESSED)
      {
        currentConfig.yarnsensorEnable = true;
      }
      else if (btn == BUTTON_DOWN_PRESSED || btn == BUTTON_DOWN_LONG_PRESSED)
      {
        currentConfig.yarnsensorEnable = false;
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
    case mnuCmdResetToDefaults:
      if (btn == BUTTON_SELECT_LONG_PRESSED)
      {
        currentConfig.setDefaults();
        setBacklightBrightness(currentConfig.displayBrightness);
        lcd.setCursor(1, 1);
        lcd.print("Defaults loaded");
      }
      else if (/*btn == BUTTON_SELECT_SHORT_RELEASE ||*/ btn == BUTTON_SELECT_LONG_RELEASE)
      {
        complete = true;
      }
      break;    // TODO Process menu commands here:
      default:
    break;
  }

 if (configChanged && (cmdId != mnuCmdResetToDefaults || cmdId != mnuCmdClearBnd)) {
    lcd.setCursor(1, 1);
    lcd.print(rpad(strbuf, currentConfig.getFormattedStr(cmdId))); // Display config value.
  }
  if (complete) {
    currentConfig.save();
    currentRowCount = currentConfig.rowCount;
  }
  return complete;
}


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


//----------------------------------------------------------------------
const char EmptyStr[] = "";

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

  if (withTopic == true)
  {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Rows to knit:");
  }

  lcd.setCursor(0,1);

  inttostr(intbuf, rowCount);

  if (rowCount == 1)
  {
    fmt(strbuf, 2, intbuf, " row");
  } else {
    fmt(strbuf, 2, intbuf, " rows");
  }
    
  lcd.print(strbuf);
}

//----------------------------------------------------------------------
void loop()
{ 
  btn = getButton();

  if (btn && currentConfig.buttonBeep && appMode != APP_ALARM)
  {
    byte btnFlags = btn & 192;

    if (btnFlags == BUTTON_PRESSED_IND)   // if any button pressed.
    {
      digitalWrite(ALARM_PIN, HIGH);
      delay(3);
      digitalWrite(ALARM_PIN, LOW);
    }
  }

  switch (appMode)
  {
    case APP_NORMAL_MODE :
     if (btn == BUTTON_SELECT_LONG_PRESSED)
      {
        if (currentRowCount > 0)
        {
          appMode = APP_CARRIAGE_RUNNING;
          startMillis = millis();
          lcd.setCursor(0, 0);
          lcd.print(rpad(strbuf,"In progress:"));
        }
      }
      if (btn == BUTTON_UP_LONG_PRESSED)
      {
        lcd.clear();
        menuStartTime = millis();
        appMode = APP_MENU_MODE;
        refreshMenuDisplay(REFRESH_DESCEND);
      } 
      if (btn == BUTTON_DOWN_LONG_PRESSED)
      {
        currentConfig.rowCount = 0;
        currentConfig.save();
        currentRowCount = currentConfig.rowCount;
        printRowCount(currentRowCount, true);
        appMode = APP_MENU_MODE;
        refreshMenuDisplay(REFRESH_DESCEND);
      } 
      break;
    case APP_CARRIAGE_RUNNING:
      // simulate carriage run
      if ((startMillis+1000) <= millis())
      {
        currentRowCount -= 1;
        startMillis = millis();
      }
      if (oldRowCount != currentRowCount){

        // TODO: add stepper running routine

        currentConfig.rowCount = currentRowCount;
        currentConfig.save();
        printRowCount(currentRowCount, false);
        oldRowCount = currentRowCount;

        if (currentRowCount <= 0)
        {
              currentConfig.rowCount = 0;
              currentConfig.save();
              currentRowCount = currentConfig.rowCount;
              appMode = APP_ALARM;
              alarmStartTime = millis();
              digitalWrite(ALARM_PIN, HIGH);
              lcd.blink();
        }
      }
      if (btn == BUTTON_SELECT_SHORT_RELEASE)
      {
        appMode = APP_NORMAL_MODE;
      }
      if (appMode == APP_NORMAL_MODE || appMode == APP_ALARM)
      {
        printRowCount(currentRowCount, true);
      }
      break;
    case APP_MENU_MODE :
    {
      // exit menu after 5 seconds
      if ((menuStartTime + 5 * 1000) <= millis())
      {
        printRowCount(currentRowCount, true);
        appMode = APP_NORMAL_MODE;
      } else 
      {
        byte menuMode = Menu1.handleNavigation(getNavAction, refreshMenuDisplay);

        // reset 5 second countdown on keypress
        if (btn)
        {
          menuStartTime = millis();
        }

        if (menuMode == MENU_EXIT)
        {
          printRowCount(currentRowCount, true);
          appMode = APP_NORMAL_MODE;
        }
        else if (menuMode == MENU_INVOKE_ITEM)
        {
          appMode = APP_PROCESS_MENU_CMD;

          // Indicate selected item.
          if (Menu1.getCurrentItemCmdId() != mnuCmdResetToDefaults)
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
    case APP_ALARM:
      if (btn)
      {
        byte btnFlags = btn & 192;

        if (btnFlags == BUTTON_SHORT_RELEASE_IND || btnFlags == BUTTON_LONG_RELEASE_IND)
        {
          appMode = APP_NORMAL_MODE;
        }
      }
      else if (millis() - alarmStartTime >= (short)currentConfig.alarmDuration * 1000)
      {
        appMode = APP_NORMAL_MODE;
      }

      if (appMode == APP_NORMAL_MODE)
      {
        lcd.noBlink();
        digitalWrite(ALARM_PIN, LOW);
      }
      break;
    case APP_PROCESS_MENU_CMD :
    {
      byte processingComplete = processMenuCommand(Menu1.getCurrentItemCmdId());

      if (processingComplete)
      {
        menuStartTime = millis();
        appMode = APP_MENU_MODE;
        // clear forward arrow
        lcd.setCursor(0, 1);
        strbuf[0] = ' '; // clear forward arrow
        strbuf[1] = 0;
        lcd.print(strbuf);
      }
      break;
    }
  }
}