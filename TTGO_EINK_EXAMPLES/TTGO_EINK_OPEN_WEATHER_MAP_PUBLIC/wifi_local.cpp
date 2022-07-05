#include "wifi_local.h"
#include "display.h"
#include "weather.h"
#include "EEPROM.h"

// Need to add all the above as wifi config prarmeters
WiFiManagerParameter custom_owm_key("WIFI_OWM_KEY", "Open Weather Map KEY", "YOUR OWM API KEY", 64);
WiFiManagerParameter custom_owm_server("WIFI_OWM_SERVER", "Open Weather Map Server", "api.openweathermap.org", 64);
WiFiManagerParameter custom_owm_city("WIFI_OWM_CITY", "City", "City", 30);
WiFiManagerParameter custom_owm_country("WIFI_OWM_COUNTRY", "Country", "GB", 4);
WiFiManagerParameter custom_owm_language("WIFI_OWM_LANG", "Language", "EN" , 4);
WiFiManagerParameter custom_owm_hemisphere("WIFI_OWM_HEMI", "Hemisphere", "north", 10);
WiFiManagerParameter custom_owm_units("WIFI_OWM_UNITS", "Units", "M", 2);
WiFiManagerParameter custom_owm_timezone("WIFI_OWM_TIMEZONE", "Timezone", "GMT0BST,M3.5.0/01,M10.5.0/02", 64);


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

  // Add parameters to config screen:
  wm.addParameter(&custom_owm_key);
  wm.addParameter(&custom_owm_server);
  wm.addParameter(&custom_owm_city);
  wm.addParameter(&custom_owm_country);
  wm.addParameter(&custom_owm_language);
  wm.addParameter(&custom_owm_hemisphere);
  wm.addParameter(&custom_owm_units);
  wm.addParameter(&custom_owm_timezone);
  wm.setSaveParamsCallback(saveParamsCallback);

  // Automatically connect using saved credentials,
  // if connection fails, it starts an access point with the specified name
  // if empty will auto generate SSID, if password is blank it will be anonymous AP (wm.autoConnect())
  // then goes into a blocking loop awaiting configuration and will return success result

  uint32_t timeout_seconds = 120;   // 2 mins before it times out and starts again...
  wm.setConfigPortalBlocking(false);
  wm.setConfigPortalTimeout(timeout_seconds);

  bool res;
  res = wm.autoConnect("WeatherMap_AP", "openweather"); // password protected ap
  if (!res)
  {
    // If not connecting to wifi then create an Access Point for wifi data
    // This will last for "timeout_seconds" seconds
    Serial.println("Configportal running");
    displaySSID("WeatherMap_AP", "openweather");  // Display on EINK
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

//#########################################################################################
void saveParamsCallback () {
  Serial.println("Get Params:");
  Serial.print(custom_owm_key.getID());
  Serial.print(" : ");
  if (owm_settings.OWM_KEY != custom_owm_key.getValue())
  {
    owm_settings.OWM_KEY = custom_owm_key.getValue();
    EEPROM.writeString(0x0, owm_settings.OWM_KEY);
    EEPROM.commit();
  }
  Serial.println(owm_settings.OWM_KEY);

  Serial.print(custom_owm_server.getID());
  Serial.print(" : ");
  if (owm_settings.OWM_SERVER != custom_owm_server.getValue())
  {
    owm_settings.OWM_SERVER = custom_owm_server.getValue();
    EEPROM.writeString(0x100, owm_settings.OWM_SERVER);
    EEPROM.commit();
  }
  Serial.println(owm_settings.OWM_SERVER);

  Serial.print(custom_owm_city.getID());
  Serial.print(" : ");
  if (owm_settings.MY_CITY != custom_owm_city.getValue())
  {
    owm_settings.MY_CITY = custom_owm_city.getValue();
    EEPROM.writeString(0x150, owm_settings.MY_CITY);
    EEPROM.commit();
  }
  Serial.println(owm_settings.MY_CITY);

  Serial.print(custom_owm_country.getID());
  Serial.print(" : ");
  if (owm_settings.MY_COUNTRY != custom_owm_country.getValue())
  {
    owm_settings.MY_COUNTRY = custom_owm_country.getValue();
    EEPROM.writeString(0x200, owm_settings.MY_COUNTRY);
    EEPROM.commit();
  }
  Serial.println(owm_settings.MY_COUNTRY);

  Serial.print(custom_owm_language.getID());
  Serial.print(" : ");
  if (owm_settings.MY_LANGUAGE != custom_owm_language.getValue())
  {
    owm_settings.MY_LANGUAGE = custom_owm_language.getValue();
    EEPROM.writeString(0x250, owm_settings.MY_LANGUAGE);
    EEPROM.commit();
  }
  Serial.println(owm_settings.MY_LANGUAGE);

  Serial.print(custom_owm_hemisphere.getID());
  Serial.print(" : ");
  if (  owm_settings.MY_HEMISPHERE != custom_owm_hemisphere.getValue())
  {
    owm_settings.MY_HEMISPHERE = custom_owm_hemisphere.getValue();
    EEPROM.writeString(0x260, owm_settings.MY_HEMISPHERE);
    EEPROM.commit();
  }
  Serial.println(owm_settings.MY_HEMISPHERE);

  Serial.print(custom_owm_units.getID());
  Serial.print(" : ");
  if (owm_settings.MY_UNITS != custom_owm_units.getValue())
  {
    owm_settings.MY_UNITS = custom_owm_units.getValue();
    EEPROM.writeString(0x270, owm_settings.MY_UNITS);
    EEPROM.commit();
  }
  Serial.println(owm_settings.MY_UNITS);

  Serial.print(custom_owm_timezone.getID());
  Serial.print(" : ");
  if (owm_settings.MY_TIMEZONE != custom_owm_timezone.getValue())
  {
    owm_settings.MY_TIMEZONE = custom_owm_timezone.getValue();
    EEPROM.writeString(0x280, owm_settings.MY_TIMEZONE);
    EEPROM.commit();
  }
  Serial.println(owm_settings.MY_TIMEZONE);
}
