/*
* RGB LED Example for LilyGo T5 Board 1.02 Inch
* 
* Libs:
* Adafruit NEO Pixel - https://github.com/adafruit/Adafruit_NeoPixel
* 
* Inspired by  LilyGo Ink Screen Series u8g2Fonts Test - Created by Kaibin he
* Adapted by Stefan Fambach visit http://www.fambach.net if you want
*/
#include <Arduino.h>

#define ADC_PIN                 (35)
#define RGB_STRIP_PIN           (4)
#define RGB_STRIP_COUNT         (1)
#include <Adafruit_NeoPixel.h>
Adafruit_NeoPixel strip(RGB_STRIP_COUNT, RGB_STRIP_PIN, NEO_GRBW + NEO_KHZ800);

/** show the 4 different colors R,G,B,W 
*/
void stripTest(){
    int i = 0;
    uint32_t color[] = {0xFF0000, 0x00FF00, 0x0000FF, 0xFFFFFF,0x000000};
    while (i < 5) {
        
        strip.setPixelColor(0, color[i]);
        strip.show();
        delay(1000);
        i++;
    }
    strip.setPixelColor(0, 0);
    strip.show();
}

void setup() {

    // setup strip
    strip.begin();
    strip.setPixelColor(0, 0);
    strip.show();
}

void loop()
{
    stripTest();
    delay(2000);
}
