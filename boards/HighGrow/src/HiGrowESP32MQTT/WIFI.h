#ifndef MY_WIFI_H
#define MY_WIFI_H

#include <WiFi.h>
#include "debug.h"
#include "credentials.h"
WiFiClient espClient;

void wifiConnect()
{
  int count = 0;
  DEBUG_PRINT("[WIFI] Connecting to ");
  DEBUG_PRINTLN(mySSID);
  WiFi.disconnect();
  WiFi.mode(WIFI_STA);
  WiFi.setHostname(hostname);
  DEBUG_PRINTLN(WiFi.macAddress());
 
  while (WiFi.status() != WL_CONNECTED) {
    WiFi.begin(mySSID, myPW);
    delay(1000);
    DEBUG_PRINT(".");
  }
  /*while (WiFi.status() != WL_CONNECTED) 
  {
    WiFi.begin(mySSID, myPW);
    delay(1000);
    DEBUG_PRINT(".");
    count++;
    if (count > 20) ESP.restart();
  }*/
  WiFi.setHostname(hostname);
  DEBUG_PRINTLN("[WIFI] connected");
  DEBUG_PRINT("[WIFI] IP address: ");
  DEBUG_PRINTLN(WiFi.localIP());
  
}

void wifiDisconnect(){
  DEBUG_PRINTLN("[WIFI] Going to sleep...");
  WiFi.disconnect(true);
  WiFi.mode(WIFI_OFF);
  btStop();
  
}

#endif // #define MY_WIFI_H
