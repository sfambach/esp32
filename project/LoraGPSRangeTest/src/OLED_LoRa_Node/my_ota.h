#ifndef MY_OTA_H
#define MY_OTA_H

#include "debug.h"
#include <ESPmDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>

void otaSetup(){
  ArduinoOTA
    .onStart([]() {
      String type;
      if (ArduinoOTA.getCommand() == U_FLASH)
        type = "sketch";
      else // U_SPIFFS
        type = "filesystem";

      // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
      DEBUG_PRINTLN("Start updating " + type);
    })
    .onEnd([]() {
      DEBUG_PRINTLN("\nEnd");
    })
    .onProgress([](unsigned int progress, unsigned int total) {
      DEBUG_PRINT("Progress: ");
      DEBUG_PRINTLN((progress / (total / 100)));
    })
    .onError([](ota_error_t error) {
     // DEBUG_PRINTLNF("Error[%u]: ", error);
      if (error == OTA_AUTH_ERROR) DEBUG_PRINTLN("Auth Failed");
      else if (error == OTA_BEGIN_ERROR) DEBUG_PRINTLN("Begin Failed");
      else if (error == OTA_CONNECT_ERROR) DEBUG_PRINTLN("Connect Failed");
      else if (error == OTA_RECEIVE_ERROR) DEBUG_PRINTLN("Receive Failed");
      else if (error == OTA_END_ERROR) DEBUG_PRINTLN("End Failed");
    });

  ArduinoOTA.begin();
 
}

void otaLoop(){
  // simple call handle 
  ArduinoOTA.handle();
}


#endif // #ifndef MY_OTA_H
