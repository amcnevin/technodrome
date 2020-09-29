#include <ESP8266WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"
#include <Adafruit_BME680.h>
#include <bme680.h>
#include <bme680_defs.h>

#define WLAN_SSID       "<SSID>"
#define WLAN_PASS       "<PASSWORD>"

#define TECHNODROME_SERVER      "<IP_ADDR>"
#define TECHNODROME_PORT  1883

// TODO find local sea level pressure
#define SEALEVELPRESSURE_HPA (1013.25)

Adafruit_BME680 bme; // I2C
String LOCATION = "HALLWAY";
String DEVICE = "BME680";
String TOPIC = "technodrome/HALLWAY/BME680";

float interval = 1.8e+6;

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
void setupDevice() {
  if (!bme.begin()) {
    Serial.println("Could not read BME680 sensor");
    while (1);
  }

  // Set up oversampling and filter initialization
  bme.setTemperatureOversampling(BME680_OS_8X);
  bme.setHumidityOversampling(BME680_OS_2X);
  bme.setPressureOversampling(BME680_OS_4X);
  bme.setIIRFilterSize(BME680_FILTER_SIZE_3);
  bme.setGasHeater(320, 150); // 320*C for 150 ms
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
  return buildPayload("temperature", "*F", String((bme.temperature * 9/5) + 32));
}

/**
 * reads the Pressure and builds the payload
 */
String getPressure() {
  return buildPayload("pressure", "hPa", String(bme.pressure / 100.0));
}

/**
 * reads the humidity and builds the payload
 */
String getHumidity() {
  return buildPayload("humidity", "%", String(bme.humidity));
}

/**
 * reads the VOC and builds the payload
 */
String getVOC() {
  return buildPayload("voc", "KOhms", String(bme.gas_resistance / 1000.0));
}

/**
 * reads the altitude and builds the payload
 */
String getAltitude() {
  return buildPayload("altitude", "m", String(bme.readAltitude(SEALEVELPRESSURE_HPA)));
}


void loop() {
  // ensure connection to MQTT Broker, check onboard LED
  connectToBroker();

  // publish temperature reading
  if (!publisher.publish(getTemperature().c_str())) {
    Serial.println(F("Failed reading temperature"));
  }

  // publish pressure reading
  if (!publisher.publish(getPressure().c_str())) {
    Serial.println(F("Failed reading pressure"));
  } 

  // publish humidity reading
  if (!publisher.publish(getHumidity().c_str())) {
    Serial.println(F("Failed reading humidity"));
  }
  
  // publish VOC reading
  if (!publisher.publish(getVOC().c_str())) {
    Serial.println(F("Failed reading VOC"));
  }

  // publish altitude
  if (!publisher.publish(getAltitude().c_str())) {
    Serial.println(F("Failed reading altitude"));
  } 

  // sleep for interval
  delay(interval);  
}
