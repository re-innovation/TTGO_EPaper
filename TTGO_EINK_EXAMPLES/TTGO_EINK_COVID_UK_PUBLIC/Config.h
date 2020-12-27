#pragma once

// General Function Libraries - Already installed in Arduino IDE
#include <Arduino.h>
#include <stdint.h>
#include <stdio.h>
#include <EEPROM.h>        // For writing values to the EEPROM

//Set your location according to COVID API developers Guide: https://coronavirus.data.gov.uk/details/developers-guide
// Need to create this string:
// https://api.coronavirus.data.gov.uk/v1/data?filters=areaType=nation;areaName=england;date=2020-12-20&structure
// ={%22date%22:%22date%22,%22name%22:%22areaName%22,%22code%22:%22areaCode%22,%22cases%22:{%22daily%22:%22newCases
// ByPublishDate%22,%22cumulative%22:%22cumCasesByPublishDate%22},%22deaths%22:{%22daily%22:%22newDeathsByDeathDate%22,
// %22cumulative%22:%22cumDeathsByDeathDate%22}}
#define   COVID_SERVER      "api.coronavirus.data.gov.uk/v1"
#define   MY_CITY           "Nottingham"
#define   MY_COUNTRY        "england"

#define   MAX_ATTEMPTS          2
#define   LED_GPIO              19

// If we are using SLEEP mode:
#define   TIME_TO_SLEEP      900          // Time (in seconds) between wake ups - every 15mins
#define   uS_TO_S_FACTOR     1000000ULL   /* Conversion factor for micro seconds to seconds */

// If we are using TOUCH WAKE UP mode:
//Setup interrupt on Touch Pad 9 (GPIO32)
#define   WAKE_UP_PIN       T8        // This is called T8 NOT T9 due to an error in Arduino IDE
#define   THRESHOLD         50        /* Greater the value, more the sensitivity */

#define   DATA_STORED       30

#define   DEBUG_FLAG        true

typedef struct {
  // For data
  String    date;
  String    areaName;
  int       newCases;
  int       totalCases;
  int       rateCases;
  int       newDeaths;
  int       totalDeaths;
  int       rateDeaths;
} COVID_record_type;
