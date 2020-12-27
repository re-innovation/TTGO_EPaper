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
//String  TimeStr, DateStr, ErrorMessage;     // strings to hold time and date

COVID_record_type  covid_data[DATA_STORED];             // This holds the past 100 days of data if needed

String dateArray[DATA_STORED];  // This holds all the dates to look for

const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 0;
const int   daylightOffset_sec = 3600;

RTC_DATA_ATTR int device_mode;

void callback() {
  // Happens if the unit is already awake!
  Serial.println("Touch press");
  // Can be used to do something with additional press... Maybe change from local to national?
}

void setup() {

  pinMode(LED_GPIO, OUTPUT);
  digitalWrite(LED_GPIO, HIGH);   // Switch ON led - show woken up!
  Serial.begin(115200);

  device_mode++;
  if (device_mode > 1)
  {
    device_mode = 0;
  }
  
  // Display we are getting an update
  // Show "Getting info for: Country, City"
  displayInit();          // Initialise the display.
  displayShowCELogo();
  if(device_mode==0)
  {
    displayUpdatingScreen(MY_CITY);
  }
  else if(device_mode==1)
  {
    displayUpdatingScreen("Overview");
  }
  displayUpdate();        // actually show the display...

  // Start Wifi - Connect if we have the SSID/PASS or set up AP if not.
  setup_wifi();

  // Here we get the COVID data from UK GOV website.
  if ((WiFi.status() == WL_CONNECTED))
  {
    // First find the date from an NTP server:
    // Init and get the time
    configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);

    // This just gives us the string for today.
    // What we really want is the UTC in seconds then subtract and convert into date.
    for (int n = 0; n < DATA_STORED; n++)
    {
      // Here we want to create an array of dates to check the data
      // Each one is a different day
      // Adjust the time by a certain amount of seconds and get the date
      dateArray[n] = returnDate((n + 1));
      Serial.print("Date: ");
      Serial.println(dateArray[n]);
      // At this point we have an array of dates to use to find the data.
    }

    // Next we want to get the data
    WiFiClientSecure client1; // wifi client object
    bool got_data;
    int attempts;
    for (int y = 0; y < DATA_STORED; y++)
    {
      got_data = false;
      attempts = 0;
      while (got_data == false && attempts < MAX_ATTEMPTS)
      {
        got_data = obtain_covid_data(client1, dateArray[y], covid_data, y, device_mode);
        attempts++;
      }
    }
    stopWiFi(); // Reduces power consumption

    // Refresh screen if data was received OK, otherwise wait until the next timed check
    if (DEBUG_FLAG == true)
    {
      Serial.println(); // Give us a new line,
      for (int ii = 0; ii < DATA_STORED; ii++)
      {
        Serial.print(covid_data[ii].date);
        Serial.print(" : "); Serial.print(covid_data[ii].areaName);
        Serial.print(" : "); Serial.print(covid_data[ii].newCases);
        Serial.print(" : "); Serial.print(covid_data[ii].totalCases);
        Serial.print(" : "); Serial.print(covid_data[ii].newDeaths);
        Serial.print(" : "); Serial.println(covid_data[ii].totalDeaths);
      }
    }

    // ****** Display all the data here: *************************
    displayClear();
    // Here we need to deal with data which is JSON format & also display it.
    displayCOVIDInfo(covid_data);
    displayCOVIDGraph(covid_data, 10, 85, 50, 220);
    displayUpdate();  // actually show the display...
  }
  digitalWrite(LED_GPIO, LOW);    // Switch off LED - got data

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

String returnDate(int _n)
{
  struct tm timeinfo;
  while (!getLocalTime(&timeinfo)) {
    Serial.println("Failed to obtain time");
    delay(500);
  }
  time_t timeUTC = mktime(&timeinfo);

  // Want to convert tm into UTC seconds.
  // Then we can convert back into different dates by subtracting seconds.
  timeUTC = timeUTC - (_n * 86400); // Remove seconds in a day

  //  // Set timezone to China Standard Time
  //  setenv("TZ", "CST-8", 1);
  //  tzset();

  localtime_r(&timeUTC, &timeinfo);

  //  Serial.print("UTC value is: ");
  //  Serial.println(timeUTC);

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
