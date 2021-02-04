#include <ESP8266WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"


#define WLAN_SSID       "<SSID>"
#define WLAN_PASS       "<PASSWORD>"

#define TECHNODROME_SERVER      "<IP_ADDR>"
#define TECHNODROME_PORT  1883



String LOCATION = "BASEMENT";
String DEVICE = "SOUTH_SUMP_PUMP";
String TOPIC = "technodrome/BASEMENT/SOUTH_SUMP_PUMP";

#define trigPin 14
#define echoPin 12

/* 1 minute interval instead */
float interval = 60000;

WiFiClient client;
Adafruit_MQTT_Client mqtt(&client, TECHNODROME_SERVER, TECHNODROME_PORT, "", "");
Adafruit_MQTT_Publish publisher = Adafruit_MQTT_Publish(&mqtt, TOPIC.c_str()); 

void setup() {
  Serial.begin(9600);
  pinMode(0, OUTPUT);
  digitalWrite(0, HIGH); // board LED for liveness
  connectToWifi();
  setupDevice();
}
 
/**
 * Connect to the wifi
 */
void connectToWifi(){
  WiFi.begin(WLAN_SSID, WLAN_PASS);
  WiFi.mode(WIFI_STA);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }

  Serial.println("WiFi connected");
  Serial.print("IP address: "); 
  Serial.println(WiFi.localIP());
} 

/**
 * Initiate Device reading and Sampling
 */
void setupDevice() {
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input  
}

/**
 * Connect to the MQTT Broker
 */
void connectToBroker() {
  int8_t ret;
  
  if (mqtt.connected()) {
    return;
  }

  uint8_t retries = 3;
  while ((ret = mqtt.connect()) != 0) { 
    Serial.println(mqtt.connectErrorString(ret));
    Serial.println("Retrying MQTT connection in 5 seconds...");
    mqtt.disconnect();
    delay(5000);  // wait 5 seconds
    retries--;
    if (retries == 0) {
      digitalWrite(0, LOW); // visual indicator of not connected
      while (1);
    }
  }
  Serial.println("MQTT Connected!");
}

/**
 * build a Payload for downstream ingestion into InfluxDB
 * format example:
 * technodrome,location=HALWAY,sensor=BME680,unit=*F,type=temperature value=89
 */
String buildPayload(String type, String measurement, String value) {
   return String("technodrome,location="+LOCATION+",sensor="+DEVICE+",unit="+measurement+",type="+type+"  value="+value);
}

/**
 * reads the Distance and builds the payload
 */
String getDistance() {
  long duration;
  String distance;

  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);

  // Calculating the distance in inches
  distance= String((duration*0.034/2)/2.54);
  
  return buildPayload("depth", "in", distance);
}

void loop() {
  // ensure connection to MQTT Broker, check onboard LED
  connectToBroker();
  // publish distance reading
  if (!publisher.publish(getDistance().c_str())) {
    Serial.println(F("Failed reading distance"));
  }
  //Serial.println(getDistance().c_str());


  // sleep for interval
  delay(interval);  
}
