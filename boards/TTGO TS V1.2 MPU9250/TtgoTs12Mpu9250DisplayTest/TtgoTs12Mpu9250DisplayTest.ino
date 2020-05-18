/***************************************************
  This sketch creates a connection to a esp cam
  and gets continuesly snapshots from the camera

  the project is inspired by
  https://github.com/moononournation/M5Stack-Cam-Viewer/blob/master/M5Stack_Cam_Viewer.ino
    
  
  the sketch was tested with the ttgo ts v1.2
  but should also work with other boards also with a 1.8" display

  For JPeg decoding:
  https://github.com/Bodmer/JPEGDecoder

  For the Display an Adafruid lib is used,
  please support them they are doing good work.

  If you like visit also:
  http://www.fambach.net
  for further informations.

  Licnense is GPLv2
****************************************************/


/***************************************************/
// display setup
#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7735.h> // Hardware-specific library
#include <SPI.h>



//#define DISP_180
//#define DISP_096
#define DISP_144


#if defined (DISP_180)
int height = 160;
int width = 128;
#elif defined (DISP_144)
int height = 128;
int width = 128;
#elif defined (DISP_096)
int height = 128;
int width = 128;
#else
#error Please select dislay first DISP_180, DISP_144 or  DISP_096
#endif

#define TFT_CS 16
#define TFT_RST 9
#define TFT_DC 17
#define TFT_SCLK 5
#define TFT_MOSI 23

//Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS,  TFT_DC, TFT_RST);
Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCLK, TFT_RST);

//float p = 3.1415926;

int curLine = 0;
int lineHeight = 10;
int maxLines = height / lineHeight;

void paintLine(String text, uint16_t color) {
  // clear line by drawing an recht
  tft.fillRect(0, curLine * lineHeight, lineHeight, width, ST7735_BLACK);
  tft.setCursor(0, curLine * lineHeight);
  tft.setTextColor(color);
  tft.setTextWrap(true);
  tft.print(text);
  curLine ++;
  if ( curLine >= maxLines) {
    curLine = 0;

  }
}

//void paintText(F("Display init Success"), ST7735_WHITE) {
//
//
//}




void initDisplay() {

  Serial.print(F("init Display"));
#if defined( DISP_180 )
  // Use this initializer if you're using a 1.8" TFT
  //tft.initR(INITR_BLACKTAB);   // initialize a ST7735S chip, black tab

#elif defined ( DISP_144)
  // Use this initializer (uncomment) if you're using a 1.44" TFT
  tft.initR(INITR_144GREENTAB);   // initialize a ST7735S chip, black tab
#elif defined (DISP_096)
  // Use this initializer (uncomment) if you're using a 0.96" 180x60 TFT
  //tft.initR(INITR_MINI160x80);   // initialize a ST7735S chip, mini display
#else
#error Please select dislay first DISP_180, DISP_144 or  DISP_096
#endif

  tft.fillScreen(ST7735_BLACK);

  Serial.print(F("Setup finished you should also see it on the display"));
  paintLine(F("Display init Success"), ST7735_WHITE);
}

void loopDisplay() {


}


/***************************************************/
// Jpeg decode
#include <JPEGDecoder.h>  // JPEG decoder library

// this function determines the minimum of two numbers
#define minimum(a,b)     (((a) < (b)) ? (a) : (b))

void renderJPEG(int xpos, int ypos) {

  // retrieve infomration about the image
  uint16_t *pImg;
  uint16_t mcu_w = JpegDec.MCUWidth;
  uint16_t mcu_h = JpegDec.MCUHeight;
  uint32_t max_x = JpegDec.width;
  uint32_t max_y = JpegDec.height;

  // Jpeg images are draw as a set of image block (tiles) called Minimum Coding Units (MCUs)
  // Typically these MCUs are 16x16 pixel blocks
  // Determine the width and height of the right and bottom edge image blocks
  uint32_t min_w = minimum(mcu_w, max_x % mcu_w);
  uint32_t min_h = minimum(mcu_h, max_y % mcu_h);

  // save the current image block size
  uint32_t win_w = mcu_w;
  uint32_t win_h = mcu_h;

  // record the current time so we can measure how long it takes to draw an image
  uint32_t drawTime = millis();

  // save the coordinate of the right and bottom edges to assist image cropping
  // to the screen size
  max_x += xpos;
  max_y += ypos;

  // read each MCU block until there are no more
  while ( JpegDec.read()) {

    // save a pointer to the image block
    pImg = JpegDec.pImage;

    // calculate where the image block should be drawn on the screen
    int mcu_x = JpegDec.MCUx * mcu_w + xpos;
    int mcu_y = JpegDec.MCUy * mcu_h + ypos;

    // check if the image block size needs to be changed for the right and bottom edges
    if (mcu_x + mcu_w <= max_x) win_w = mcu_w;
    else win_w = min_w;

    // check if the image block size needs to be changed for the bottom edge
    if (mcu_y + mcu_h <= max_y) win_h = mcu_h;
    else win_h = min_h;

    // calculate how many pixels must be drawn
    uint32_t mcu_pixels = win_w * win_h;

    // copy pixels into a contiguous block
    if (win_w != mcu_w)
    {
      for (int h = 1; h < win_h-1; h++)
      {
        memcpy(pImg + h * win_w, pImg + (h + 1) * mcu_w, win_w << 1);
      }
    }

    // draw image MCU block only if it will fit on the screen
    if ( ( mcu_x + win_w) <= tft.width() && ( mcu_y + win_h) <= tft.height())
    {
      tft.drawRGBBitmap(mcu_x, mcu_y, pImg, win_w, win_h);
    }

    else if ( ( mcu_y + win_h) >= tft.height()) JpegDec.abort();
  }

  // calculate how long it took to draw the image
  drawTime = millis() - drawTime; // Calculate the time it took

  // print the results to the serial port
  Serial.print  ("Total render time was    : "); Serial.print(drawTime); Serial.println(" ms");
  Serial.println("=====================================");

}

