#ifndef INFARRED_HPP

#include <Arduino.h>
#include <mqtt.hpp>

#define KEY_PIN 21
void listen_infrared(void *ptr)
{
    while (1)
    {
        if (Serial2.available() > 0)
        {
            Serial.printf("收到红外信号: %s\n", Serial2.readStringUntil('\n'));
            doc["securityState"] = true;
            serializeJson(doc, jsonBuffer);
            mqtt_client.publish("home-assistant/security/fire/state", jsonBuffer);
        }
        vTaskDelay(1);
    }
}

void setup_infrared()
{
    Serial2.begin(9600);
    pinMode(KEY_PIN, OUTPUT);
    digitalWrite(KEY_PIN, HIGH);
    xTaskCreatePinnedToCore(listen_infrared, "listen_infrared", 2048, nullptr, 0, NULL, 1);
}

void infrared_pair()
{
    Serial.println("执行红外探头配对逻辑");
    digitalWrite(KEY_PIN, LOW);
    vTaskDelay(150);
    digitalWrite(KEY_PIN, HIGH);
    vTaskDelay(200);
    digitalWrite(KEY_PIN, LOW);
    vTaskDelay(150);
    digitalWrite(KEY_PIN, HIGH);
}

#define INFARRED_HPP
#endif