# My Adoption of Claus Kuehnels HiGow MQTT Project

## Changelog

### Initial changes
- Fix error wrong Integer value size
- Put all settings in the setting credentials.h
- Create own structure for MQTT
- ... and some more i can't no longer remember ;)

### 020212104 I Have done some changes ... 
- MQTT Struture to be dynamically filled by defines
- Added a JSON Mqtt which is easier to parse by node red 
- Check the result of the temp and humidity sensor, if it is nan, a new sample will be done as long as it is no longer nan.
- Implemented Support for LillyGo HiGrow sensor
- Add a Calibration value for the Temp Sensor


#Credits

## Original Code
My code is based on (c) Claus Kuehnel 2018-03-19 info@ckuehnel.ch 's code.

The program HiGrowESP32MQTT.ino refers to the following two posts

https://ckblog2016.wordpress.com/2018/03/18/higrow-sensor-sorgt-fuer-das-wohl-der-pflanzen/
https://ckblog2016.wordpress.com/2018/03/19/higrow-sensor-daten-erfassen-und-versenden/

describing the use of HiGrow sensor for plant monitoring.
   
Here the temperature and humdity of the air, the soil moisture and light were measured by the HiGrow
sensor and were published as MQTT message. 
   
I use the CloudMQTT as MQTT Broker offered by the Swedish company 84codes AB in Stockholm.
   
You have to change the file credentials.h to your conditions - WiFi and MQTT access.

(c) Claus Kuehnel 2018-03-19 info@ckuehnel.ch

## Adopted
By S. Fambach, 
Visit Fambach.net if you want ;) 

