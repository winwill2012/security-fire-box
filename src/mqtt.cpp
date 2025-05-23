#include <mqtt.h>

const char *ssid = "Xiaomi_E15A";
const char *password = "19910226";
const char *mqtt_server = "www.welinklab.com";
const char *mqtt_username = "esp32";
const char *mqtt_password = "";

WiFiClient espClient;
PubSubClient mqtt_client(espClient);
JsonDocument doc;
char jsonBuffer[512];

void setup_wifi()
{
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    ws2812_show(CRGB::Black);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }
    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
}

void reconnect()
{
    while (!mqtt_client.connected())
    {
        Serial.println("Attempting MQTT connection...");
        if (mqtt_client.connect("ESP32", mqtt_username, mqtt_password))
        {
            Serial.println("mqtt connected");
            mqtt_client.subscribe("/command");
            mqtt_client.publish("home-assistant/security/fire/online/state", "online", true);
        }
        else
        {
            Serial.print("failed, rc=");
            Serial.print(mqtt_client.state());
            Serial.println(" try again in 5 seconds");
            vTaskDelay(3000);
        }
        vTaskDelay(1);
    }
}

void check_mqtt(void *ptr)
{
    while (1)
    {
        if (!mqtt_client.connected())
        {
            reconnect();
        }
        mqtt_client.loop();
        vTaskDelay(1);
    }
}

void callback(char *topic, byte *payload, unsigned int length)
{
    char message[length];
    for (int i = 0; i < length; i++)
    {
        message[i] = (char)payload[i];
    }
    message[length] = '\0';
    Serial.printf("收到消息: %s\n", message);
    if (strcmp(message, "pair") == 0) // 探头配对
    {
        pair_infrared();
    }
    else if (strcmp(message, "clear") == 0) // 清空探头
    {
        clear_infrared();
    }
}

void setup_mqtt()
{
    setup_wifi();
    mqtt_client.setServer(mqtt_server, 1883);
    mqtt_client.setCallback(callback);
    reconnect();
    ws2812_show(CRGB::Green);
    xTaskCreatePinnedToCore(check_mqtt, "check_mqtt", 8192, nullptr, 1, NULL, 1);
    // xTaskCreate(check_mqtt, "check_mqtt", 4096, nullptr, 1, NULL);
}
