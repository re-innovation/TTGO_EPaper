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

#include "SPIFFS.h"

// Set **destination** filesystem by uncommenting one of these:
#define DEST_FS_USES_SPIFFS
//#define DEST_FS_USES_FFAT
//#define DEST_FS_USES_SD
//#define DEST_FS_USES_SD_MMC
//#define DEST_FS_USES_LITTLEFS
#include <ESP32-targz.h>

//#########################################################################################

bool obtain_covid_data(WiFiClientSecure& _client, const String& _RequestDate, COVID_record_type _covid_data[], int _y, int _mode)
{
  // Find the rootCAcertificate to use for SSL. Use this tutorial. Firefox has changed, though!
  // https://techtutorialsx.com/2017/11/18/esp32-arduino-https-get-request/
  const char* rootCACertificate = \
                                  "-----BEGIN CERTIFICATE-----\n" \
                                  "MIIDrzCCApegAwIBAgIQCDvgVpBCRrGhdWrJWZHHSjANBgkqhkiG9w0BAQUFADBh\n" \
                                  "MQswCQYDVQQGEwJVUzEVMBMGA1UEChMMRGlnaUNlcnQgSW5jMRkwFwYDVQQLExB3\n" \
                                  "d3cuZGlnaWNlcnQuY29tMSAwHgYDVQQDExdEaWdpQ2VydCBHbG9iYWwgUm9vdCBD\n" \
                                  "QTAeFw0wNjExMTAwMDAwMDBaFw0zMTExMTAwMDAwMDBaMGExCzAJBgNVBAYTAlVT\n" \
                                  "MRUwEwYDVQQKEwxEaWdpQ2VydCBJbmMxGTAXBgNVBAsTEHd3dy5kaWdpY2VydC5j\n" \
                                  "b20xIDAeBgNVBAMTF0RpZ2lDZXJ0IEdsb2JhbCBSb290IENBMIIBIjANBgkqhkiG\n" \
                                  "9w0BAQEFAAOCAQ8AMIIBCgKCAQEA4jvhEXLeqKTTo1eqUKKPC3eQyaKl7hLOllsB\n" \
                                  "CSDMAZOnTjC3U/dDxGkAV53ijSLdhwZAAIEJzs4bg7/fzTtxRuLWZscFs3YnFo97\n" \
                                  "nh6Vfe63SKMI2tavegw5BmV/Sl0fvBf4q77uKNd0f3p4mVmFaG5cIzJLv07A6Fpt\n" \
                                  "43C/dxC//AH2hdmoRBBYMql1GNXRor5H4idq9Joz+EkIYIvUX7Q6hL+hqkpMfT7P\n" \
                                  "T19sdl6gSzeRntwi5m3OFBqOasv+zbMUZBfHWymeMr/y7vrTC0LUq7dBMtoM1O/4\n" \
                                  "gdW7jVg/tRvoSSiicNoxBN33shbyTApOB6jtSj1etX+jkMOvJwIDAQABo2MwYTAO\n" \
                                  "BgNVHQ8BAf8EBAMCAYYwDwYDVR0TAQH/BAUwAwEB/zAdBgNVHQ4EFgQUA95QNVbR\n" \
                                  "TLtm8KPiGxvDl7I90VUwHwYDVR0jBBgwFoAUA95QNVbRTLtm8KPiGxvDl7I90VUw\n" \
                                  "DQYJKoZIhvcNAQEFBQADggEBAMucN6pIExIK+t1EnE9SsPTfrgT1eXkIoyQY/Esr\n" \
                                  "hMAtudXH/vTBH1jLuG2cenTnmCmrEbXjcKChzUyImZOMkXDiqw8cvpOp/2PV5Adg\n" \
                                  "06O/nVsJ8dWO41P0jmP6P6fbtGbfYmbW0W5BjfIttep3Sp+dWOIrWcBAI+0tKIJF\n" \
                                  "PnlUkiaY4IBIqDfv8NZ5YBberOgOzW6sRBc4L0na4UU+Krk2U886UAb3LujEV0ls\n" \
                                  "YSEY1QSteDwsOoBrp+uvFRTp2InBuThs4pFsiv9kuXclVzDAGySj4dzp30d8tbQk\n" \
                                  "CAUw7C29C79Fv1C5qfPrmAESrciIxpg0X40KPMbp1ZWVbd4=\n" \
                                  "-----END CERTIFICATE-----\n"; \

  // Example URL here:
  // https://api.coronavirus.data.gov.uk/v1/data?filters=areaName=Nottingham;date=2020-11-20&structure=
  // {%22date%22:%22date%22,%22areaName%22:%22areaName%22,%22newCasesByPublishDate%22:%22newCasesByPublishDate%22,
  // %22cumCasesByPublishDate%22:%22cumCasesByPublishDate%22,%22newDeathsByDeathDate%22:%22newDeathsByDeathDate%22,
  // %22cumDeathsByDeathDate%22:%22cumDeathsByDeathDate%22}

  String url_local = "https://" + (String)COVID_SERVER + "/data?filters=areaName=" + (String)MY_CITY +
                     ";date=" + _RequestDate + "&structure={%22date%22:%22date%22,%22areaName%22:%22areaName%22,%22code%22:%22areaCode" +
                     "%22,%22cases%22:{%22newCasesBySpecimenDate%22:%22newCasesBySpecimenDate%22,%22cumCasesBySpecimenDate%22:%22cumCasesBySpecimenDate%22}," +
                     "%22deaths%22:{%22newDeathsByDeathDate%22:%22newDeathsByDeathDate%22,%22cumDeathsByDeathDate%22:%22cumDeathsByDeathDate%22}}";

  //  String url_country = "https://" + (String)COVID_SERVER + "/data?filters=areaType=nation;areaName=" + (String)MY_CITY +
  //                       ";date=" + _RequestDate + "&structure={%22date%22:%22date%22,%22name%22:%22areaName%22,%22code%22:%22areaCode" +
  //                       "%22,%22cases%22:{%22daily%22:%22newCasesByPublishDate%22,%22cumulative%22:%22cumCasesByPublishDate%22}," +
  //                       "%22deaths%22:{%22daily%22:%22newDeathsByDeathDate%22,%22cumulative%22:%22cumDeathsByDeathDate%22}}";

  String url_overview =  "https://" + (String)COVID_SERVER + "/data?filters=areaType=overview;date=" + _RequestDate +
                         "&structure={%22date%22:%22date%22,%22areaName%22:%22areaName%22,%22newCasesByPublishDate%22:" +
                         "%22newCasesByPublishDate%22,%22cumCasesByPublishDate%22:%22cumCasesByPublishDate%22," +
                         "%22newDeaths28DaysByPublishDate%22:%22newDeaths28DaysByPublishDate%22,%22cumDeaths28DaysByPublishDate%22" +
                         ":%22cumDeaths28DaysByPublishDate%22}";

  // Mount the SPIFFS ready for storing the data.

  if (!SPIFFS.begin(true)) {
    Serial.println("An Error has occurred while mounting SPIFFS");
    return false;
  }
  File file = SPIFFS.open("/stored.gz", FILE_WRITE);
  if (!file) {
    Serial.println("There was an error opening the file for writing");
    return false;
  }
  WiFiClientSecure *client = new WiFiClientSecure;
  if (client)
  {
    client -> setCACert(rootCACertificate);
    {
      // Add a scoping block for HTTPClient https to make sure it is destroyed before WiFiClientSecure *client is
      HTTPClient https;
      Serial.print("[HTTPS] begin...\n");

      String url_request;
      if (_mode == 0)
      {
        url_request = url_local;
      }
      else if (_mode == 1)
      {
        url_request = url_overview;
      }

      if (DEBUG_FLAG == true)
      {
        Serial.println(url_request);

      }
      if (https.begin(*client, url_request))   // HTTPS
      {
        Serial.print("[HTTPS] GET...\n");
        // start connection and send HTTP header
        int httpCode = https.GET();
        // httpCode will be negative on error
        if (httpCode > 0) {
          // HTTP header has been send and Server response header has been handled
          Serial.printf("[HTTPS] GET... code: %d\n", httpCode);
          // file found at server
          if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY)
          {
            String payload = https.getString();
            // Problem is the data is gzip'd up So looks like gibberish.
            // need to unpack data
            file.print(payload);
          }
        } else
        {
          Serial.printf("[HTTPS] GET... failed, error: %s\n", https.errorToString(httpCode).c_str());
          return false;
        }
        https.end();
        file.close();
      }
      else
      {
        Serial.printf("[HTTPS] Unable to connect\n");
        return false;
      }
    }
    delete client;
  }
  else
  {
    Serial.println("Unable to create client");
    return false;
  }
  // At this point we have got a file with the gzip data in SPIFFS
  // We need to decompress it to another location in SPIFFS
  if ( ! gzExpander(SPIFFS, "/stored.gz", SPIFFS, "/stored.txt") ) {
    Serial.printf("operation failed with return code #%d", tarGzGetError() );
    return false;
  }
  else
  {
    // Only want to do this if data is OK.
    if (_mode == 0)
    {
      DecodeCOVID("/stored.txt", "localData", _covid_data, _y);
    }
    else if (_mode == 1)
    {
      DecodeCOVID("/stored.txt", "overview", _covid_data, _y);
    }
    return true;
  }

}
//#########################################################################################
// Problems with stucturing JSON decodes, see here: https://arduinojson.org/assistant/

