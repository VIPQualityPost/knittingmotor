#include "Config.h"
#include "LcdKeypad.h"
#include "MenuData.h"
#include <avr/eeprom.h>

const char NotImp[] = " right=>execute";
static char strbuf[LCD_COLS+1];


//------------------------------------------------------------------------------
long addToVal(short delta, long currval, long minval, long maxval)
{
  long newval = currval+delta;

  if (newval >= minval && newval <= maxval) return newval;

  return currval;
}

//------------------------------------------------------------------------------
char *Config::getFormattedStr(byte cmdId)
{
  char intbuf[8];
  
  switch (cmdId)
  {
    case mnuCmdResetToDefaults:
      strbuf[0] = 0;
      break;
    case mnuCmdSetRowCount:
      fmt(strbuf, 2, inttostr(intbuf, rowCount), " row(s)");
      break;
    case mnuCmdLeftBnd:
      fmt(strbuf, 2, inttostr(intbuf, leftBoundary), " pos");
      break;
    case mnuCmdRightBnd:
      fmt(strbuf, 2, inttostr(intbuf, rightBoundary), " pos");
      break;
    case mnuCmdAlarmDuration:
      fmt(strbuf, 2, inttostr(intbuf, alarmDuration), " seconds");
      break;
    case mnuCmdCarSpeed:
      fmt(strbuf, 2, inttostr(intbuf, carriageSpeed/10), " rpm/sec");
      break;
    case mnuCmdButtonBeep :
      if (buttonBeep)
      {
        fmt(strbuf, 1, "on");
      }
      else
      {
        fmt(strbuf, 1, "off");
      }
      break;
    case mnuCmdYarnsensor :
      if (yarnsensorEnable)
      {
        fmt(strbuf, 1, "on");
      }
      else
      {
        fmt(strbuf, 1, "off");
      }
      break;
    case mnuCmdDisplayBrightness :
    {
      byte brightnessPC = ((unsigned short) displayBrightness * 100) / 3;
      inttostr(intbuf, brightnessPC);
      fmt(strbuf, 2, intbuf, "%");
      break;
    }
    default:
      strcpy(strbuf, NotImp);
      break;
  }
  return strbuf;
}


//------------------------------------------------------------------------------
void Config::save()
{
  eeprom_write_block(this, (void *)0, sizeof (Config));
}


//------------------------------------------------------------------------------
void Config::load()
{
  // Attempt to load config from EEPROM
  eeprom_read_block(this, (void *)0, sizeof (Config));

  if (strcmp(this->appNameAndVersion, NameAndVersion) != 0)
  {
    setDefaults();
  }
}


//------------------------------------------------------------------------------
void Config::setDefaults()
{
  strcpy(appNameAndVersion, NameAndVersion);

  rowCount = 0;
  leftBoundary = 0;
  rightBoundary = 0;
  alarmDuration = 2;
  buttonBeep = true;
  displayBrightness = 3;
  yarnsensorEnable = true;
  carriageSpeed = 600;
}


//------------------------------------------------------------------------------
void Config::copyTo(Config *dest)
{
  memcpy(dest, this, sizeof(Config));
}
