#ifndef CONFIG_H_
#define CONFIG_H_

#include "Arduino.h"

const char NameAndVersion[] = "Knitting Motor Configuration V0.7";

extern long addToVal(short delta, long timeval, long minval = 1, long maxval = 1);

class Config
{
  public:
    /// <configValues>
    
    /// If adding/removing/changing config items,
    /// then increment version in NameAndVersion string above.
    /// Must also modify body of setDefaults() and getFormattedStr() methods of this class.
    /// It may also be necessary to modify processMenuCommand() method in LcdCountDownTimer file.
    
    char appNameAndVersion[sizeof(NameAndVersion)];

    /// Last setup row counts
    unsigned int rowCount;

    /// knitting boundaries
    unsigned int leftBoundary;
    unsigned int rightBoundary;
    
    char alarmDuration;         // in seconds
    byte buttonBeep;            // true/false
    byte displayBrightness;     // 1=33%, 2=66%, 3=100%
    byte yarnsensorEnable;      // true/false
    byte opMode;                // operation mode true=row count down (auto), false=row count up (manual)
    byte footMode;              // foot pedal mode: single / continuous
    byte arrowMode;             // show arrow after knitting true/false
    unsigned int carriageSpeed; // see myStepper.setSpeed( 800 );  // 80 Rev/Min ( if stepsPerRev is set correctly )

    /// </configValues>
    
    char *getFormattedStr(byte cmdId);  // Returns formatted config value associated with menu command id.
    void save();                        // Saves config to EEPROM.
    void load();                        // Loads config from EEPROM.
    void setDefaults();                 // Sets config to default values.
    void copyTo(Config *dest);          // Copies current instance to destination instance.
};

#endif