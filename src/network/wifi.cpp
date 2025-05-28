#include "../../include/network/wifi.h"
#include "../include/config.h"

long rssi = 0;

void connectWiFi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.println("Connecting...");
    delay(1000);
  }
  Serial.println("WiFi Connected to :" + String(ssid));
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
  checkRSSI();
}

void checkRSSI() {
  rssi = WiFi.RSSI();
  Serial.print("RSSI: ");
  Serial.println(rssi);
}