void jpegInfo() {
  Serial.println(F("==============="));
  Serial.println(F("JPEG image info"));
  Serial.println(F("==============="));
  Serial.print(F(  "Width      :")); Serial.println(JpegDec.width);
  Serial.print(F(  "Height     :")); Serial.println(JpegDec.height);
  Serial.print(F(  "Components :")); Serial.println(JpegDec.comps);
  Serial.print(F(  "MCU / row  :")); Serial.println(JpegDec.MCUSPerRow);
  Serial.print(F(  "MCU / col  :")); Serial.println(JpegDec.MCUSPerCol);
  Serial.print(F(  "Scan type  :")); Serial.println(JpegDec.scanType);
  Serial.print(F(  "MCU width  :")); Serial.println(JpegDec.MCUWidth);
  Serial.print(F(  "MCU height :")); Serial.println(JpegDec.MCUHeight);
  Serial.println(F("==============="));
}

/***************************************************/
// network setup
#include "secrets.h"
#include <WiFi.h>
#include <HTTPClient.h>

//WiFiClient client;
//HTTPClient http;
String url;

void initNetwork() {

  paintLine(F("Connecting to: "), ST7735_WHITE);
  paintLine(String(SSID), ST7735_WHITE);

  WiFi.begin(SSID, PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    paintLine(".", ST7735_WHITE);
  }

  paintLine("WiFi connected", ST7735_WHITE);
  paintLine("IP address: ", ST7735_WHITE);
  paintLine(WiFi.localIP().toString(), ST7735_WHITE);


  // connecting to server
  paintLine(F("Connect to Server"), ST7735_WHITE);
  //    paintLine(host,ST7735_WHITE);
  //    if (!client.connect(host, httpPort)) {
  //        paintLine(F("FAILED"),ST7735_RED );
  //        delay(1000);
  //        ESP.restart();
  //    }
  //     paintLine(F("PASSED"),ST7735_GREEN);

  paintLine(F("Open URL"), ST7735_WHITE);

    //url = "http://192.168.1.66/jpg";

    url = "http://192.168.1.66/capture";
  
//  url = "http://";
//  url += host;
//  url += ":";
//  url += String(port);
//  url += site;
//  paintLine(url, ST7735_WHITE);
//  http.begin(client, url);
//
//  paintLine(F("Result"), ST7735_WHITE);
//
//  int httpCode = http.GET();
//
//  // httpCode will be negative on error
//  if (httpCode > 0) {
//    paintLine(F("SUCCESS"), ST7735_GREEN);
//  } else {
//    paintLine(F("FAILED"), ST7735_RED);
//  }
//
//  if (http.connected()) {
//    paintLine(F("SUCCESS"), ST7735_GREEN);
//  } else {
//    paintLine(F("FAILED"), ST7735_RED);
//  }
//  delay(2000);

}

void loopNetwork() {
//
//  if (!http.connected()) {
//    ESP.restart();
//    paintLine(F("HTTP not connected!"), ST7735_RED);
//    delay(2000);
//  }

  if (WiFi.status() != WL_CONNECTED) {
    ESP.restart();
    paintLine(F("WIFI not connected!"), ST7735_RED);
    delay(2000);
  }

  HTTPClient http;

  Serial.print("[HTTP] begin...\n");
  http.begin(url);

  Serial.print("[HTTP] GET...\n");
  int httpCode = http.GET();

  Serial.printf("[HTTP] GET... code: %d\n", httpCode);
  // HTTP header has been send and Server response header has been handled
  if (httpCode <= 0) {
    Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
  } else {
    if (httpCode != HTTP_CODE_OK) {
      Serial.printf("[HTTP] Not OK!\n");
    } else {
      // get lenght of document (is -1 when Server sends no Content-Length header)
      uint32_t len = http.getSize();
      Serial.printf("[HTTP] size: %d\n", len);

      if (len <= 0) {
        Serial.printf("[HTTP] Unknow content size: %d\n", len);
      } else {
        // create buffer for read
        uint8_t buff[len] = { 0 };

        // get tcp stream
        WiFiClient * stream = http.getStreamPtr();

        // read all data from server
        uint8_t* p = buff;
        int l = len;
        while (http.connected() && (l > 0 || len == -1)) {
          // get available data size
          size_t size = stream->available();

          if (size) {
            int s = ((size > sizeof(buff)) ? sizeof(buff) : size);
            int c = stream->readBytes(p, s);
            p += c;

            //Serial.printf("[HTTP] read: %d\n", c);

            if (l > 0) {
              l -= c;
            }
          }
        }

        Serial.println();
        Serial.print("[HTTP] connection closed.\n"); 

        // M5.Lcd.drawJpg();

        JpegDec.decodeArray(buff, len);

        jpegInfo();
//
        renderJPEG(0, 0);
        
      }
    }
  }

  http.end();
}


void setup(void) {
  Serial.begin(115200);
  Serial.print(F("Setup Sketch"));


  initDisplay();
  initNetwork();

  delay(1000);
}

void loop() {
  loopNetwork();
}

