#ifndef ttgotsv12_h
#define ttgotsv12_h
/**************************************************






*/

/***************************************************/
// display setup
#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7735.h> // Hardware-specific library
#include <SPI.h>
#include <EasyBuzzer.h>
#include <WiFi.h>
#include <ESPmDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include <TimeLib.h>
#include "secrets.h"
#include "mySD.h"


//#define DISP_180
//#define DISP_096
#define DISP_144
#define LED_BUILTIN 22



class TtgoTsV12 {

  private:
    int _curLine;
    int _lineHeight;
    int _maxLines;
    File _root;
    long _swTimes[3];



    void initNetwork() {

      paintText(F("Connecting to: "), ST7735_WHITE);
      paintText(String(MY_SSID), ST7735_WHITE, false);

      WiFi.disconnect();
      WiFi.mode(WIFI_STA);

      while (WiFi.status() != WL_CONNECTED) {
        WiFi.begin(MY_SSID, PASSWORD);
        delay(1000);
        paintText(".", ST7735_WHITE, false );
      }
      paintText("OK", ST7735_GREEN);
      paintText(F("WiFi connected"), ST7735_WHITE);
      paintText(F("IP address: "), ST7735_WHITE);
      paintText(WiFi.localIP().toString(), ST7735_WHITE);


      ArduinoOTA
      .onStart([]() {
        String type;
        if (ArduinoOTA.getCommand() == U_FLASH)
          type = "sketch";
        else // U_SPIFFS
          type = "filesystem";

        // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
        Serial.println("Start updating " + type);
      })
      .onEnd([]() {
        Serial.println("\nEnd");
      })
      .onProgress([](unsigned int progress, unsigned int total) {
        Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
      })
      .onError([](ota_error_t error) {
        Serial.printf("Error[%u]: ", error);
        if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
        else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
        else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
        else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
        else if (error == OTA_END_ERROR) Serial.println("End Failed");
      });

      ArduinoOTA.begin();

    }

    void initDisplay() {


      _curLine = 0;
      _lineHeight = 10;
      _display = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCLK, TFT_RST);
      _maxLines = _display.height() / _lineHeight;

      Serial.print(F("init Display"));

      _display.initR(INITR_144GREENTAB);   // initialize a ST7735S chip, black tab
      _display.fillScreen(ST7735_BLACK);

      Serial.print(F("Setup finished you should also see it on the display"));
      paintText(F("Display init Success"), ST7735_WHITE);
    }

    void initSwitches() {
      pinMode(SW1, INPUT_PULLUP);
      pinMode(SW2, INPUT_PULLUP);
      pinMode(SW3, INPUT_PULLUP);

    }

    void initSpeaker() {
      pinMode(SPEAKER, OUTPUT);
      EasyBuzzer.setPin(SPEAKER);
    }

    void initTime() {
      configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
      paintText(getLocalTime(), ST7735_WHITE);
    }

    boolean initSD() {
      if (!SD.begin(13, 15, 2, 14)) {
        paintText(F("SD failed!"), ST7735_RED );
        return false;
      }

      _root = SD.open("/");
      return true;
    }


  public:

    static const uint8_t SW1 = 39;
    static const uint8_t SW2 = 35;
    static const uint8_t SW3 = 34;
    static const uint8_t SPEAKER = 25;

    const char* ntpServer = "pool.ntp.org";
    const long  gmtOffset_sec = 3600;
    const int   daylightOffset_sec = 3600;

    static const uint8_t TFT_CS = 16 ;
    static const uint8_t TFT_RST = 9;
    static const uint8_t TFT_DC = 17;
    static const uint8_t TFT_SCLK = 5;
    static const uint8_t TFT_MOSI = 23;

    // LCD
    Adafruit_ST7735 _display;

    TtgoTsV12(): _display(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCLK, TFT_RST) {

    }

    //Power
    // POWER Power;

    bool getSwitch(uint8_t switchPin) {
      return !digitalRead(switchPin);
    }



    void init() {
      initDisplay();
      initNetwork();
      initSwitches();
      initSpeaker();
      initTime();
      if (initSD()) {
        paintText(F("SD OK."), ST7735_GREEN);
      }
    }

    void loop() {
      ArduinoOTA.handle();
      EasyBuzzer.update();
      if (WiFi.status() != WL_CONNECTED) {
        initNetwork();
      }

      // handle switches
    }

    void clearScreen() {
      _display.fillScreen(ST7735_BLACK);
      _curLine = 0;
    }

    void paintText(String text, uint16_t color, bool newLine = true) {
      // clear line by drawing an recht
      _display.fillRect(0, _curLine * _lineHeight, _display.width(), _lineHeight, ST7735_BLACK);
      _display.setCursor(0, _curLine * _lineHeight);
      _display.setTextColor(color);
      _display.setTextWrap(false);
      _display.print(text);

      if (newLine) {
        _curLine ++;
        if ( _curLine >= _maxLines) {
          _curLine = 0;
        }
      }
    }

    void beep() {
      EasyBuzzer.beep(500, 1);
    }
    String getLocalTime()
    {
      // struct tm timeinfo;

      String time = "";
      time += hour();
      time += ":";
      time += minute();
      time += ":";
      time += second();
      time += ":";

      return time;
    }


    void displayDirectory(File dir) {
      clearScreen();
      if ( !_root) {
        boolean success = initSD();
        if (!success) {
          return;
        }
      }

      while (true) {
        File entry =  dir.openNextFile();
        if (! entry) {
          break;
        }

        if (entry.isDirectory()) {
          // Print the name
          paintText(entry.name(), ST7735_GREEN, true );
        } else {

          String text = entry.name();
          text.reserve(20);
          text += 
          text +="\t";
          text +=  entry.size();
          
          paintText(text, ST7735_WHITE, true );
        }

      }
    }
    };


#endif // ttgotsv12_h
