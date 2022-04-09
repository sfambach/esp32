/* Simple Example for LilyGo T5 E-Paper board 1.02 inch display
 *  Libs:
 *  Adafruit GFX - https://github.com/adafruit/Adafruit-GFX-Library
 *  GxEPD - https://github.com/lewisxhe/GxEPD
 *  NeoPixel - https://github.com/adafruit/Adafruit_NeoPixel
 *  U2g for Adafruit_GFX -https://github.com/olikraus/U8g2_for_Adafruit_GFX
 *  
 *  Inspired by LilyGo Ink Screen Series Test - Created by Lewis he
 *  Adapted by Stefan Fambach visit http://www.fambach.net if you want
*/

// setup the board
#define LILYGO_T5_V102
#define RGB_STRIP_COUNT         (1)
#define RGB_STRIP_PIN           (4)

// include the display lib
#include <boards.h>
#include <GxEPD.h>
#include <GxGDGDEW0102T4/GxGDGDEW0102T4.h> //1.02" b/w
#include <Adafruit_NeoPixel.h>             //Depend  
#include <GxIO/GxIO_SPI/GxIO_SPI.h>
#include <GxIO/GxIO.h>
#include <WiFi.h>
#include <U8g2_for_Adafruit_GFX.h>

U8G2_FOR_ADAFRUIT_GFX u8g2Fonts;
GxIO_Class io(SPI,  EPD_CS, EPD_DC,  EPD_RSET);
GxEPD_Class display(io, EPD_RSET, EPD_BUSY);

Adafruit_NeoPixel strip(RGB_STRIP_COUNT, RGB_STRIP_PIN, NEO_GRBW + NEO_KHZ800);


void setup()
{
    // init serial
    Serial.println("Setup Serial");
    Serial.begin(115200);
    Serial.println();
 

    // Spi init
    Serial.println("Setup SPI");
    SPI.begin(EPD_SCLK, EPD_MISO, EPD_MOSI);

    // setup display 
    Serial.println("Setup Display");
    display.init();
    display.setTextColor(GxEPD_BLACK);
    display.setRotation(1);             // use it as landscape display
    Serial.println("Fill screen");
    display.fillScreen(GxEPD_WHITE);
    display.update();
    delay(1000);

    // setup font
    u8g2Fonts.begin(display);
    u8g2Fonts.setFontMode(1);                           // use u8g2 transparent mode (this is default)
    u8g2Fonts.setFontDirection(0);                      // left to right (this is default)
    u8g2Fonts.setForegroundColor(GxEPD_BLACK);          // apply Adafruit GFX color
    u8g2Fonts.setBackgroundColor(GxEPD_WHITE);          // apply Adafruit GFX color
    u8g2Fonts.setFont(u8g2_font_helvR10_tf);            // select u8g2 font from here: https://github.com/olikraus/u8g2/wiki/fntlistall    
}

void loop()
{

    // display visit 
    display.fillScreen(GxEPD_WHITE);
    const char * str = "Visit";
    int swidth = u8g2Fonts.getUTF8Width(str);
    u8g2Fonts.setCursor((int16_t) ((display.width()/2) - (swidth/2)), display.height()/2 + (display.getFontHeight()/2));
    u8g2Fonts.print(str);
    display.update();
    delay(2000);

    // Display fambach.net + size of screen
    display.fillScreen(GxEPD_WHITE);
    str = "fambach.net";
    swidth = u8g2Fonts.getUTF8Width(str);
    u8g2Fonts.setCursor((int16_t) ((display.width()/2) - (swidth/2)), display.height()/2 + (display.getFontHeight()/2));
    u8g2Fonts.print(str);

    // draw frame around the screen
    display.drawRect(0,0,display.width(), display.height(),GxEPD_BLACK );

    // draw h line 
    display.drawLine(0,20,display.width(),20,GxEPD_BLACK); 
    u8g2Fonts.setCursor(60, 18);                          // start writing at this position
    u8g2Fonts.print(display.width());
 
    // draw v line
    display.drawLine(20,0,20,display.height(),GxEPD_BLACK);
    u8g2Fonts.setCursor(2, display.height()/2 + (10/2));                          // start writing at this position
    u8g2Fonts.print(display.height());
    display.update();
    delay(5000);


    // ready to sleep 
    display.powerDown();
    esp_sleep_enable_ext1_wakeup(((uint64_t)(((uint64_t)1) << BUTTON_1)), ESP_EXT1_WAKEUP_ALL_LOW);
    esp_deep_sleep_start();
}
