/* modified font created at http://oleddisplay.squix.ch/ */
#include "modified_font.h"

/*  SSD1306 */
#include "SSD1306.h" 
#define SDA_PIN 4// GPIO21 -> SDA
#define SCL_PIN 15// GPIO22 -> SCL
#define SSD_ADDRESS 0x3c
SSD1306  display(SSD_ADDRESS, SDA_PIN, SCL_PIN);

void setup() {

  pinMode(16,OUTPUT);
  pinMode(2,OUTPUT);
  
  digitalWrite(16, LOW);    // set GPIO16 low to reset OLED
  delay(50); 
  digitalWrite(16, HIGH); // while OLED is running, must set GPIO16 in hi
  Serial.begin(115200);
  Serial.println("");
  Serial.println("ESP32 DHT Temperature and Humidity - OLED Display");
  Serial.println("");
  
  display.init();

  display.flipScreenVertically();
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.setFont(Open_Sans_Condensed_Light_20); // set a font
}

void loop() 
{
  displayData();
  delay(1000);
}

/***************************************************
* Display Data
****************************************************/
int counter = 0;
void displayData() 
{

  display.clear();   // clear the display

  display.drawString(0, 32-20/2,  String("Stefan's Blog"));
  if(counter > 999){
    counter = 0;
  }
  String s = String(counter++);
  uint16_t numberWidth = display.getStringWidth(s);
  display.drawString(128-numberWidth, 32-20/2,s);  

  display.display();   // write the buffer to the display
  delay(10);
}
