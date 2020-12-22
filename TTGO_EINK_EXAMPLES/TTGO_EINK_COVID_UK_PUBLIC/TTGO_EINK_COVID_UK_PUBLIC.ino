//  ***************** E-Paper Display Unit ***************************
//  ***************** Example Code - COVID Data UK Unit **************
//
//  This code was:
//  Written by: Matt Little
//  Date: 21/12/2020
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
//  - checks the local COVID cases data
//
// If using Arduino IDE: Must use "DOIT ESP32 DEVKIT V1"
//
// You MUST include the following libraries:
//    GxEPD by Jean-Marc Zingg        - I needed to make sure I had the correct EPaper driver.
//    ESP_Wifi_Manager by Khoi Howang - I had to use the older type with cpp and h files. See github for details.
//    ESP32-targz.h by *****          - For opening the gzip data returned.
//

#if !( defined(ESP8266) ||  defined(ESP32) )
#error This code is intended to run on the ESP8266 or ESP32 platform! Please check your Tools->Board setting.
#endif

//Ported to ESP32
#ifdef ESP32
#include <esp_wifi.h>
#include <WiFi.h>
//#include <WiFiClient.h>
#include <WiFiClientSecure.h>
#include <HTTPClient.h>
#define  ESP_getChipId()   ((uint32_t)ESP.getEfuseMac())

#else
#include <ESP8266WiFi.h>          //https://github.com/esp8266/Arduino
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#define  ESP_getChipId()   (ESP.getChipId())
#endif

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
#include "covid.h"

// Variables for the COVID data calls
boolean RxCOVID = false;                    // Initialise these - have we got the data?
String  TimeStr, DateStr, ErrorMessage;     // strings to hold time and date

COVID_record_type  covid_data[1];

const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = -86400;   
// We want to get yesturdays data, as todays not availabl yet?
// Subtract 1 days worth of seconds
const int   daylightOffset_sec = 3600;

void callback() {
  // Happens if the unit is already awake!
  Serial.println("Touch press");
  // Can be used to do something with additional press?
}

void setup() {

  pinMode(LED_GPIO, OUTPUT);
  digitalWrite(LED_GPIO, HIGH);   // Switch ON led - show woken up!
  Serial.begin(115200);

  // Display we are getting an update
  // Show "Getting info for: Country, City"
  displayInit();          // Initialise the display.
  displayShowCELogo();
  displayUpdatingScreen();
  displayUpdate();        // actually show the display...

  // Start Wifi - Connect if we have the SSID/PASS or set up AP if not.
  setup_wifi();

  // Here we get the weather data from Open Weather Maps
  // Need to register account with them and get API key - put that into the Config.h file
  if ((WiFi.status() == WL_CONNECTED))
  {
    // First find the date from an NTP server:

    // Init and get the time
    configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
    //printLocalTime();
    String date = returnDate();
    Serial.print("Date: ");
    Serial.println(date);

    // Next we want to get the data
    byte Attempts = 1;
    WiFiClientSecure client1; // wifi client object

    while ((RxCOVID == false) && Attempts <= MAX_ATTEMPTS)
    {
      // Try up to MAX_ATTEMPTS for data
      // ******** SORT OUT DATE RANGE *************

      if (RxCOVID  == false) RxCOVID  = obtain_covid_data(client1, date, covid_data);
      Attempts++;
    }
    // Refresh screen if data was received OK, otherwise wait until the next timed check
    if (RxCOVID)
    {
      stopWiFi(); // Reduces power consumption
      // ****** Display all the data here: *************************
      displayClear();
      // Here we need to deal with data which is JSON format & also display it.
      displayCOVIDInfo(covid_data);
      displayUpdate();  // actually show the display...
    }
    else
    {
      stopWiFi(); // Reduces power consumption
      Serial.println("Failed to get Data");
      // Want to show this on the screen as well
      // ****** TO DO ***************************
    }
  }
  //delay(1000);
  digitalWrite(LED_GPIO, LOW);    // Switch off LED - got data

  // To DO: Can I use both sleep and touch mode?

  // Now go to sleep: zzzzzz....
  // Setup interrupt on Touch Pad 9 (GPIO32)
  // This is called T8 NOT T9 due to an error in Arduino ID:
  touchAttachInterrupt(WAKE_UP_PIN, callback, THRESHOLD);
  esp_sleep_enable_touchpad_wakeup();
  Serial.println("ESP will wake up on touch - pin GPIO32");
  Serial.println("Going to sleep now");
  //Serial.flush();
  esp_deep_sleep_start();
  Serial.println("This will never be printed");
}

void loop()
{
  // Using deep sleep we never enter here!
}

String returnDate() {
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    Serial.println("Failed to obtain time");
    return ("NA");
  }

  String timeDate = (String)(timeinfo.tm_year + 1900);
  if ((timeinfo.tm_mon + 1) < 10)
  {
    timeDate += "-0" + (String)(timeinfo.tm_mon + 1);
  }
  else
  {
    timeDate += "-" + (String)(timeinfo.tm_mon + 1);
  }
  if (timeinfo.tm_mday < 10)
  {
    timeDate += "-0" + (String)(timeinfo.tm_mday);
  }
  else
  {
    timeDate += "-" + (String)(timeinfo.tm_mday);
  }
  return (timeDate);
}
