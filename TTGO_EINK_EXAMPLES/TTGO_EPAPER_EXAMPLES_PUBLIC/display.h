#pragma once

#include <stdio.h>
#include <Arduino.h>
#include "Config.h"

#ifdef WEATHER_EXAMPLE
#include "weather.h"
#endif

// UPDATED 14/4/2026:
// For Eink Paper - Use GxEPD2 Library
// include library, include base class, make path known

// base class GxEPD2_GFX can be used to pass references or pointers to the display instance as parameter, uses ~1.2k more code
// enable or disable GxEPD2_GFX base class
#define ENABLE_GxEPD2_GFX 0

#include <GxEPD2_BW.h>
#include <GxEPD2_3C.h>
#include <GxEPD2_4C.h>
#include <GxEPD2_7C.h>

#include "board_def.h"
#include "CE_Icons.h"  // Curious Electric Icons

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

static bool isInit = false;


void displayTest();
void displayShowCELogo();
void displayInit();
void displayText(const String &str, int16_t y, uint8_t alignment);
void displaySSID(String DISPLAY_SSID, String DISPLAY_PASS);
void displayShutDown();
void displayUpdatingScreen();
void displayClear();
void displayShowError();

#ifdef QUOTATION_EXAMPLE
void displayQuote(String my_quote, String my_author);
#endif

#ifdef WEATHER_EXAMPLE
void displayWeatherIcon(int x, int y, String IconName);
void displayWeatherInfo(Forecast_record_type my_WxConditions[], Forecast_record_type my_WxForecast[]);
#endif

#ifdef GRIDFREQ_EXAMPLE
void drawFreqGraph(int my_data_points[], int my_x, int my_y, float my_recent_f);
void writeDashedHLine(int16_t my_x0, int16_t my_y0, int16_t my_x1, int16_t my_dash_size);
void writeDashedVLine(int16_t my_y0, int16_t my_x0, int16_t my_y1, int16_t my_dash_size);
#endif
