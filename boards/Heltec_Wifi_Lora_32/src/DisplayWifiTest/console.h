#ifndef CONSOLE_H
#define CONSOLE

#include <WiFi.h>
#include <Wire.h>  // Only needed for Arduino 1.6.5 and earlier
#include "SSD1306.h" // alias for `#include "SSD1306Wire.h"`

class Console : public SSD1306Wire {


  public:

    Console(uint8_t _address, uint8_t _sda, uint8_t _scl, OLEDDISPLAY_GEOMETRY g = GEOMETRY_128_64): SSD1306Wire(_address, _sda, _scl, g) {
    }

    void init() {

      pinMode(16, OUTPUT);
      digitalWrite(16, LOW);    // set GPIO16 low to reset OLED
      delay(50);
      digitalWrite(16, HIGH); // while OLED is running, must set GPIO16 in high

      // Initialising the UI will init the display too.
      SSD1306Wire::init();

      SSD1306Wire::flipScreenVertically();
      SSD1306Wire::setFont(ArialMT_Plain_10);

      // display.setTextSize(1);
      SSD1306Wire::setTextAlignment(TEXT_ALIGN_LEFT);
      SSD1306Wire::setLogBuffer(5, 30);
    }

    void println(String s) {
      SSD1306Wire::clear();
      SSD1306Wire::println(s);
      SSD1306Wire::drawLogBuffer(0, 0);
      SSD1306Wire::display();
    }

    void print(String s) {
      clear();
      SSD1306Wire::print(s);
      drawLogBuffer(0, 0);
      display();
    }
};
#endif
