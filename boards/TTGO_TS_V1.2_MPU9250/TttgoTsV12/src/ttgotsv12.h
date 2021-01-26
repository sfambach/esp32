#ifndef ttgotsv12_h
#define ttgotsv12_h


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

class TtgoTsV12{
	
	
	private:
	
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
	
	
	
	public:
	
	static Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCLK, TFT_RST);

void init(){
	
	
}


void loop(){
	
}


void static paintLine(String text, uint16_t color) {
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

	static 

};


#endif // ttgotsv12_h