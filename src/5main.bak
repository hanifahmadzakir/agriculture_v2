//defined pin use
#define led1 LED_BUILTIN

//defined MQTT parameter
#define MQTT_BROKER IPAddress(192.168.1.1) //if using cloud broker, replace with cloud/broker domain URL
#define MQTT_BROKER2 "broker.emqx.io"
#define MQTT_BROKER3 "192.168.0.208"
#define MQTT_PORT 1883
#define MQTT_USERNAME "emqx"
#define MQTT_PASSWORD "public"

//defined MQTT Topics
#define MQTT_PUB_TEMP "agriculture/62626262/parameter/AHT10/temperature"
#define MQTT_PUB_HUMIDITY "agriculture/62626262/parameter/AHT10/humidity"
#define MQTT_PUB_PRESSURE "agriculture/62626262/parameter/BMP180/pressure"
#define MQTT_PUB_PROVALT "agriculture/62626262/parameter/BMP180/provalt" //provided altitude
#define MQTT_PUB_TEMP180 "agriculture/62626262/parameter/BMP180/temp180" //temp from BMP180
#define MQTT_PUB_COMPALT "agriculture/62626262/parameter/BMP180/compalt" //computed altitude
#define MQTT_PUB_ABSALT "agriculture/62626262/parameter/BMP180/absalt" //absolute altitude
#define MQTT_PUB_RELALT "agriculture/62626262/parameter/BMP180/relalt" //relative altitude
#define MQTT_TEST_TOPIC "ESP8266/test/hanif/62626262/data" //test topic
#define MQTT_COMMAND "ESP8266/test/hanif/62626262/command" //test topic

//Library <Arduino.h> is mandatory when use PlatformIO
#include <Arduino.h>
#include <Adafruit_AHTX0.h>
#include <AHT10.h>
#include <Wire.h>
#include <Adafruit_BMP085.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <PubSubClient.h>
#include <SoftwareSerial.h>
#include <InfluxDbClient.h>
#include <InfluxDbCloud.h>
#include <ESP8266WebServer.h>

//Define InfluxDB Credential and URLs
#define INFLUXDB_URL "https://us-east-1-1.aws.cloud2.influxdata.com"
#define INFLUXDB_TOKEN "U-Sy7ToxYbhcqvcjoCPNwlpMlYvay0PgKH7GzgjciUjiMDUfuOQsXYo83rmuTiJa_ELsb9irdpTidAF99S5YpQ=="
#define INFLUXDB_ORG "5192782596477b75"
#define INFLUXDB_BUCKET "89ade82a2d784fcf"
#define TZ_INFO "UTC+7"
#define DEVICE "ESP8266"
// Declare InfluxDB client instance with preconfigured InfluxCloud certificate
InfluxDBClient influxClient(INFLUXDB_URL, INFLUXDB_ORG, INFLUXDB_BUCKET, INFLUXDB_TOKEN, InfluxDbCloud2CACert);
//Declare InfluxDB Datapoint
Point sensor("Agriculture");

//Holding global variable
//AHT10 Variable
float AHT10_temperature;
float AHT10_humidity;

//BMP180 variable
float BMP180_temperature;
float BMP180_pressure;
float BMP180_Alttitude;
float BMP180_SeaLevelPressure;

//General variable
float battVoltage;
int selector = 0;
int analogValue = 0;
long rssi;
int testValue = 100;
const size_t eventInterval = 1000;
size_t previousTime = 0;
String command;
String mqttCommand = "";

//sensor parameter/object
uint8_t readStatus = 0;
AHT10 myAHT10(AHT10_ADDRESS_0X38);
Adafruit_BMP085 BMP180;

SoftwareSerial rs485(D0, D5); //(RX, TX)
SoftwareSerial GPS(D8, D7); //(RX, TX)
WiFiClient espClient;
PubSubClient client(espClient);

//wifi/network credential
char ssid[] = "Penguin Network_2,4G";
char pass[] = "bilangwowdulu";
ESP8266WebServer server(80);

