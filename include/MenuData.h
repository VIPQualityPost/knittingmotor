#ifndef _knittingMenu_
#define _knittingMenu_
#include "MenuManager.h"
#include <avr/pgmspace.h>
#include "Lang.h"

/*

Generated using LCD Menu Builder at https://lcd-menu-bulder.cohesivecomputing.co.uk/
For more information, visit https://www.cohesivecomputing.co.uk/hackatronics/arduino-lcd-menu-library/

All our hackatronics projects are free for personal use. If you find them helpful or useful, please consider
making a small donation to our hackatronics fund using the donate buttons on our web pages. Thank you.
		
*/

enum knittingMenuCommandId
{
  mnuCmdBack = 0,
  mnuCmdSetRowCount,
  mnuCmdBoundariesRoot,
  mnuCmdLeftBnd,
  mnuCmdRightBnd,
  mnuCmdClearBnd,
  mnuCmdToolsRoot,
  mnuCmdHomeing,
  mnuCmdMoveToStart,
  mnuCmdMoveToMid,
  mnuCmdMoveToEnd,
  mnuCmdSettingsRoot,
  mnuCmdOpMode,
  mnuCmdFootMode,
  mnuCmdCarSpeed,
  mnuCmdYarnMain,
  mnuCmdYarnSec,
  mnuCmdOverloadsensor,
  mnuCmdArrowMode,
  mnuCmdButtonBeep,
  mnuCmdAlarmTune,
  mnuCmdDisplayBrightness,
  mnuCmdInfo,
  mnuCmdResetToDefaults
};

PROGMEM const char knittingMenu_back[] = MENU_back;
PROGMEM const char knittingMenu_exit[] = MENU_exit;

PROGMEM const char knittingMenu_2_1[] = MENU_2_1;
PROGMEM const char knittingMenu_2_2[] = MENU_2_2;
PROGMEM const char knittingMenu_2_3[] = MENU_2_3;
PROGMEM const MenuItem knittingMenu_List_2[] = {{mnuCmdLeftBnd, knittingMenu_2_1}, {mnuCmdRightBnd, knittingMenu_2_2}, {mnuCmdClearBnd, knittingMenu_2_3}, {mnuCmdBack, knittingMenu_back}};

PROGMEM const char knittingMenu_3_1[] = MENU_3_1;
PROGMEM const char knittingMenu_3_2[] = MENU_3_2;
PROGMEM const char knittingMenu_3_3[] = MENU_3_3;
PROGMEM const char knittingMenu_3_4[] = MENU_3_4;
PROGMEM const MenuItem knittingMenu_List_3[] = {{mnuCmdHomeing, knittingMenu_3_1}, {mnuCmdMoveToStart, knittingMenu_3_2}, {mnuCmdMoveToMid, knittingMenu_3_3}, {mnuCmdMoveToEnd, knittingMenu_3_4}, {mnuCmdBack, knittingMenu_back}};

PROGMEM const char knittingMenu_4_1[] = MENU_4_1;
PROGMEM const char knittingMenu_4_2[] = MENU_4_2;
PROGMEM const char knittingMenu_4_3[] = MENU_4_3;
PROGMEM const char knittingMenu_4_4[] = MENU_4_4;
PROGMEM const char knittingMenu_4_5[] = MENU_4_5;
PROGMEM const char knittingMenu_4_6[] = MENU_4_6;
PROGMEM const char knittingMenu_4_7[] = MENU_4_7;
PROGMEM const char knittingMenu_4_8[] = MENU_4_8;
PROGMEM const char knittingMenu_4_9[] = MENU_4_9;
PROGMEM const char knittingMenu_4_10[] = MENU_4_10;
PROGMEM const MenuItem knittingMenu_List_4[] = {{mnuCmdOpMode, knittingMenu_4_1}, {mnuCmdFootMode, knittingMenu_4_2}, {mnuCmdCarSpeed, knittingMenu_4_3}, {mnuCmdYarnMain, knittingMenu_4_4}, {mnuCmdYarnSec, knittingMenu_4_5}, {mnuCmdOverloadsensor, knittingMenu_4_6}, {mnuCmdArrowMode, knittingMenu_4_7}, {mnuCmdButtonBeep, knittingMenu_4_8}, {mnuCmdAlarmTune, knittingMenu_4_9}, {mnuCmdDisplayBrightness, knittingMenu_4_10}, {mnuCmdBack, knittingMenu_back}};

