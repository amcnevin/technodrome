# technodrome


#### key commands
<code>docker-compose up</code>


#### exec into broker
<code>docker exec -it broker sh
mosquitto_pub -V mqttv311 -h localhost -d -t technodrome/front/door -m "<PAYLOAD>"
</code>

example payloads:
- technodrome,location=office,unit=*C,type=temperature value=29.25
- technodrome,location=office,unit=hpa,type=pressure value=972.80
- technodrome,location=office,unit=%,type=humidity value=42.76
- technodrome,location=office,unit=KOhms,type=voc value=194.70
- technodrome,location=office,unit=m.type=altitude value=342.27


#### exec and query influxdb
<code>docker exec -it influxdb sh</br>
influx</br>
use technodrome</br>
select * from technodrome</br>
</code>



