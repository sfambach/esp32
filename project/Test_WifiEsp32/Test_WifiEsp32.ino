/** Test the wifi connection of an esp 32 board
 *  This sketch uses wifi multi, but only one ssd was added, If you want add more;
 *  
 *  LIBS (included in this project)
 *  debug.h
 *  mywfi.h 
 *  
 *  Credits
 *  
 *  Created by S. Fambach visit https://www.fambach.net if you want.
 *  
 *  I got some Informations out of the examples of the Ardunio ESP32 Lib
 *  Link: https://github.com/espressif/arduino-esp32/tree/master/libraries/WiFi/src
 *  
 *  GPL2
 *  
 */
 
#include "debug.h"
#include "credentials.h"
#include "mywifi.h"

void setup(){
  DEBUG_INIT;
  wifiSetup();
  wifiStartScan();
}


void loop(){

  wifiLoop();
  printSignalStrength();
  if(wifiScan > 0){
    printScannedNetworks();
    wifiStartScan();
    wifiScan = 0;
  }
  delay(4000);
}
