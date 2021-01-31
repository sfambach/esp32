 #ifndef MY_WIFI_H
 #define MY_WIFI_H
/** TTGO wrist band wifi helper
 *  Created by SteFam http://www.fambach.net
 */
#include "config.h"
 
 void setupSTA(){
  WiFi.disconnect();
  WiFi.mode(WIFI_STA); // kann auch ein anderer Mode sein
  while (WiFi.status() != WL_CONNECTED) {
    WiFi.begin(SSID, PASSWORD);
    delay(1000);
  }

  /**Serial.println("Ready");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  */
 }


 void loopSTA(){
    if( WiFi.status() != WL_CONNECTED) {
        WiFi.disconnect();
        WiFi.mode(WIFI_STA); // kann auch ein anderer Mode sein
        WiFi.begin(SSID, PASSWORD);
        delay(1000);
    }
 }
#endif // MY_WIFI_H
