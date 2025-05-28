#include "../../include/network/mqtt.h"
#include "../include/config.h"
#include "../../include/sensors/aht10.h"

WiFiClient espClient;
PubSubClient client(espClient);
String mqttCommand = "";

void connectMqttBroker() {
  while(!client.connected()) {
    Serial.println("Connecting MQTT");
    if(client.connect("ESP8266")) {
      client.subscribe(MQTT_COMMAND);
    }
  }
}

void recieveMqttBroker(char *topic, byte* msg, size_t msgLength) {
  if(String(topic) == MQTT_COMMAND) {
    Serial.println(topic);
    for (size_t i=0; i<msgLength; i++) {
      Serial.print(char(msg[i]));
      mqttCommand += String(char(msg[i]));
    }
  }

  //action if message is recieved
  Serial.println(mqttCommand);
  if(mqttCommand == "ON") {
    Serial.println("ON COMMAND");
    mqttCommand = "";//flushing mqttcommand variabel
  }
  else if (mqttCommand == "OFF") {
    Serial.println("OFF COMMAND");
    mqttCommand = "";//flushing mqttcommand variabel
  }
  else {
    Serial.println("Command not found");
    mqttCommand = "";//flushing mqttcommand variabel
  }
}

void mqttPublishBroker() {
  //check connection to broker
  if(!client.connected()) {
    connectMqttBroker();
    Serial.println("Broker connected");
  }
  //loop MQTT function
  client.loop();
  //publish temperature to defined topic
  client.publish(MQTT_PUB_TEMP, String(AHT10_temperature).c_str());
}

void mqttVoidSetup() {
  client.setServer(MQTT_BROKER3, MQTT_PORT);
  client.setCallback(recieveMqttBroker);
}