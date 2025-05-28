#ifndef WIFI_H
#define WIFI_H

#include <Arduino.h>
#include <ESP8266WiFi.h>

extern long rssi;

void connectWiFi();
void checkRSSI();

#endif