//function redefined before the loop, after add function below
//main code, please redefined below here
void connectWiFi();
void checkRSSI();
void checkAHT10();
void connectMqttBroker();
void recieveMqttBroker(char *topic, byte*msg, size_t msgLength);
void mqttPublishBroker();
void mqttVoidSetup();
void readSensorAHT10();
void readSensorBMP180();
void checkVoltage();
void sendRS485();
void checkBMP180();
void syncTime();
void sensorTag();
void sendSensorField();
void checkInfluxDBConnection();
void initializeWebServer();

//Main code
void setup(){
  pinMode(led1, OUTPUT);
  Serial.begin(9600);
  rs485.begin(9600);
  delay(1000);
  connectWiFi();
  delay(2000);
  checkAHT10();
  checkBMP180();
  syncTime();
  checkInfluxDBConnection();
  sensorTag();
  delay(1000);
  initializeWebServer();
}

void loop(){ 

  server.handleClient();

  //using millis instead of delay for checking voltage
  size_t currentTime = millis();
  if(currentTime - previousTime >= eventInterval)
  {
    checkVoltage();
    readSensorAHT10();
    readSensorBMP180();
    sendSensorField();
    previousTime = currentTime;
  }
}




//Function Section, please add function below and redefined
//the function before void setup()
//connect to Wifi SSID, credential defined in "wifi credential"

void connectWiFi(){
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid,pass);
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.println("Connecting...");
    delay(1000);
  }
  Serial.println("WiFi Connected to :"+String(ssid));
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
  checkRSSI();
  
}

//check WiFi RSSI(Recieved Signal Strength Indicator)
void checkRSSI(){
  rssi = WiFi.RSSI();
  Serial.print("RSSI: ");
  Serial.println(rssi);
}

//check AHT10 connection
void checkAHT10(){
  while(myAHT10.begin() != true)
  {
     Serial.println(F("AHT10 not connected or fail to load calibration coefficient"));
  }
  Serial.println("AHT10 OK");
}

void checkBMP180(){
  while(BMP180.begin() != true){
    Serial.println("Could not find valid BMP085/BMP180, check wiring!");
  }
  Serial.println("BMP180 OK!");

}

//connect to MQTT Broker and Subscribe to a topic
void connectMqttBroker(){
   while(!client.connected()){
    Serial.println("Connecting MQTT");
    if(client.connect("ESP8266")){
      client.subscribe(MQTT_COMMAND);
    }
   }
}

//Subscribe and Recieve MQTT Topic
void recieveMqttBroker(char *topic, byte*msg, size_t msgLength){
  if(String(topic) == MQTT_COMMAND)
  {
    Serial.println(topic);
    for (size_t i=0; i<msgLength; i++)
    {
      Serial.print(char(msg[i]));
      mqttCommand += String(char(msg[i]));
    }
  }

  //action if message is recieved
  Serial.println(mqttCommand);
    if(mqttCommand == "ON"){
      Serial.println("ON COMMAND");
      mqttCommand = "";//flushing mqttcommand variabel
    }
    else if (mqttCommand == "OFF"){
      Serial.println("OFF COMMAND");
      mqttCommand = "";//flushing mqttcommand variabel
    }
    else{
      Serial.println("Command not found");
      mqttCommand = "";//flushing mqttcommand variabel
    }
}

void mqttPublishBroker(){
    //check connection to broker
  if(!client.connected()){
    connectMqttBroker();
    Serial.println("Broker connected");
    // client.publish(MQTT_TEST_TOPIC,"ESP8266 Online");
  }
  //loop MQTT function
  client.loop();
  //publish temperature to defined topic, mandatory to convert any value to string
  client.publish(MQTT_PUB_TEMP, String(AHT10_temperature).c_str());//format (MQTT topic, String(payload).c_str())
}

//mandatory to call in void setup() if using MQTT
void mqttVoidSetup(){
    //connect to MQTT Broker and Port
  client.setServer(MQTT_BROKER3, MQTT_PORT);
  //handle MQTT recieved message
  client.setCallback(recieveMqttBroker);
}

