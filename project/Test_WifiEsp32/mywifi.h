#ifndef MY_WIFI_H
#define MY_WIFI_H

#include <WiFi.h>
#include <WiFiMulti.h>
#include "debug.h"
#include "credentials.h"
WiFiClient espClient;

static int wifiScan = 0;
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



void wifiStartScan(){
  WiFi.scanNetworks(true, true, false);
}
void wifiSetup() {
  WiFi.disconnect(true);
  WiFi.setHostname(hostname);
  
  WiFi.mode(WIFI_STA);

  WiFi.onEvent(wifiGotIP, WiFiEvent_t::SYSTEM_EVENT_STA_GOT_IP);
  WiFi.onEvent(wifiScanDone, WiFiEvent_t::SYSTEM_EVENT_SCAN_DONE);
  WiFi.onEvent(wifiDisconnected, WiFiEvent_t::SYSTEM_EVENT_STA_DISCONNECTED);
  WiFi.onEvent(wifiConnected, WiFiEvent_t::SYSTEM_EVENT_STA_CONNECTED);

  // starting wifi
  for(int i = 0; i < SSID_SIZE ; i ++){
    wifiMulti.addAP(ssids[i].ssid, ssids[i].pw); // ssid to list of connectable aps
  }
 
  wifiMulti.run();
  wifiMacAdr = String(WiFi.macAddress());
  wifiScan = 0;

}

bool wifiConnected(){
  return WiFi.isConnected();
}

void wifiLoop() {

}

void wifiDisconnect() {
  wifiMulti.run(false);

}

void printSignalStrength(){
  DEBUG_PRINT("Connection strength: ");
  DEBUG_PRINT(WiFi.RSSI());
  DEBUG_PRINTLN(" dbm");
}

void printScannedNetworks(){

    if(wifiScan < 1){
      //DEBUG_PRINTLN("No Scan");
      return;
    }

    for (int i = 0; i < wifiScan; ++i) {
      // Print SSID and RSSI for each network found
      DEBUG_PRINT(i + 1);
      DEBUG_PRINT(": ");
      DEBUG_PRINT(WiFi.SSID(i));
      DEBUG_PRINT(" (");
      DEBUG_PRINT(WiFi.RSSI(i));
      DEBUG_PRINT(")");
      DEBUG_PRINTLN((WiFi.encryptionType(i) == WIFI_AUTH_OPEN)?" ":"*");
    }
}


#endif // #define MY_WIFI_H
