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
#include <HTTPClient.h>
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

#include <ArduinoJson.h>     // https://github.com/bblanchon/ArduinoJson

#include "Esp.h"
#include "board_def.h"
#include "Config.h"
//#include "Wx_Icons.h"        // Weather Icons
#include "CE_Icons.h"        // Curious Electric Icons
#include "epaper_fonts.h"
#include "time.h"            // Built-in

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
// io.adafruit.com SHA1 fingerprint
//static const char *fingerprint PROGMEM = "77 00 54 2D DA E7 D8 03 27 31 23 99 EB 27 DB CB A5 4C 57 18";
/************* Feeds for Adafruit IO *********************************/
// Subscribe to a feed called 'airRadiation' for subscribing to changes on the airRadiation Feed
// This is a public feed that I have set up
Adafruit_MQTT_Subscribe airRadiation = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/airradiation");
Adafruit_MQTT_Publish getRadiation = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/airradiation/get");

#include <TimeLib.h>  // Include time functions

String dataValue;
String oldDataValue;
int dataNumber;

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

// Variables for the weather calls
boolean RxWeather = false, RxForecast = false;    // Initialise these - have we got the data?
String  TimeStr, DateStr, ErrorMessage;           // strings to hold time and date

typedef struct { // For current Day and Day 1, 2, 3, etc
  float    lat;
  float    lon;
  String   Dt;
  String   Period;
  float    Temperature;
  float    Humidity;
  String   Icon;
  float    High;
  float    Low;
  float    Rainfall;
  float    Snowfall;
  float    Pressure;
  int      Cloudcover;
  int      Visibility;
  String   Trend;
  float    Winddir;
  float    Windspeed;
  String   Main0;
  String   Forecast0;
  String   Forecast1;
  String   Forecast2;
  String   Description;
  String   Time;
  int      Sunrise;
  int      Sunset;
  String   Country;
  int      TimeUTC;
  int      TimeZone;
} Forecast_record_type;

#define max_readings 5

Forecast_record_type  WxConditions[1];
Forecast_record_type  WxForecast[max_readings];

