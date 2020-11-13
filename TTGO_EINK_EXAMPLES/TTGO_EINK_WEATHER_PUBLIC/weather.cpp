//#include "config.h"
#include <stdio.h>
#include <Arduino.h>
#include "weather.h"

#include <esp_wifi.h>
#include <WiFi.h>
#include <WiFiClient.h>
//#include <WiFiClientSecure.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>     // https://github.com/bblanchon/ArduinoJson

//#########################################################################################
bool obtain_wx_data(WiFiClient& _client, const String& _RequestType, Forecast_record_type _WxConditions[], Forecast_record_type _WxForecast[]) 
{
  const String units = (MY_UNITS == "M" ? "metric" : "imperial");
  _client.stop(); // close connection before sending a new request
  HTTPClient http;
  String uri = "/data/2.5/" + _RequestType + "?q=" + MY_CITY + "," + MY_COUNTRY + "&APPID=" + OWM_KEY + "&mode=json&units=" + units + "&lang=" + MY_LANGUAGE;
  if (_RequestType != "weather")
  {
    uri += "&cnt=" + String(MAX_WEATHER_READINGS);
  }

  http.useHTTP10(true); // switch to HTTP version 1.0 to do http.getStream()
  //http.begin(uri,test_root_ca); //HTTPS example connection
  http.begin(_client, OWM_SERVER, 80, uri);
  int httpCode = http.GET();
  if (httpCode == HTTP_CODE_OK) {
    if (!DecodeWeather(http.getStream(), _RequestType, _WxConditions, _WxForecast)) {
      //ErrorMessage = "JSON decoding problem detected...";
      Serial.printf("JSON decoding problem detected...");
      return false;
    }
    _client.stop();
    http.end();
    return true;
  }
  else
  {
    //ErrorMessage = "Connection failed, error: " + String(http.errorToString(httpCode).c_str());
    Serial.printf("connection failed, error: %s", http.errorToString(httpCode).c_str());
    _client.stop();
    http.end();
    return false;
  }
  return true;
}
//#########################################################################################
// Problems with stucturing JSON decodes, see here: https://arduinojson.org/assistant/
bool DecodeWeather(WiFiClient& json, String Type, Forecast_record_type _WxConditions[], Forecast_record_type _WxForecast[]) 
{
  Serial.print(F("Creating object...and "));
  DynamicJsonDocument doc((20 * 1024) + 1173 ); // allocate the JsonDocument

  DeserializationError error = deserializeJson(doc, json);

  if (error) { // Test if parsing succeeds.// Deserialize the JSON document
    Serial.print("DeserializeJson() failed: ");
    Serial.println(error.c_str());
    //String ErrorMessage = "DeserializeJson() failed: " + String(error.c_str());
    return false;
  }
  JsonObject root = doc.as<JsonObject>(); // convert it to a JsonObject
  Serial.println(" Decoding " + Type + " data");

  if (Type == "weather") {
    // All Serial.println statements are for diagnostic purposes and not required, remove if not needed
    _WxConditions[0].lon         = root["coord"]["lon"];
    _WxConditions[0].lat         = root["coord"]["lat"];
    _WxConditions[0].Main0       = root["weather"][0]["main"].as<char*>();         Serial.print("Main0: "); Serial.println(_WxConditions[0].Main0);
    _WxConditions[0].Forecast0   = root["weather"][0]["description"].as<char*>();  Serial.print("Fore0: "); Serial.println(_WxConditions[0].Forecast0);
    _WxConditions[0].Forecast1   = root["weather"][1]["main"].as<char*>();         Serial.print("Fore1: "); Serial.println(_WxConditions[0].Forecast1);
    _WxConditions[0].Forecast2   = root["weather"][2]["main"].as<char*>();         Serial.print("Fore2: "); Serial.println(_WxConditions[0].Forecast2);
    _WxConditions[0].Icon        = root["weather"][0]["icon"].as<char*>();         Serial.print("Icon : "); Serial.println(_WxConditions[0].Icon);
    _WxConditions[0].Temperature = root["main"]["temp"];                           Serial.print("Temp : "); Serial.println(_WxConditions[0].Temperature);
    _WxConditions[0].Pressure    = root["main"]["pressure"];                       Serial.print("Pres : "); Serial.println(_WxConditions[0].Pressure);
    _WxConditions[0].Humidity    = root["main"]["humidity"];                       Serial.print("Humi : "); Serial.println(_WxConditions[0].Humidity);
    _WxConditions[0].Low         = root["main"]["temp_min"];                       Serial.print("TLow : "); Serial.println(_WxConditions[0].Low);
    _WxConditions[0].High        = root["main"]["temp_max"];                       Serial.print("THigh: "); Serial.println(_WxConditions[0].High);
    _WxConditions[0].Windspeed   = root["wind"]["speed"];                          Serial.print("WSpd : "); Serial.println(_WxConditions[0].Windspeed);
    _WxConditions[0].Winddir     = root["wind"]["deg"];                            Serial.print("WDir : "); Serial.println(_WxConditions[0].Winddir);
    _WxConditions[0].Cloudcover  = root["clouds"]["all"];                          Serial.print("CCov : "); Serial.println(_WxConditions[0].Cloudcover); // in % of cloud cover
    _WxConditions[0].Visibility  = root["visibility"];                             Serial.print("Visi : "); Serial.println(_WxConditions[0].Visibility); // in metres
    _WxConditions[0].Country     = root["sys"]["country"].as<char*>();             Serial.print("Coun : "); Serial.println(_WxConditions[0].Country);
    _WxConditions[0].Sunrise     = root["sys"]["sunrise"];                         Serial.print("SunR : "); Serial.println(_WxConditions[0].Sunrise);
    _WxConditions[0].Sunset      = root["sys"]["sunset"];                          Serial.print("SunS : "); Serial.println(_WxConditions[0].Sunset);
    _WxConditions[0].TimeUTC     = root["dt"];                                     Serial.print("UTC  : "); Serial.println(_WxConditions[0].TimeUTC);
    _WxConditions[0].TimeZone    = root["timezone"];                               Serial.print("TZ   : "); Serial.println(_WxConditions[0].TimeZone);

  }
  if (Type == "forecast") {
    //Serial.println(json);
    JsonArray list              = root["list"];
    Serial.print("\nReceiving Forecast period-"); //------------------------------------------------
    for (byte r = 0; r < MAX_WEATHER_READINGS; r++) {
      Serial.println("\nPeriod-" + String(r) + "--------------");
      _WxForecast[r].Dt          = list[r]["dt"].as<char*>();
      _WxForecast[r].Temperature = list[r]["main"]["temp"];                          Serial.print("Temp : "); Serial.println(_WxForecast[r].Temperature);
      _WxForecast[r].Low         = list[r]["main"]["temp_min"];                      Serial.print("TLow : "); Serial.println(_WxForecast[r].Low);
      _WxForecast[r].High        = list[r]["main"]["temp_max"];                      Serial.print("THig : "); Serial.println(_WxForecast[r].High);
      _WxForecast[r].Pressure    = list[r]["main"]["pressure"];                      Serial.print("Pres : "); Serial.println(_WxForecast[r].Pressure);
      _WxForecast[r].Humidity    = list[r]["main"]["humidity"];                      Serial.print("Humi : "); Serial.println(_WxForecast[r].Humidity);
      _WxForecast[r].Forecast0   = list[r]["weather"][0]["main"].as<char*>();        Serial.print("Fore0: "); Serial.println(_WxForecast[r].Forecast0);
      _WxForecast[r].Forecast0   = list[r]["weather"][1]["main"].as<char*>();        Serial.print("Fore1: "); Serial.println(_WxForecast[r].Forecast1);
      _WxForecast[r].Forecast0   = list[r]["weather"][2]["main"].as<char*>();        Serial.print("Fore2: "); Serial.println(_WxForecast[r].Forecast2);
      _WxForecast[r].Description = list[r]["weather"][0]["description"].as<char*>(); Serial.print("Desc : "); Serial.println(_WxForecast[r].Description);
      _WxForecast[r].Icon        = list[r]["weather"][0]["icon"].as<char*>();        Serial.print("Icon : "); Serial.println(_WxForecast[r].Icon);
      _WxForecast[r].Cloudcover  = list[r]["clouds"]["all"];                         Serial.print("CCov : "); Serial.println(_WxForecast[0].Cloudcover); // in % of cloud cover
      _WxForecast[r].Windspeed   = list[r]["wind"]["speed"];                         Serial.print("WSpd : "); Serial.println(_WxForecast[r].Windspeed);
      _WxForecast[r].Winddir     = list[r]["wind"]["deg"];                           Serial.print("WDir : "); Serial.println(_WxForecast[r].Winddir);
      _WxForecast[r].Rainfall    = list[r]["rain"]["3h"];                            Serial.print("Rain : "); Serial.println(_WxForecast[r].Rainfall);
      _WxForecast[r].Snowfall    = list[r]["snow"]["3h"];                            Serial.print("Snow : "); Serial.println(_WxForecast[r].Snowfall);
      _WxForecast[r].Period      = list[r]["dt_txt"].as<char*>();                    Serial.print("Peri : "); Serial.println(_WxForecast[r].Period);
    }
    //------------------------------------------
    float pressure_trend = _WxForecast[0].Pressure - _WxForecast[1].Pressure; // Measure pressure slope between ~now and later
    pressure_trend = ((int)(pressure_trend * 10)) / 10.0; // Remove any small variations less than 0.1
    _WxConditions[0].Trend = "0";
    if (pressure_trend > 0)  _WxConditions[0].Trend = "+";
    if (pressure_trend < 0)  _WxConditions[0].Trend = "-";
    if (pressure_trend == 0) _WxConditions[0].Trend = "0";
    //if (Units == "I") Convert_Readings_to_Imperial();
  }
  return true;
}
