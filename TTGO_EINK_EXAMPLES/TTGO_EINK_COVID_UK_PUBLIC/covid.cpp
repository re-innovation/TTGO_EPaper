//#include "config.h"
#include <stdio.h>
#include <Arduino.h>
#include "covid.h"

#include <esp_wifi.h>
#include <WiFi.h>
//#include <WiFiClient.h>
#include <WiFiClientSecure.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>     // https://github.com/bblanchon/ArduinoJson

//#########################################################################################
bool obtain_covid_data(WiFiClientSecure& _client, const String& _RequestDate, COVID_record_type _covid_data[])
{
  // This is GandiStandardSSLCA2.pem, the root Certificate Authority that signed
  // the server certifcate for the demo server https://jigsaw.w3.org in this
  // example. This certificate is valid until Sep 11 23:59:59 2024 GMT
  const char* rootCACertificate = \
    "-----BEGIN CERTIFICATE-----\n" \
    "MIIGPjCCBSagAwIBAgIQB4iBmDWfrNHDYa7Dbp9/IzANBgkqhkiG9w0BAQsFADBN\n" \
    "MQswCQYDVQQGEwJVUzEVMBMGA1UEChMMRGlnaUNlcnQgSW5jMScwJQYDVQQDEx5E\n" \
    "aWdpQ2VydCBTSEEyIFNlY3VyZSBTZXJ2ZXIgQ0EwHhcNMjAxMDEwMDAwMDAwWhcN\n" \
    "MjExMDEwMTIwMDAwWjB6MQswCQYDVQQGEwJVUzETMBEGA1UECBMKV2FzaGluZ3Rv\n" \
    "bjEQMA4GA1UEBxMHUmVkbW9uZDEeMBwGA1UEChMVTWljcm9zb2Z0IENvcnBvcmF0\n" \
    "aW9uMSQwIgYDVQQDExthcGkuY29yb25hdmlydXMuZGF0YS5nb3YudWswggEiMA0G\n" \
    "CSqGSIb3DQEBAQUAA4IBDwAwggEKAoIBAQCtWSzW2P6qhWkNWbfyKrdifYKb3uon\n" \
    "fRDVacRSfGrdphIq4WIGgLoTelzwKF+e/Wl5km9WsPwSv86Bc9VQltVLEVIB+2El\n" \
    "fgiHm8RcMWc68KSbcCnWyIz55xjnGGhBjK5YcdfzvWagjK2mMwLvRd44XShxZRX3\n" \
    "jXveOPMZrrw5cI4Z3S6xUigC57DAihw1Y3Up+cKsHdiXEqqBHZUCbeDksAlqkp5H\n" \
    "VauKCoUCuzefN+QQ58y7Q9E736mINkS8nrwU35B/6IuWJMJi8iJybmOg5HdK/pxy\n" \
    "/nOioP9Hxi+lRl1ARkHpRos3Bk1OI3BbkeG+7UKc7Z5Rz974RZ06m011AgMBAAGj\n" \
    "ggLrMIIC5zAfBgNVHSMEGDAWgBQPgGEcgjFh1S8o541GOLQs4cbZ4jAdBgNVHQ4E\n" \
    "FgQUiSMSGyHpTMo8KaGkdVKAmk+t4zQwJgYDVR0RBB8wHYIbYXBpLmNvcm9uYXZp\n" \
    "cnVzLmRhdGEuZ292LnVrMA4GA1UdDwEB/wQEAwIFoDAdBgNVHSUEFjAUBggrBgEF\n" \
    "BQcDAQYIKwYBBQUHAwIwawYDVR0fBGQwYjAvoC2gK4YpaHR0cDovL2NybDMuZGln\n" \
    "aWNlcnQuY29tL3NzY2Etc2hhMi1nNy5jcmwwL6AtoCuGKWh0dHA6Ly9jcmw0LmRp\n" \
    "Z2ljZXJ0LmNvbS9zc2NhLXNoYTItZzcuY3JsMEwGA1UdIARFMEMwNwYJYIZIAYb9\n" \
    "bAEBMCowKAYIKwYBBQUHAgEWHGh0dHBzOi8vd3d3LmRpZ2ljZXJ0LmNvbS9DUFMw\n" \
    "CAYGZ4EMAQICMHwGCCsGAQUFBwEBBHAwbjAkBggrBgEFBQcwAYYYaHR0cDovL29j\n" \
    "c3AuZGlnaWNlcnQuY29tMEYGCCsGAQUFBzAChjpodHRwOi8vY2FjZXJ0cy5kaWdp\n" \
    "Y2VydC5jb20vRGlnaUNlcnRTSEEyU2VjdXJlU2VydmVyQ0EuY3J0MAwGA1UdEwEB\n" \
    "/wQCMAAwggEFBgorBgEEAdZ5AgQCBIH2BIHzAPEAdgD2XJQv0XcwIhRUGAgwlFaO\n" \
    "400TGTO/3wwvIAvMTvFk4wAAAXUUpWbDAAAEAwBHMEUCIEtiTCoGl3/sL+UZvI9o\n" \
    "qGm8VmAYmR7ypJo5hR3SwqwFAiEA5oDFMZwlCRpB9cgSL1/5VL0wiar2Sv4cttQj\n" \
    "4Fysj78AdwBc3EOS/uarRUSxXprUVuYQN/vV+kfcoXOUsl7m9scOygAAAXUUpWcS\n" \
    "AAAEAwBIMEYCIQDhOU/+84Nh3fPXrFbsasp5vsavzvcnUZ0f1GRHkcjUCgIhANGD\n" \
    "fy0oSfjbWpCU3oVpvWSNfC5cJbcrpKSrqtvtQBhHMA0GCSqGSIb3DQEBCwUAA4IB\n" \
    "AQCUfEz2dXKLl7tOv/PeC9VjE9REJ7dmtu/q5oQVs6NZxk9rX4rY+Oltj/TjkBt5\n" \
    "PKJsekBANsn2o5O19NIgeozf4WwTPtgUzAH84SXEDBP5RD8BesNPnIkktOmGGwKU\n" \
    "e28pN8/dzwuyZYJ+4wevImk3g0nlsAdjGDfO6ohYDjUc45zpveBlBXWKBdCrQJTM\n" \
    "ogFwmLHfwYWo28nry/nPRsbyizyh1haXY5v8t1CeNJXu2asu9faa2VgnevXkPAM3\n" \
    "FmpePhQTSDspLNbvCKgNzN02iCSJXB6gswn+hNFwicHoH1sDfrtoVyAEWfmuUcEY\n" \
    "F9VmurieXu+Ov9BqI1nSB9Bq=\n" \
    "-----END CERTIFICATE-----\n";

  String url = "https://" + (String)COVID_SERVER + "/data?filters=areaName=Nottingham;date=2020-11-20&structure={%22date%22:%22date%22,%22areaName%22:%22areaName%22}";
  WiFiClientSecure *client = new WiFiClientSecure;
  if (client)
  {
    client -> setCACert(rootCACertificate);
    {
      // Add a scoping block for HTTPClient https to make sure it is destroyed before WiFiClientSecure *client is
      HTTPClient https;
      Serial.print("[HTTPS] begin...\n");
      // if (https.begin(*client, "https://jigsaw.w3.org/HTTP/connection.html")) {  // HTTPS
      if (https.begin(*client, url))   // HTTPS
      {
        Serial.print("[HTTPS] GET...\n");
        // start connection and send HTTP header
        int httpCode = https.GET();
        // httpCode will be negative on error
        if (httpCode > 0) {
          // HTTP header has been send and Server response header has been handled
          Serial.printf("[HTTPS] GET... code: %d\n", httpCode);
          // file found at server
          if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
            String payload = https.getString();
            Serial.println(payload);
          }
        } else {
          Serial.printf("[HTTPS] GET... failed, error: %s\n", https.errorToString(httpCode).c_str());
        }
        https.end();
      }
      else
      {
        Serial.printf("[HTTPS] Unable to connect\n");
      }
      // End extra scoping block
    }
    delete client;
  }
  else
  {
    Serial.println("Unable to create client");
  }
  //
  //  HTTPClient https;
  //  String url = "https://" + (String)COVID_SERVER + "/data?filters=areaName=Nottingham;date=2020-11-20&structure={%22date%22:%22date%22,%22areaName%22:%22areaName%22}";
  //  Serial.print("Connecting to: ");
  //  Serial.println(url);
  //
  //  //http.begin(url, root_ca); //Specify the URL and certificate
  //  https.begin(_client, COVID_SERVER, 443, url);
  //
  //  int httpCode = http.GET();
  //  if (httpCode > 0) //Check for the returning code
  //  {
  //    String payload = http.getString();
  //    Serial.println(httpCode);
  //    Serial.println(payload);
  //  }
  //  else
  //  {
  //    Serial.println("Error on HTTP request");
  //  }
  //
  //  http.end(); //Free the resources
  //
  //  //
  //  //  Serial.print("[HTTPS] begin...\n");
  //  //  if (https.begin(_client, url)) {  // HTTPS
  //  //    Serial.print("[HTTPS] GET...\n");
  //  //    // start connection and send HTTP header
  //  //    int httpCode = https.GET();
  //  //
  //  //    // httpCode will be negative on error
  //  //    if (httpCode > 0) {
  //  //      // HTTP header has been send and Server response header has been handled
  //  //      Serial.printf("[HTTPS] GET... code: %d\n", httpCode);
  //  //      // file found at server
  //  //      if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
  //  //        String payload = https.getString();
  //  //        Serial.println(payload);
  //  //        //WiFiClient _local_client = https.getStream();
  //  //        // Need to decode the JSON here:
  //  //        //if (!DecodeCOVID(_local_client, _RequestDate, _covid_data))
  //  //        if (!DecodeCOVID(payload , _RequestDate, _covid_data))
  //  //        {
  //  //          Serial.printf("JSON decoding problem detected...");
  //  //          return false;
  //  //        }
  //  //
  //  //      }
  //  //    }
  //  //    else
  //  //    {
  //  //      Serial.printf("[HTTPS] GET... failed, error: %s\n", https.errorToString(httpCode).c_str());
  //  //    }
  //  //    https.end();
  //  //    return true;
  //  //  }
  //  //  else
  //  //  {
  //  //    Serial.printf("[HTTPS] Unable to connect\n");
  //  //    return false;
  //  //  }
  return true;
}
//#########################################################################################
// Problems with stucturing JSON decodes, see here: https://arduinojson.org/assistant/

