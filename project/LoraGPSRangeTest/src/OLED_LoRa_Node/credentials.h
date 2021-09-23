#ifndef CREDENTIALS_H
#define CREDENTIALS_H
// define debug for debug output
// #define debug
#include "debug.h"

// WiFi
const char* mySSID = "NODES";
const char* myPW = "HappyNodes1234";


// Lora settings
#define SCK     5    // GPIO5  -- SX1278's SCK
#define MISO    19   // GPIO19 -- SX1278's MISO
#define MOSI    27   // GPIO27 -- SX1278's MOSI
#define SS      18   // GPIO18 -- SX1278's CS
#define RST     14   // GPIO14 -- SX1278's RESET
#define DI0     26   // GPIO26 -- SX1278's IRQ(Interrupt Request)
#define BAND    868E6  //433E6, 868E6, 915E6, 8E6

// Oled Settings:
#define DISPLAY_SDA 4
#define DISPLAY_SCL 15
#define DISPLAY_RST 16
#define SSD_ADDRESS 0x3c



// gps Settings
#define RXD2 34 // 32
#define TXD2 35 //33
#define GPS_BOUD 9800 // 4800 // 9800
#define GPS_SAVE_DELAY 1000 // interval for taking a measurment
#define GPS_QUEUE_LENGTH 5
#define GPS_DELAY_IF_NO_DATA 300
#define MILLIES_READ_TIMEOUT 500
#define TIME_CALIBRATION_H 2000000

// home location for the distance calculations
static const double HOME_LAT = 50.135673;
static const double HOME_LNG = 8.521051;


// choose the data sink
#ifdef USE_MQTT // mqtt
#include "MQTT.h"

const char* BROKER = "192.168.1.10";
uint16_t    BRPORT = 1883;
const char* BRUSER = "admin";
const char* BRPWD  = "admin";
const char* CLIENTID = "LoraGPSTracker";


#define JSON_TOPIC  "TEST/GPS_LORA"
#define JSON_TOPIC_TEMPLATE  "[{\"TIME\": \"%s\",\"DATE\":\"%s\",\"LATITUDE\":%.4f,\"LONGITUDE\":%.4f,\"mac\":%s}]"


#else // use influx db
#include <InfluxDbClient.h>
// E.g. http://192.168.1.48:8086 (In InfluxDB 2 UI -> Load Data -> Client Libraries), 
// InfluxDB database name
#define INFLUXDB_DB_NAME "NODE_DATA"



#endif

#endif // #ifndef CREDENTIALS_H
