#pragma once

// General Function Libraries - Already installed in Arduino IDE
#include <Arduino.h>
#include <stdint.h>
#include <stdio.h>
//#include <Millis.h>
#include <EEPROM.h>        // For writing values to the EEPROM

#define   MAX_ATTEMPTS          2
#define   MAX_WEATHER_READINGS  5
#define   LED_GPIO              19

//#define   WAKE_UP_MODE        "TIMER"     // If = "TIMER" then it will wake up every TIME_TO_SLEEP seconds
#define   WAKE_UP_MODE        "TOUCH"     // If = "TOUCH" then it will wake up one a touch.

// If we are using SLEEP mode:
#define   TIME_TO_SLEEP      900          // Time (in seconds) between wake ups - every 15mins
#define   uS_TO_S_FACTOR     1000000ULL   /* Conversion factor for micro seconds to seconds */

// If we are using TOUCH WAKE UP mode:
//Setup interrupt on Touch Pad 9 (GPIO32)
#define   WAKE_UP_PIN       T9
#define   THRESHOLD         100        /* Greater the value, more the sensitivity */

//Setup interrupt on GPIO39 to reset and put unit into AP mode (even if it could connect
// GPIO39 = RTC_GPIO03
//#define   CONFIG_WAKE_UP_PIN    GPIO_NUM_39
#define BUTTON_PIN_BITMASK      0x8000000000    // 2^39 in HEX

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