//bool DecodeCOVID(WiFiClient& _json, String _Type, COVID_record_type _covid_data[])
bool DecodeCOVID(String _payload, String _Type, COVID_record_type _covid_data[])
{
  Serial.print(F("Creating object...and "));
  DynamicJsonDocument doc((20 * 1024) + 1173 ); // allocate the JsonDocument

  DeserializationError error = deserializeJson(doc, _payload);

  if (error) { // Test if parsing succeeds.// Deserialize the JSON document
    Serial.print("DeserializeJson() failed: ");
    Serial.println(error.c_str());
    //String ErrorMessage = "DeserializeJson() failed: " + String(error.c_str());
    return false;
  }
  JsonObject root = doc.as<JsonObject>(); // convert it to a JsonObject
  Serial.println(" Decoding " + _Type + " data");

  //  if (Type == "weather") {
  //    // All Serial.println statements are for diagnostic purposes and not required, remove if not needed
  //    _WxConditions[0].lon         = root["coord"]["lon"];
  //    _WxConditions[0].lat         = root["coord"]["lat"];
  //    _WxConditions[0].Main0       = root["weather"][0]["main"].as<char*>();         Serial.print("Main0: "); Serial.println(_WxConditions[0].Main0);
  //    _WxConditions[0].Forecast0   = root["weather"][0]["description"].as<char*>();  Serial.print("Fore0: "); Serial.println(_WxConditions[0].Forecast0);
  //    _WxConditions[0].Forecast1   = root["weather"][1]["main"].as<char*>();         Serial.print("Fore1: "); Serial.println(_WxConditions[0].Forecast1);
  //    _WxConditions[0].Forecast2   = root["weather"][2]["main"].as<char*>();         Serial.print("Fore2: "); Serial.println(_WxConditions[0].Forecast2);
  //    _WxConditions[0].Icon        = root["weather"][0]["icon"].as<char*>();         Serial.print("Icon : "); Serial.println(_WxConditions[0].Icon);
  //    _WxConditions[0].Temperature = root["main"]["temp"];                           Serial.print("Temp : "); Serial.println(_WxConditions[0].Temperature);
  //    _WxConditions[0].Pressure    = root["main"]["pressure"];                       Serial.print("Pres : "); Serial.println(_WxConditions[0].Pressure);
  //    _WxConditions[0].Humidity    = root["main"]["humidity"];                       Serial.print("Humi : "); Serial.println(_WxConditions[0].Humidity);
  //    _WxConditions[0].Low         = root["main"]["temp_min"];                       Serial.print("TLow : "); Serial.println(_WxConditions[0].Low);
  //    _WxConditions[0].High        = root["main"]["temp_max"];                       Serial.print("THigh: "); Serial.println(_WxConditions[0].High);
  //    _WxConditions[0].Windspeed   = root["wind"]["speed"];                          Serial.print("WSpd : "); Serial.println(_WxConditions[0].Windspeed);
  //    _WxConditions[0].Winddir     = root["wind"]["deg"];                            Serial.print("WDir : "); Serial.println(_WxConditions[0].Winddir);
  //    _WxConditions[0].Cloudcover  = root["clouds"]["all"];                          Serial.print("CCov : "); Serial.println(_WxConditions[0].Cloudcover); // in % of cloud cover
  //    _WxConditions[0].Visibility  = root["visibility"];                             Serial.print("Visi : "); Serial.println(_WxConditions[0].Visibility); // in metres
  //    _WxConditions[0].Country     = root["sys"]["country"].as<char*>();             Serial.print("Coun : "); Serial.println(_WxConditions[0].Country);
  //    _WxConditions[0].Sunrise     = root["sys"]["sunrise"];                         Serial.print("SunR : "); Serial.println(_WxConditions[0].Sunrise);
  //    _WxConditions[0].Sunset      = root["sys"]["sunset"];                          Serial.print("SunS : "); Serial.println(_WxConditions[0].Sunset);
  //    _WxConditions[0].TimeUTC     = root["dt"];                                     Serial.print("UTC  : "); Serial.println(_WxConditions[0].TimeUTC);
  //    _WxConditions[0].TimeZone    = root["timezone"];                               Serial.print("TZ   : "); Serial.println(_WxConditions[0].TimeZone);
  //
  //  }
  //  if (Type == "forecast") {
  //    //Serial.println(json);
  //    JsonArray list              = root["list"];
  //    Serial.print("\nReceiving Forecast period-"); //------------------------------------------------
  //    for (byte r = 0; r < MAX_WEATHER_READINGS; r++) {
  //      Serial.println("\nPeriod-" + String(r) + "--------------");
  //      _WxForecast[r].Dt          = list[r]["dt"].as<char*>();
  //      _WxForecast[r].Temperature = list[r]["main"]["temp"];                          Serial.print("Temp : "); Serial.println(_WxForecast[r].Temperature);
  //      _WxForecast[r].Low         = list[r]["main"]["temp_min"];                      Serial.print("TLow : "); Serial.println(_WxForecast[r].Low);
  //      _WxForecast[r].High        = list[r]["main"]["temp_max"];                      Serial.print("THig : "); Serial.println(_WxForecast[r].High);
  //      _WxForecast[r].Pressure    = list[r]["main"]["pressure"];                      Serial.print("Pres : "); Serial.println(_WxForecast[r].Pressure);
  //      _WxForecast[r].Humidity    = list[r]["main"]["humidity"];                      Serial.print("Humi : "); Serial.println(_WxForecast[r].Humidity);
  //      _WxForecast[r].Forecast0   = list[r]["weather"][0]["main"].as<char*>();        Serial.print("Fore0: "); Serial.println(_WxForecast[r].Forecast0);
  //      _WxForecast[r].Forecast0   = list[r]["weather"][1]["main"].as<char*>();        Serial.print("Fore1: "); Serial.println(_WxForecast[r].Forecast1);
  //      _WxForecast[r].Forecast0   = list[r]["weather"][2]["main"].as<char*>();        Serial.print("Fore2: "); Serial.println(_WxForecast[r].Forecast2);
  //      _WxForecast[r].Description = list[r]["weather"][0]["description"].as<char*>(); Serial.print("Desc : "); Serial.println(_WxForecast[r].Description);
  //      _WxForecast[r].Icon        = list[r]["weather"][0]["icon"].as<char*>();        Serial.print("Icon : "); Serial.println(_WxForecast[r].Icon);
  //      _WxForecast[r].Cloudcover  = list[r]["clouds"]["all"];                         Serial.print("CCov : "); Serial.println(_WxForecast[0].Cloudcover); // in % of cloud cover
  //      _WxForecast[r].Windspeed   = list[r]["wind"]["speed"];                         Serial.print("WSpd : "); Serial.println(_WxForecast[r].Windspeed);
  //      _WxForecast[r].Winddir     = list[r]["wind"]["deg"];                           Serial.print("WDir : "); Serial.println(_WxForecast[r].Winddir);
  //      _WxForecast[r].Rainfall    = list[r]["rain"]["3h"];                            Serial.print("Rain : "); Serial.println(_WxForecast[r].Rainfall);
  //      _WxForecast[r].Snowfall    = list[r]["snow"]["3h"];                            Serial.print("Snow : "); Serial.println(_WxForecast[r].Snowfall);
  //      _WxForecast[r].Period      = list[r]["dt_txt"].as<char*>();                    Serial.print("Peri : "); Serial.println(_WxForecast[r].Period);
  //    }
  //    //------------------------------------------
  //    float pressure_trend = _WxForecast[0].Pressure - _WxForecast[1].Pressure; // Measure pressure slope between ~now and later
  //    pressure_trend = ((int)(pressure_trend * 10)) / 10.0; // Remove any small variations less than 0.1
  //    _WxConditions[0].Trend = "0";
  //    if (pressure_trend > 0)  _WxConditions[0].Trend = "+";
  //    if (pressure_trend < 0)  _WxConditions[0].Trend = "-";
  //    if (pressure_trend == 0) _WxConditions[0].Trend = "0";
  //    //if (Units == "I") Convert_Readings_to_Imperial();
  //  }
  return true;
}
