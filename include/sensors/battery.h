#ifndef BATTERY_H
#define BATTERY_H

#include <Arduino.h>

// Declare without defining (extern tells compiler it's defined elsewhere)
extern float battVoltage;

// Function declaration
void checkVoltage();

#endif