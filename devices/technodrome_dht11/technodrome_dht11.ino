#include <DHT.h>

#include <ESP8266WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"
#define DHTTYPE DHT11 
#define DHTPIN 5


#define WLAN_SSID       "<SSID>"
#define WLAN_PASS       "<PASSWORD>"

#define TECHNODROME_SERVER      "<IP_ADDR>"
#define TECHNODROME_PORT  1883

String LOCATION = "ATTIC";
String DEVICE = "DHT11";
String TOPIC = "technodrome/ATTIC/DHT11";

float interval = 1.8e+6;


DHT dht(DHTPIN, DHTTYPE);
WiFiClient client;
Adafruit_MQTT_Client mqtt(&client, TECHNODROME_SERVER, TECHNODROME_PORT, "", "");
Adafruit_MQTT_Publish publisher = Adafruit_MQTT_Publish(&mqtt, TOPIC.c_str()); 

void setup() {
  pinMode(0, OUTPUT);
  digitalWrite(0, HIGH); // board LED for liveness
  Serial.begin(115200);
  delay(10);
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
void setupDevice(){
  dht.begin();
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
 * reads the Temperature and builds the payload
 */
String getTemperature() {
   String payload = buildPayload("temperature", "*F", String((dht.readTemperature() * 9/5) + 32));
   Serial.println(payload);
   return payload;
}


/**
 * reads the humidity and builds the payload
 */
String getHumidity() {
  String payload = buildPayload("humidity", "%", String(dht.readHumidity()));
  Serial.println(payload);
  return payload;
}



void loop() {
  // ensure connection to MQTT Broker, check onboard LED
  connectToBroker();

  // publish temperature reading
  if (!publisher.publish(getTemperature().c_str())) {
    Serial.println(F("Failed reading temperature"));
  }


  // publish humidity reading
  if (!publisher.publish(getHumidity().c_str())) {
    Serial.println(F("Failed reading humidity"));
  }
  
   // sleep for interval
  delay(interval);  
}
