#include "wifi_local.h"
#include "display.h"
#include "Config.h"

////define your default values here, if there are different values in config.json, they are overwritten.
//char mqtt_server[40];
//char mqtt_port[6] = "8080";
//char api_token[34] = "YOUR_API_TOKEN";
//#define AIO_SERVER          "io.adafruit.com"                   // direct
//#define AIO_SERVERPORT      1883                                // 8883  // Use 8883 if at all possible!
//#define AIO_USERNAME        "YOUR ADAFRUIT IO USER NAME HERE"   // This is your Adafruit IO username
//#define AIO_KEY             "YOUR ADAFRUIT IO KEY HERE"         // This is your Adafruit IO Key


//WiFiManagerParameter custom_mqtt_server("server", "mqtt server", "io.adafruit.com", 64);
//WiFiManagerParameter custom_mqtt_port("port", "mqtt port", "1883", 6);
//WiFiManagerParameter custom_mqtt_username("username", "mqtt username", "ADAFRUIT IO USERNAME", 50);
//WiFiManagerParameter custom_mqtt_key("key", "mqtt key", "ADAFRUIT IO KEY", 200);


//########################################################################
void setup_wifi(bool _reset_settings_flag) {
  bool wifi_ok_flag = true;
  int n = 0;  // Counter for attempts for wifi connection
  int t = 0;  // how many times to try start the .begin
  // Need to figure out here if the stored SSID works or not. If it doesn't then start an access point...
  // Try the wifi connection:

  WiFi.mode(WIFI_STA); // explicitly set mode, esp defaults to STA+AP

  //  // The extra parameters to be configured (can be either global or just in the setup)
  //  // After connecting, parameter.getValue() will get you the configured value
  //  // id/name placeholder/prompt default length

  //WiFiManager, Local intialization. Once its business is done, there is no need to keep it around
  WiFiManager wm;


  //  // reset settings - wipe stored credentials for testing or for changing
  //  // these are stored by the esp library
  if (_reset_settings_flag == true)
  {
    Serial.println("Reset Settings and start AP mode");
    wm.resetSettings();
  }


  uint32_t timeout_seconds = 120;

  wm.setConfigPortalBlocking(false);
  wm.setConfigPortalTimeout(timeout_seconds);
  bool res;
  res = wm.autoConnect("AdafruitIO_AP", "adafruitio"); // password protected ap
  if (!res)
  {
    // If not connecting to wifi then create an Access Point for wifi data
    // This will last for timeout_seconds
    Serial.println("Configportal running");
    displaySSID("AdafruitIO_AP", "adafruitio");  // Display on EINK
  }

  uint32_t millis_store = millis();

  while (!res) {
    wm.process();
    if (WiFi.status() == WL_CONNECTED)
    {
      break;
    }
    if (millis() > millis_store + (timeout_seconds * 1000))
    {
      // Here the unit has timed out and not connected, so shut down ready for new start
      displayShutDown();  // Display on EINK
      break;
    }
  }
  //if you get here you should! have connected to the WiFi
  Serial.println("WiFi connected");
}

//#########################################################################################
void stopWiFi() {
  WiFi.disconnect();
  WiFi.mode(WIFI_OFF);
}
