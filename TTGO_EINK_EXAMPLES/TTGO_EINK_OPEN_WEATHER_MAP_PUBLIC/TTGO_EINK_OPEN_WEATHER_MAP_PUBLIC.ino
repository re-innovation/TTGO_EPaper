//  ***************** E-Paper Display Unit ***************************
//  ***************** Example Code - Weather Unit ********************
//
//  This code was:
//  Written by: Matt Little
//  Date: 11/11/2020
//  But is based on lots of other peoples examples!
//  This code is open and can be shared freely.
//  Contact:  hello@curiouselectric.co.uk
//
// This is a test code for touch wakeup mode on the
// E-Paper Screen of the TTGO 2.19" V2.3 ESP32 Unit
// When the touch pad is pressed
// This code wakes up, checks if WiFi credentials are stored
// If not an Acess point is started to enter in the details - these are stored
// If they are stored then the unit does the following (in this example):
//  - checks the weather from Open Weather API
//
// Ensure ESP32 is installed~:
// Put: https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json in your preferences
// The Board Manager -> search for "esp32" and include those boards
//
// If using Arduino IDE: Must use "ESP32 Dev Module"
//
// You MUST include the following libraries:
//    WiFiManager by tzapu           https://github.com/tzapu/WiFiManager   (via Library Manager)
//    ArduinoJson.h                // https://github.com/bblanchon/ArduinoJson   (via Library Manager)
//    GxEPD by Lewisxhe at Lilygo   Download from here: https://github.com/lewisxhe/GxEPD
//    Then install using library -> add ZIP library
//
// You MUST then copy the "GxGDE0213B72B_CE" & "GxDEPG0213BN_CE" folders from the examples in the Curious Electric repository
// Copy them BOTH to the "your sketchbook\yourlibraries\GxEPD\src" folder in your arduino libraries (on my computer this is "D:\sketchbook\libraries\GxEPD\src\"). 
// You will see a big list of the various boards available.
// Please delete the original "GxGDE0213B72B" & "GxDEPG0213BN" as they confuse the compiler.

// Annoyingly there are two different EPaper displays used by Lilygo.
// You might need to try both and see which looks best
// Change this within the "board_def.h" page in this arduino code - comment out either version 1 or 2.
// This will be a process of trial and error - the wrong driver will give you a 'washed out' display and look faded:
// eg:
// Version 1:
// //#include <GxGDE0213B72B_CE/GxGDE0213B72B.h>    // 2.13" b/w     GxGDE0213B72 升级版本 默认LilyGO的出厂屏幕都是这种
// Version 2:
// #include <GxDEPG0213BN_CE/GxDEPG0213BN.h>      // 2.13" b/w  form Curious Electric


#include "EEPROM.h"

#include <WiFiManager.h> // https://github.com/tzapu/WiFiManager

// This is for dealing with the JSON returned data.
#include <ArduinoJson.h>     // https://github.com/bblanchon/ArduinoJson
#include <TimeLib.h>  // Include time functions
#include "Config.h"

#include "CE_Icons.h"        // Curious Electric Icons
#include "display.h"
#include "board_def.h"
#include "wifi_local.h"
#include "epaper_fonts.h"
#include "time.h"            // Built-in
#include "utilities.h"
#include "weather.h"


// WiFiFlientSecure for SSL/TLS support
//WiFiClientSecure client;
WiFiClient client;

// Variables for the weather calls
boolean RxWeather = false, RxForecast = false;    // Initialise these - have we got the data?
String  TimeStr, DateStr, ErrorMessage;           // strings to hold time and date

Forecast_record_type  WxConditions[1];
Forecast_record_type  WxForecast[MAX_WEATHER_READINGS];

void callback() {
  // Happens if the unit is already awake!
  Serial.println("Touch press");
  // Can be used to do something with additional press?
}

