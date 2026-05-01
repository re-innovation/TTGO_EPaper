/*  ***************** E-Paper Display Unit ***************************
//  ***************** Example Code - Quotation Unit ********************
//
//  This code was:
//  Written by: Matt Little
//  Date: 29/06/2022
//  But is based on lots of other peoples examples!
//  This code is open and can be shared freely.
//  Contact:  hello@curiouselectric.co.uk
//
// This is a test code for touch wakeup mode on the
// E-Paper Screen of the TTGO T5 2.13" V2.3 ESP32 Unit
// When the touch pad is pressed:
// This code wakes up, checks if WiFi credentials are stored
// If not an Acess point is started to enter in the details - these are stored
// If they are stored then the unit does the following:

// 1: If QUOTATION_EXAMPLE is selected in the config.h file then:
// - downloads a random quote and displays the text and author.
// - displays the quote on the E-Ink display
// - goes to sleep

// 2: If WEATHER_EXAMPLE is selected in the config.h file then:
// - Using the OWM (open weather map) settings in "weather.h"
// - (Note: You must have an OWM API Key and know the area you are trying to get the information for)
// - Code has been updated for API version 3.0 from OWM
// - The local weather data and forecast is downloaded
// - displays the information along with graphics for the next 5 x 3hour slots. It also shows the time the data was taken.
// - goes to sleep

3: If GRIDFREQ_EXAMPLE is selected in the config.h file then:
This example is for tyhe UK only.
This example downloads the days grid frequency from a nantional database of grid data.
It highlights if the grid freq is high (so switch on things) or grid freq is low (so switch things off)
It draws this data as a graph of the previous 24hrs in 10 min averaged blocks.
Using the Arduino Library Manager, install "Time by Michael Margolis".


// Please follow the set-up instructions found here: https://github.com/re-innovation/TTGO_EPaper/blob/master/README.md
// This explains the libraries needed to upload this example code.
//
// When uploading using Arduino IDE: Must use "ESP32 Dev Module"
//*/
#include <WiFiManager.h>  // https://github.com/tzapu/WiFiManager

#ifdef WEATHER_EXAMPLE
// This is for dealing with the JSON returned data.
#include <ArduinoJson.h>  // https://github.com/bblanchon/ArduinoJson
#include <TimeLib.h>      // Include time functions
#endif

#include "Config.h"
#include "CE_Icons.h"  // Curious Electric Icons
#include "display.h"
#include "board_def.h"
#include "wifi_local.h"
#include "epaper_fonts.h"
#include "time.h"  // Built-in
#include "utilities.h"

#ifdef QUOTATION_EXAMPLE
#include "quote.h"
// Global variables
String payload;  // Holds the reply from the HTTP request
String quote;    // Holds the final quote
String author;   // Holds the author of the quote
#endif

#ifdef WEATHER_EXAMPLE
#include "weather.h"
// Variables for the weather calls
boolean RxWeather = false, RxForecast = false;  // Initialise these - have we got the data?
String TimeStr, DateStr, ErrorMessage;          // strings to hold time and date
Forecast_record_type WxConditions[1];
Forecast_record_type WxForecast[MAX_WEATHER_READINGS];
#endif

#ifdef GRIDFREQ_EXAMPLE
// Must also: Using the Arduino Library Manager, install "Time by Michael Margolis".
#include "time.h"
#include "gridfreq.h"
#include <ArduinoJson.h>  // https://github.com/bblanchon/ArduinoJson
String payload;           // Holds the reply from the HTTP request

#endif

