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
#include "settings.h"






/** XXXXXXXX *****************************************************************/
/** XXXXXXXX *****************************************************************/


/** Main Programm ************************************************************/
void setup() {
  Serial.begin(115200);
  Serial.setDebugOutput(true);

#ifdef WIFI_ACTIVE
  log_v("Setup Wifi");
  setupWifi(SSID,SSID_PASSWD);
#endif  // WIFI_ACTIVE


// do the ota
#ifdef OTA_ACTIVE
  log_v("Setup Ota");
  setupOTA();
#endif  //  OTA_ACTIVE

  
// handle NTP
#ifdef NTP_ACTIVE
  log_v("Setup NTP");
  setupNTP(NTP_URL);
  
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
  printTimeNTP();
#endif  // NTP_ACTIVE

  

  delay(2000);

}


/** Rest of implementations **************************************************/
