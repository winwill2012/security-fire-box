#pragma once
#include <WiFiUdp.h>
#include <NTPClient.h>
extern WiFiUDP ntpUDP;
extern NTPClient timeClient;

void setup_ntp();

String get_current_time();
