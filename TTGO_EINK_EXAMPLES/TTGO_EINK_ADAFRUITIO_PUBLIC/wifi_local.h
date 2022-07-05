#pragma once

// Wifimanager
#include <WiFiManager.h> // https://github.com/tzapu/WiFiManager

void setup_wifi(bool my_reset_settings_flag = false);
void stopWiFi();
void saveParamsCallback();
