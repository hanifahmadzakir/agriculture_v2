#include "../../include/sensors/bmp180.h"
#include "../include/config.h"

float BMP180_temperature = 0;
float BMP180_pressure = 0;
float BMP180_Alttitude = 0;
Adafruit_BMP085 BMP180;

void checkBMP180() {
  while(BMP180.begin() != true) {
    Serial.println("Could not find valid BMP085/BMP180, check wiring!");
    delay(1000);
  }
  Serial.println("BMP180 OK!");
}

void readSensorBMP180() {
  BMP180_temperature = BMP180.readTemperature();
  BMP180_pressure = BMP180.readPressure();
  BMP180_Alttitude = BMP180.readAltitude();

  Serial.print("BMP180 Temperature: ");
  Serial.println(BMP180_temperature);
  Serial.print("BMP180 Pressure: ");
  Serial.println(BMP180_pressure);
  Serial.print("BMP180 Real Alttitude: ");
  Serial.println(BMP180_Alttitude);
}