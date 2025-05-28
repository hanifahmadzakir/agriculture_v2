#include "../include/sensors/battery.h"
#include "../include/config.h"

// Define the variable here (only once in the project)
float battVoltage = 0;

// Define the function implementation
void checkVoltage() {
  int analogValue = analogRead(A0);
  battVoltage = analogValue * 5.0 / 1023;
}