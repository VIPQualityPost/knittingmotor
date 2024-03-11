#include <EEPROM.h>
#include "Config.h"
#include "LcdKeypad.h"
#include "MenuData.h"
#include "RTTTLTunes.h"
#include "Lang.h"

char NotImp[] = CFG_hold;
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
    /* case mnuCmdResetToDefaults:
      strbuf[0] = 0;
      break;
    case mnuCmdClearBnd:
      strbuf[0] = 0;
      break; */
    case mnuCmdSetRowCount:
      fmt(strbuf, 2, inttostr(intbuf, cfg.rowCount), CFG_rows);
      break;
    case mnuCmdLeftBnd:
      fmt(strbuf, 2, inttostr(intbuf, cfg.leftBoundary), CFG_pos);
      break;
    case mnuCmdRightBnd:
      fmt(strbuf, 2, inttostr(intbuf, cfg.rightBoundary), CFG_pos);
      break;
    case mnuCmdCarSpeed:
      fmt(strbuf, 2, inttostr(intbuf, cfg.carriageSpeed), CFG_rpm);
      break;
    case mnuCmdArrowMode :
      if (cfg.arrowMode)
      {
        fmt(strbuf, 1, CFG_show);
      }
      else
      {
        fmt(strbuf, 1, CFG_hide);
      }
      break;
    case mnuCmdButtonBeep :
      if (cfg.buttonBeep)
      {
        fmt(strbuf, 1, CFG_on);
      }
      else
      {
        fmt(strbuf, 1, CFG_off);
      }
      break;
    case mnuCmdOpMode :
      if (cfg.opMode)
      {
        fmt(strbuf, 1, CFG_auto);
      }
      else
      {
        fmt(strbuf, 1, CFG_manual);
      }
      break;
    case mnuCmdFootMode :
      if (cfg.footMode)
      {
        fmt(strbuf, 1, CFG_single);
      }
      else
      {
        fmt(strbuf, 1, CFG_continuous);
      }
      break;
    case mnuCmdYarnMain :
      if (cfg.yarnsensorMainEnable)
      {
        fmt(strbuf, 1, CFG_on);
      }
      else
      {
        fmt(strbuf, 1, CFG_off);
      }
      break;
    case mnuCmdYarnSec :
      if (cfg.yarnsensorSecEnable)
      {
        fmt(strbuf, 1, CFG_on);
      }
      else
      {
        fmt(strbuf, 1, CFG_off);
      }
      break;
    case mnuCmdOverloadsensor :
      if (cfg.overloadsensorEnable)
      {
        fmt(strbuf, 1, CFG_on);
      }
      else
      {
        fmt(strbuf, 1, CFG_off);
      }
      break;
    case mnuCmdDisplayBrightness :
    {
      byte brightnessPC = ((unsigned short) cfg.displayBrightness * 100) / 3;
      inttostr(intbuf, brightnessPC);
      fmt(strbuf, 2, intbuf, "%");
      break;
    }
    case mnuCmdAlarmTune :
      // fmt(strbuf, 1, (const char *)pgm_read_word(&(Songs[cfg.alarmTune])));
      fmt(strbuf, 1, Songs[cfg.alarmTune]);
      break;
    default:
      NotImp[0] = 0b01111110; // forward arrow representing input prompt.
      strcpy(strbuf, NotImp);
      break;
  }
  return strbuf;
}


//------------------------------------------------------------------------------
void Config::save()
{
  // eeprom_write_block(this, (void *)0, sizeof (Config));
  EEPROM.put(0, this->cfg);
}


//------------------------------------------------------------------------------
void Config::load()
{
  // Attempt to load config from EEPROM
  // eeprom_read_block(this, (void *)0, sizeof (Config));
  EEPROM.get(0, this->cfg);

  if (strcmp(this->cfg.appNameAndVersion, NameAndVersion) != 0)
  {
    setDefaults();
  }
}


//------------------------------------------------------------------------------
void Config::setDefaults()
{
  strcpy(cfg.appNameAndVersion, NameAndVersion);

  cfg.rowCount = 0;
  cfg.leftBoundary = 0;
  cfg.rightBoundary = 0;
  cfg.alarmTune = 4;
  cfg.buttonBeep = true;
  cfg.displayBrightness = 3;
  cfg.yarnsensorMainEnable = false;
  cfg.yarnsensorSecEnable = false;
  cfg.overloadsensorEnable = false;
  cfg.carriageSpeed = 80;
  cfg.opMode = true;
  cfg.arrowMode = true;
}

/*
//------------------------------------------------------------------------------
void Config::copyTo(Config *dest)
{
  memcpy(dest, this, sizeof(Config));
}
*/