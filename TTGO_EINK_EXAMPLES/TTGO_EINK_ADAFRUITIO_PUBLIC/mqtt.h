#pragma once
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"

bool MQTT_connect(Adafruit_MQTT_Client my_mqtt);
int getRadiationMQTT(Adafruit_MQTT_Client my_mqtt, Adafruit_MQTT_Subscribe my_airRadiation, Adafruit_MQTT_Publish my_getRadiation);
