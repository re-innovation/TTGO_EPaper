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
#define AIO_USERNAME        "***YOUR AIO USER KERE ***" // This is your Adafruit IO username
#define AIO_KEY             "***YOUR AIO KEY KERE ***" // This is your Adafruit IO Key

// Use your own API key by signing up for a free developer account at https://openweathermap.org/
String OWM_KEY              = "*** YOUR OPEN WEATHER MAP KEY KERE ***";                   // See: https://openweathermap.org/
const char OWM_SERVER[]     = "api.openweathermap.org";

//Set your location according to OWM locations
String City          = "Nottingham";                    // Your home city See: http://bulk.openweathermap.org/sample/
String Country       = "GB";                            // Your country  
String Language      = "EN";                            // NOTE: Only the weather description (not used) is translated by OWM
                                                        // Arabic (AR) Czech (CZ) English (EN) Greek (EL) Persian(Farsi) (FA) Galician (GL) Hungarian (HU) Japanese (JA)
                                                        // Korean (KR) Latvian (LA) Lithuanian (LT) Macedonian (MK) Slovak (SK) Slovenian (SL) Vietnamese (VI)
String Hemisphere    = "north";                         // or "south"  
String Units         = "M";                             // Use 'M' for Metric or I for Imperial 
const char* Timezone = "GMT0BST,M3.5.0/01,M10.5.0/02";  // Choose your time zone from: https://github.com/nayarsystems/posix_tz_db/blob/master/zones.csv 

// Example time zones
//const char* Timezone = "MET-1METDST,M3.5.0/01,M10.5.0/02"; // Most of Europe
//const char* Timezone = "CET-1CEST,M3.5.0,M10.5.0/3";       // Central Europe
//const char* Timezone = "EST-1METDST,M3.5.0/01,M10.5.0/02"; // Most of Europe
//const char* Timezone = "EST5EDT,M3.2.0,M11.1.0";           // EST USA  
//const char* Timezone = "CST6CDT,M3.2.0,M11.1.0";           // CST USA
//const char* Timezone = "MST7MDT,M4.1.0,M10.5.0";           // MST USA
//const char* Timezone = "NZST-12NZDT,M9.5.0,M4.1.0/3";      // Auckland
//const char* Timezone = "EET-2EEST,M3.5.5/0,M10.5.5/0";     // Asia
//const char* Timezone = "ACST-9:30ACDT,M10.1.0,M4.1.0/3":   // Australia

const byte led_gpio = 19;

#define TIME_TO_SLEEP      900      // Time (in seconds) between wake ups - every 15mins
#define uS_TO_S_FACTOR     1000000ULL  /* Conversion factor for micro seconds to seconds */
