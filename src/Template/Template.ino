/******************************************************************************
* ESP Template projekt
* Libs:
* None specific
*
* Licence: AGPL3
* Author: S. Fambach
* Visit http://www.fambach.net if you want
******************************************************************************/
#include <Arduino.h>

#include "credentials.h"
#include "debug.h"





/** XXXXXXXX *****************************************************************/
/** XXXXXXXX *****************************************************************/


/** Main Programm ************************************************************/
void setup() {
  Serial.begin(115200);
  DEBUG_PRINTLN("Setup");

#ifdef WIFI_ACTIVE
DEBUG_PRINT("Setup Wifi");
  setupWifi(SSID,SSID_PASSWD);
DEBUG_PRINTLN("... OK");
#endif  // WIFI_ACTIVE
DEBUG_PRINTLN("Wifi NOK");

// do the ota
#ifdef OTA_ACTIVE
  setupOTA();
  DEBUG_PRINTLN("OTA OK");
#endif  //  OTA_ACTIVE


// handle NTP
#ifdef NTP_ACTIVE
  setupNTP(NTP_URL);
  DEBUG_PRINTLN("NTP OK");
#endif  // NTP_ACTIVE

}


void loop() {
//DEBUG_PRINTLN("Main Loop");

#ifdef WIFI_ACTIVE
  loopWifi();
#endif  // WIFI_ACTIVE


// handle ota
#ifdef OTA_ACTIVE
  ArduinoOTA.handle();
#endif  // OTA_ACTIVE


// handle NTP
#ifdef NTP_ACTIVE
  loopNTP();
#endif  // NTP_ACTIVE

  printTimeNTP();

  delay(2000);

}


/** Rest of implementations **************************************************/
