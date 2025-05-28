#include "../../include/network/influxdb.h"
#include "../include/config.h"
#include "../../include/sensors/aht10.h"
#include "../../include/sensors/bmp180.h"
#include "../../include/network/wifi.h"

InfluxDBClient influxClient(INFLUXDB_URL, INFLUXDB_ORG, INFLUXDB_BUCKET, INFLUXDB_TOKEN, InfluxDbCloud2CACert);
Point sensor("Agriculture");

void checkInfluxDBConnection() {
  if (influxClient.validateConnection()) {
    Serial.print("Connected to InfluxDB: ");
    Serial.println(influxClient.getServerUrl());
  } else {
    Serial.print("InfluxDB connection failed: ");
    Serial.println(influxClient.getLastErrorMessage());
  }
}

void sensorTag() {
  sensor.addTag("device", DEVICE);
  sensor.addTag("SSID", WiFi.SSID());
}

void sendSensorField() {
  sensor.clearFields();
  sensor.addField("AHT10Temperature", AHT10_temperature);
  sensor.addField("AHT10Humidity", AHT10_humidity);
  sensor.addField("BMP180Temperature", BMP180_temperature);
  sensor.addField("BMP180Pressure", BMP180_pressure);
  sensor.addField("BMP180Altitude", BMP180_Alttitude);
  
  Serial.print("Writing: ");
  Serial.println(sensor.toLineProtocol());

  if (!influxClient.writePoint(sensor)) {
    Serial.print("InfluxDB write failed: ");
    Serial.println(influxClient.getLastErrorMessage());
    
    if (!influxClient.validateConnection()) {
      Serial.println("Reconnecting to InfluxDB...");
      influxClient.setConnectionParams(INFLUXDB_URL, INFLUXDB_ORG, INFLUXDB_BUCKET, INFLUXDB_TOKEN);
    }
  }
}