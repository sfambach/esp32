// This file contains credentials to access several services

// WiFi
const char* mySSID = "NODES";
const char* myPW = "HappyNodes1234";

// MQTT (CloudMQTT)
const char* BROKER = "pi1";
uint16_t    BRPORT = 1024;
const char* BRUSER = "herbert";
const char* BRPWD  = "flieger";
const char* CLIENTID = "HiGrow2";


#define DHTTYPE DHT11   // DHT 11
//#define DHTTYPE DHT21   // DHT 21 (AM2301)
//#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321
#define uS_TO_S_FACTOR 1000000

//maximum is 400 days
uint64_t  DEEPSLEEP_SECONDS = 10; // 10 seconds
// uint64_t DEEPSLEEP_SECONDS = 1800; // 30 min
//uint64_t  DEEPSLEEP_SECONDS = 180; // 3 min
// uint64_t  DEEPSLEEP_SECONDS = 3600; // 60 min number to big
//uint64_t  DEEPSLEEP_SECONDS = 7200; // every two hours
// uint64_t  DEEPSLEEP_SECONDS = 3600*12; // twice a day
// uint64_t  DEEPSLEEP_SECONDS = 3600*24; // once a day

#define WOHNEN_BENJAMINI
/* topics */
#ifdef BUERO_BOGENHANF
#define TEMP_TOPIC    "Büro/Bogenhanf/Temperatur"
#define HUMI_TOPIC    "Büro/Bogenhanf/Luft_Feuchtigkeit"
#define SOIL_TOPIC    "Büro/Bogenhanf/Boden_Feuchtigkeit"
#define LIGHT_TOPIC   "Büro/Bogenhanf/Licht"

#elif defined WOHNEN_BENJAMINI
#define TEMP_TOPIC    "Wohnen/Benjamini/Temperatur"
#define HUMI_TOPIC    "Wohnen/Benjamini/Luft_Feuchtigkeit"
#define SOIL_TOPIC    "Wohnen/Benjamini/Boden_Feuchtigkeit"
#define LIGHT_TOPIC   "Wohnen/Benjamini/Licht"

#elif defined KUECHE_AREKE
#define TEMP_TOPIC    "Küche/Areca Palme/Temperatur"
#define HUMI_TOPIC    "Küche/Areca Palme/Luft_Feuchtigkeit"
#define SOIL_TOPIC    "Küche/Areca Palme/Boden_Feuchtigkeit"
#define LIGHT_TOPIC   "Küche/Areca Palme/Licht"

#endif
