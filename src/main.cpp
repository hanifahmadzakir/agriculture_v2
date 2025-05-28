#include <Arduino.h>
#include <Wire.h>
#include <SoftwareSerial.h>

// Include all necessary headers
#include "config.h"
#include "sensors/aht10.h"
#include "sensors/bmp180.h"
#include "network/wifi.h"
#include "network/mqtt.h"
#include "network/influxdb.h"
#include "webserver/webserver.h"
#include "sensors/battery.h"

// General variables

int selector = 0;
int analogValue = 0;
int testValue = 100;
const size_t eventInterval = 1000;
size_t previousTime = 0;
String command;

// Serial interfaces
SoftwareSerial rs485(D0, D5); // (RX, TX)
SoftwareSerial GPS(D8, D7);   // (RX, TX)

// void checkVoltage() {
//   analogValue = analogRead(A0);
//   battVoltage = analogValue * 5.0 / 1023;
// }

void syncTime() {
  configTime(0, 0, "pool.ntp.org", "time.google.com", "time.windows.com");
  
  Serial.print("Syncing time");
  int attempts = 0;
  while (time(nullptr) < 1000000000 && attempts < 10) {
    Serial.print(".");
    delay(1000);
    attempts++;
  }
  
  time_t now = time(nullptr);
  Serial.println("\nCurrent time: " + String(ctime(&now)));
  
  if (now < 1000000000) {
    Serial.println("Failed to obtain time, retrying in 30s");
    delay(30000);
    syncTime(); // Recursive retry
  }
}

void setup() {
  pinMode(led1, OUTPUT);
  Serial.begin(9600);
  rs485.begin(9600);
  delay(1000);
  
  connectWiFi();
  delay(2000);
  
  checkAHT10();
  checkBMP180();
  
  syncTime();
  checkInfluxDBConnection();
  sensorTag();
  delay(1000);
  
  initializeWebServer();
}

void loop() {
  server.handleClient();

  // Using millis instead of delay for checking voltage
  size_t currentTime = millis();
  if (currentTime - previousTime >= eventInterval) {
    checkVoltage();
    readSensorAHT10();
    readSensorBMP180();
    sendSensorField();
    previousTime = currentTime;
  }
}