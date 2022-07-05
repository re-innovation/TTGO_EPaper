#pragma once

#include <esp_wifi.h>
#include <WiFi.h>
#include <WiFiClient.h>
//#include <WiFiClientSecure.h>
#include <HTTPClient.h>
#include "Config.h"

bool obtain_wx_data(WiFiClient& my_client, const String& my_RequestType, Forecast_record_type my_WxConditions[], Forecast_record_type my_WxForecast[]);
bool DecodeWeather(WiFiClient& my_json, String my_Type, Forecast_record_type my_WxConditions[], Forecast_record_type my_WxForecast[]);
void printStoredData(); // Serial print the waether parameters 

class owm_settings_ {
  public:
    // Use your own API key by signing up for a free developer account at https://openweathermap.org/
    // This needs an account created.

    String  OWM_KEY;         //eg "YOUR OPEN WEATHER API KEY HERE";      // See: https://openweathermap.org/
    String  OWM_SERVER;     // eg  "api.openweathermap.org";
    String  MY_CITY;        // eg "Ledbury";  Your home city See: http://bulk.openweathermap.org/sample/
    String  MY_COUNTRY;     // eg "GB";       Your country
    String  MY_LANGUAGE;    // eg "EN";       NOTE: Only the weather description (not used) is translated by OWM
    String  MY_HEMISPHERE;  //eg "north";     or "south"
    String  MY_UNITS;       // eg "M";        "M"etric or "I"mperial
    String  MY_TIMEZONE;    // eg "GMT0BST,M3.5.0/01,M10.5.0/02"; Choose your time zone from: https://github.com/nayarsystems/posix_tz_db/blob/master/zones.csv
                
    // Example time zones
    //#define   MY_TIMEZONE = "MET-1METDST,M3.5.0/01,M10.5.0/02"; // Most of Europe
    //#define   MY_TIMEZONE = "CET-1CEST,M3.5.0,M10.5.0/3";       // Central Europe
    //#define   MY_TIMEZONE = "EST-1METDST,M3.5.0/01,M10.5.0/02"; // Most of Europe
    //#define   MY_TIMEZONE = "EST5EDT,M3.2.0,M11.1.0";           // EST USA
    //#define   MY_TIMEZONE = "CST6CDT,M3.2.0,M11.1.0";           // CST USA
    //#define   MY_TIMEZONE = "MST7MDT,M4.1.0,M10.5.0";           // MST USA
    //#define   MY_TIMEZONE = "NZST-12NZDT,M9.5.0,M4.1.0/3";      // Auckland
    //#define   MY_TIMEZONE = "EET-2EEST,M3.5.5/0,M10.5.5/0";     // Asia
    //#define   MY_TIMEZONE = "ACST-9:30ACDT,M10.1.0,M4.1.0/3":   // Australia

};

extern owm_settings_ owm_settings;
