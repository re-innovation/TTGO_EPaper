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
// - downloads a random quote and displays the text and author.
//
// If using Arduino IDE: Must use "ESP32 Dev Module"
//
// You MUST include the following libraries:
//    GxEPD by Jean-Marc Zingg        - I needed to make sure I had the correct EPaper driver.
//    ESP_Wifi_Manager by Khoi Howang - I had to use the older type with cpp and h files. See github for details.
//

#if !( defined(ESP8266) ||  defined(ESP32) )
#error This code is intended to run on the ESP8266 or ESP32 platform! Please check your Tools->Board setting.
#endif

//Ported to ESP32
#ifdef ESP32
#include <esp_wifi.h>
#include <WiFi.h>
#include <WiFiClient.h>
//#include <WiFiClientSecure.h>
#include <HTTPClient.h>
#define  ESP_getChipId()   ((uint32_t)ESP.getEfuseMac())
#else
#include <ESP8266WiFi.h>          //https://github.com/esp8266/Arduino
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#define  ESP_getChipId()   (ESP.getChipId())
#endif

//// This is for dealing with the JSON returned data.
//#include <ArduinoJson.h>     // https://github.com/bblanchon/ArduinoJson
//#include <TimeLib.h>  // Include time functions

#include "Config.h"
#include "CE_Icons.h"        // Curious Electric Icons
#include "display.h"
#include "board_def.h"
#include "wifi_local.h"
#include "epaper_fonts.h"
#include "time.h"            // Built-in
#include "utilities.h"
#include "quote.h"

// Global variables
String payload; // Holds the reply from the HTTP request
String quote; // Holds the final quote
String author;  // Holds the author of the quote
//String textDisplay; // Holds the data to show on the OLED screen
//bool quoteLong = false; // This is a flag. If set then the quote is shown in two parts

void callback() {
  // Happens if the unit is already awake!
  Serial.println("Touch press");
  // Can be used to do something with additional press?
}

void setup() {

  pinMode(LED_GPIO, OUTPUT);
  digitalWrite(LED_GPIO, HIGH);   // Switch ON led - show woken up!
  Serial.begin(115200);

  // Want to display that we are getting an update
  displayInit();          // Initialise the display.
  displayShowCELogo();
  displayUpdatingScreen();
  displayUpdate();        // actually show the display...

  // Start Wifi - Connect if we have the SSID/PASS or set up AP if not.
  setup_wifi();

  // Here we create the URL to check for getting the quote information
  // Sample URL is: http://api.forismatic.com/api/1.0/?method=getQuote&key=457653&format=text&lang=en
  String url = QUOTE_SERVER;
  // Create the full URL with method, format and language
  url += "?method=getQuote&key=";
  url += QUOTE_SEED;
  url += "&format=text&lang=";
  url += QUOTE_LANGUAGE;
  Serial.println(url);

  if ((WiFi.status() == WL_CONNECTED))
  {
    HTTPClient http;
    bool quote_length_flag = false;
    http.begin(url);
    while (quote_length_flag == false)
    {
      int httpCode = http.GET();
      // httpCode will be negative on error
      if (httpCode > 0) {
        // HTTP header has been send and Server response header has been handled
        Serial.printf("[HTTP] GET... code: %d\n", httpCode);
        // file found at server
        if (httpCode == HTTP_CODE_OK) {
          payload = http.getString();
          Serial.println(payload);
          if (payload.length() < 132)
          {
            quote_length_flag = true; // Quote is short so all OK
          }
        }
      } else {
        delay(500);
        // String too long or failed to get it
        Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
      }
    }
    http.end();
  }

  stopWiFi(); // Reduces power consumption

  String quote = parseQuote(payload);
  Serial.println(quote);
  String author = parseAuthor(payload);
  Serial.println(author);

  // ****** Display quote here: *************************
  displayClear();
  displayQuote(quote, author);
  displayUpdate();  // actually show the display...

  digitalWrite(LED_GPIO, LOW);    // Switch off LED - got data
  // Now go to sleep: zzzzzz....

  // ################ SLEEP TYPE #####################################
  //    // **** TIMER WAKE UP ******************************************
  //    esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);
  //    Serial.println("Setup ESP32 to sleep for every " + String(TIME_TO_SLEEP) + " Seconds");

  // **** TOUCH PAD WAKE UP ******************************************
  // Setup interrupt on Touch Pad 9 (GPIO32)
  // This is called T8 NOT T9 due to an error in Arduino ID:
  touchAttachInterrupt(WAKE_UP_PIN, callback, THRESHOLD);
  // touchAttachInterrupt(T8, callback, THRESHOLD);
  esp_sleep_enable_touchpad_wakeup();
  Serial.println("ESP will wake up on touch - pin GPIO32");
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
