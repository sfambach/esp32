/**
   Lora Node, receives/sends .

  LIBS
  https://github.com/sandeepmistry/arduino-LoRa
  https://github.com/ThingPulse/esp8266-oled-ssd1306
  https://github.com/mikalhart/TinyGPSPlus (only for the distance)
  https://github.com/knolleary/pubsubclient

  https://github.com/tobiasschuerg/InfluxDB-Client-for-Arduino

  Created by S.Fambach visit https://www.fambach.net
  With help of the examples in the libs
*/


#include <Wire.h>
#include "credentials.h"
#include "debug.h"
#include "gps.h"
#include "display.h"
#include "my_lora.h"
#include "my_ota.h"
#include "message.h"
#include <WiFi.h>
#include <ESPmDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>

int packageCount = 0;
String rssi = "--";
String packSize = "--";
String packet ;

TaskHandle_t DisplayTask;
TaskHandle_t postMessageHandle;

void postMessageTask(void* params) {
  while (true) {

    Message* msg = new Message();
    msg->_time = gpsGetTime();
    msg->_date = gpsGetDate();
    msg->_lat = gpsGetLat(6);
    msg->_lng = gpsGetLng(6);
    msg->_mac = WiFi.macAddress();

    if ( wifiConnected() ) { // if wifi is connected send the data via mqtt


      #ifdef USE_MQTT
        DEBUG_PRINTLN("Send Data via WIFI");
        mqttPublish(JSON_TOPIC, msg->toJSON());
        delete msg;
      #else


          gps.speed.kmph()
          gps.altitude.meters()
          gps.satellites.value()
          gps.failedChecksum()
          
          // Define data point with measurement name 'device_status`
          Point pointDevice("gps_data");
          // Set tags
          pointDevice.addTag("mac", WiFi.macAddress());
          pointDevice.addTag("time", WiFi.SSID());
          // Add data
          pointDevice.addField("rssi", WiFi.RSSI());
          pointDevice.addField("uptime", millis());
          
          // Write data
          client.writePoint(pointDevice);

      #endif

    } else { // if not send via lora
      DEBUG_PRINTLN("Send Data via Lora");
      loraAddMessage(msg);
      packageCount++;
      delete msg;

    }

    delay(1000);

  }

}


/** post the gained informations */
void postData(Message* msg) {



  //  char buf2[255];
  //
  //  long distance = (long)TinyGPSPlus::distanceBetween(
  //      _lat.toFloat(),
  //      _lng.toFloat(),
  //      HOME_LAT,
  //      HOME_LON);
  //  distance += GPS_CALIBRATION;
  //

}


void cbk(int packetSize) {
  String packet = "";
  packet.reserve(50);

  for (int i = 0; i < packetSize; i++) {
    packet += (char) LoRa.read();
  }
  rssi = "RSSI " + String(LoRa.packetRssi(), DEC) ;
  //displayData();
  //parseReceiveMessage(packet);
}

void displayDataTask(void * pvParameters) {

  while (true) { // run forever ...

    displayLoop();

    // display something
    table.clear();

    table.setText(0, 0, wifiMacAdr, true);
    table.setText(0, 1, wifiIpAdr, true);

    if (gpsConnected) {
      //table.setText(0, 0, "Lat/Lng" , true);
      table.setText(1, 0, gpsGetDate(), true);
      table.setText(1, 1, gpsGetTime(), true);


      //table.setText(1, 0, "Lat/Lng" , true);
      table.setText(2, 0, gpsGetLat(6) , true);
      table.setText(2, 1, gpsGetLng(6) , true);
    }

    table.setText(3, 0, "Packets" , true);
    table.setText(3, 1, String(packageCount) , true);

    table.refresh();

    delay(700);
  }
}

void setup() {

  // displaySetup();
  pinMode(25, OUTPUT);

  DEBUG_INIT;

  // setup display
  displaySetup();
  xTaskCreatePinnedToCore(
    displayDataTask,   /* Task function. */
    "DisplayTask",     /* name of task. */
    10000,       /* Stack size of task */
    NULL,        /* parameter of the task */
    1,           /* priority of the task */
    &DisplayTask,      /* Task handle to keep track of created task */
    1);          /* pin task to core 1 */
  delay(500);


  // init gps
  gpsSetup();

  // lora setup
  loraSetup();

  // wifi
  wifiSetup();

  // init ota
  otaSetup();

  //
  mqttSetup();



  xTaskCreatePinnedToCore(
    postMessageTask,   /* Task function. */
    "DisplayTask",     /* name of task. */
    10000,       /* Stack size of task */
    NULL,        /* parameter of the task */
    1,           /* priority of the task */
    &postMessageHandle,      /* Task handle to keep track of created task */
    1);          /* pin task to core 1 */
  delay(500);

}

void loop() {
  mqttLoop();

  loraLoop();
  if (loraPacketSize) {
    cbk(loraPacketSize);
  }

  //gpsDisplayInfo();

  otaLoop();

  //  // mqtt
  //  char buf2[200];
  //  sprintf(buf2, JSON_TOPIC_TEMPLATE, gpsGetTime, gpsGetDate(), gpsGetLat(6), gpsGetLng(6), WiFi.macAddress());
  //  DEBUG_PRINTLN(buf2);
  //  mqttPublish(JSON_TOPIC, buf2);

}
