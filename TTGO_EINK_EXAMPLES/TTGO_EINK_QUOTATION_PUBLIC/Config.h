#pragma once

// General Function Libraries - Already installed in Arduino IDE
#include <Arduino.h>
#include <stdint.h>
#include <stdio.h>
#include <EEPROM.h>        // For writing values to the EEPROM

// The information for the quotation connection:
// From: https://forismatic.com/en/api/
// Sample URL is: http://api.forismatic.com/api/1.0/?method=getQuote&key=457653&format=text&lang=en

#define   QUOTE_SERVER    "http://api.forismatic.com/api/1.0/"    // This supplies the random quote
#define   QUOTE_SEED      ""                                      // This is a seed (6 digit) to help specify the quote - leave blank for totally random
#define   QUOTE_LANGUAGE  "en"                                    // The language for the unit - at present "en" or "ru". russian does not display at the moment...

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

#define EP_OFFSET           6   // Slight change in display unit - different 0?
//#define EP_OFFSET           0   // Original version.
