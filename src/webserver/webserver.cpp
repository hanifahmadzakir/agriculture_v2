#include "../include/config.h"
#include "../../include/sensors/aht10.h"
#include "../../include/sensors/bmp180.h"
#include "../../include/network/wifi.h"
#include "../../include/webserver/webserver.h"
#include "../include/sensors/battery.h"

ESP8266WebServer server(80);

void handleRoot() {
  String message = "Welcome to ESP8266 REST API\n";
  message += "Available endpoints:\n";
  message += "/api/sensors - Get all sensor data\n";
  message += "/api/temperature - Get temperature data\n";
  message += "/api/humidity - Get humidity data\n";
  message += "/api/pressure - Get pressure data\n";
  server.send(200, "text/plain", message);
}

void handleNotFound() {
  String message = "Endpoint not found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
}

void handleGetAllSensors() {
  String jsonResponse = "{";
  jsonResponse += "\"temperature\":" + String(AHT10_temperature) + ",";
  jsonResponse += "\"humidity\":" + String(AHT10_humidity) + ",";
  jsonResponse += "\"pressure\":" + String(BMP180_pressure) + ",";
  jsonResponse += "\"altitude\":" + String(BMP180_Alttitude) + ",";
  jsonResponse += "\"battery_voltage\":" + String(battVoltage) + ",";
  jsonResponse += "\"rssi\":" + String(rssi);
  jsonResponse += "}";
  
  server.send(200, "application/json", jsonResponse);
}

void handleGetTemperature() {
  String jsonResponse = "{\"temperature\":" + String(AHT10_temperature) + "}";
  server.send(200, "application/json", jsonResponse);
}

void handleGetHumidity() {
  String jsonResponse = "{\"humidity\":" + String(AHT10_humidity) + "}";
  server.send(200, "application/json", jsonResponse);
}

void handleGetPressure() {
  String jsonResponse = "{\"pressure\":" + String(BMP180_pressure) + "}";
  server.send(200, "application/json", jsonResponse);
}

void initializeWebServer() {
  server.on("/", handleRoot);
  server.on("/api/sensors", handleGetAllSensors);
  server.on("/api/temperature", handleGetTemperature);
  server.on("/api/humidity", handleGetHumidity);
  server.on("/api/pressure", handleGetPressure);
  server.onNotFound(handleNotFound);
  
  server.begin();
  Serial.println("HTTP server started");
}