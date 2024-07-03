#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <mqtt.hpp>
#include <sensor.hpp>
#include <ws2812.hpp>
#include <Infrared.hpp>

void setup()
{
  Serial.begin(115200);
  setup_ws2812();
  setup_sensor();
  setup_infrared();
  setup_mqtt();
}

void loop()
{
}