PROGMEM const char knittingMenu_1[] = MENU_1;
PROGMEM const char knittingMenu_2[] = MENU_2;
PROGMEM const char knittingMenu_3[] = MENU_3;
PROGMEM const char knittingMenu_4[] = MENU_4;
PROGMEM const char knittingMenu_5[] = MENU_5;
PROGMEM const char knittingMenu_6[] = MENU_6;
PROGMEM const MenuItem knittingMenu_Root[] = {{mnuCmdSetRowCount, knittingMenu_1}, {mnuCmdBoundariesRoot, knittingMenu_2, knittingMenu_List_2, menuCount(knittingMenu_List_2)}, {mnuCmdToolsRoot, knittingMenu_3, knittingMenu_List_3, menuCount(knittingMenu_List_3)}, {mnuCmdSettingsRoot, knittingMenu_4, knittingMenu_List_4, menuCount(knittingMenu_List_4)}, {mnuCmdInfo, knittingMenu_5}, {mnuCmdResetToDefaults, knittingMenu_6}, {mnuCmdBack, knittingMenu_exit}};

/*
case mnuCmdSetRowCount :
	break;
case mnuCmdLeftBnd :
	break;
case mnuCmdRightBnd :
	break;
case mnuCmdClearBnd :
	break;
case mnuCmdHomeing :
	break;
case mnuCmdMoveToStart :
	break;
case mnuCmdMoveToMid :
	break;
case mnuCmdMoveToEnd :
	break;
case mnuCmdOpMode :
	break;
case mnuCmdFootMode :
	break;
case mnuCmdCarSpeed :
	break;
case mnuCmdYarnMain :
	break;
case mnuCmdYarnSec :
	break;
case mnuCmdOverloadsensor :
	break;
case mnuCmdArrowMode :
	break;
case mnuCmdButtonBeep :
	break;
case mnuCmdAlarmTune :
	break;
case mnuCmdDisplayBrightness :
	break;
case mnuCmdInfo :
	break;
case mnuCmdResetToDefaults :
	break;
*/

/*
<?xml version="1.0"?>
<RootMenu xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xmlns:xsd="http://www.w3.org/2001/XMLSchema">
  <Config IdPrefix="mnuCmd" VarPrefix="knittingMenu" UseNumbering="false" IncludeNumberHierarchy="false"
          MaxNameLen="16" MenuBackFirstItem="false" BackText="Back" ExitText="Exit" AvrProgMem="true"/>
  <MenuItems>
    <Item Id="SetRowCount"  Name="Set row count"/>
    <Item Id="BoundariesRoot" Name="Boundaries">
      <MenuItems>
        <Item Id="LeftBnd"  Name="Left boundary"/>
        <Item Id="RightBnd"  Name="Right boundary"/>
        <Item Id="ClearBnd"  Name="Clear boundaries"/>
      </MenuItems>
    </Item>
    <Item Id="ToolsRoot"        Name="Tools">
      <MenuItems>
        <Item Id="Homeing"  Name="Home carriage"/>
        <Item Id="MoveToStart"  Name="Move to start"/>
        <Item Id="MoveToMid"  Name="Move to middle"/>
        <Item Id="MoveToEnd"    Name="Move to end"/>
      </MenuItems>
    </Item>
    <Item Id="SettingsRoot" Name="Settings">
      <MenuItems>
        <Item Id="OpMode"   Name="Operation mode"/>
        <Item Id="FootMode"   Name="Foot pedal mode"/>
        <Item Id="CarSpeed"   Name="Carriage speed"/>
        <Item Id="YarnMain" Name="Yarn sensor main"/>
        <Item Id="YarnSec"  Name="Yarn sensor sec"/>
        <Item Id="Overloadsensor" Name="Overload sensor"/>
        <Item Id="ArrowMode" Name="Direction arrow"/>
        <Item Id="ButtonBeep" Name="Button Beep"/>
        <Item Id="AlarmTune" Name="Alarm tune"/>
        <Item Id="DisplayBrightness" Name="LCD backlight"/>
      </MenuItems>
    </Item>
    <Item Id="Info" Name="Info"/>
    <Item Id="ResetToDefaults" Name="Reset all"/>
  </MenuItems>
</RootMenu>
*/
#endif
