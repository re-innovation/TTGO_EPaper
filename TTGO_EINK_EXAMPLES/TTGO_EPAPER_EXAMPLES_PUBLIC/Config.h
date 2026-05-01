#pragma once

// General Function Libraries - Already installed in Arduino IDE
#include <Arduino.h>
#include <stdint.h>
#include <stdio.h>
#include <EEPROM.h>        // For writing values to the EEPROM

//#define QUOTATION_EXAMPLE   // This is for the Quotation example code
//#define WEATHER_EXAMPLE   // This is for the Quotation example code
#define GRIDFREQ_EXAMPLE   // This is for the Quotation example code

#ifdef QUOTATION_EXAMPLE
// The information for the quotation connection:
// From: https://forismatic.com/en/api/
// Sample URL is: http://api.forismatic.com/api/1.0/?method=getQuote&key=457653&format=text&lang=en
#define   QUOTE_SERVER    "http://api.forismatic.com/api/1.0/"    // This supplies the random quote
#define   QUOTE_SEED      ""                                      // This is a seed (6 digit) to help specify the quote - leave blank for totally random
#define   QUOTE_LANGUAGE  "en"                                    // The language for the unit - at present "en" or "ru". russian does not display at the moment...
#endif

#ifdef WEATHER_EXAMPLE
#define   MAX_ATTEMPTS          2
#define   MAX_WEATHER_READINGS  5
typedef struct {
  // For current Day and Day 1, 2, 3, etc
  float    lat;
  float    lon;
  String   Dt;
  String   Period;
  float    Temperature;
  float    Humidity;
  String   Icon;
  float    High;
  float    Low;
  float    Rainfall;
  float    Snowfall;
  float    Pressure;
  int      Cloudcover;
  int      Visibility;
  String   Trend;
  float    Winddir;
  float    Windspeed;
  String   Main0;
  String   Forecast0;
  String   Forecast1;
  String   Forecast2;
  String   Description;
  String   Time;
  int      Sunrise;
  int      Sunset;
  String   Country;
  int      TimeUTC;
  int      TimeZone;
} Forecast_record_type;
#endif

#ifdef GRIDFREQ_EXAMPLE 
// We want to use GMT for the Elexon data, so do not need the daylight savings offset
#define ntpServer           "pool.ntp.org"
#define gmtOffset_sec       0 
#define daylightOffset_sec  0

//#define GRIDFREQ_SERVER "https://data.elexon.co.uk/bmrs/api/v1/system/frequency"
//#define GRIDFREQ_SERVER "https://data.elexon.co.uk/bmrs/api/v1/system/frequency?from=2026-04-16T13%3A00%3A00Z&to=2026-04-16T14%3A00%3A00Z&format=csv"
#define GRIDFREQ_SERVER "https://data.elexon.co.uk/bmrs/api/v1/system/frequency?from="
#define GREDFREQ_FORMAT "&format=json"

#define GRAPH_SCALE       300.0   // 0.1 = 30 pixels. Grid freq should be kept within 0.2Hz so display from 49.8 to 50.2. Convert 0.1 -> 30 pixels

#endif


// Need to define these for correct aligment of text
// #define displayWidth        250
// #define displayHeight       122
#define displayWidth        250
#define displayHeight       122


#define   LED_GPIO              19
//#define   WAKE_UP_MODE        "TIMER"     // If = "TIMER" then it will wake up every TIME_TO_SLEEP seconds
#define   WAKE_UP_MODE        "TOUCH"     // If = "TOUCH" then it will wake up one a touch.

// If we are using TIMER mode:
#define   TIME_TO_SLEEP      900          // Time (in seconds) between wake ups - every 15mins
#define   uS_TO_S_FACTOR     1000000ULL   /* Conversion factor for micro seconds to seconds */

// If we are using TOUCH WAKE UP mode:
//Setup interrupt on Touch Pad 9 (GPIO32)
#define   WAKE_UP_PIN       T9        
#define   THRESHOLD         50        /* Greater the value, more the sensitivity */

//Setup interrupt on GPIO39 to reset and put unit into AP mode (even if it could connect
// GPIO39 = RTC_GPIO03
//#define   CONFIG_WAKE_UP_PIN    GPIO_NUM_39
#define BUTTON_PIN_BITMASK      0x8000000000    // 2^39 in HEX
