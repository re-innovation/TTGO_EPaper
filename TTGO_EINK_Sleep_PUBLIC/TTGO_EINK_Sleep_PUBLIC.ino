// This is a basic test for sleep wakeup mode on the
// E-Paper Screen of the TTGO 2.19" V2.3 ESP32 Unit
//
// This code wakes up, checks if WiFi credentials are stored
// If not an Acess point is started to enter in the details
// If they are then the unit wakes up and subscribes to a MQTT channel
// The unit then sleeps until a set time

#if !( defined(ESP8266) ||  defined(ESP32) )
#error This code is intended to run on the ESP8266 or ESP32 platform! Please check your Tools->Board setting.
#endif

//Ported to ESP32
#ifdef ESP32
#include <esp_wifi.h>
#include <WiFi.h>
#include <WiFiClient.h>
#define ESP_getChipId()   ((uint32_t)ESP.getEfuseMac())
#else
#include <ESP8266WiFi.h>          //https://github.com/esp8266/Arduino
//needed for library
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#define ESP_getChipId()   (ESP.getChipId())
#endif

// ESP_wfifmanager
#include <ESP_WiFiManager.h>              //https://github.com/khoih-prog/ESP_WiFiManager
#include <WiFiClientSecure.h>

// include library, include base class, make path known
#include <GxEPD.h>
#include <GxIO/GxIO_SPI/GxIO_SPI.h>
#include <GxIO/GxIO.h>

#include <Fonts/FreeMono9pt7b.h>
#include <Fonts/FreeMonoBoldOblique9pt7b.h>
#include <Fonts/FreeMonoBold9pt7b.h>
#include <Fonts/FreeMonoOblique9pt7b.h>
#include <Fonts/FreeSans9pt7b.h>
#include <Fonts/FreeSansBold9pt7b.h>
#include <Fonts/FreeSansBoldOblique9pt7b.h>
#include <Fonts/FreeSansOblique9pt7b.h>
#include <Fonts/FreeSerif9pt7b.h>
#include <Fonts/FreeSerifBold9pt7b.h>
#include <Fonts/FreeSerifBoldItalic9pt7b.h>
#include <Fonts/FreeSerifItalic9pt7b.h>


//#define DEFALUT_FONT  FreeMono9pt7b
// #define DEFALUT_FONT  FreeMonoBoldOblique9pt7b
// #define DEFALUT_FONT FreeMonoBold9pt7b
// #define DEFALUT_FONT FreeMonoOblique9pt7b
#define DEFALUT_FONT FreeSans9pt7b
// #define DEFALUT_FONT FreeSansBold9pt7b
// #define DEFALUT_FONT FreeSansBoldOblique9pt7b
// #define DEFALUT_FONT FreeSansOblique9pt7b
// #define DEFALUT_FONT FreeSerif9pt7b
// #define DEFALUT_FONT FreeSerifBold9pt7b
// #define DEFALUT_FONT FreeSerifBoldItalic9pt7b
// #define DEFALUT_FONT FreeSerifItalic9pt7b

#include "Esp.h"
#include "board_def.h"
#include "Config.h"

// SSID and PW for your Router
String Router_SSID;
String Router_Pass;

// SSID and PW for AP Config Portal
String AP_SSID;
String AP_PASS;

#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"
// ********* For the ADAFRUIT IO MQTT CONNECTION **********************/
// WiFiFlientSecure for SSL/TLS support
//WiFiClientSecure client;

WiFiClient client;
// Setup the MQTT client class by passing in the WiFi client and MQTT server and login details.
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);

//// io.adafruit.com SHA1 fingerprint
//static const char *fingerprint PROGMEM = "77 00 54 2D DA E7 D8 03 27 31 23 99 EB 27 DB CB A5 4C 57 18";

/************* Feeds for Adafruit IO *********************************/
// Subscribe to a feed called 'airRadiation' for subscribing to changes on the airRadiation Feed
// This is a public feed that I have set up
Adafruit_MQTT_Subscribe airRadiation = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/airradiation");
Adafruit_MQTT_Publish getRadiation = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/airradiation/get");

String dataValue;
String oldDataValue;

