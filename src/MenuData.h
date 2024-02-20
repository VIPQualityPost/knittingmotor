#ifndef _knittingMenu_
#define _knittingMenu_
#include "MenuManager.h"
#include <avr/pgmspace.h>

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
  mnuCmdKnitConti,
  mnuCmdToolsRoot,
  mnuCmdHomeing,
  mnuCmdMoveToStart,
  mnuCmdMoveToEnd,
  mnuCmdSettingsRoot,
  mnuCmdCarSpeed,
  mnuCmdYarnsensor,
  mnuCmdButtonBeep,
  mnuCmdAlarmDuration,
  mnuCmdDisplayBrightness,
  mnuCmdResetToDefaults
};

PROGMEM const char knittingMenu_back[] = "Back";
PROGMEM const char knittingMenu_exit[] = "Exit";

PROGMEM const char knittingMenu_2_1[] = "Left boundary";
PROGMEM const char knittingMenu_2_2[] = "Right boundary";
PROGMEM const char knittingMenu_2_3[] = "Clear boundaries";
PROGMEM const MenuItem knittingMenu_List_2[] = {{mnuCmdLeftBnd, knittingMenu_2_1}, {mnuCmdRightBnd, knittingMenu_2_2}, {mnuCmdClearBnd, knittingMenu_2_3}, {mnuCmdBack, knittingMenu_back}};

PROGMEM const char knittingMenu_4_1[] = "Home carriage";
PROGMEM const char knittingMenu_4_2[] = "Move to start";
PROGMEM const char knittingMenu_4_3[] = "Move to end";
PROGMEM const MenuItem knittingMenu_List_4[] = {{mnuCmdHomeing, knittingMenu_4_1}, {mnuCmdMoveToStart, knittingMenu_4_2}, {mnuCmdMoveToEnd, knittingMenu_4_3}, {mnuCmdBack, knittingMenu_back}};

PROGMEM const char knittingMenu_5_1[] = "Carriage speed";
PROGMEM const char knittingMenu_5_2[] = "Yarn sensor";
PROGMEM const char knittingMenu_5_3[] = "Button Beep";
PROGMEM const char knittingMenu_5_4[] = "Alarm Duration";
PROGMEM const char knittingMenu_5_5[] = "LCD backlight";
PROGMEM const MenuItem knittingMenu_List_5[] = {{mnuCmdCarSpeed, knittingMenu_5_1}, {mnuCmdYarnsensor, knittingMenu_5_2}, {mnuCmdButtonBeep, knittingMenu_5_3}, {mnuCmdAlarmDuration, knittingMenu_5_4}, {mnuCmdDisplayBrightness, knittingMenu_5_5}, {mnuCmdBack, knittingMenu_back}};

PROGMEM const char knittingMenu_1[] = "Set row count";
PROGMEM const char knittingMenu_2[] = "Boundaries";
PROGMEM const char knittingMenu_3[] = "Knit continuous";
PROGMEM const char knittingMenu_4[] = "Tools";
PROGMEM const char knittingMenu_5[] = "Settings";
PROGMEM const char knittingMenu_6[] = "Reset all";
PROGMEM const MenuItem knittingMenu_Root[] = {{mnuCmdSetRowCount, knittingMenu_1}, {mnuCmdBoundariesRoot, knittingMenu_2, knittingMenu_List_2, menuCount(knittingMenu_List_2)}, {mnuCmdKnitConti, knittingMenu_3}, {mnuCmdToolsRoot, knittingMenu_4, knittingMenu_List_4, menuCount(knittingMenu_List_4)}, {mnuCmdSettingsRoot, knittingMenu_5, knittingMenu_List_5, menuCount(knittingMenu_List_5)}, {mnuCmdResetToDefaults, knittingMenu_6}, {mnuCmdBack, knittingMenu_exit}};

/*
case mnuCmdSetRowCount :
	break;
case mnuCmdLeftBnd :
	break;
case mnuCmdRightBnd :
	break;
case mnuCmdClearBnd :
	break;
case mnuCmdKnitConti :
	break;
case mnuCmdHomeing :
	break;
case mnuCmdMoveToStart :
	break;
case mnuCmdMoveToEnd :
	break;
case mnuCmdCarSpeed :
	break;
case mnuCmdYarnsensor :
	break;
case mnuCmdButtonBeep :
	break;
case mnuCmdAlarmDuration :
	break;
case mnuCmdDisplayBrightness :
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
    <Item Id="KnitConti"    Name="Knit continuous"/>
    <Item Id="ToolsRoot"        Name="Tools">
      <MenuItems>
        <Item Id="Homeing"  Name="Home carriage"/>
        <Item Id="MoveToStart"  Name="Move to start"/>
        <Item Id="MoveToEnd"    Name="Move to end"/>
      </MenuItems>
    </Item>
    <Item Id="SettingsRoot" Name="Settings">
      <MenuItems>
        <Item Id="CarSpeed"   Name="Carriage speed"/>
        <Item Id="Yarnsensor" Name="Yarn sensor"/>
        <Item Id="ButtonBeep" Name="Button Beep"/>
        <Item Id="AlarmDuration" Name="Alarm Duration"/>
        <Item Id="DisplayBrightness" Name="LCD backlight"/>
      </MenuItems>
    </Item>
    <Item Id="ResetToDefaults" Name="Reset all"/>
  </MenuItems>
</RootMenu>
*/
#endif
