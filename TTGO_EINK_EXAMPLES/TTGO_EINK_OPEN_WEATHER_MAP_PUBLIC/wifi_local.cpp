#include "wifi_local.h"
#include "display.h"


//########################################################################
void setup_wifi() {
  // SSID and PW for your Router
  String Router_SSID;
  String Router_Pass;
  // SSID and PW for AP Config Portal
  String AP_SSID;
  String AP_PASS;
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

    displaySSID(AP_SSID, AP_PASS);  // Display on EINK

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
void stopWiFi() {
  WiFi.disconnect();
  WiFi.mode(WIFI_OFF);
}