void setup() {
  pinMode(LED_GPIO, OUTPUT);
  digitalWrite(LED_GPIO, HIGH);  // Switch ON led - show woken up!
  Serial.begin(115200);

  // What is the wake up reason?
  bool reset_wifi_flag = print_wakeup_reason();
  print_wakeup_touchpad();

  if (reset_wifi_flag) {
    Serial.print(F("Enable Access Point mode"));
  }

#ifdef WEATHER_EXAMPLE
  //Init EEPROM
  EEPROM.begin(1000);
  // Check EEPROM for the unit data:
  owm_settings.OWM_KEY = EEPROM.readString(0x0);
  owm_settings.OWM_SERVER = EEPROM.readString(0x100);
  owm_settings.MY_CITY = EEPROM.readString(0x150);
  owm_settings.MY_COUNTRY = EEPROM.readString(0x200);
  owm_settings.MY_LANGUAGE = EEPROM.readString(0x250);
  owm_settings.MY_HEMISPHERE = EEPROM.readString(0x260);
  owm_settings.MY_UNITS = EEPROM.readString(0x270);
  owm_settings.MY_TIMEZONE = EEPROM.readString(0x280);
  printStoredData();  // Shows the Stored data for connecting to Open Weather Map
#endif

  // Want to display that we are getting an update
  displayInit();  // Initialise the display.
  displayShowCELogo();
  // Start Wifi - Connect if we have the SSID/PASS or set up AP if not.
  setup_wifi(reset_wifi_flag);

#ifdef WEATHER_EXAMPLE
  // Here we get the weather data from Open Weather Maps
  // Need to register account with them and get API key - put that into the Config.h file
  if ((WiFi.status() == WL_CONNECTED)) {
    // Here want to get the weather data:
    byte Attempts = 1;
    WiFiClient client1;                                                                // wifi client object
    while ((RxWeather == false || RxForecast == false) && Attempts <= MAX_ATTEMPTS) {  // Try up to MAX_ATTEMPTS for Weather and Forecast data
      if (RxWeather == false) RxWeather = obtain_wx_data(client1, "weather", WxConditions, WxForecast);
      if (RxForecast == false) RxForecast = obtain_wx_data(client1, "forecast", WxConditions, WxForecast);
      Attempts++;
      delay(250);
    }
    // Refresh screen if data was received OK, otherwise wait until the next timed check
    if (RxWeather || RxForecast) {
      stopWiFi();  // Reduces power consumption
      // ****** Display all the data here: *************************
      displayClear();
      // Here we need to deal with data which is JSON format & also display it.
      displayWeatherInfo(WxConditions, WxForecast);
    } else {
      stopWiFi();  // Reduces power consumption
      Serial.println("Failed to get Wx Data, " + String(RxWeather ? "" : " Failed RxWeather") + String(RxForecast ? "" : " Failed RxForecast"));
      displayShowError();
    }
  }
#endif


#ifdef QUOTATION_EXAMPLE
  // Here we create the URL to check for getting the quote information
  // Sample URL is: http://api.forismatic.com/api/1.0/?method=getQuote&key=457653&format=text&lang=en
  String url = QUOTE_SERVER;
  // Create the full URL with method, format and language
  url += F("?method=getQuote&key=");
  url += QUOTE_SEED;
  url += F("&format=text&lang=");
  url += QUOTE_LANGUAGE;
  Serial.println(url);
  if ((WiFi.status() == WL_CONNECTED)) {
    HTTPClient http;
    bool quote_length_flag = false;
    http.begin(url);
    while (quote_length_flag == false) {
      int httpCode = http.GET();
      // httpCode will be negative on error
      if (httpCode > 0) {
        // HTTP header has been send and Server response header has been handled
        Serial.printf("[HTTP] GET... code: %d\n", httpCode);
        // file found at server
        if (httpCode == HTTP_CODE_OK) {
          payload = http.getString();
          Serial.println(payload);
          if (payload.length() < 132) {
            quote_length_flag = true;  // Quote is short so all OK
          } else {
            delay(500);
          }
        }
      } else {
        delay(500);
        // String too long or failed to get it
        Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
        displayShowError();
      }
    }
    http.end();
  }
  stopWiFi();  // Reduces power consumption
  if (payload.length() > 0) {
    // Only do this if we have some data
    String quote = parseQuote(payload);
    Serial.println(quote);
    String author = parseAuthor(payload);
    Serial.println(author);

    // ****** Display quote here: *************************
    displayClear();
    displayQuote(quote, author);
  }
#endif


#ifdef GRIDFREQ_EXAMPLE
  // First we need to get the correct time and date:
  // Init and get the time
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  bool time_ok_flag = printLocalTime();
  String url;  // This holds the URL to check for getting the grid frequency data
  float most_recent_freq;
  float freq_data_array[144];   // This holds the 10 min averaged frequency data
  int freq_drawing_array[144];  // This holds the 10 min averaged frequency data
  // First lets get the instantaneous value for the most recent grid frequency:
  url = GRIDFREQ_SERVER;
  // Want to create a URL that looks like:
  //"https://data.elexon.co.uk/bmrs/api/v1/system/frequency?from=2026-04-16T13%3A00%3A00Z&to=2026-04-16T14%3A00%3A00Z&format=csv"
  url += returnTimeNow(360);  // Just look for 120 seconds ago - should give 2 or 3 values, as each value is 15 seconds
  url += "&to=";
  url += returnTimeNow(0);
  url += GREDFREQ_FORMAT;  // Create the full URL with method, format and language
  // Serial.println(url);     // Just for testing

  if ((WiFi.status() == WL_CONNECTED) && time_ok_flag) {
    HTTPClient http;
    http.begin(url);
    int httpCode = http.GET();
    // httpCode will be negative on error
    if (httpCode > 0) {
      // HTTP header has been send and Server response header has been handled
      // Serial.printf("[HTTP] GET... code: %d\n", httpCode);
      // file found at server
      if (httpCode == HTTP_CODE_OK) {
        payload = http.getString();
      }
    } else {
      delay(500);
      // String too long or failed to get it
      Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
      displayShowError();
    }
    http.end();
    // Now we can display the data on the display:
    if (payload.length() > 0 && time_ok_flag) {
      // // Here we should have a couple of the most recent values
      JsonDocument doc;
      DeserializationError error = deserializeJson(doc, payload);
      if (error) {
        Serial.print("deserializeJson() failed: ");
        Serial.println(error.c_str());
        return;
      }

      float grid_freq_array[10];
      int n = 0;
      for (JsonObject data_item : doc["data"].as<JsonArray>()) {
        const char* data_item_measurementTime = data_item["measurementTime"];  // "2026-04-17T15:21:15Z", ...
        grid_freq_array[n] = data_item["frequency"];                           // 49.942, 49.948, 49.97
        //Serial.println("Time: " + (String)data_item_measurementTime + " Freq:" + grid_freq_array[n]);
        n++;
      }
      most_recent_freq = grid_freq_array[n - 1];
      Serial.println("Most Recent Freq:" + (String)most_recent_freq);
    }

    for (int i = 0; i < 144; i++) {
      // Serial.printf("Loop: %d\n", i);
      url = GRIDFREQ_SERVER;
      // Want to create a URL that looks like:
      //"https://data.elexon.co.uk/bmrs/api/v1/system/frequency?from=2026-04-16T13%3A00%3A00Z&to=2026-04-16T14%3A00%3A00Z&format=csv"
      url += returnTimeNow((i + 1) * 600);  // Just look for 600 seconds (10 min) ago
      url += "&to=";
      url += returnTimeNow(i * 600);
      url += GREDFREQ_FORMAT;  // Create the full URL with method, format and language
      http.begin(url);
      int httpCode = http.GET();
      // httpCode will be negative on error
      if (httpCode > 0) {
        // HTTP header has been send and Server response header has been handled
        // Serial.printf("[HTTP] GET... code: %d\n", httpCode);
        // file found at server
        if (httpCode == HTTP_CODE_OK) {
          payload = http.getString();
        }
      } else {
        // String too long or failed to get it
        Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
        displayShowError();
        delay(500);
      }
      http.end();

      // Now we can display the data on the display:
      if (payload.length() > 0 && time_ok_flag) {
        // Here we should have each hours data (240 points, 15 seconds per data point)
        JsonDocument doc;
        DeserializationError error = deserializeJson(doc, payload);
        if (error) {
          Serial.print("deserializeJson() failed: ");
          Serial.println(error.c_str());
          return;
        }
        float total_freq = 0;
        int n = 0;
        for (JsonObject data_item : doc["data"].as<JsonArray>()) {
          const char* data_item_measurementTime = data_item["measurementTime"];  // "2026-04-17T15:21:15Z", ...
          total_freq += (float)data_item["frequency"];                           // 49.942, 49.948, 49.97
          n++;
        }
        freq_data_array[i] = total_freq / (float)n;
      }
    }
  } else {
    Serial.println(F("Time Error - No Data"));
  }
  stopWiFi();  // Reduces power consumption

  // Now we have got the data lets display it on the epaper screen:
  // This prints the data on the serial port, just for testing:
  for (int y = 0; y < 144; y++) {
    Serial.println((String)freq_data_array[y]);
    // We need to convert the freq data into higher resolution for drawing on the screen
    freq_drawing_array[y] = (int)((freq_data_array[y] - 50.0) * -1 * GRAPH_SCALE);  // 0.1 * GRAPHSCALE = pixels height
    // if (y < 72) {
    //   freq_drawing_array[y] = (int)((50.1 - 50.0) * -1 * GRAPH_SCALE);  // 0.1 * GRAPHSCALE = pixels height
    // } else {
    //   freq_drawing_array[y] = (int)((49.9 - 50.0) * -1 * GRAPH_SCALE);  // 0.1 * GRAPHSCALE = pixels height
    // }
  }
  drawFreqGraph(freq_drawing_array, 0, (displayHeight / 2), most_recent_freq);

#endif



  digitalWrite(LED_GPIO, LOW);  // Switch off LED - got data
  // Now go to sleep: zzzzzz....
  // ################ SLEEP TYPE #####################################
  if (WAKE_UP_MODE == "TIMER") {
    // **** TIMER WAKE UP ******************************************
    esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);
    esp_sleep_enable_ext1_wakeup(BUTTON_PIN_BITMASK, ESP_EXT1_WAKEUP_ALL_LOW);  // Also add an external pin wake up
    Serial.println("ESP to sleep for every " + String(TIME_TO_SLEEP) + " s");
    Serial.println(PUSH_GIO39);
  } else if (WAKE_UP_MODE == "TOUCH") {
    // **** TOUCH PAD WAKE UP ******************************************
    // Setup interrupt on Touch Pad 9 (GPIO32)
    touchSleepWakeUpEnable(WAKE_UP_PIN, THRESHOLD);
    esp_sleep_enable_ext1_wakeup(BUTTON_PIN_BITMASK, ESP_EXT1_WAKEUP_ALL_LOW);  // Also add an external pin wake up
    Serial.println(TOUCH_GPIO32);
    Serial.println(PUSH_GIO39);

  } else {
    // Default case, just in case, is touch - you can change this
    // **** TOUCH PAD WAKE UP ******************************************
    // Setup interrupt on Touch Pad 9 (GPIO32)
    touchSleepWakeUpEnable(WAKE_UP_PIN, THRESHOLD);
    esp_sleep_enable_ext1_wakeup(BUTTON_PIN_BITMASK, ESP_EXT1_WAKEUP_ALL_LOW);  // Also add an external pin wake up
    Serial.println(F("No Sleep Mode Set!!!"));
    Serial.println(TOUCH_GPIO32);
    Serial.println(PUSH_GIO39);
  }
  //################################################################
  Serial.println(F("ZZzz.."));
  Serial.flush();
  esp_deep_sleep_start();
  Serial.println(F("This will never be printed"));
}

void loop() {
  // Using deep sleep we never enter here!
}
