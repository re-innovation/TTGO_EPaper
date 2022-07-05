#pragma once

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

extern bool print_wakeup_reason();
extern void print_wakeup_touchpad();
