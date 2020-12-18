#pragma once

// General Function Libraries - Already installed in Arduino IDE
#include <Arduino.h>
#include <stdint.h>
#include <stdio.h>
#include <Millis.h>
#include <EEPROM.h>        // For writing values to the EEPROM

//Set your location according to COVID API developers Guide
// Need to create this string:
// https://api.coronavirus.data.gov.uk/v1/data?filters=areaName=Nottingham;date=2020-11-20&structure=
// {%22date%22:%22date%22,%22areaName%22:%22areaName%22,%22newCasesByPublishDate%22:%22newCasesByPublishDate%22,
// %22cumCasesByPublishDate%22:%22cumCasesByPublishDate%22,
// %22newDeathsByDeathDate%22:%22newDeathsByDeathDate%22,%22cumDeathsByDeathDate%22:%22cumDeathsByDeathDate%22}

#define   COVID_SERVER      "api.coronavirus.data.gov.uk/v1"
#define   MY_CITY           "Nottingham"

#define   MAX_ATTEMPTS          2
#define   LED_GPIO              19

// If we are using SLEEP mode:
#define   TIME_TO_SLEEP      900          // Time (in seconds) between wake ups - every 15mins
#define   uS_TO_S_FACTOR     1000000ULL   /* Conversion factor for micro seconds to seconds */

// If we are using TOUCH WAKE UP mode:
//Setup interrupt on Touch Pad 9 (GPIO32)
#define   WAKE_UP_PIN       T8        // This is called T8 NOT T9 due to an error in Arduino IDE
#define   THRESHOLD         50        /* Greater the value, more the sensitivity */

typedef struct {
  // For data
  String    date;
  String    areaName;
  String    newCasesByPublishDate;
  String    cumCasesByPublishDate;
  String    newDeathsByDeathDate;
  String    cumDeathsByDeathDate;
} COVID_record_type;
