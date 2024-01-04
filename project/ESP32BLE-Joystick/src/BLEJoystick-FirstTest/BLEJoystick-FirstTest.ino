/******************************************************************************
* 
* Licence: AGPL3
* Author: S. Fambach
* Visit http://www.fambach.net if you want
******************************************************************************/


/** Debuging *****************************************************************/
#define DEBUG
#ifdef DEBUG
#define DEBUG_PRINTLN(x) Serial.println(x)
#define DEBUG_PRINT(x) Serial.print(x)
#else
#define DEBUG_PRINTLN(x)
#define DEBUG_PRINT(x)
#endif


/** NimBle *****************************************************************/
#define ENABLED_NIMBLE
#ifdef ENABLED_NIMBLE

#define DEVICE_NAME "MyJoy"
#define BLE_ID "4A30E531-B262-402F-9D5A-605D36DDBC15" // generated with https://bleid.netlify.app/
#include NimBLEDevice.h

void setupBLE(){
  NimBLEDevice::init(DEVICE_NAME)
}
#endif
/** XXXXXXXX *****************************************************************/
/** XXXXXXXX *****************************************************************/
/** XXXXXXXX *****************************************************************/
/** XXXXXXXX *****************************************************************/


/** Main Programm ************************************************************/
void setup() {
  Serial.begin(115200);
  DEBUG_PRINTLN("Setup");

  #ifdef ENABLED_NIMBLE
  setupBLE();
  #endif
}


void loop() {
  //DEBUG_PRINTLN("Main Loop");
}

void serialEvent(){
  DEBUG_PRINTLN("Serial Event");
}

/** Rest of implementations **************************************************/