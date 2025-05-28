#include "../../include/sensors/aht10.h"
#include "../include/config.h"

float AHT10_temperature = 0;
float AHT10_humidity = 0;
AHT10 myAHT10(AHT10_ADDRESS_0X38);

void checkAHT10() {
  while(myAHT10.begin() != true) {
    Serial.println(F("AHT10 not connected or fail to load calibration coefficient"));
    delay(1000);
  }
  Serial.println("AHT10 OK");
}

void readSensorAHT10() {
  AHT10_humidity = myAHT10.readHumidity();
  AHT10_temperature = myAHT10.readTemperature();
  delay(10000);

  Serial.print("AHT10 Temperature : ");
  Serial.println(AHT10_temperature);
  Serial.print("AHT10 Humidity: ");
  Serial.println(AHT10_humidity);
}