const GFXfont *fonts[] = {
  &FreeMono9pt7b,
  &FreeMonoBoldOblique9pt7b,
  &FreeMonoBold9pt7b,
  &FreeMonoOblique9pt7b,
  &FreeSans9pt7b,
  &FreeSansBold9pt7b,
  &FreeSansBoldOblique9pt7b,
  &FreeSansOblique9pt7b,
  &FreeSerif9pt7b,
  &FreeSerifBold9pt7b,
  &FreeSerifBoldItalic9pt7b,
  &FreeSerifItalic9pt7b
};

typedef enum {
  RIGHT_ALIGNMENT = 0,
  LEFT_ALIGNMENT,
  CENTER_ALIGNMENT,
} Text_alignment;


GxIO_Class io(SPI, ELINK_SS, ELINK_DC, ELINK_RESET);
GxEPD_Class display(io, ELINK_RESET, ELINK_BUSY);

//Badge_Info_t info;
static const uint16_t input_buffer_pixels = 20;       // may affect performance
static const uint16_t max_palette_pixels = 256;       // for depth <= 8
uint8_t mono_palette_buffer[max_palette_pixels / 8];  // palette buffer for depth <= 8 b/w
uint8_t color_palette_buffer[max_palette_pixels / 8]; // palette buffer for depth <= 8 c/w
uint8_t input_buffer[3 * input_buffer_pixels];        // up to depth 24

void displayText(const String &str, int16_t y, uint8_t alignment)
{
  int16_t x = 0;
  int16_t x1, y1;
  uint16_t w, h;
  display.setCursor(x, y);
  display.getTextBounds(str, x, y, &x1, &y1, &w, &h);

  switch (alignment) {
    case RIGHT_ALIGNMENT:
      display.setCursor(display.width() - w - x1, y);
      break;
    case LEFT_ALIGNMENT:
      display.setCursor(0, y);
      break;
    case CENTER_ALIGNMENT:
      display.setCursor(display.width() / 2 - ((w + x1) / 2), y);
      break;
    default:
      break;
  }
  display.println(str);
}

void displayInit(void)
{
  static bool isInit = false;
  if (isInit) {
    return;
  }
  isInit = true;
  display.init();
  display.setRotation(1);
  display.eraseDisplay();
  display.setTextColor(GxEPD_BLACK);
  display.setFont(&DEFALUT_FONT);
  display.setTextSize(0);
  display.fillScreen(GxEPD_WHITE);
  display.update();
}

void setup_wifi() {
  // Only want to do an AP if we dont already have the SSID and password saved?
  Serial.println("\nChecking if SSID available already");

  // Use this to default DHCP hostname to ESP8266-XXXXXX or ESP32-XXXXXX
  //ESP_WiFiManager ESP_wifiManager;
  // Use this to personalize DHCP hostname (RFC952 conformed)
  ESP_WiFiManager ESP_wifiManager("AutoConnectAP");

  //reset settings - for testing
  //ESP_wifiManager.resetSettings();

  ESP_wifiManager.setDebugOutput(true);

  //set custom ip for portal (set to static IP of 192.168.4.1)
  ESP_wifiManager.setAPStaticIPConfig(IPAddress(192, 168, 4, 1), IPAddress(192, 168, 4, 1), IPAddress(255, 255, 255, 0));

  ESP_wifiManager.setMinimumSignalQuality(-1);

  // We can't use WiFi.SSID() in ESP32 as it's only valid after connected.
  // SSID and Password stored in ESP32 wifi_ap_record_t and wifi_config_t are also cleared in reboot
  // Have to create a new function to store in EEPROM/SPIFFS for this purpose
  Router_SSID = ESP_wifiManager.WiFi_SSID();
  Router_Pass = ESP_wifiManager.WiFi_Pass();

  //Remove this line if you do not want to see WiFi password printed
  //Serial.println("Stored: SSID = " + Router_SSID + ", Pass = " + Router_Pass);

  if (Router_SSID == "")
  {
    // If there is no stored SSID then start an access point
    Serial.println("No stored Credentials. Start AP Mode for Entering Details");
    String chipID = String(ESP_getChipId(), HEX);
    chipID.toUpperCase();
    // SSID and PW for Config Portal
    AP_SSID = "ESP_" + chipID + "_AP";
    AP_PASS = "password";
    Serial.print("AP SSID:");
    Serial.print(AP_SSID);
    Serial.print(" with PW:");
    Serial.print(AP_PASS);
    Serial.print(" on 198.162.4.1");

    // Want to display the info here for people to log in:
    displayInit();
    display.setTextSize(1);
    displayText("Please connect to:", 15, CENTER_ALIGNMENT);
    displayText("198.168.4.1", 30, CENTER_ALIGNMENT);
    displayText("SSID:", 45, CENTER_ALIGNMENT);
    displayText(AP_SSID, 60, CENTER_ALIGNMENT);
    displayText("PASS:", 75, CENTER_ALIGNMENT);
    displayText(AP_PASS, 90, CENTER_ALIGNMENT);
    display.update();

    //it starts an access point
    //and goes into a blocking loop awaiting configuration
    if (!ESP_wifiManager.startConfigPortal(AP_SSID.c_str(), AP_PASS.c_str()))
    {
      Serial.println("Not connected to WiFi but continuing anyway.");
    }
    else
    {
      //if you get here you have connected to the WiFi
      Serial.println("connected to WiFi...");
    }
  }
  // At this point we have the correct SSID and PW configured and start the wifi

#ifdef ESP32
  WiFi.mode(WIFI_STA);

  // We start by connecting to a WiFi network
  Serial.print("Connecting to ");
  Serial.print(Router_SSID);

  WiFi.begin(Router_SSID.c_str(), Router_Pass.c_str());

  while (WiFi.status() != WL_CONNECTED)
  {
    WiFi.begin(Router_SSID.c_str(), Router_Pass.c_str());
    //Keep trying until we are connected
    Serial.print(".");
    delay(1000);
  }
#endif
  //if you get here you have connected to the WiFi
  Serial.println("WiFi connected");
}


