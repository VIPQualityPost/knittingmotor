/*!`
 * \file Lang.h
 *
 * This file is part of Open Knitting Motor Project OKMP.
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
 *    Original Work Copyright 2024 Holger Pandel
 *    http://pages.open-mind.space/knittingmotor
 */

#ifndef LANG_H_
#define LANG_H_

/*
  Adding a new language:

  To add a new language block, create new lines between the marked two comments at the end of the file:

  1) Start with a new #elif line to give your language block a name
  2) Then, copy'n'paste a whole language set from an already existing language after this first line
  3) Begin translating, but keep in mind that you ABSOLUTELY HAVE TO use text that is no more than 16 single
     characters. See `LCDeypadCharacters.gif`in the `ressource` folder for supported western letters and symbols.

     If you are using an eastern version of the LCD keypad, you have to adjust and test yourself.

  If you ready, it will look like this (but with your translation of course!):

      ///////////////////////////////////////
      // ADD NEW LANGUAGE BLOCK FROM HERE
      #elif defined LANG_MYNEWLANGUAGE

      //                        |1-------------16|
      #define CFG_hold        " SELECT halten"
      #define CFG_rows       " Reihe(n)"
      #define CFG_pos        " pos"
      #define CFG_rpm        " UPM"
      #define CFG_show        "zeigen"
      #define CFG_hide        "verstecken"
      ....

      // KEEP EVERYTHING AFTER THIS LINE
      //////////////////////////////////////

*/


#if defined LANG_EN
//                        |1-------------16|
#define CFG_hold         " Hold SELECT"
#define CFG_rows         " row(s)"
#define CFG_pos          " pos"
#define CFG_rpm          " rpm"
#define CFG_show         "show"
#define CFG_hide         "hide"
#define CFG_on           "on"
#define CFG_off          "off"
#define CFG_max          "maximum"
#define CFG_boundary     "boundary"
#define CFG_auto         "auto"
#define CFG_manual       "manual"
#define CFG_single       "single"
#define CFG_continuous   "continuous"
#define MENU_back        "Back"
#define MENU_exit        "Exit"
#define MENU_2_1         "Left boundary"
#define MENU_2_2         "Right boundary"
#define MENU_2_3         "Clear boundaries"
#define MENU_3_1         "Home carriage"
#define MENU_3_2         "Move to start"
#define MENU_3_3         "Move to middle"
#define MENU_3_4         "Move to end"
#define MENU_4_1         "Operation mode"
#define MENU_4_2         "Foot pedal mode"
#define MENU_4_3         "Navigation mode"
#define MENU_4_4         "Carriage speed"
#define MENU_4_5         "Yarn sensor main"
#define MENU_4_6         "Yarn sensor sec"
#define MENU_4_7         "Overload sensor"
#define MENU_4_8         "Direction arrow"
#define MENU_4_9         "Button Beep"
#define MENU_4_10        "Alarm tune"
#define MENU_4_11        "LCD backlight"
#define MENU_1           "Set row count"
#define MENU_2           "Boundaries"
#define MENU_3           "Tools"
#define MENU_4           "Settings"
#define MENU_5           "Info"
#define MENU_6           "Reset all"
#define MAIN_rtk         "Rows to knit:"
#define MAIN_rk          "Rows knitted:"
#define MAIN_done        "Done."
#define MAIN_row         " row"
#define MAIN_rows        " rows"
#define MAIN_boundaries  "Boundaries"
#define MAIN_cleared     "cleared."
#define MAIN_defload     "Defaults loaded"
#define MAIN_home1       "Searching for"
#define MAIN_home2       "home position..."
#define MAIN_home3       "...then going to"
#define MAIN_home4       "the maximum..."
#define MAIN_home5       "...and now back"
#define MAIN_home6       "home in: 3"
#define MAIN_home7       "home in: 2"
#define MAIN_home8       "home in: 1"
#define MAIN_home9       "Please wait..."
#define MAIN_start1      "Press SELECT to"
#define MAIN_start2      "start homeing"
#define MAIN_err1        "HIT 1 - MAX"
#define MAIN_err2        "HIT 2 - HOME"
#define MAIN_err3        "YARN 3 - MAIN"
#define MAIN_err4        "YARN 4 - SEC"
#define MAIN_err5        "ERR 1 - FOOT"
#define MAIN_err6        "ERR 2 - OVRL"
#define MAIN_err_sel     "SELECT TO RESUME"
#define MAIN_rowcount1   "Please set row"
#define MAIN_rowcount2   "count to knit."
#define MAIN_goingto     "Going to"
#define MAIN_zero        "home position..."
#define MAIN_max         "max position..."
#define MAIN_lbnd        "left bound..."
#define MAIN_rbnd        "right bound..."
#define MAIN_stopknit1   "Stop knitting"
#define MAIN_stopknit2   "after row..."

