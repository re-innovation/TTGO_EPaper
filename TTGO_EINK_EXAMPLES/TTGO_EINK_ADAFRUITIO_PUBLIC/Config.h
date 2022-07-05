#pragma once

// General Function Libraries - Already installed in Arduino IDE
#include <Arduino.h>
#include <stdint.h>
#include <stdio.h>
#include <EEPROM.h>        // For writing values to the EEPROM

//Adafruit IO info
// This needs an account created.
//https://io.adafruit.com/api/docs/mqtt.html#mqtt-connection-details

#define AIO_SERVER          "io.adafruit.com"                   // direct
#define AIO_SERVERPORT      1883                                // 8883  // Use 8883 if at all possible!
#define AIO_USERNAME        "YOUR ADAFRUIT IO USER NAME HERE"   // This is your Adafruit IO username
#define AIO_KEY             "YOUR ADAFRUIT IO KEY HERE"         // This is your Adafruit IO Key
#define AIO_FEED            "TOUR FEED HERE"                    //  eg: "/feeds/airradiation"

#define   LED_GPIO              19

#define   WAKE_UP_MODE        "TIMER"     // If = "TIMER" then it will wake up every TIME_TO_SLEEP seconds
//#define   WAKE_UP_MODE        "TOUCH"     // If = "TOUCH" then it will wake up one a touch.

// If we are using SLEEP mode:
#define   TIME_TO_SLEEP      7200          // Time (in seconds) between wake ups - every 2 hour
#define   uS_TO_S_FACTOR     1000000ULL   /* Conversion factor for micro seconds to seconds */

// If we are using TOUCH WAKE UP mode:
//Setup interrupt on Touch Pad 9 (GPIO32)
#define   WAKE_UP_PIN       T9
#define   THRESHOLD         40        /* Greater the value, more the sensitivity */

//Setup interrupt on GPIO39 to reset and put unit into AP mode (even if it could connect
// GPIO39 = RTC_GPIO03
//#define   CONFIG_WAKE_UP_PIN    GPIO_NUM_39
#define BUTTON_PIN_BITMASK      0x8000000000    // 2^39 in HEX

#define   RADIATION_LOW_CPM   40      // Below this is LOW
#define   RADIATION_MED_CPM   70      // Below this is MED, above is HIGH