void setup() {
  Serial.begin(115200);

  // Sort out Wifi
  setup_wifi();

  // MQTT subscription with adafruit IO is
  mqtt.subscribe(&airRadiation);

  MQTT_connect();

  Adafruit_MQTT_Subscribe *subscription;

  getRadiation.publish(0);  // This publishes to /get of the radiation feed

  while ((subscription = mqtt.readSubscription(5000))) {
    if (subscription == &airRadiation) {
      Serial.print(F("Got: "));
      Serial.println((char *)airRadiation.lastread);
      dataValue = (char *)airRadiation.lastread;
    }
  }
  // Should always get a data value here... 
  // But need to handle it if we dont! (not yet implemented)
  
  // Got the data now need to display it...
  displayInit();
  display.setTextSize(1);
  displayText("Radiation:", 15, CENTER_ALIGNMENT);
  display.setTextSize(4);
  // Want to convert the dataValue (a string) into an int
  // Then re-cast to a string with limited decimal places
  int dataNumber = dataValue.toInt();
  displayText(String(dataNumber), 80, CENTER_ALIGNMENT);
  display.setTextSize(0);
  display.update();

  /* Now go to sleep:
  First we configure the wake up source
  We set our ESP32 to wake up every 5 seconds
  */
  esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);
  Serial.println("Setup ESP32 to sleep for every " + String(TIME_TO_SLEEP) + " Seconds");
  
  Serial.println("Going to sleep now");
  Serial.flush(); 
  esp_deep_sleep_start();
  Serial.println("This will never be printed"); 
}

void loop()
{
 // Using deep sleep we never enter here! 
}

// Function to connect and reconnect as necessary to the MQTT server.
// Should be called in the loop function and it will take care if connecting.
void MQTT_connect() {
  int8_t ret;

  // Stop if already connected.
  if (mqtt.connected()) {
    return;
  }

  Serial.print("Connecting to MQTT... ");
  uint8_t retries = 3;
  while ((ret = mqtt.connect()) != 0) { // connect will return 0 for connected
    Serial.println(mqtt.connectErrorString(ret));
    Serial.println("Retrying MQTT connection in 5 seconds...");
    mqtt.disconnect();
    delay(5000);  // wait 10 seconds
    retries--;
    if (retries == 0) {
      // basically die and wait for WDT to reset me
      while (1);
    }
  }
  Serial.println("MQTT Connected!");
}
