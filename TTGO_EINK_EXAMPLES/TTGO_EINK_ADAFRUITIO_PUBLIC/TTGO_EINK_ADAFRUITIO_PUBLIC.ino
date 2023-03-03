//  ***************** E-Paper Display Unit *****************************************
//  ***************** Example Code - AdafruitIO connection Unit ********************
//
//  This code was:
//  Written by: Matt Little
//  Date: 11/11/2020
//  Updated: 30/6/2022
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
// 1 - checks an MQTT channel through Adafruit IO
//
//
// Please follow the set-up instructions found here: https://github.com/re-innovation/TTGO_EPaper/blob/master/README.md
// This explains the libraries needed to upload this example code.
//
// When uploading using Arduino IDE: Must use "ESP32 Dev Module"
//

#include <WiFiManager.h> // https://github.com/tzapu/WiFiManager

// This is for dealing with the JSON returned data.
#include <ArduinoJson.h>     // https://github.com/bblanchon/ArduinoJson
#include "Config.h"

#include "CE_Icons.h"        // Curious Electric Icons
#include "display.h"
#include "board_def.h"
#include "wifi_local.h"
#include "epaper_fonts.h"
#include "time.h"            // Built-in
#include "utilities.h"
#include "mqtt.h"

// ********* For the ADAFRUIT IO MQTT CONNECTION **********************/
// WiFiFlientSecure for SSL/TLS support
//WiFiClientSecure client;
WiFiClient client;
// Setup the MQTT client class by passing in the WiFi client and MQTT server and login details.
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);
// io.adafruit.com SHA1 fingerprint
//static const char *fingerprint PROGMEM = "77 00 54 2D DA E7 D8 03 27 31 23 99 EB 27 DB CB A5 4C 57 18";
/************* Feeds for Adafruit IO *********************************/
// Subscribe to a feed called 'airRadiation' for subscribing to changes on the airRadiation Feed
// This is a public feed that I have set up
//Adafruit_MQTT_Subscribe airRadiation = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/airradiation");
//Adafruit_MQTT_Publish   getRadiation = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/airradiation/get");

Adafruit_MQTT_Subscribe airRadiation = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME AIO_FEED);
Adafruit_MQTT_Publish   getRadiation = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME  AIO_FEED  "/get");


String dataValue;
String oldDataValue;
int radiationValue;


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
  // ***** TO DO ****
  // Show "Getting info for: Country, City"
  displayInit();          // Initialise the display.
  displayShowCELogo();
  displayUpdatingScreen();
  displayUpdate();        // actually show the display...

  // Start Wifi - Connect if we have the SSID/PASS or set up AP if not.
  setup_wifi(reset_wifi_flag);

  if ((WiFi.status() == WL_CONNECTED))
  {
    // Only do this if we are connected, otherwise go to sleep!

    // Get radiation data from Adafruit IO via MQTT call
    // This is an example to show how to get info from Adafruit.IO
    radiationValue = getRadiationMQTT(mqtt, airRadiation, getRadiation);
    displayClear();
    displayRadiationInfo(radiationValue);
    displayUpdate();        // actually show the display...
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
