/******************************************************************************
# Testing the web serial Lib

##Settings
See settings.h file

## Libraries
WebSerial Bibliothek 
https://github.com/ayushsharma82/WebSerial

ESPAsyncTCP 
https://github.com/me-no-dev/AsyncTCP

ESPAsyncWebServer 
https://github.com/me-no-dev/ESPAsyncWebServer


## Credits 
* Licence: AGPL3
* Author:  S. Fambach
* Web:  http://www.fambach.net 
******************************************************************************/
#include <Arduino.h>
#if defined(ESP8266)
  #include <ESP8266WiFi.h>
  #include <ESPAsyncTCP.h>
#elif defined(ESP32)
  #include <WiFi.h>
  #include <AsyncTCP.h>
#endif
#include <ESPAsyncWebServer.h>
#include <WebSerial.h>

/*****************************************************************************/
// Debuging
#define DEBUG
#ifdef DEBUG
#define DEBUG_PRINTLN(x) Serial.println(x)
#define DEBUG_PRINT(x) Serial.print(x)
#else
#define DEBUG_PRINTLN(x)
#define DEBUG_PRINT(x)
#endif

/*****************************************************************************/
// settings

#include "my_settings.h" // copy of settings.h
// #include "settings.h"

/*****************************************************************************/
// Webserver
AsyncWebServer server(80);


/*****************************************************************************/
// Webserial
/* Message callback of WebSerial */
void recvMsg(uint8_t *data, size_t len){
  WebSerial.println("Received Data...");
  String d = "";
  for(int i=0; i < len; i++){
    d += char(data[i]);
  }
  WebSerial.println(d);
}

/*****************************************************************************/
// XXXXXXXX

/*****************************************************************************/
// XXXXXXXX


/*****************************************************************************/
// Main Programm 

void setup() {
  Serial.begin(115200);
  DEBUG_PRINTLN(F("Setup - Start with WIFI"));

  // init wifi 
    WiFi.mode(WIFI_STA);
    WiFi.begin(SSID, SSID_PASSWD);
    if (WiFi.waitForConnectResult() != WL_CONNECTED) {
        DEBUG_PRINTLN(F("WiFi Failed!\n"));
        return;
    }
    
    
    DEBUG_PRINTLN(F("WIFI OK -> Init Webserial"));
    // WebSerial is accessible at "<IP Address>/webserial" in browser
    WebSerial.begin(&server);
    /* Attach Message Callback */
    WebSerial.msgCallback(recvMsg);
    server.begin();

    DEBUG_PRINT(F("You can access webserial at: http://"));
    DEBUG_PRINT(WiFi.localIP());
    DEBUG_PRINTLN(F("/webserial"));

}


void loop() {
   delay(2000);
    
    WebSerial.print(F("IP address: "));
    WebSerial.println(WiFi.localIP());
    WebSerial.printf("Millis=%lu\n", millis());
    WebSerial.printf("Free heap=[%u]\n", ESP.getFreeHeap());
}
/*****************************************************************************/
