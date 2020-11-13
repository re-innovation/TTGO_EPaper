#pragma once
#include <esp_wifi.h>
#include <WiFi.h>
#include <WiFiClient.h>
//#include <WiFiClientSecure.h>
#include <HTTPClient.h>
#include "Config.h"

bool obtain_wx_data(WiFiClient& my_client, const String& my_RequestType, Forecast_record_type my_WxConditions[], Forecast_record_type my_WxForecast[]);
bool DecodeWeather(WiFiClient& my_json, String my_Type, Forecast_record_type my_WxConditions[], Forecast_record_type my_WxForecast[]);