bool DecodeCOVID(String _fileName, String _Type, COVID_record_type _covid_data[], int _yy)
{
  File _file2 = SPIFFS.open(_fileName);
  if (!_file2)
  {
    Serial.println("Failed to open file for reading");
    return false;
  }
  // Read the saved data into one big ole string...
  String _jsonString;
  while (_file2.available())
  {
    _jsonString += (char)_file2.read();
  }
  _file2.close();
  Serial.println(_jsonString);

  Serial.print(F("Creating object...and "));
  DynamicJsonDocument doc(4000); // allocate the JsonDocument
  DeserializationError error = deserializeJson(doc, _jsonString);

  if (error) { // Test if parsing succeeds.// Deserialize the JSON document
    Serial.print("DeserializeJson() failed: ");
    Serial.println(error.c_str());
    return false;
  }
  JsonObject root = doc.as<JsonObject>(); // convert it to a JsonObject
  Serial.println(" Decoding " + _Type + " data");
  if (_Type == "localData")
  {
    // This is for the local data
    _covid_data[_yy].date         = root["data"][0]["date"].as<char*>();
    _covid_data[_yy].areaName     = root["data"][0]["areaName"].as<char*>();
    _covid_data[_yy].newCases     = root["data"][0]["cases"]["newCasesBySpecimenDate"];
    _covid_data[_yy].totalCases   = root["data"][0]["cases"]["cumCasesBySpecimenDate"];
    _covid_data[_yy].newDeaths    = root["data"][0]["deaths"]["newDeathsByDeathDate"];
    _covid_data[_yy].totalDeaths  = root["data"][0]["deaths"]["cumDeathsByDeathDate"];
  }
  else if (_Type == "overview")
  {
    // This is for the local data
    _covid_data[_yy].date         = root["data"][0]["date"].as<char*>();
    _covid_data[_yy].areaName     = root["data"][0]["areaName"].as<char*>();
    _covid_data[_yy].newCases     = root["data"][0]["newCasesByPublishDate"];
    _covid_data[_yy].totalCases   = root["data"][0]["cumCasesByPublishDate"];
    _covid_data[_yy].newDeaths    = root["data"][0]["newDeaths28DaysByPublishDate"];
    _covid_data[_yy].totalDeaths  = root["data"][0]["cumDeaths28DaysByPublishDate"];
  }
  return true;
}
