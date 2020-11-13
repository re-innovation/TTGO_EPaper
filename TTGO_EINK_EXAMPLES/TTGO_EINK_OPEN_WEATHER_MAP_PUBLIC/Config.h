#pragma once

// General Function Libraries - Already installed in Arduino IDE
#include <Arduino.h>
#include <stdint.h>
#include <stdio.h>
#include <Millis.h>
#include <EEPROM.h>        // For writing values to the EEPROM


// Use your own API key by signing up for a free developer account at https://openweathermap.org/
// This needs an account created.
#define   OWM_KEY       "YOUR OPEN WEATHER API KEY HERE"                  // See: https://openweathermap.org/
#define   OWM_SERVER    "api.openweathermap.org"

//Set your location according to Open Weather Map locations
#define   MY_CITY        "Nottingham"                   // Your home city See: http://bulk.openweathermap.org/sample/
#define   MY_COUNTRY     "GB"                           // Your country
#define   MY_LANGUAGE    "EN"                            // NOTE: Only the weather description (not used) is translated by OWM
// Arabic (AR) Czech (CZ) English (EN) Greek (EL) Persian(Farsi) (FA) Galician (GL) Hungarian (HU) Japanese (JA)
// Korean (KR) Latvian (LA) Lithuanian (LT) Macedonian (MK) Slovak (SK) Slovenian (SL) Vietnamese (VI)
#define   MY_HEMISPHERE  "north"                         // or "south"
#define   MY_UNITS       "M"                             // Use "M" for Metric or I for Imperial
#define   MY_TIMEZONE    "GMT0BST,M3.5.0/01,M10.5.0/02"  // Choose your time zone from: https://github.com/nayarsystems/posix_tz_db/blob/master/zones.csv

// Example time zones
//#define   MY_TIMEZONE = "MET-1METDST,M3.5.0/01,M10.5.0/02"; // Most of Europe
//#define   MY_TIMEZONE = "CET-1CEST,M3.5.0,M10.5.0/3";       // Central Europe
//#define   MY_TIMEZONE = "EST-1METDST,M3.5.0/01,M10.5.0/02"; // Most of Europe
//#define   MY_TIMEZONE = "EST5EDT,M3.2.0,M11.1.0";           // EST USA
//#define   MY_TIMEZONE = "CST6CDT,M3.2.0,M11.1.0";           // CST USA
//#define   MY_TIMEZONE = "MST7MDT,M4.1.0,M10.5.0";           // MST USA
//#define   MY_TIMEZONE = "NZST-12NZDT,M9.5.0,M4.1.0/3";      // Auckland
//#define   MY_TIMEZONE = "EET-2EEST,M3.5.5/0,M10.5.5/0";     // Asia
//#define   MY_TIMEZONE = "ACST-9:30ACDT,M10.1.0,M4.1.0/3":   // Australia

#define   MAX_ATTEMPTS          2
#define   MAX_WEATHER_READINGS  5
#define   LED_GPIO              19

// If we are using SLEEP mode:
#define   TIME_TO_SLEEP      900          // Time (in seconds) between wake ups - every 15mins
#define   uS_TO_S_FACTOR     1000000ULL   /* Conversion factor for micro seconds to seconds */

// If we are using TOUCH WAKE UP mode:
//Setup interrupt on Touch Pad 9 (GPIO32)
#define   WAKE_UP_PIN       T8        // This is called T8 NOT T9 due to an error in Arduino IDE
#define   THRESHOLD         40        /* Greater the value, more the sensitivity */

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
