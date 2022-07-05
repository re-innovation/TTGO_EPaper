//  ***************** E-Paper Display Unit ***************************
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
// E-Paper Screen of the TTGO 2.13" V2.3 ESP32 Unit
// When the touch pad is pressed
// This code wakes up, checks if WiFi credentials are stored
// If not an Acess point is started to enter in the details - these are stored
// If they are stored then the unit does the following (in this example):
// - downloads a random quote and displays the text and author.
//
// Ensure ESP32 is installed~:
// Put: https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json in your preferences
// The Board Manager -> search for "esp32" and include those boards
//
// If using Arduino IDE: Must use "ESP32 Dev Module"
//
// You MUST include the following libraries:
//    WiFiManager by tzapu https://github.com/tzapu/WiFiManager
//    GxEPD by Lewisxhe at Lilygo   Download from here: https://github.com/lewisxhe/GxEPD
//    Then install using library -> add ZIP library
//    
//    
// Annoyingly there are two different EPaper displays used by Lilygo.
// You might need to try both and see which looks best
// Version 1:
// Then need to copy the "GxGDE0213B72B" folder from the examples on my github
// Copy it to the GxEPD folder in your arduino libraries. then place in src along with the other board definitions
// 
// Version 2:
// The EPaper driver is already installed
//    

#include <WiFiManager.h> // https://github.com/tzapu/WiFiManager

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

  // Want to display that we are getting an update
  displayInit();          // Initialise the display.
  displayShowCELogo();
  displayUpdatingScreen();
  displayUpdate();        // actually show the display...

  // Start Wifi - Connect if we have the SSID/PASS or set up AP if not.
  setup_wifi(reset_wifi_flag);

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
  if (payload.length() > 0)
  {
    // Only do this if we have some data
    String quote = parseQuote(payload);
    Serial.println(quote);
    String author = parseAuthor(payload);
    Serial.println(author);

    // ****** Display quote here: *************************
    displayClear();
    displayQuote(quote, author);
    displayUpdate();  // actually show the display...
  }

  digitalWrite(LED_GPIO, LOW);    // Switch off LED - got data
  // Now go to sleep: zzzzzz....

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
