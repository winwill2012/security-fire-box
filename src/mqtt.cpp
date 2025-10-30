#include "WiFiManager.h"
#include <mqtt.h>

auto mqtt_server = "www.welinklab.com";
auto mqtt_username = "esp32";
auto mqtt_password = "5887188QFGqfg!@#";

WiFiClient espClient;
PubSubClient mqtt_client(espClient);
JsonDocument doc;
char jsonBuffer[512];

void setup_wifi() {
    ws2812_show(CRGB::Black);
    WiFiManager manager;
    manager.autoConnect("ESP32安防报警器");
    Serial.println("准备连接WiFi，请打开手机搜索WiFi热点[ESP32安防报警器]，连接网络");
    while (!WiFi.isConnected()) {
        Serial.print(".");
        delay(1000);
    }
    Serial.println("");
    Serial.println("WiFi连接成功");
}

void reconnect() {
    while (!mqtt_client.connected()) {
        Serial.println("Attempting MQTT connection...");
        if (mqtt_client.connect("ESP32-power-", mqtt_username, mqtt_password)) {
            Serial.println("mqtt connected");
            mqtt_client.subscribe("/command");
            mqtt_client.publish("home-assistant/security/fire/online/state", "online", true);
        } else {
            Serial.print("failed, rc=");
            Serial.print(mqtt_client.state());
            Serial.println(" try again in 5 seconds");
            vTaskDelay(3000);
        }
        vTaskDelay(1);
    }
}

void check_mqtt(void *ptr) {
    while (true) {
        if (!mqtt_client.connected()) {
            reconnect();
        }
        mqtt_client.loop();
        vTaskDelay(1);
    }
}

void callback(char *topic, const byte *payload, const unsigned int length) {
    char message[length];
    for (int i = 0; i < length; i++) {
        message[i] = static_cast<char>(payload[i]);
    }
    message[length] = '\0';
    Serial.printf("收到消息: %s\n", message);
    if (strstr(message, "on")) {
        digitalWrite(2, 1);
        ws2812_show(CRGB::Red);
    } else if (strstr(message, "off")) {
        digitalWrite(2, 0);
        ws2812_show(CRGB::Black);
    } else if (strstr(message, "add_sensor")) // 探头配对
    {
        pair_infrared();
    } else if (strstr(message, "clear_sensor")) // 清空探头
    {
        clear_infrared();
    }
}

void setup_mqtt() {
    setup_wifi();
    mqtt_client.setServer(mqtt_server, 1883);
    mqtt_client.setCallback(callback);
    reconnect();
    ws2812_show(CRGB::Green);
    xTaskCreatePinnedToCore(check_mqtt, "check_mqtt", 8192, nullptr, 1, nullptr, 1);
}
