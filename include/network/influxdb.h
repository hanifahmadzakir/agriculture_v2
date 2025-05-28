#ifndef INFLUXDB_H
#define INFLUXDB_H

#include <Arduino.h>
#include <InfluxDbClient.h>
#include <InfluxDbCloud.h>

extern InfluxDBClient influxClient;
extern Point sensor;

void checkInfluxDBConnection();
void sensorTag();
void sendSensorField();

#endif