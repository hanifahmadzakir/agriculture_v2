#ifndef WEBSERVER_H
#define WEBSERVER_H

#include <Arduino.h>
#include <ESP8266WebServer.h>

extern ESP8266WebServer server;

void handleRoot();
void handleNotFound();
void handleGetAllSensors();
void handleGetTemperature();
void handleGetHumidity();
void handleGetPressure();
void initializeWebServer();

#endif