//reading AHT10 sensor
void readSensorAHT10(){
  AHT10_humidity = myAHT10.readHumidity();
  AHT10_temperature = myAHT10.readTemperature();
  delay(10000);

  Serial.print("AHT10 Temperature : ");
  Serial.println(AHT10_temperature);
  Serial.print("AHT10 Humidity: ");
  Serial.println(AHT10_humidity);
}

//reading BMP180 sensor
void readSensorBMP180(){
  BMP180_temperature = BMP180.readTemperature();
  BMP180_pressure = BMP180.readPressure();
  BMP180_Alttitude = BMP180.readAltitude();

  Serial.print("BMP180 Temperature: ");
  Serial.println(BMP180_temperature);
  Serial.print("BMP180 Pressure: ");
  Serial.println(BMP180_pressure);
  Serial.print("BMP180 Real Alttitude: ");
  Serial.println(BMP180_Alttitude);
}

//check Battery voltage
void checkVoltage(){
  analogValue = analogRead(A0);
  battVoltage = analogValue * 5.0/1023;
}

//send data to RS485
void sendRS485(){
  if(selector == 1)
  {
    rs485.print("Temperature : ");
    rs485.println(AHT10_temperature);
    rs485.print("Humidity : ");
    rs485.println(AHT10_humidity);
    rs485.print("RSSI : ");
    rs485.println(rssi);
    rs485.print("Batt Voltage : ");
    rs485.println(battVoltage);
  }

}

void syncTime() {
  configTime(0, 0, "pool.ntp.org", "time.google.com", "time.windows.com");
  
  Serial.print("Syncing time");
  int attempts = 0;
  while (time(nullptr) < 1000000000 && attempts < 10) {
    Serial.print(".");
    delay(1000);
    attempts++;
  }
  
  time_t now = time(nullptr);
  Serial.println("\nCurrent time: " + String(ctime(&now)));
  
  if (now < 1000000000) {
    Serial.println("Failed to obtain time, retrying in 30s");
    delay(30000);
    syncTime(); // Recursive retry
  }
}

void checkInfluxDBConnection(){
  // Check server connection
    if (influxClient.validateConnection()) {
      Serial.print("Connected to InfluxDB: ");
      Serial.println(influxClient.getServerUrl());
    } else {
      Serial.print("InfluxDB connection failed: ");
      Serial.println(influxClient.getLastErrorMessage());
    }
}

void sensorTag(){
    sensor.addTag("device", DEVICE);
    sensor.addTag("SSID", WiFi.SSID());
}

void sendSensorField() {
  // Clear previous fields (keep tags)
  sensor.clearFields();
  
  // Add current measurements
  sensor.addField("AHT10Temperature", AHT10_temperature);
  sensor.addField("AHT10Humidity", AHT10_humidity);
  sensor.addField("BMP180Temperature", BMP180_temperature);
  sensor.addField("BMP180Pressure", BMP180_pressure);
  sensor.addField("BMP180Altitude", BMP180_Alttitude);
  
  // Debug output
  Serial.print("Writing: ");
  Serial.println(sensor.toLineProtocol());

  // Write to InfluxDB
  if (!influxClient.writePoint(sensor)) {
    Serial.print("InfluxDB write failed: ");
    Serial.println(influxClient.getLastErrorMessage());
    
    // Check if connection needs to be reestablished
    if (!influxClient.validateConnection()) {
      Serial.println("Reconnecting to InfluxDB...");
      influxClient.setConnectionParams(INFLUXDB_URL, INFLUXDB_ORG, INFLUXDB_BUCKET, INFLUXDB_TOKEN);
    }
  }
}

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
    // Initialize REST API server
  server.on("/", handleRoot);
  server.on("/api/sensors", handleGetAllSensors);
  server.on("/api/temperature", handleGetTemperature);
  server.on("/api/humidity", handleGetHumidity);
  server.on("/api/pressure", handleGetPressure);
  server.onNotFound(handleNotFound);
  
  server.begin();
  Serial.println("HTTP server started");
}