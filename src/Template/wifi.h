#include "esp_wifi_types.h"
#ifndef WIFI_H
#define WIFI_H

#include <Arduino.h>
//#include "settings.h"

#ifdef ESP8266
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#elif defined ESP32
#include <WiFi.h>
#include <ESPmDNS.h>
#include <esp_wifi.h>
#else
//#error Unknown controller
#endif

wifi_country_t mycountry =
{
  .cc = "DE",
  .schan = 1,
  .nchan = 13,
  .policy = WIFI_COUNTRY_POLICY_AUTO
};



void logWifiStatus() {

  wl_status_t status = WiFi.status();
  switch (status) {
    case wl_status_t::WL_NO_SHIELD: log_e("Wifi - Stopped"); break;
    //case wl_status_t::WL_STOPPED: log_v("Wifi - Stopped");break;
    case wl_status_t::WL_IDLE_STATUS: log_v("Wifi - IDLE"); break;
    case wl_status_t::WL_NO_SSID_AVAIL: log_e("Wifi - NO SSID"); break;
    case wl_status_t::WL_SCAN_COMPLETED: log_v("Wifi - Scan completed"); break;
    case wl_status_t::WL_CONNECTED: log_v("Wifi - connected"); break;
    case wl_status_t::WL_CONNECT_FAILED: log_e("Wifi - Failed"); break;
    case wl_status_t::WL_CONNECTION_LOST: log_d("Wifi - Connection Lost"); break;
    case wl_status_t::WL_DISCONNECTED: log_v("Wifi - Disconnected"); break;
  }
}

void logWifiScanNetwork() {

  esp_wifi_clear_ap_list();
  int n = WiFi.scanNetworks();
  log_v("Scan done");
  if (n == 0) {
    log_e("WiFi Scan Network - no networks found");
  } else {
    log_v("WiFi Scan Network - %i networks found", n);

    log_v("Nr | SSID                             | BSSID | RSSI | CH | Encryption");
    for (int i = 0; i < n; ++i) {

      String encryption;
      encryption.reserve(10);

      switch (WiFi.encryptionType(i)) {
        case WIFI_AUTH_OPEN:
          encryption = "open";
          break;
        case WIFI_AUTH_WEP:
          encryption = "WEP";
          break;
        case WIFI_AUTH_WPA_PSK:
          encryption = "WPA";
          break;
        case WIFI_AUTH_WPA2_PSK:
          encryption = "WPA2";
          break;
        case WIFI_AUTH_WPA_WPA2_PSK:
          encryption = "WPA+WPA2";
          break;
        case WIFI_AUTH_WPA2_ENTERPRISE:
          encryption = "WPA2-EAP";
          break;
          /*    case WIFI_AUTH_WPA3_PSK:
                Serial.print("WPA3");
                break;
            case WIFI_AUTH_WPA2_WPA3_PSK:
                encryption="WPA2+WPA3";
                break;
            case WIFI_AUTH_WAPI_PSK:
                encryption="WAPI";
                break;*/
        default:
          encryption = "unknown";
      }
      log_v("%2d  | %-32.32s | %5d | %4d | %2d | %s", i + 1, WiFi.SSID(i).c_str(), WiFi.BSSID(i), WiFi.RSSI(i), WiFi.channel(i), encryption);
    }
  }

  // Delete the scan result to free memory for code below.
  WiFi.scanDelete();
}

void clearWifi() {
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);
}

void setupWifi(const char* ssid, const char* passwd) {

  //

  WiFi.disconnect(true, true);
  WiFi.mode(WIFI_OFF);
  delay(100);

  logWifiScanNetwork();
  WiFi.setTxPower(WIFI_POWER_8_5dBm);
  WiFi.mode(WIFI_STA);

  //esp_wifi_set_country(&mycountry);

  //wifi_country_t oldccode;
  //esp_wifi_get_country(&oldccode);
  //log_v("Country: %s low:%i high:%i policy: %i", oldccode.cc, oldccode.schan, oldccode.nchan, oldccode.policy);
  log_v("%s : %s ", ssid, passwd);

  IPAddress staticIP(192, 168, 10, 186);
  IPAddress gateway(192, 168, 10, 1);
  IPAddress subnet(255, 255, 255, 0);
  IPAddress dns(192, 168, 10, 1);

  //const uint8_t* bssid

  //WiFi.config(staticIP, gateway,subnet,dns,dns);
  WiFi.begin(ssid, passwd);  //, 0,  = NULL, true);


  log_v("wifi begin ssid: %s ... connecting", ssid);
  delay(1000);
  /*while (WiFi.waitForConnectResult() != WL_CONNECTED) { */
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    log_v(".");
  }
}

void loopWifi() {

  if (!WiFi.isConnected()) {
    ESP.restart();
  }
}
#endif  // WIFI_H
