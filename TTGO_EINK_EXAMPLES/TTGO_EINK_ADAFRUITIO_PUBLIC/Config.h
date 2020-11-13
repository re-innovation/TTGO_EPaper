#pragma once

// General Function Libraries - Already installed in Arduino IDE
#include <Arduino.h>
#include <stdint.h>
#include <stdio.h>
#include <Millis.h>
#include <EEPROM.h>        // For writing values to the EEPROM

//Adafruit IO info
// This needs an account created.
//https://io.adafruit.com/api/docs/mqtt.html#mqtt-connection-details
#define AIO_SERVER          "io.adafruit.com"                   // direct
#define AIO_SERVERPORT      1883                                // 8883  // Use 8883 if at all possible!
#define AIO_USERNAME        "YOUR ADAFRUIT IO USER NAME HERE"                            // This is your Adafruit IO username
#define AIO_KEY             "YOUR ADAFRUIT IO KEY HERE"  // This is your Adafruit IO Key

#define   LED_GPIO              19

// If we are using SLEEP mode:
#define   TIME_TO_SLEEP      7200          // Time (in seconds) between wake ups - every 2 hour
#define   uS_TO_S_FACTOR     1000000ULL   /* Conversion factor for micro seconds to seconds */

// If we are using TOUCH WAKE UP mode:
//Setup interrupt on Touch Pad 9 (GPIO32)
#define   WAKE_UP_PIN       T8        // This is called T8 NOT T9 due to an error in Arduino IDE
#define   THRESHOLD         40        /* Greater the value, more the sensitivity */

#define   RADIATION_LOW_CPM   40      // Below this is LOW
#define   RADIATION_MED_CPM   70      // Below this is MED, above is HIGH