// Umlaite in Oktal
// \341  ä
// \357  ö
// \365  ü
// \342  ß
#elif defined LANG_DE

//                        |1-------------16|
#define CFG_hold        " SELECT halten"
#define CFG_rows       " Reihe(n)"
#define CFG_pos        " pos"
#define CFG_rpm        " UPM"
#define CFG_show        "zeigen"
#define CFG_hide        "verstecken"
#define CFG_on          "an"
#define CFG_off         "aus"
#define CFG_max          "Maximum"
#define CFG_boundary     "Grenze"
#define CFG_auto        "auto"
#define CFG_manual      "manuell"
#define CFG_single      "einzel"
#define CFG_continuous  "dauernd"
#define MENU_back       "Zur\365ck"
#define MENU_exit       "Ende"
#define MENU_2_1        "Grenze links"
#define MENU_2_2        "Grenze rechts"
#define MENU_2_3        "Grenzen l\357schen"
#define MENU_3_1        "Bett einmessen"
#define MENU_3_2        "Zum Start"
#define MENU_3_3        "Zur Mitte"
#define MENU_3_4        "Zum Ende"
#define MENU_4_1        "Strickmodus"
#define MENU_4_2        "Fu\342schalter"
#define MENU_4_3        "Steuerung"
#define MENU_4_4        "Wagentempo"
#define MENU_4_5        "Garnsensor Haupt"
#define MENU_4_6        "Garnsensor Zwei"
#define MENU_4_7        "Lastsensor"
#define MENU_4_8        "Richtungspfeil"
#define MENU_4_9        "Tastenton"
#define MENU_4_10       "Alarmsong"
#define MENU_4_11       "LCD Beleuchtung"
#define MENU_1          "Setze Reihenzahl"
#define MENU_2          "Grenzen"
#define MENU_3          "Werkzeug"
#define MENU_4          "Einstellungen"
#define MENU_5          "Info"
#define MENU_6          "Werkseinstellung"
#define MAIN_rtk        "Zu stricken:"
#define MAIN_rk         "Gestrickt:"
#define MAIN_done       "Fertig."
#define MAIN_row        " Reihe"
#define MAIN_rows       " Reihen"
#define MAIN_boundaries "Grenzen"
#define MAIN_cleared    "gel\357scht."
#define MAIN_defload    "Geladen"
#define MAIN_home1      "Suche nach"
#define MAIN_home2      "Anfangsposition"
#define MAIN_home3      "...jetzt zum"
#define MAIN_home4      "Endpunkt..."
#define MAIN_home5      "...nun zur\365ck"
#define MAIN_home6      "Start in: 3"
#define MAIN_home7      "Start in: 2"
#define MAIN_home8      "Start in: 1"
#define MAIN_home9      "Bitte warten..."
#define MAIN_start1     "SELECT dr\365cken"
#define MAIN_start2     "f\365r Einmessung"
#define MAIN_err1       "HIT 1 - Ende"
#define MAIN_err2       "HIT 2 - Start"
#define MAIN_err3       "GARN 3 - Haupt"
#define MAIN_err4       "GARN 4 - Zwei"
#define MAIN_err5       "FEH 1 - Fu\342"
#define MAIN_err6       "FEH 2 - Last"
#define MAIN_err_sel    "SELECT = weiter"
#define MAIN_rowcount1  "Bitte Reihenzahl"
#define MAIN_rowcount2  "setzen."
#define MAIN_goingto    "Gehe zu"
#define MAIN_zero       "Startposition..."
#define MAIN_max        "Endposition..."
#define MAIN_lbnd       "Grenze links..."
#define MAIN_rbnd       "Grenze rechts..."
#define MAIN_stopknit1  "Stoppe Wagen"
#define MAIN_stopknit2  "nach Reihe..."

//////////////////////////////////////
// ADD NEW LANGUAGE BLOCK FROM HERE

// KEEP EVERYTHING AFTER THIS LINE
//////////////////////////////////////

#else
#error No language defined!
#endif

#endif