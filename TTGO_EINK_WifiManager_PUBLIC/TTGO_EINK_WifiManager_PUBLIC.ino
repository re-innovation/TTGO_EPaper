// This is a basic test just to show text on the
// E-Paper Screen of the TTGO 2.19" V2.3 ESP32 Unit


// Based on code from:
// EPaper Display code:


// Wifimanager Code:
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

// SSID and PW for your Router
String Router_SSID;
String Router_Pass;

// You will need to install and include the following libraries
// As of 25/06/2020


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

#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"

#include <WiFiClientSecure.h>

// ********* For the ADAFRUIT IO MQTT CONNECTION **********************/
// WiFiFlientSecure for SSL/TLS support
//WiFiClientSecure client;
WiFiClient client;
// Setup the MQTT client class by passing in the WiFi client and MQTT server and login details.
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);
//// io.adafruit.com SHA1 fingerprint
//static const char *fingerprint PROGMEM = "77 00 54 2D DA E7 D8 03 27 31 23 99 EB 27 DB CB A5 4C 57 18";

// io.adafruit.com SHA1 fingerprint
//const char* fingerprint = "26 96 1C 2A 51 07 FD 15 80 96 93 AE F7 32 CE B9 0D 01 55 C4";



/************* Feeds for Adafruit IO *********************************/
// Setup a feed called 'test' for publishing.
// Notice MQTT paths for AIO follow the form: <username>/feeds/<feedname>
//Adafruit_MQTT_Publish fermenterTemp = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/fermenterTemp");
//Adafruit_MQTT_Publish airTemp = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/fermenterAirTemp");

// Setup a feed called 'airRadiation' for subscribing to changes on the airRadiation Feed
Adafruit_MQTT_Subscribe airRadiation = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/airradiation", MQTT_QOS_1);

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

int n = 0;  // Just a counter

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
  Serial.println("\nStarting AutoConnectAP");

  // Use this to default DHCP hostname to ESP8266-XXXXXX or ESP32-XXXXXX
  //ESP_WiFiManager ESP_wifiManager;
  // Use this to personalize DHCP hostname (RFC952 conformed)
  ESP_WiFiManager ESP_wifiManager("AutoConnectAP");

  ESP_wifiManager.setDebugOutput(true);

  //set custom ip for portal
  ESP_wifiManager.setAPStaticIPConfig(IPAddress(192, 168, 4, 1), IPAddress(192, 168, 4, 1), IPAddress(255, 255, 255, 0));

  ESP_wifiManager.setMinimumSignalQuality(-1);
  // Set static IP, Gateway, Subnetmask, DNS1 and DNS2. New in v1.0.5+
  //ESP_wifiManager.setSTAStaticIPConfig(stationIP, gatewayIP, netMask, dns1IP, dns2IP);

  // We can't use WiFi.SSID() in ESP32 as it's only valid after connected.
  // SSID and Password stored in ESP32 wifi_ap_record_t and wifi_config_t are also cleared in reboot
  // Have to create a new function to store in EEPROM/SPIFFS for this purpose
  Router_SSID = ESP_wifiManager.WiFi_SSID();
  Router_Pass = ESP_wifiManager.WiFi_Pass();

  //Remove this line if you do not want to see WiFi password printed
  // Serial.println("Stored: SSID = " + Router_SSID + ", Pass = " + Router_Pass);

  if (Router_SSID != "")
  {
    ESP_wifiManager.setConfigPortalTimeout(5); //If no access point name has been previously entered disable timeout.
    Serial.println("Got stored Credentials. Timeout 5s");
  }
  else
  {
    Serial.println("No stored Credentials. No timeout");
  }

  String chipID = String(ESP_getChipId(), HEX);
  chipID.toUpperCase();

  // SSID and PW for Config Portal
  String AP_SSID = "ESP_" + chipID + "_AutoConnectAP";
  String AP_PASS = "password";

  // Get Router SSID and PASS from EEPROM, then open Config portal AP named "ESP_XXXXXX_AutoConnectAP" and PW "MyESP_XXXXXX"
  // 1) If got stored Credentials, Config portal timeout is 60s
  // 2) If no stored Credentials, stay in Config portal until get WiFi Credentials
  ESP_wifiManager.autoConnect(AP_SSID.c_str(), AP_PASS.c_str());
  //or use this for Config portal AP named "ESP_XXXXXX" and NULL password
  //ESP_wifiManager.autoConnect();

  //if you get here you have connected to the WiFi
  Serial.println("WiFi connected");
}


//void verifyFingerprint() {
//
//  const char* host = AIO_SERVER;
//
//  Serial.print("Connecting to ");
//  Serial.println(host);
//
//  if (! client.connect(host, AIO_SERVERPORT)) {
//    Serial.println("Connection failed. Halting execution.");
//    while (1);
//  }
//  delay(500);
//  if (client.verify(fingerprint, host)) {
//    Serial.println("Connection secure.");
//  } else {
//    Serial.println("Connection insecure! Halting execution.");
//    while (1);
//  }
//}


void setup() {
  Serial.begin(115200);

  displayInit();
  //display.fillScreen(GxEPD_WHITE);
  displayText("Starting...", 30, RIGHT_ALIGNMENT);
  display.update();

  // Sort out Wifi
  setup_wifi();

  // check the fingerprint of io.adafruit.com's SSL cert
  //client.setCACert(fingerprint);
  //client.setCertificate(fingerprint);
  //client.setPrivateKey(fingerprint);
  //verifyFingerprint();
  mqtt.subscribe(&airRadiation);

}

void loop() {
  MQTT_connect();

  Adafruit_MQTT_Subscribe *subscription;
  while ((subscription = mqtt.readSubscription(5000))) {
    if (subscription == &airRadiation) {
      Serial.print(F("Got: "));
      Serial.println((char *)airRadiation.lastread);
      dataValue = (char *)airRadiation.lastread;
    }
  }

  if (dataValue != oldDataValue)
  {
    display.fillScreen(GxEPD_WHITE);
    display.setTextSize(1);
    displayText("Radiation:", 10, LEFT_ALIGNMENT);
    display.setTextSize(4);
    displayText(dataValue, 70, LEFT_ALIGNMENT);
    display.setTextSize(0);
    display.update();
    oldDataValue = dataValue;

  }

  delay(1000);
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