void setup() {

  pinMode(LED_GPIO, OUTPUT);
  digitalWrite(LED_GPIO, HIGH);   // Switch ON led - show woken up!
  Serial.begin(115200);

  // What is the wake up reason?
  bool reset_wifi_flag = print_wakeup_reason();
  print_wakeup_touchpad();
  Serial.print("Put unit into AP mode? ");
  Serial.println(reset_wifi_flag);

  //Init EEPROM
  EEPROM.begin(1000);

  // Check EEPROM for the unit data:
  owm_settings.OWM_KEY      = EEPROM.readString(0x0);
  owm_settings.OWM_SERVER   = EEPROM.readString(0x100);
  owm_settings.MY_CITY      = EEPROM.readString(0x150);
  owm_settings.MY_COUNTRY   = EEPROM.readString(0x200);
  owm_settings.MY_LANGUAGE  = EEPROM.readString(0x250);
  owm_settings.MY_HEMISPHERE = EEPROM.readString(0x260);
  owm_settings.MY_UNITS     = EEPROM.readString(0x270);
  owm_settings.MY_TIMEZONE  = EEPROM.readString(0x280);

  printStoredData();
  // Display we are getting an update
  // Show "Getting info for: Country, City"
  displayInit();          // Initialise the display.
  displayShowCELogo();
  displayUpdatingScreen();
  displayUpdate();        // actually show the display...

  // Start Wifi - Connect if we have the SSID/PASS or set up AP if not.
  setup_wifi(reset_wifi_flag);

  // Here we get the weather data from Open Weather Maps
  // Need to register account with them and get API key - put that into the Config.h file
  if ((WiFi.status() == WL_CONNECTED))
  {
    // Here want to get the weather data:
    byte Attempts = 1;
    WiFiClient client1; // wifi client object
    while ((RxWeather == false || RxForecast == false) && Attempts <= MAX_ATTEMPTS)
    { // Try up to MAX_ATTEMPTS for Weather and Forecast data
      if (RxWeather  == false) RxWeather  = obtain_wx_data(client1, "weather", WxConditions, WxForecast);
      if (RxForecast == false) RxForecast = obtain_wx_data(client1, "forecast", WxConditions, WxForecast);
      Attempts++;
    }

    // Refresh screen if data was received OK, otherwise wait until the next timed check
    if (RxWeather || RxForecast)
    {
      stopWiFi(); // Reduces power consumption
      // ****** Display all the data here: *************************
      displayClear();
      // Here we need to deal with data which is JSON format & also display it.
      displayWeatherInfo(WxConditions, WxForecast);
      displayUpdate();  // actually show the display...
    }
    else
    {
      stopWiFi(); // Reduces power consumption
      Serial.println("Failed to get Wx Data, " + String(RxWeather ? "" : " Failed RxWeather") + String(RxForecast ? "" : " Failed RxForecast"));
    }
  }
  digitalWrite(LED_GPIO, LOW);    // Switch off LED - got data
  // ################ SLEEP TYPE #####################################
  if (WAKE_UP_MODE == "TIMER")
  {
    // **** TIMER WAKE UP ******************************************
    esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);
    esp_sleep_enable_ext1_wakeup(BUTTON_PIN_BITMASK, ESP_EXT1_WAKEUP_ALL_LOW);        // Also add an external pin wake up
    Serial.println("ESP to sleep for every " + String(TIME_TO_SLEEP) + " s");
    Serial.println("Push GPIO39 to start config AP");
  }
  else if (WAKE_UP_MODE == "TOUCH")
  {
    // **** TOUCH PAD WAKE UP ******************************************
    // Setup interrupt on Touch Pad 9 (GPIO32)
    touchAttachInterrupt(WAKE_UP_PIN, callback, THRESHOLD);
    esp_sleep_enable_touchpad_wakeup();
    esp_sleep_enable_ext1_wakeup(BUTTON_PIN_BITMASK, ESP_EXT1_WAKEUP_ALL_LOW);        // Also add an external pin wake up
    Serial.println("ESP will wake up on touch - pin GPIO32");
    Serial.println("Push GPIO39 to start config AP");
  }
  else
  {
    // Default case, just in case, is touch - you can change this
    // **** TOUCH PAD WAKE UP ******************************************
    // Setup interrupt on Touch Pad 9 (GPIO32)
    touchAttachInterrupt(WAKE_UP_PIN, callback, THRESHOLD);
    esp_sleep_enable_touchpad_wakeup();
    esp_sleep_enable_ext1_wakeup(BUTTON_PIN_BITMASK, ESP_EXT1_WAKEUP_ALL_LOW);        // Also add an external pin wake up
    Serial.println("No Sleep Mode Set!!!");
    Serial.println("ESP will wake up on touch - pin GPIO32");
    Serial.println("Push GPIO39 to start config AP");
  }
  //################################################################
  Serial.println("Going to sleep now");
  Serial.flush();
  esp_deep_sleep_start();
  Serial.println("This will never be printed");
}

void loop()
{
  // Using deep sleep we never enter here!
}
