#ifndef TTGO_TS_12_H
#define TTGO_TS_12_H

#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7735.h> // Hardware-specific library
#include <SPI.h>

//#define DISP_180
//#define DISP_096
#define DISP_144

#define TFT_CS 16
#define TFT_RST 9
#define TFT_DC 17
#define TFT_SCLK 5
#define TFT_MOSI 23

Adafruit_ST7735 Display = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCLK, TFT_RST);




#endif // TTGO_TS_12_H
