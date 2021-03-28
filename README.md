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
- Django
- Postgres

#### key commands
<code>docker-compose up</code>


#### exec into broker and publish test messages
<code>docker exec -it broker sh</code>
<br/>
<code>
mosquitto_pub -V mqttv311 -h localhost -d -t TOPIC -m "PAYLOAD"
</code>

<br/>

example topics (WIP): 
- technodrome/ROOM/SENSOR
- technodrome/hallway/bme680
- technodrome/basement/dht11

example payloads (WIP):
- technodrome,location=HALLWAY,sensor=BME680,unit=*C,type=temperature value=29.25
- technodrome,location=ATTIC,sensor=DHT11,unit=*F,type=temperature  value=81.86
- technodrome,location=BASEMENT,sensor=BME680,unit=hpa,type=pressure value=972.80
- technodrome,location=BASEMENT,sensor=BME680,unit=%,type=humidity value=42.76
- technodrome,location=BASEMENT,sensor=BME680,unit=KOhms,type=voc value=194.70
- technodrome,location=BASEMENT,sensor=BME680,unit=m,type=altitude value=342.27

Persistence:
The payload example above will afford that telegraf will write to the technodrome database and table.
The columns will reflect: location, sensor, topic, type(measuring), unit and value

#### exec and query influxdb
<code>docker exec -it influxdb sh</code>
<br/>
<code>influx</code>
<br/>
<code>use technodrome</code>
<br/>
<code>select * from technodrome</code>
<br/>

## Micropython commands:
<code>esptool.py --baud 460900 write_flash --flash_size=detect 0 esp8266-20210202-v1.14.bin </code><br/>
<code>ampy --port COM3 --baud 115200 put devices/mp/main.py </code><br/>
<code>picocom /dev/ttyS3 -b115200</code><br/>

## Initialize Django Server
Initialize DB:
<br/>
<code>python3 manage.py migrate</code>
<br/>
Initialize SuperUser Account:
<br/>
<code>python3 manage.py createsuperuser</code>
<br/>

## Sump Pump Monitoring 
#### South Side Pump Setup
|           | height(in)| diameter(in) | volume(in^3) | Gallons   |
|-----------|--------|----------|--------------|-----------|
| Total     | 22     | 18       | 5598.32      | 24.24     |
| Pre-Pump  | 6      | 18       | 1526.81      | 6.61      |
| Post-Pump | 2      | 18       | 508.94       | 2.21      |
| Delta     |        |          | 1,017.87     | 4.41     |

* not accounting for existing pump displacement


#### North Side Pump Setup

-Coming soon or never!
