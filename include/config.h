#ifndef CONFIG_H
#define CONFIG_H

// Pin definitions
#define led1 LED_BUILTIN

// MQTT parameters
#define MQTT_BROKER IPAddress(192, 168, 1, 1)
#define MQTT_BROKER2 "broker.emqx.io"
#define MQTT_BROKER3 "192.168.0.208"
#define MQTT_PORT 1883
#define MQTT_USERNAME "emqx"
#define MQTT_PASSWORD "public"

// MQTT Topics
#define MQTT_PUB_TEMP "agriculture/62626262/parameter/AHT10/temperature"
#define MQTT_PUB_HUMIDITY "agriculture/62626262/parameter/AHT10/humidity"
#define MQTT_PUB_PRESSURE "agriculture/62626262/parameter/BMP180/pressure"
#define MQTT_PUB_PROVALT "agriculture/62626262/parameter/BMP180/provalt"
#define MQTT_PUB_TEMP180 "agriculture/62626262/parameter/BMP180/temp180"
#define MQTT_PUB_COMPALT "agriculture/62626262/parameter/BMP180/compalt"
#define MQTT_PUB_ABSALT "agriculture/62626262/parameter/BMP180/absalt"
#define MQTT_PUB_RELALT "agriculture/62626262/parameter/BMP180/relalt"
#define MQTT_TEST_TOPIC "ESP8266/test/hanif/62626262/data"
#define MQTT_COMMAND "ESP8266/test/hanif/62626262/command"

// InfluxDB Credentials
#define INFLUXDB_URL "https://us-east-1-1.aws.cloud2.influxdata.com"
#define INFLUXDB_TOKEN "U-Sy7ToxYbhcqvcjoCPNwlpMlYvay0PgKH7GzgjciUjiMDUfuOQsXYo83rmuTiJa_ELsb9irdpTidAF99S5YpQ=="
#define INFLUXDB_ORG "5192782596477b75"
#define INFLUXDB_BUCKET "89ade82a2d784fcf"
#define TZ_INFO "UTC+7"
#define DEVICE "ESP8266"

// WiFi credentials
const char ssid[] = "Penguin Network_2,4G";
const char pass[] = "bilangwowdulu";

#endif