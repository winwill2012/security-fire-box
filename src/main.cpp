#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <mqtt.h>
#include <sensor.h>
#include <ws2812.h>
#include <infrared.h>
#include <ntp.h>

void setup()
{
  Serial.begin(115200);
  pinMode(2, OUTPUT);
  pinMode(19, OUTPUT);
  setup_ws2812();
  setup_sensor();
  setup_infrared();
  setup_mqtt();
  setup_ntp();
}

void loop()
{

}
