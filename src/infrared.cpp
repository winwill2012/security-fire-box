#include <infrared.h>
#include <ntp.h>
#include "ArduinoJson.h"
#include "mqtt.h"
#define KEY_PIN 21

void listen_infrared(void *ptr) {
    while (true) {
        if (Serial2.available() > 0) {
            while (Serial2.available() > 0) {
                Serial.printf("收到红外信号: %s\n", Serial2.readStringUntil('\n').c_str());
                String currentTime = get_current_time();
                doc["securityState"] = "on";
                serializeJson(doc, jsonBuffer);
                mqtt_client.publish("home-assistant/security/fire/state", jsonBuffer);
            }
        } else {
            doc["securityState"] = "off";
            serializeJson(doc, jsonBuffer);
            mqtt_client.publish("home-assistant/security/fire/state", jsonBuffer);
        }
        vTaskDelay(1000);
    }
}

void setup_infrared() {
    Serial2.begin(9600);
    pinMode(KEY_PIN, OUTPUT);
    digitalWrite(KEY_PIN, HIGH);
    xTaskCreatePinnedToCore(listen_infrared, "listen_infrared", 2048, nullptr, 0, nullptr, 1);
}

// 连续短按两次开始配对
void pair_infrared() {
    Serial.println("执行红外探头配对逻辑");
    digitalWrite(KEY_PIN, LOW);
    vTaskDelay(150);
    digitalWrite(KEY_PIN, HIGH);
    vTaskDelay(200);
    digitalWrite(KEY_PIN, LOW);
    vTaskDelay(150);
    digitalWrite(KEY_PIN, HIGH);
}

// key键长按8秒以上，清除所有配对信息
// 使用文档参考：https://www.yuque.com/office/yuque/0/2023/pdf/2993906/1687171925329-86dc69bc-4f4a-4224-af53-ce8d46a9ffbb.pdf?from=https%3A%2F%2Fwww.yuque.com%2Ffengniaowuxianjeck%2Fjswd%2Fbt78cy
void clear_infrared() {
    Serial.println("执行红外探头清空逻辑");
    digitalWrite(KEY_PIN, LOW);
    vTaskDelay(8500);
    digitalWrite(KEY_PIN, HIGH);
}
