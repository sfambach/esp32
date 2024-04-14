#ifndef SETTINGS_H
#define SETTINGS_H

#define MY_SETTINGS // define your own settings

#ifdef MY_SETTINGS
  #include "my_settings.h"
#else 

/***********************************************************************************/
// activate functions
#define WIFI_ACTIVE
#define OTA_ACTIVE
//#define WIFI_MANAGER_ACTIVE
#define NTP_ACTIVE
//#define MQTT_ACTIVE

/***********************************************************************************/



/***********************************************************************************/
// wifi credentials

#ifdef WIFI_ACTIVE
  #include "wifi.h"

 
    #define SSID "SSID"
    #define SSID_PASSWD "*******"
 
#endif // WIFI_ACTIVE


/***********************************************************************************/
// ota 

#ifdef OTA_ACTIVE

#include "ota.h"
#define OTA_NAME "NewOtaClient"



#endif // OTA_ACTIVE

/***********************************************************************************/
// wifimanager

#ifdef WIFI_MANAGER_ACTIVE
#include "wifimanager.h"

#endif // WIFI_MANAGER_ACTIVE


/***********************************************************************************/
// ntp
#ifdef NTP_ACTIVE
#include "ntp.h"
#define NTP_URL "europe.pool.ntp.org"

#endif //NTP_ACTIVE


/***********************************************************************************/
// mqtt
#ifdef MQTT_ACTIVE
#include "mqtt.h"

#endif //MQTT_ACTIVE

#endif // my_settings

#endif // SETTINGS_H
