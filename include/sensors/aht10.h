#ifndef AHT10_H
#define AHT10_H

#include <Arduino.h>
#include <AHT10.h>

extern float AHT10_temperature;
extern float AHT10_humidity;
extern AHT10 myAHT10;

void checkAHT10();
void readSensorAHT10();

#endif