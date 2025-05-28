#ifndef BMP180_H
#define BMP180_H

#include <Arduino.h>
#include <Adafruit_BMP085.h>

extern float BMP180_temperature;
extern float BMP180_pressure;
extern float BMP180_Alttitude;
extern Adafruit_BMP085 BMP180;

void checkBMP180();
void readSensorBMP180();

#endif