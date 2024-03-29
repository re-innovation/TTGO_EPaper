#include "mqtt.h"
#include "display.h"

#include <stdio.h>
#include <Arduino.h>

#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"

//##########################################################################################
int getRadiationMQTT(Adafruit_MQTT_Client _mqtt, Adafruit_MQTT_Subscribe _airRadiation, Adafruit_MQTT_Publish _getRadiation) {
  String _dataValue;
  // MQTT subscription with adafruit IO is
  _mqtt.subscribe(&_airRadiation);
  bool MQTT_OK = MQTT_connect(_mqtt);
  if (MQTT_OK)
  {
    Adafruit_MQTT_Subscribe *subscription;
    _getRadiation.publish(0);  // This publishes to /get of the radiation feed
    while ((subscription = _mqtt.readSubscription(5000))) {
      if (subscription == &_airRadiation) {
        Serial.print(F("Got: "));
        Serial.println((char *)_airRadiation.lastread);
        _dataValue = (char *)_airRadiation.lastread;
      }
    }
    // Want to convert the dataValue (a string) into an int
    // Then re-cast to a string with limited decimal places
    int _dataNumber = _dataValue.toInt();
    return (_dataNumber);
    // Should always get a data value here...
    // But need to handle it if we dont! (not yet implemented)
  }
  else
  {
    return (0); // No MQTT so no data!
  }
}

//#########################################################################################
// Function to connect and reconnect as necessary to the MQTT server.
// Should be called in the loop function and it will take care if connecting.
bool MQTT_connect(Adafruit_MQTT_Client _mqtt) {
  int8_t ret;

  // Stop if already connected.
  if (_mqtt.connected()) {
    return (true);
  }
  Serial.print("Connecting to MQTT... ");
  uint8_t retries = 3;
  while ((ret = _mqtt.connect()) != 0) { // connect will return 0 for connected
    Serial.println(_mqtt.connectErrorString(ret));
    Serial.println("Retrying MQTT connection in 5 seconds...");
    _mqtt.disconnect();
    delay(5000);  // wait 10 seconds
    retries--;
    if (retries == 0)
    {
      // Here we want to say it has not connected to the MQTT feed
      displayNoMQTT();
      return (false);
    }
  }
  Serial.println("MQTT Connected!");
  return (true);
}
