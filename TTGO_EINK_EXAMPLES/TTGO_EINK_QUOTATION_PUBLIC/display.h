#pragma once

#include <stdio.h>
#include <Arduino.h>
#include "Config.h"


// For Eink Paper - Use gxEPD Library
// include library, include base class, make path known
#include <GxEPD.h>
#include <GxIO/GxIO_SPI/GxIO_SPI.h>
#include <GxIO/GxIO.h>

#include "board_def.h"
#include "CE_Icons.h"        // Curious Electric Icons

#include <Fonts/FreeMono9pt7b.h>
#include <Fonts/FreeMonoBoldOblique9pt7b.h>
#include <Fonts/FreeMonoBold9pt7b.h>
#include <Fonts/FreeMonoOblique9pt7b.h>
#include <Fonts/FreeSans9pt7b.h>
#include <Fonts/FreeSansBold9pt7b.h>
#include <Fonts/FreeSansBoldOblique9pt7b.h>
#include <Fonts/FreeSansOblique9pt7b.h>
#include <Fonts/FreeSerif9pt7b.h>
#include <Fonts/FreeSerifBold9pt7b.h>
#include <Fonts/FreeSerifBoldItalic9pt7b.h>
#include <Fonts/FreeSerifItalic9pt7b.h>

// #define DEFALUT_FONT FreeMono9pt7b
// #define DEFALUT_FONT FreeMonoBoldOblique9pt7b
// #define DEFALUT_FONT FreeMonoBold9pt7b
// #define DEFALUT_FONT FreeMonoOblique9pt7b
#define DEFALUT_FONT FreeSans9pt7b
// #define DEFALUT_FONT FreeSansBold9pt7b
// #define DEFALUT_FONT FreeSansBoldOblique9pt7b
// #define DEFALUT_FONT FreeSansOblique9pt7b
// #define DEFALUT_FONT FreeSerif9pt7b
// #define DEFALUT_FONT FreeSerifBold9pt7b
// #define DEFALUT_FONT FreeSerifBoldItalic9pt7b
// #define DEFALUT_FONT FreeSerifItalic9pt7b

//const GFXfont *fonts[] = {
//  &FreeMono9pt7b,
//  &FreeMonoBoldOblique9pt7b,
//  &FreeMonoBold9pt7b,
//  &FreeMonoOblique9pt7b,
//  &FreeSans9pt7b,
//  &FreeSansBold9pt7b,
//  &FreeSansBoldOblique9pt7b,
//  &FreeSansOblique9pt7b,
//  &FreeSerif9pt7b,
//  &FreeSerifBold9pt7b,
//  &FreeSerifBoldItalic9pt7b,
//  &FreeSerifItalic9pt7b
//};

typedef enum {
  RIGHT_ALIGNMENT = 0,
  LEFT_ALIGNMENT,
  CENTER_ALIGNMENT,
} Text_alignment;


void displayTest();
void displayShowCELogo();
void displayInit();
void displayUpdate();
void displayText(const String &str, int16_t y, uint8_t alignment);
void displayCEIcon(int x, int y, String IconName);
void displaySSID(String DISPLAY_SSID, String DISPLAY_PASS);
void displayUpdatingScreen();
void displayClear();
void displayQuote(String my_quote, String my_author);
