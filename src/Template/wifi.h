#ifndef WIFI_H
#define WIFI_H

#include "credentials.h"

#ifdef ESP8266
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#elif defined ESP32
#include <WiFi.h>
#include <ESPmDNS.h>
#else
//#error Unknown controller
#endif




void setupWifi() {
  WiFi.mode(WIFI_STA);


#ifdef SSID
  WiFi.begin(SSID, SSID_PASSWD);
  DEBUG_PRINTLN("wifi begin");
#endif  // SSID

  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("Connection Failed! Rebooting...");
    delay(5000);
    ESP.restart();
  }
}

void loopWifi() {

  if (!WiFi.isConnected()) {
    ESP.restart();
  }
}
#endif  // WIFI_H
