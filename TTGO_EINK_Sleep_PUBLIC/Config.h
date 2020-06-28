#pragma once

// General Function Libraries - Already installed in Arduino IDE
#include <Arduino.h>
#include <stdint.h>
#include <Millis.h>
#include <EEPROM.h>        // For writing values to the EEPROM

//Adafruit IO info
//https://io.adafruit.com/api/docs/mqtt.html#mqtt-connection-details
#define AIO_SERVER          "io.adafruit.com" // direct
#define AIO_SERVERPORT      1883  // 8883  // Use 8883 if at all possible!
#define AIO_USERNAME        "***YOUR USERNAME****" // This is your Adafruit IO username
#define AIO_KEY             "***YOUR AIO KEY*****" // This is your Adafruit IO Key

#define TIME_TO_SLEEP      30      // Time (in sceonds) between wake ups
#define uS_TO_S_FACTOR     1000000ULL  /* Conversion factor for micro seconds to seconds */
