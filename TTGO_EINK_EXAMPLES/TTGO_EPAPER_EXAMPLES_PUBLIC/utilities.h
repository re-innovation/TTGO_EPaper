#pragma once
#include "Config.h"

//Ported to ESP32
#ifdef ESP32
  #include <esp_wifi.h>
  #include <WiFi.h>
  #include <WiFiClient.h>
  #include <HTTPClient.h>
  #define  ESP_getChipId()   ((uint32_t)ESP.getEfuseMac())
#else
  #include <ESP8266WiFi.h>          //https://github.com/esp8266/Arduino
  #include <DNSServer.h>
  #include <ESP8266WebServer.h>
  #define  ESP_getChipId()   (ESP.getChipId())
#endif

// These are just useful info to save in memory and re-use
#define PUSH_GIO39    "Push GPIO39 to start config AP"
#define TOUCH_GPIO32  "Touch GPIO32 to wake up"

extern bool print_wakeup_reason();
extern void print_wakeup_touchpad();

#ifdef WEATHER_EXAMPLE
String digitalClockDisplay(long int my_timeUTC);
String findHours(String my_period);
String findDate(String my_period);
String addTimeZone(String my_displayHours, int my_timeZone);
#endif