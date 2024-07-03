#pragma once
#include <Arduino.h>
#include <mqtt.h>

void listen_infrared(void *ptr);
void setup_infrared();
void pair_infrared();
void clear_infrared();