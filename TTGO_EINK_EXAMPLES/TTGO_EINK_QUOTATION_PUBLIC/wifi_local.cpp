#include "wifi_local.h"
#include "display.h"


//########################################################################
void setup_wifi(bool _reset_settings_flag) {

  bool wifi_ok_flag = true;
  int n = 0;  // Counter for attempts for wifi connection
  int t = 0;  // how many times to try start the .begin
  // Need to figure out here if the stored SSID works or not. If it doesn't then start an access point...
  // Try the wifi connection:

  WiFi.mode(WIFI_STA); // explicitly set mode, esp defaults to STA+AP

  //WiFiManager, Local intialization. Once its business is done, there is no need to keep it around
  WiFiManager wm;

  //  // reset settings - wipe stored credentials for testing or for changing
  //  // these are stored by the esp library
  if (_reset_settings_flag == true)
  {
    Serial.println("Reset Settings and start AP mode");
    wm.resetSettings();
  }

  // Automatically connect using saved credentials,
  // if connection fails, it starts an access point with the specified name
  // if empty will auto generate SSID, if password is blank it will be anonymous AP (wm.autoConnect())
  // then goes into a blocking loop awaiting configuration and will return success result

 
  uint32_t timeout_seconds = 120; // 2 mins before it times out and starts again...
  
  wm.setConfigPortalBlocking(false);
  wm.setConfigPortalTimeout(timeout_seconds);
  bool res;
  res = wm.autoConnect("QuotationMachineAP", "quotation"); // password protected ap
  if (!res)
  {
    // If not connecting to wifi then create an Access Point for wifi data
    // This will last for "timeout_seconds" seconds
    Serial.println("Configportal running");
    displaySSID("QuotationMachineAP", "quotation");  // Display on EINK
  }

  uint32_t millis_store = millis();
    
  while (!res) {
    wm.process();
    if (WiFi.status() == WL_CONNECTED)
    {
      break;
    }
    if(millis() > millis_store + (timeout_seconds*1000))
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
