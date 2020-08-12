#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClientSecureBearSSL.h>
#include <WiFiClient.h>
#include <dht11.h>
#define DHT11PIN 5

/**
 *  Simple setup for connecting Slack with an ESP8266/DHT11 
 * 
 *  future state is replace slack integration with MQTT
 *  
 **/

dht11 DHT11;
const char* ssid     = "";
const char* password = "";
const char* location = "ATTIC";
const char* webhook = "";


void setup() {
  pinMode(0, OUTPUT);
  Serial.begin(115200);
  delay(100);
 
  connect_to_wifi(); 
}


void loop() {
  Serial.println();

  int chk = DHT11.read(DHT11PIN);
  float humidity = DHT11.humidity;
  float temperature = DHT11.temperature;

  // For debug purposes
  Serial.print("Humidity (%): ");
  Serial.println((float)humidity, 2);

  Serial.print("Temperature (C): ");
  Serial.println((float)temperature, 2);

  post_to_slack(humidity, temperature);

  // TODO make this configurable
  delay(1.8e+6);
}

void post_to_slack(float humidity, float temperature) {
  if(WiFi.status()== WL_CONNECTED){
    
    BearSSL::WiFiClientSecure client;
    HTTPClient https;
    client.setInsecure();

    Serial.print("[HTTPS] begin...\n");
    if (https.begin(client, webhook)) {  // HTTPS

      Serial.print("[HTTPS] POST...\n");

      // POST to Slack
      https.addHeader("Content-Type", "application/json");
      int httpCode = https.POST(build_payload(humidity, temperature));

      if (httpCode > 0) {
        Serial.printf("[HTTPS] POST... code: %d\n", httpCode);
      } else {
        Serial.printf("[HTTPS] POST... failed, error: %s\n", https.errorToString(httpCode).c_str());
      }

      https.end();
    } else {
      Serial.printf("[HTTPS] Unable to connect\n");
    }
  }
  

}

// TODO build a more parsable/MQTT message
String build_payload(float humidity, float temperature) {
  return String("{ \"text\": \"Location: " + String(location) + " Humidity (%): " + String(humidity) + "| Temperature (C): " + String(temperature) + "\"}");
  
}

void connect_to_wifi() {
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
 
  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

}
 
