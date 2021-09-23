#ifndef MY_WIFI_H
#define MY_WIFI_H

#include <WiFi.h>
#include <WiFiMulti.h>
#include "debug.h"
#include "credentials.h"
WiFiClient espClient;

int wifiScan = -1;
WiFiMulti wifiMulti;
String wifiIpAdr;
String wifiMacAdr;

// event got an ip
void wifiGotIP(WiFiEvent_t event, WiFiEventInfo_t info)
{
  DEBUG_PRINTLN("IP address: ");
  wifiIpAdr = IPAddress(info.got_ip.ip_info.ip.addr).toString();
  DEBUG_PRINTLN(wifiIpAdr);
}


// ewvent wifi connected
void wifiConnected(WiFiEvent_t event, WiFiEventInfo_t info) {
  DEBUG_PRINTLN("Wifi Connected");
}

// event scan done
void wifiScanDone(WiFiEvent_t event, WiFiEventInfo_t info)
{
  DEBUG_PRINTLN("Scan done");
  wifiScan = WiFi.scanComplete();
}

void wifiDisconnected(WiFiEvent_t event, WiFiEventInfo_t info) {
  Serial.println("Wifi Disconnected");
  wifiIpAdr = "-.-.-.-";
}


void wifiSetup() {
  WiFi.disconnect(true);
  WiFi.mode(WIFI_STA);

  WiFi.onEvent(wifiGotIP, WiFiEvent_t::SYSTEM_EVENT_STA_GOT_IP);
  WiFi.onEvent(wifiScanDone, WiFiEvent_t::SYSTEM_EVENT_SCAN_DONE);
  WiFi.onEvent(wifiDisconnected, WiFiEvent_t::SYSTEM_EVENT_STA_DISCONNECTED);
  WiFi.onEvent(wifiConnected, WiFiEvent_t::SYSTEM_EVENT_STA_CONNECTED);

  // starting wifi
  wifiMulti.addAP(mySSID, myPW); // ssid to list of connectable aps
  wifiMulti.run();

  wifiMacAdr = String(WiFi.macAddress());
}

bool wifiConnected(){
  return WiFi.isConnected();
}

void wifiLoop() {

}

void wifiDisconnect() {
  wifiMulti.run(false);

}

#endif // #define MY_WIFI_H
