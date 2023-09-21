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




void setupWifi(const char* ssid, const char* passwd) {

  DEBUG_PRINT("wifi begin");
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, passwd);

  DEBUG_PRINT("... connecting ");
  while (WiFi.waitForConnectResult() != WL_CONNECTED) {

    Serial.println("Connection Failed! Rebooting...");
    delay(1000);
    ESP.restart();
  }
}

void loopWifi() {

  if (!WiFi.isConnected()) {
    ESP.restart();
  }
}
#endif  // WIFI_H
