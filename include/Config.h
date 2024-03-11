#ifndef CONFIG_H_
#define CONFIG_H_

#include <Arduino.h>


const char NameAndVersion[] = "Knitting Motor Configuration V1.7";

extern long addToVal(short delta, long timeval, long minval = 1, long maxval = 1);

struct cfgparams {
  char appNameAndVersion[sizeof(NameAndVersion)];
  unsigned int rowCount;        // setup row counts
  unsigned int leftBoundary;    // knitting boundaries
  unsigned int rightBoundary;   // knitting boundaries
  byte alarmTune;               // sing no.
  byte buttonBeep;              // true/false
  byte displayBrightness;       // 1=33%, 2=66%, 3=100%
  byte yarnsensorMainEnable;    // true/false
  byte yarnsensorSecEnable;     // true/false
  byte overloadsensorEnable;    // true/false
  byte opMode;                  // operation mode true=row count down (auto), false=row count up (manual)
  byte footMode;                // foot pedal mode: single / continuous
  byte arrowMode;               // show arrow after knitting true/false
  unsigned int carriageSpeed;   // see myStepper.setSpeed( 800 );  // 80 Rev/Min ( if stepsPerRev is set correctly )
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
    
    char *getFormattedStr(byte cmdId);  // Returns formatted config value associated with menu command id.
    void save();                        // Saves config to EEPROM.
    void load();                        // Loads config from EEPROM.
    void setDefaults();                 // Sets config to default values.
    // void copyTo(Config *dest);          // Copies current instance to destination instance.
};

#endif