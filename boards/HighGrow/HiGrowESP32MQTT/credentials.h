// This file contains credentials to access several services

// WiFi
const char* mySSID = "NODES";
const char* myPW = "HappyNodes1234";

// MQTT (CloudMQTT)
const char* BROKER = "192.168.1.26";
uint16_t    BRPORT = 1883;
const char* BRUSER = "herbert";
const char* BRPWD  = "flieger";
const char* CLIENTID = "HiGrow2";



//#define DHTTYPE DHT21   // DHT 21 (AM2301)
//#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321
#define uS_TO_S_FACTOR 1000000


#ifdef LILLIGO
  #define DHTTYPE DHT11
  #define I2C_SDA             25
  #define I2C_SCL             26
  #define DHT_PIN           16
  #define BAT_ADC             33
  #define SALT_PIN            34
  #define SOIL_PIN            32
  #define BOOT_PIN            0
  #define POWER_CTRL          4
  #define USER_BUTTON         35
  #define DS18B20_PIN         21                  //18b20 data pin
#else 

  #define DHTTYPE DHT11
  #define I2C_SDA             25
  #define I2C_SCL             26
  #define DHT_PIN             22
  #define BAT_ADC             33
  #define SALT_PIN            34
  #define SOIL_PIN            32
  #define BOOT_PIN            0
  #define POWER_CTRL          4
 // #define USER_BUTTON         35
  #define DS18B20_PIN         21                  //18b20 data pin

#endif


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
