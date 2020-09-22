# Technodrome

A pet project for collecting and monitoring sensor data around my house.

Hardware:
- Adafruit Feather Huzzah ESP8266
- DHT11
- BME680
- HC-SR04

Software:
- Adafruit C libraries
- Mosquitto MQTT Broker
- Telegraf
- InfluxDB
- Grafana

#### key commands
<code>docker-compose up</code>


#### exec into broker
<code>docker exec -it broker sh</br>
mosquitto_pub -V mqttv311 -h localhost -d -t TOPIC -m "PAYLOAD"
</code>

example topics (WIP): 
- technodrome/<ROOM>/<SENSOR>
- technodrome/hallway/bme680
- technodrome/basement/dht11

example payloads (WIP):
- technodrome,location=office,unit=*C,type=temperature value=29.25
- technodrome,location=office,unit=hpa,type=pressure value=972.80
- technodrome,location=office,unit=%,type=humidity value=42.76
- technodrome,location=office,unit=KOhms,type=voc value=194.70
- technodrome,location=office,unit=m,type=altitude value=342.27

Persistence:
The payload example above will afford that telegraf will write to the technodrome database and table.
The columns will reflect: location, topic, type, unit and value

#### exec and query influxdb
<code>docker exec -it influxdb sh</br>
influx</br>
use technodrome</br>
select * from technodrome</br>
</code>


## Sump Pump Monitoring 
#### South Side Pump Setup
|           | height(in)| diameter(in) | volume(in^3) | Gallons   |
|-----------|--------|----------|--------------|-----------|
| Total     | 22     | 18       | 5598.32      | 24.24     |
| Pre-Pump  | 16     | 18       | 2035.75      | 8.8       |
| Post-Pump | 20     | 18       | 509.94       | 2.21      |
| Delta     |        |          | 1,526.81     | 6.61      |

* not accounting for existing pump displacement


#### North Side Pump Setup

-Coming soon!
