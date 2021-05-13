// This file contains credentials to access several services
#ifndef CREDENTIALS_H
#define CREDENTIALS_H
// WiFi
const char* mySSID = "NODES";
const char* myPW = "HappyNodes1234";

// MQTT (CloudMQTT)
//const char* BROKER = "192.168.1.26";
//uint16_t    BRPORT = 1883;
//const char* BRUSER = "herbert";
//const char* BRPWD  = "flieger";
const char* BROKER = "192.168.1.10";
uint16_t    BRPORT = 1883;
const char* BRUSER = "admin";
const char* BRPWD  = "admin";
const char* CLIENTID = "HiGrow2";

#define HW_UART_SPEED                 115200L



//#define DHTTYPE DHT21   // DHT 21 (AM2301)
//#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321
#define uS_TO_S_FACTOR 1000000
//#define LILYGO

#ifdef LILYGO
  #define DHTTYPE DHT11  // DHT12
  #define I2C_SDA             25
  #define I2C_SCL             26
  #define DHT_PIN             16 //22  // 16
  #define BAT_ADC             33
  #define SALT_PIN            34
  #define SOIL_PIN            32
  #define BOOT_PIN            0
  #define POWER_CTRL          4
  #define USER_BUTTON         35
  #define DS18B20_PIN         21                  //18b20 data pin
  #define TEMP_CALIBRATION -7.7 // calibrate the temp sensor to the correct temperature
#else 

  #define DHTTYPE DHT11
  #define I2C_SDA             25
  #define I2C_SCL             26
  #define DHT_PIN             22
  //#define BAT_ADC             33
  #define SALT_PIN            34
  #define SOIL_PIN            32
  #define BOOT_PIN            0
  #define POWER_CTRL          4
 // #define USER_BUTTON         35
  #define DS18B20_PIN         21                  //18b20 data pin
  #define TEMP_CALIBRATION -4.3          // calibrate the temp sensor to the correct temperature

#endif

#define OB_BH1750_ADDRESS       (0x23)
#define OB_BME280_ADDRESS       (0x77)

#define SOIL_MIN 1500
#define SOIL_MAX 4000


//maximum is 400 days
// uint64_t  DEEPSLEEP_SECONDS = 10; // 10 seconds
uint64_t DEEPSLEEP_SECONDS = 1800; // 30 min
// uint64_t  DEEPSLEEP_SECONDS = 180; // 3 min
// uint64_t  DEEPSLEEP_SECONDS = 3600; // 60 min number to big
// uint64_t  DEEPSLEEP_SECONDS = 7200; // every two hours
// uint64_t  DEEPSLEEP_SECONDS = 3600*12; // twice a day
// uint64_t  DEEPSLEEP_SECONDS = 3600*24; // once a day

/** define some mqtt things */

/*** define topics */
#define FLOOR "EG"
//#define ROOM  "Büro"
//#define ROOM  "Diele"
#define ROOM  "Wohnen"
//#define PLANT "Bogenhanf"
//#define PLANT "Yucca"
//#define PLANT "Benjamini"
#define PLANT "Gummibaum"
#define TEMP  "Temperatur"
#define HUMI  "Luftfeuchte"
#define SOIL  "Bodenfeuchte"
#define SALT  "Salz"
#define LIGHT "Helligkeit"
#define BAT   "Battery"

/*** define what values should be send Doccument out the unwanted */
#define SINGLE 0x00
#define JSON  0x01

#ifdef SINGLE 
  #define TEMP_TOPIC   FLOOR "/" ROOM "/" PLANT "/"  TEMP
  #define HUMI_TOPIC   FLOOR "/" ROOM "/"  PLANT "/" HUMI
  #define SOIL_TOPIC   FLOOR "/" ROOM "/"  PLANT "/" SOIL
  #define SALT_TOPIC   FLOOR "/" ROOM "/"  PLANT "/" SALT
  #define LIGHT_TOPIC  FLOOR "/" ROOM "/"  PLANT "/" LIGHT
  #define BAT_TOPIC    FLOOR "/" ROOM "/"  PLANT "/" BAT
#endif

#ifdef JSON
  #define JSON_TOPIC  FLOOR "/" ROOM "/"  PLANT "/json" 
  #define JSON_TOPIC_TEMPLATE  "[{\"" TEMP "\": %f,\"" HUMI "\":%f,\"" SOIL "\":%i,\"" SALT "\":%i,\"" LIGHT "\":%i,\"" BAT "\":%1.2f}]"
  
#endif

#endif // #define CREDENTIALS_H
