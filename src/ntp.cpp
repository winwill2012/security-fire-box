#include <ntp.h>
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org");
void setup_ntp()
{
    timeClient.begin();
    timeClient.setTimeOffset(8 * 3600);
}

String get_current_time()
{
    // 更新时间
    timeClient.update();

    // 获取当前时间
    unsigned long epochTime = timeClient.getEpochTime();
    struct tm *t;
    t = localtime((time_t *)&epochTime);

    // 打印当前时间
    char buf[30];
    strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", t);
    Serial.printf("当前时间: %s\n", buf);
    return String(buf);
}