#pragma once
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <WiFi.h>
#include <ws2812.h>
#include <infrared.h>

extern PubSubClient mqtt_client;
extern JsonDocument doc;
extern char jsonBuffer[512];

void setup_wifi();

void setup_mqtt();
