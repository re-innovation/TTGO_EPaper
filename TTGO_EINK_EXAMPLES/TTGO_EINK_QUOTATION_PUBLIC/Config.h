#pragma once

// General Function Libraries - Already installed in Arduino IDE
#include <Arduino.h>
#include <stdint.h>
#include <stdio.h>
#include <Millis.h>
#include <EEPROM.h>        // For writing values to the EEPROM

// The information for the quotation connection:
// From: https://forismatic.com/en/api/
// Sample URL is: http://api.forismatic.com/api/1.0/?method=getQuote&key=457653&format=text&lang=en

#define   QUOTE_SERVER    "http://api.forismatic.com/api/1.0/"    // This supplies the random quote
#define   QUOTE_SEED      ""                                      // This is a seed (6 digit) to help specify the quote - leave blank for totally random
#define   QUOTE_LANGUAGE  "en"                                    // The language for the unit - at present "en" or "ru". russian does not display at the moment...

#define   LED_GPIO              19

// If we are using SLEEP mode:
#define   TIME_TO_SLEEP      900          // Time (in seconds) between wake ups - every 15mins
#define   uS_TO_S_FACTOR     1000000ULL   /* Conversion factor for micro seconds to seconds */

// If we are using TOUCH WAKE UP mode:
//Setup interrupt on Touch Pad 9 (GPIO32)
#define   WAKE_UP_PIN       T8        // This is called T8 NOT T9 due to an error in Arduino IDE
#define   THRESHOLD         40        /* Greater the value, more the sensitivity */
