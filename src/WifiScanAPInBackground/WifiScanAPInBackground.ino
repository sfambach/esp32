/** Scan the aps
 *  
 *  Credits
 *  This project is derived from the example project in the arduino esp32 lib
 *  Author Stefan Fambach
 *  Visit https://Fambach.net/f you want.
 */

#include <WiFiGeneric.h>
#include <WiFi.h>
#include <WiFiMulti.h>

// WiFi
const char* mySSID = "NODES";
const char* myPW = "HappyNodes1234";

WiFiMulti wifiMulti;

// // event comes after wifi connect
void wifiGotIP(WiFiEvent_t event, WiFiEventInfo_t info)
{
  Serial.println("IP address: ");
  Serial.println(IPAddress(info.got_ip.ip_info.ip.addr));
}
int wifiScan = -1;

void wifiConnected(WiFiEvent_t event, WiFiEventInfo_t info) {
  Serial.println("Wifi Connected");
}


void wifiScanDone(WiFiEvent_t event, WiFiEventInfo_t info)
{
  Serial.println("Scan done");
  wifiScan = WiFi.scanComplete();

}




void wifiDisconnected(WiFiEvent_t event, WiFiEventInfo_t info) {
  Serial.println("Wifi Disconnected");
}

void setup() {


  Serial.begin(115200);
  WiFi.disconnect(true);
  delay(1000);

  wifiMulti.addAP(mySSID, myPW); // ssid to list of connectable aps

  // Examples of different ways to register wifi events
  WiFi.onEvent(wifiGotIP, arduino_event_id_t::ARDUINO_EVENT_WIFI_STA_GOT_IP);
  WiFi.onEvent(wifiScanDone, arduino_event_id_t::ARDUINO_EVENT_WIFI_SCAN_DONE);
  WiFi.onEvent(wifiDisconnected, arduino_event_id_t::ARDUINO_EVENT_WIFI_STA_DISCONNECTED);
  WiFi.onEvent(wifiConnected, arduino_event_id_t::ARDUINO_EVENT_WIFI_STA_CONNECTED);


  //    Wifi.onEvent();
  //
  //
  //    WiFiEventId_t eventID = WiFi.onEvent([](WiFiEvent_t event, WiFiEventInfo_t info){
  //        Serial.print("WiFi lost connection. Reason: ");
  //        Serial.println(info.wifi_sta_disconnected.reason);
  //    }, WiFiEvent_t::ARDUINO_EVENT_WIFI_STA_DISCONNECTED);
  //
  //    // Remove WiFi event
  //    Serial.print("WiFi Event ID: ");
  //    Serial.println(eventID);
  //    // WiFi.removeEvent(eventID);

  //WiFi.begin(mySSID, myPW);
  // start wifi network
  wifiMulti.run();

}



void loop() {

  if (wifiScan > 0 ) {
    Serial.println("Got Scan:");
    for (int i = 0; i < wifiScan; ++i) {

      // Print SSID and RSSI for each network found
      Serial.print(i + 1);
      Serial.print(": ");
      Serial.print(WiFi.SSID(i));
      Serial.print(" (");
      Serial.print(WiFi.RSSI(i));
      Serial.print(")");
      Serial.println((WiFi.encryptionType(i) == WIFI_AUTH_OPEN) ? " " : "*");
      delay(10);
    }
    wifiScan = -1;
    // init next async scan
    WiFi.scanNetworks(true, true, false);
//    Wifi.scan();
  }


  delay(1000);




}
/*
  0  SYSTEM_EVENT_WIFI_READY               < ESP32 WiFi ready
  1  SYSTEM_EVENT_SCAN_DONE                < ESP32 finish scanning AP
  2  SYSTEM_EVENT_STA_START                < ESP32 station start
  3  SYSTEM_EVENT_STA_STOP                 < ESP32 station stop
  4  SYSTEM_EVENT_STA_CONNECTED            < ESP32 station connected to AP
  5  SYSTEM_EVENT_STA_DISCONNECTED         < ESP32 station disconnected from AP
  6  SYSTEM_EVENT_STA_AUTHMODE_CHANGE      < the auth mode of AP connected by ESP32 station changed
  7  SYSTEM_EVENT_STA_GOT_IP               < ESP32 station got IP from connected AP
  8  SYSTEM_EVENT_STA_LOST_IP              < ESP32 station lost IP and the IP is reset to 0
  9  SYSTEM_EVENT_STA_WPS_ER_SUCCESS       < ESP32 station wps succeeds in enrollee mode
  10 SYSTEM_EVENT_STA_WPS_ER_FAILED        < ESP32 station wps fails in enrollee mode
  11 SYSTEM_EVENT_STA_WPS_ER_TIMEOUT       < ESP32 station wps timeout in enrollee mode
  12 SYSTEM_EVENT_STA_WPS_ER_PIN           < ESP32 station wps pin code in enrollee mode
  13 SYSTEM_EVENT_AP_START                 < ESP32 soft-AP start
  14 SYSTEM_EVENT_AP_STOP                  < ESP32 soft-AP stop
  15 SYSTEM_EVENT_AP_STACONNECTED          < a station connected to ESP32 soft-AP
  16 SYSTEM_EVENT_AP_STADISCONNECTED       < a station disconnected from ESP32 soft-AP
  17 SYSTEM_EVENT_AP_STAIPASSIGNED         < ESP32 soft-AP assign an IP to a connected station
  18 SYSTEM_EVENT_AP_PROBEREQRECVED        < Receive probe request packet in soft-AP interface
  19 SYSTEM_EVENT_GOT_IP6                  < ESP32 station or ap or ethernet interface v6IP addr is preferred
  20 SYSTEM_EVENT_ETH_START                < ESP32 ethernet start
  21 SYSTEM_EVENT_ETH_STOP                 < ESP32 ethernet stop
  22 SYSTEM_EVENT_ETH_CONNECTED            < ESP32 ethernet phy link up
  23 SYSTEM_EVENT_ETH_DISCONNECTED         < ESP32 ethernet phy link down
  24 SYSTEM_EVENT_ETH_GOT_IP               < ESP32 ethernet got IP from connected AP
  25 SYSTEM_EVENT_MAX
*/
