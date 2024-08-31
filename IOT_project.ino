#include <WiFiNINA.h>
#include <PubSubClient.h>
#include <DHT.h>
#include <ArduinoLowPower.h>
#include "secret.h"
#include "mqtt_setup.h"

// PINS DECLARATIONS
#define DHTPIN 2
#define DHTTYPE DHT11
#define PHOTOCELL_PIN 7
#define PIR_PIN 3
// GLOBAL VARIABLES DECLARATION
#define WIFI_RECONNECTION_LIMIT 3
#define MQTT_RECONNECTION_LIMIT 3
#define NO_CONNECTION_SLEEP 600000 //10 MINUTES
#define NORMAL_SLEEP 60000 //1 MINUTE

DHT dht(DHTPIN, DHTTYPE);

// WIFI NINA DECLARATION
WiFiClient wifiClient;

// MQTT DECLARATION
PubSubClient client(wifiClient);

// USEFUL VARIABLES
//const char *ssid = SSID_MI; 
//const char *passwd = PSW_MI;
const char *ssid = SSID_LA;
const char *passwd = PSW_LA; 

int status = WL_IDLE_STATUS;
char temperature[5];
char humidity[5];
char brightness[5];
char human_presence[2];

//-----------------------------------------SETUP-------------------------------------------
void setup() {
  pinMode(PIR_PIN, INPUT);
// setting up pin for DHT sensor
  dht.begin();
  delay(1000);
// setting up MQTT
  client.setServer(BROKER, PORT);
  delay(1000);
// setting up  WiFi connection
  while(!wifiConnect()){
    LowPower.deepSleep(NO_CONNECTION_SLEEP);
  }
// wait before entering the loop to be able to update new sketch (otherwise it crashes)
  delay(5000);
}

//-----------------------------------------LOOP---------------------------------------------

void loop() {
// checking WIFI connection and commanding a deep sleep if the sensor can't connect
  while(!wifiConnect()){
    LowPower.deepSleep(NO_CONNECTION_SLEEP);
  }
// checking MQTT connection and commanding a deep sleep if the sensor can't connect
  while(!mqttConnect()){
    LowPower.deepSleep(NO_CONNECTION_SLEEP);
  }
  
//READ AND PUBLISH DHT AND PHOTOCELL MEASUREMENTS
  
// reading values from dht sensor
  float temp = dht.readTemperature();
  float hum = dht.readHumidity();
  
// photocell reading
  int bright = (analogRead(PHOTOCELL_PIN)); 
  
// PIR reading
  int human_pres = digitalRead(PIR_PIN);
  
// if there are errors report it      
  if(isnan(temp) || isnan(hum)){
    client.publish(error_topic, "DHT NOT FOUND!");
  } 
  else {  
// if everything ok
// converting floats to char arrays
    char temp_int[2]; 
    char humid_int[2];
    itoa( (int) temp, temp_int, 10);
    itoa( (int) hum, humid_int, 10);
    itoa(bright, brightness, 10);
    itoa(human_pres, human_presence, 10);
    
    temperature[0] = 0;
    strcat(temperature, temp_int);
    strcat(temperature, " °C");
    
    humidity[0] = 0;
    strcat(humidity, humid_int);
    strcat(humidity, "%");
    
// publish the measured data        
    mqttPublish(); 
  }      
// sleep
  LowPower.deepSleep(NORMAL_SLEEP);
}

//------------------------------------------------FUNCTIONS----------------------------------------------------

//MQTT CONNECTION <------------
bool mqttConnect() {
  int count = 0;
  while (!client.connected()) {
    while(!client.connect(CLIENT_ID, USER, PSW, willTopic, willQoS, willRetain, willMessage, cleanSession)){
      if (count == MQTT_RECONNECTION_LIMIT){
        return false;
      }
    count++;  
    delay(1000);
    }
  }
  client.publish(pub_topic, "Sensors connected");
  return true;
}

//WIFI CONNECTION  <-----------
bool wifiConnect(){
  int count = 0;
  while (status != WL_CONNECTED) {
    if (count == WIFI_RECONNECTION_LIMIT){
      return false;
    }
  count++;
  status = WiFi.begin(ssid, passwd);
  delay(1000);
  }
  return true;
}

//MQTT MEASURE PUBLISHING <----------
void mqttPublish(){
  if (wifiConnect() && client.connected()){
    client.publish(temperature_topic, temperature);
    client.publish(humidity_topic, humidity);
    client.publish(brightness_topic, brightness);
    client.publish(human_presence_topic, human_presence);
  }
}
