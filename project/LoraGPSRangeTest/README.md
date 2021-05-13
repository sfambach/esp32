#Lora GPS Test 
![Project Image](https://github.com/sfambach/esp32/blob/master/project/LoraGPSRangeTest/img/EspLora32GPSProjectCombination.jpg)

## Hardware 
2* TTGO-Lora-Oled V1
1-2 Neo-6M GPS Modules (One for the sender, and optional one for the reveiver to calculate the distance)


## Libs
<p><a href="https://github.com/sandeepmistry/arduino-LoRa">https://github.com/sandeepmistry/arduino-LoRa</a></p>
<p><a href="https://github.com/ThingPulse/esp8266-oled-ssd1306">https://github.com/ThingPulse/esp8266-oled-ssd1306</a></p>
<p><a href="https://github.com/mikalhart/TinyGPSPlus">https://github.com/mikalhart/TinyGPSPlus</a></p>
<p><a href="https://github.com/knolleary/pubsubclient">https://github.com/knolleary/pubsubclient</a></p>
<p><a href="https://github.com/sfambach/OledTable">https://github.com/sfambach/OledTable</a></p>


## Software
### Sender
Gets GPS data from the module and sends it via lora to the receiver

### Receiver
Receives the GPS data from lora and sends them to a MQTT Broker via WIFI. 

## Credits
Project programmed by Stefan Fambach visit fambach.net if you want. 
