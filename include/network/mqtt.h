#ifndef MQTT_H
#define MQTT_H

#include <Arduino.h>
#include <PubSubClient.h>
#include <WiFiClient.h>

extern WiFiClient espClient;
extern PubSubClient client;
extern String mqttCommand;

void connectMqttBroker();
void recieveMqttBroker(char *topic, byte* msg, size_t msgLength);
void mqttPublishBroker();
void mqttVoidSetup();

#endif