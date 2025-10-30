#include <sensor.h>
#include <ModbusMaster.h>
#include <ArduinoJson.h>
#include <mqtt.h>

ModbusMaster node;
HardwareSerial mySerial(1);

float temp, hum, ppm;
bool success;
uint32_t voltage;
uint8_t result;

void upload_sensor_data(void *ptr) {
    result = node.readInputRegisters(0x0001, 2);
    success = false;
    if (result == ModbusMaster::ku8MBSuccess) // 如果成功获取温湿度传感器返回的数据
    {
        temp = static_cast<double>(node.getResponseBuffer(0)) / 10.0; // 从结果中获取第一个字节的数据
        hum = static_cast<double>(node.getResponseBuffer(1)) / 10.0; // 从结果中获取第二个字节的数据
        success = true;
    }
    voltage = 2 * analogReadMilliVolts(34);
    ppm = 47.362 * exp(1.4151 * (voltage / 1000.0));
    if (ppm > 10000) {
        ppm = 10000;
    }
    if (ppm > 500) {
        doc["fireState"] = "on";
        digitalWrite(19, 1);
    } else {
        doc["fireState"] = "off";
        digitalWrite(19, 0);
    }
    Serial.printf("ADC电压值: %d, ppm: %f, 温度: %f, 湿度: %f\n", voltage, ppm, temp, hum);
    doc["gas"] = ppm;
    if (success) {
        doc["temperature"] = temp;
        doc["humidity"] = hum;
    }
    serializeJson(doc, jsonBuffer);
    mqtt_client.publish("home-assistant/security/fire/state", jsonBuffer);
    vTaskDelay(1);
}

void setup_sensor() {
    mySerial.begin(9600, SERIAL_8N1, 22, 23);
    node.begin(1, mySerial);
    const TimerHandle_t timer = xTimerCreate("upload_sensor_data", pdMS_TO_TICKS(1000), pdTRUE, NULL,
                                             upload_sensor_data);
    xTimerStart(timer, 0);
}
