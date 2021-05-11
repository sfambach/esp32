
// define debug for debug output
// #define debug
#include "debug.h"


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
