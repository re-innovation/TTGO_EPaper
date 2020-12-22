#pragma once
#include <esp_wifi.h>
#include <WiFi.h>
//#include <WiFiClient.h>
#include <WiFiClientSecure.h>
#include <HTTPClient.h>
#include "Config.h"

bool obtain_covid_data(WiFiClientSecure& my_client, const String& my_RequestDate, COVID_record_type my_covid_data[]);
bool DecodeCOVID(String my_payload, String my_Type, COVID_record_type my_covid_data[]);