void setup() {
  pinMode(led_gpio, OUTPUT);
  digitalWrite(led_gpio, HIGH); 
  Serial.begin(115200);

  // Sort out Wifi
  setup_wifi();

  // Get radiation data from Adafruit IO via MQTT call
  dataNumber = getRadiationMQTT();

  // Here we get the weather data from Open Weather Maps
  // Need to register account with them and get API key - in config!
  if ((WiFi.status() == WL_CONNECTED))
  {
    // Here want to get the weather data:
    byte Attempts = 1;
    WiFiClient client1; // wifi client object
    while ((RxWeather == false || RxForecast == false) && Attempts <= 2) { // Try up-to twice for Weather and Forecast data
      if (RxWeather  == false) RxWeather  = obtain_wx_data(client1, "weather");
      if (RxForecast == false) RxForecast = obtain_wx_data(client1, "forecast");
      Attempts++;
    }

    // Refresh screen if data was received OK, otherwise wait until the next timed check
    if (RxWeather || RxForecast) {
      StopWiFi(); // Reduces power consumption
      // ****** Display the info here:
      // Here we need to deal with data which is JSON format.
      // We can choose what we want to display here:
      displayInit();          // Initialise the display if not already done...
      DisplayWeatherInfo();
      display.update();
    } else Serial.println("Failed to get Wx Data, " + String(RxWeather ? "" : " Failed RxWeather") + String(RxForecast ? "" : " Failed RxForecast") + ErrorMessage);
  }
  
  digitalWrite(led_gpio, LOW); 
  /* Now go to sleep: zzzzzz....
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

//#########################################################################################
void DisplayWeatherInfo() {                                // 2.13" e-paper display is 250 x 122 px resolution

  //  DisplayHeadingSection();                                 // Top line of the display
  //  DisplayTempHumiSection(0, 15);                           // Current temperature with Max/Min and Humidity
  //displayText("Radiation: 91", 10, LEFT_ALIGNMENT);

  display.setCursor(0, 11);
  display.println("CPM:");
  display.setCursor(50, 11);
  display.println(dataNumber);
  
  display.setCursor(0, 27);
  // Here want to display the time and date of most recent update:
  String displayDateTime = digitalClockDisplay(WxConditions[0].TimeUTC+WxConditions[0].TimeZone); 
  display.println(displayDateTime);

  display.setCursor(0, 45);
  display.println(WxConditions[0].Forecast0);
  display.setCursor(150, 11);
  display.println("Now:");
  // This shows the forecast for the day:
  DisplayCEIcon(200, 0, WxConditions[0].Icon);          // Weather person depiction of weather

  // This shows the 3 hr forecast for next 5 lots of 3 hours
  DisplayCEIcon(0,   72, WxForecast[0].Icon);          // Weather person depiction of weather
  display.setCursor(5, 70);
  String displayTime = addTimeZone(findHours(WxForecast[0].Period),WxConditions[0].TimeZone);
  display.println(displayTime);
  DisplayCEIcon(50,  72, WxForecast[1].Icon);          // Weather person depiction of weather
  display.setCursor(55, 68);
  displayTime = addTimeZone(findHours(WxForecast[1].Period),WxConditions[0].TimeZone);
  display.println(displayTime);

  DisplayCEIcon(100, 72, WxForecast[2].Icon);          // Weather person depiction of weather
  display.setCursor(105, 68);
  displayTime = addTimeZone(findHours(WxForecast[2].Period),WxConditions[0].TimeZone);
  display.println(displayTime);

  DisplayCEIcon(150, 72, WxForecast[3].Icon);          // Weather person depiction of weather
  display.setCursor(155, 68);
  displayTime = addTimeZone(findHours(WxForecast[3].Period),WxConditions[0].TimeZone);
  display.println(displayTime);

  DisplayCEIcon(200, 72, WxForecast[4].Icon);          // Weather person depiction of weather
  display.setCursor(205, 68);
  displayTime = addTimeZone(findHours(WxForecast[4].Period),WxConditions[0].TimeZone);
  display.println(displayTime);


  //  DisplayWxIcon(276, 15, WxConditions[0].Icon, LargeIcon); // Weather icon
  //  DisplayMainWeatherSection(0, 148);                       // Weather forecast text
  //  DisplayForecastSection(131, 172);                        // 3hr interval forecast boxes
  //  DisplayAstronomySection(131, 174);                       // Astronomy section Sun rise/set, Moon phase and Moon icon
  //  DisplayWindSection(50, 220, WxConditions[0].Winddir, WxConditions[0].Windspeed, 50); // Wind direction info
  //  DisplaySystemStatus(293, 238);
}


//#################################################################################
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
//#########################################################################
void displayInit(void)
{
  static bool isInit = false;
  if (isInit) {
    return;
  }
  isInit = true;
  display.init();
  display.setRotation(1);
  //display.eraseDisplay();
  display.setTextColor(GxEPD_BLACK);
  //  display.setFont(&DEFALUT_FONT);
  //  display.setTextSize(0);
  display.fillScreen(GxEPD_WHITE);
  display.setTextSize(1);
  display.setFont(&FreeMonoBold9pt7b);
  display.update();
}

//########################################################################
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
  bool wifi_ok_flag = true;
  int n = 0;  // Counter for attempts for wifi connection
  int t = 0;  // how many times to try start the .begin
  // Need to figure out here if the stored SSID works or not. If it doesn't then start an access point...
  // Try the wifi connection:
#ifdef ESP32
  WiFi.mode(WIFI_STA);
  WiFi.begin(Router_SSID.c_str(), Router_Pass.c_str());

  while (WiFi.status() != WL_CONNECTED)
  {
    t++;
    if (t >= 5)
    {
      WiFi.begin(Router_SSID.c_str(), Router_Pass.c_str());
      t = 0;
    }
    //Keep trying until we are connected
    Serial.print(".");
    delay(1000);
    n++;
    // If this doesn't connect in 10 seconds then start an access point
    if (n >= 20)
    {
      // failed to conenct, so start AP
      wifi_ok_flag = false;
      Serial.println("Start AP as failed to connect");
      break;
    }
  }

  if (wifi_ok_flag == false)
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

#endif
  //if you get here you should! have connected to the WiFi
  Serial.println("WiFi connected");
}

//#########################################################################################
bool obtain_wx_data(WiFiClient& client, const String& RequestType) {

  const String units = (Units == "M" ? "metric" : "imperial");
  client.stop(); // close connection before sending a new request
  HTTPClient http;
  String uri = "/data/2.5/" + RequestType + "?q=" + City + "," + Country + "&APPID=" + OWM_KEY + "&mode=json&units=" + units + "&lang=" + Language;
  if (RequestType != "weather")
  {
    uri += "&cnt=" + String(max_readings);
  }

  http.useHTTP10(true); // switch to HTTP version 1.0 to do http.getStream()
  //http.begin(uri,test_root_ca); //HTTPS example connection
  http.begin(client, OWM_SERVER, 80, uri);
  int httpCode = http.GET();
  if (httpCode == HTTP_CODE_OK) {
    if (!DecodeWeather(http.getStream(), RequestType)) {
      ErrorMessage = "JSON decoding problem detected...";
      return false;
    }
    client.stop();
    http.end();
    return true;
  }
  else
  {
    ErrorMessage = "Connection failed, error: " + String(http.errorToString(httpCode).c_str());
    Serial.printf("connection failed, error: %s", http.errorToString(httpCode).c_str());
    client.stop();
    http.end();
    return false;
  }
  return true;
}
//#########################################################################################
// Problems with stucturing JSON decodes, see here: https://arduinojson.org/assistant/
bool DecodeWeather(WiFiClient& json, String Type) {
  Serial.print(F("Creating object...and "));
  DynamicJsonDocument doc((20 * 1024) + 1173 ); // allocate the JsonDocument

  DeserializationError error = deserializeJson(doc, json);

  if (error) { // Test if parsing succeeds.// Deserialize the JSON document
    Serial.print("DeserializeJson() failed: ");
    Serial.println(error.c_str());
    ErrorMessage = "DeserializeJson() failed: " + String(error.c_str());
    return false;
  }
  JsonObject root = doc.as<JsonObject>(); // convert it to a JsonObject
  Serial.println(" Decoding " + Type + " data");

  if (Type == "weather") {
    // All Serial.println statements are for diagnostic purposes and not required, remove if not needed
    WxConditions[0].lon         = root["coord"]["lon"];
    WxConditions[0].lat         = root["coord"]["lat"];
    WxConditions[0].Main0       = root["weather"][0]["main"].as<char*>();         Serial.print("Main0: "); Serial.println(WxConditions[0].Main0);
    WxConditions[0].Forecast0   = root["weather"][0]["description"].as<char*>();  Serial.print("Fore0: "); Serial.println(WxConditions[0].Forecast0);
    WxConditions[0].Forecast1   = root["weather"][1]["main"].as<char*>();         Serial.print("Fore1: "); Serial.println(WxConditions[0].Forecast1);
    WxConditions[0].Forecast2   = root["weather"][2]["main"].as<char*>();         Serial.print("Fore2: "); Serial.println(WxConditions[0].Forecast2);
    WxConditions[0].Icon        = root["weather"][0]["icon"].as<char*>();         Serial.print("Icon : "); Serial.println(WxConditions[0].Icon);
    WxConditions[0].Temperature = root["main"]["temp"];                           Serial.print("Temp : "); Serial.println(WxConditions[0].Temperature);
    WxConditions[0].Pressure    = root["main"]["pressure"];                       Serial.print("Pres : "); Serial.println(WxConditions[0].Pressure);
    WxConditions[0].Humidity    = root["main"]["humidity"];                       Serial.print("Humi : "); Serial.println(WxConditions[0].Humidity);
    WxConditions[0].Low         = root["main"]["temp_min"];                       Serial.print("TLow : "); Serial.println(WxConditions[0].Low);
    WxConditions[0].High        = root["main"]["temp_max"];                       Serial.print("THigh: "); Serial.println(WxConditions[0].High);
    WxConditions[0].Windspeed   = root["wind"]["speed"];                          Serial.print("WSpd : "); Serial.println(WxConditions[0].Windspeed);
    WxConditions[0].Winddir     = root["wind"]["deg"];                            Serial.print("WDir : "); Serial.println(WxConditions[0].Winddir);
    WxConditions[0].Cloudcover  = root["clouds"]["all"];                          Serial.print("CCov : "); Serial.println(WxConditions[0].Cloudcover); // in % of cloud cover
    WxConditions[0].Visibility  = root["visibility"];                             Serial.print("Visi : "); Serial.println(WxConditions[0].Visibility); // in metres
    WxConditions[0].Country     = root["sys"]["country"].as<char*>();             Serial.print("Coun : "); Serial.println(WxConditions[0].Country);
    WxConditions[0].Sunrise     = root["sys"]["sunrise"];                         Serial.print("SunR : "); Serial.println(WxConditions[0].Sunrise);
    WxConditions[0].Sunset      = root["sys"]["sunset"];                          Serial.print("SunS : "); Serial.println(WxConditions[0].Sunset);
    WxConditions[0].TimeUTC     = root["dt"];                                     Serial.print("UTC  : "); Serial.println(WxConditions[0].TimeUTC);
    WxConditions[0].TimeZone    = root["timezone"];                               Serial.print("TZ   : "); Serial.println(WxConditions[0].TimeZone);

  }
  if (Type == "forecast") {
    //Serial.println(json);
    JsonArray list              = root["list"];
    Serial.print("\nReceiving Forecast period-"); //------------------------------------------------
    for (byte r = 0; r < max_readings; r++) {
      Serial.println("\nPeriod-" + String(r) + "--------------");
      WxForecast[r].Dt          = list[r]["dt"].as<char*>();
      WxForecast[r].Temperature = list[r]["main"]["temp"];                          Serial.print("Temp : "); Serial.println(WxForecast[r].Temperature);
      WxForecast[r].Low         = list[r]["main"]["temp_min"];                      Serial.print("TLow : "); Serial.println(WxForecast[r].Low);
      WxForecast[r].High        = list[r]["main"]["temp_max"];                      Serial.print("THig : "); Serial.println(WxForecast[r].High);
      WxForecast[r].Pressure    = list[r]["main"]["pressure"];                      Serial.print("Pres : "); Serial.println(WxForecast[r].Pressure);
      WxForecast[r].Humidity    = list[r]["main"]["humidity"];                      Serial.print("Humi : "); Serial.println(WxForecast[r].Humidity);
      WxForecast[r].Forecast0   = list[r]["weather"][0]["main"].as<char*>();        Serial.print("Fore0: "); Serial.println(WxForecast[r].Forecast0);
      WxForecast[r].Forecast0   = list[r]["weather"][1]["main"].as<char*>();        Serial.print("Fore1: "); Serial.println(WxForecast[r].Forecast1);
      WxForecast[r].Forecast0   = list[r]["weather"][2]["main"].as<char*>();        Serial.print("Fore2: "); Serial.println(WxForecast[r].Forecast2);
      WxForecast[r].Description = list[r]["weather"][0]["description"].as<char*>(); Serial.print("Desc : "); Serial.println(WxForecast[r].Description);
      WxForecast[r].Icon        = list[r]["weather"][0]["icon"].as<char*>();        Serial.print("Icon : "); Serial.println(WxForecast[r].Icon);
      WxForecast[r].Cloudcover  = list[r]["clouds"]["all"];                         Serial.print("CCov : "); Serial.println(WxForecast[0].Cloudcover); // in % of cloud cover
      WxForecast[r].Windspeed   = list[r]["wind"]["speed"];                         Serial.print("WSpd : "); Serial.println(WxForecast[r].Windspeed);
      WxForecast[r].Winddir     = list[r]["wind"]["deg"];                           Serial.print("WDir : "); Serial.println(WxForecast[r].Winddir);
      WxForecast[r].Rainfall    = list[r]["rain"]["3h"];                            Serial.print("Rain : "); Serial.println(WxForecast[r].Rainfall);
      WxForecast[r].Snowfall    = list[r]["snow"]["3h"];                            Serial.print("Snow : "); Serial.println(WxForecast[r].Snowfall);
      WxForecast[r].Period      = list[r]["dt_txt"].as<char*>();                    Serial.print("Peri : "); Serial.println(WxForecast[r].Period);
    }
    //------------------------------------------
    float pressure_trend = WxForecast[0].Pressure - WxForecast[1].Pressure; // Measure pressure slope between ~now and later
    pressure_trend = ((int)(pressure_trend * 10)) / 10.0; // Remove any small variations less than 0.1
    WxConditions[0].Trend = "0";
    if (pressure_trend > 0)  WxConditions[0].Trend = "+";
    if (pressure_trend < 0)  WxConditions[0].Trend = "-";
    if (pressure_trend == 0) WxConditions[0].Trend = "0";
    //if (Units == "I") Convert_Readings_to_Imperial();
  }
  return true;
}
//#########################################################################################
void DisplayCEIcon(int x, int y, String IconName) {
  // TTGO display is: 250px x 122px
  // Icon needs to be inverted to use here???
  uint16_t invert = GxEPD::bm_invert;
  //display.drawRect(x, y, 130, 130, GxEPD_BLACK);
  // NOTE: Using 'drawInvertedBitmap' and not 'drawBitmap' so that images are WYSIWYG, otherwise all images need to be inverted

  if      (IconName == "01d" || IconName == "01n")  display.drawExampleBitmap(gImage_sunny,       x, y, 50, 50, GxEPD_BLACK);
  else if (IconName == "02d" || IconName == "02n")  display.drawExampleBitmap(gImage_mostlySunny, x, y, 50, 50, GxEPD_BLACK);
  else if (IconName == "03d" || IconName == "03n")  display.drawExampleBitmap(gImage_cloudy,      x, y, 50, 50, GxEPD_BLACK);
  else if (IconName == "04d" || IconName == "04n")  display.drawExampleBitmap(gImage_mostlySunny, x, y, 50, 50, GxEPD_BLACK);
  else if (IconName == "09d" || IconName == "09n")  display.drawExampleBitmap(gImage_chanceRain,  x, y, 50, 50, GxEPD_BLACK);
  else if (IconName == "10d" || IconName == "10n")  display.drawExampleBitmap(gImage_rain,        x, y, 50, 50, GxEPD_BLACK);
  else if (IconName == "11d" || IconName == "11n")  display.drawExampleBitmap(gImage_lightning,   x, y, 50, 50, GxEPD_BLACK);
  else if (IconName == "13d" || IconName == "13n")  display.drawExampleBitmap(gImage_snow,        x, y, 50, 50, GxEPD_BLACK);
  else if (IconName == "50d")                       display.drawExampleBitmap(gImage_haze,        x, y, 50, 50, GxEPD_BLACK);
  else if (IconName == "50n")                       display.drawExampleBitmap(gImage_fog,         x, y, 50, 50, GxEPD_BLACK);
  else                                              display.drawExampleBitmap(gImage_noData,      x, y, 50, 50, GxEPD_BLACK);

}

//#########################################################################################
void StopWiFi() {
  WiFi.disconnect();
  WiFi.mode(WIFI_OFF);
}

//##########################################################################################
int getRadiationMQTT() {
  String _dataValue;
  // MQTT subscription with adafruit IO is
  mqtt.subscribe(&airRadiation);
  MQTT_connect();
  Adafruit_MQTT_Subscribe *subscription;
  getRadiation.publish(0);  // This publishes to /get of the radiation feed
  while ((subscription = mqtt.readSubscription(5000))) {
    if (subscription == &airRadiation) {
      Serial.print(F("Got: "));
      Serial.println((char *)airRadiation.lastread);
      _dataValue = (char *)airRadiation.lastread;
    }
  }
  // Want to convert the dataValue (a string) into an int
  // Then re-cast to a string with limited decimal places
  int _dataNumber = _dataValue.toInt();
  return (_dataNumber);
  // Should always get a data value here...
  // But need to handle it if we dont! (not yet implemented)
}

//#########################################################################################
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

//#########################################################################
String findHours(String _period)
{
  // This find the time from the period string
  int _startColon = _period.indexOf(" ");
  int _firstColon = _period.indexOf(":");
  int _secondColon = _period.lastIndexOf(":");
  String _hourString = _period.substring(_startColon + 1, _firstColon);
  String _minString = _period.substring(_firstColon + 1, _secondColon);
  String _displayTime = _hourString;
  return (_displayTime);
}

//#########################################################################
String findDate(String _period)
{
  // This find the time from the period string
  int _startColon = _period.indexOf(" ");
  String _dateString = _period.substring(0, _startColon);
  return (_dateString);
}

//##############################################################################
String digitalClockDisplay(long int _timeUTC)
{
  // This converts the UTC in seconds to a string to display.
  
  time_t t = _timeUTC; // store the current time in time variable t  
  // Create a String of date and time to display
  String _displayDateTime = String(hour(t))+":";
  if(minute(t)<10)
  {
    _displayDateTime = _displayDateTime+"0"+String(minute(t));
  }
  else
  {
    _displayDateTime = _displayDateTime+String(minute(t));
  }
  _displayDateTime = _displayDateTime+" on "+String(day(t))+"/"+String(month(t))+"/"+String(year(t));
  return _displayDateTime;
}

//######################################################################

String addTimeZone(String _displayHours, int _timeZone)
{
  int _displayInt = _displayHours.toInt();
  _displayInt = _displayInt + (_timeZone/3600);  // 3600 seconds in hours
  String _returnedHours = String(_displayInt);
  if(_displayInt<10)
  {
    _returnedHours = "0"+_returnedHours;
  }
  return(_returnedHours);
}
