/**
* LoRa Receiver 
*
* Created by TTGO
* Modified by StefFam https://www.fambach.net
*/

#include "OledTable.h"
#include <SPI.h>
#include <LoRa.h>
#include <Wire.h>  
#include <SSD1306.h>
#include "images.h"

#define SCK     5    // GPIO5  -- SX1278's SCK
#define MISO    19   // GPIO19 -- SX1278's MISO
#define MOSI    27   // GPIO27 -- SX1278's MOSI
#define SS      18   // GPIO18 -- SX1278's CS
#define RST     14   // GPIO14 -- SX1278's RESET
#define DI0     26   // GPIO26 -- SX1278's IRQ(Interrupt Request)
#define BAND    433E6

#define DISPLAY_SDA 4
#define DISPLAY_SCL 15
#define DISPLAY_RST 16
#define SSD_ADDRESS 0x3c

SSD1306 display(SSD_ADDRESS, DISPLAY_SDA, DISPLAY_SCL);
OledTable table(&display,3,2);

String rssi = "--";
String packSize = "--";
String packet ;


int lineCount = 5;
int lineHeight =0;//=  display.getHeight()/lineCount;
int colSize = 0 ; //display.getWidth()/2;


void loraData(){

  table.clear();
  table.setText(0,0,"Rssi");
  table.setText(0,1, String(rssi));
  
  table.setText(1,0 ,"Received ");
  table.setText(1,1 ,String(packSize)+"[bytes]");

  table.setText(2,0 ,"Content");
  table.setText(2,1 ,packet);
  
  table.refresh();
}

void cbk(int packetSize) {
  packet ="";
  packSize = String(packetSize,DEC);
  for (int i = 0; i < packetSize; i++) { packet += (char) LoRa.read(); }
  rssi = "RSSI " + String(LoRa.packetRssi(), DEC) ;
  loraData();
}

void setup() {

  // Init display 
  pinMode(DISPLAY_RST,OUTPUT);
  digitalWrite(DISPLAY_RST, LOW);    // set GPIO16 low to reset OLED
  delay(50); 
  digitalWrite(DISPLAY_RST, HIGH); // while OLED is running, must set GPIO16 in high、
  display.init();
  display.flipScreenVertically();  
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.setFont(ArialMT_Plain_10);
  
  // init table
  table.init();
    
  Serial.begin(115200);
  while (!Serial);
  Serial.println();
  Serial.println("LoRa Receiver Callback");
  SPI.begin(SCK,MISO,MOSI,SS);
  LoRa.setPins(SS,RST,DI0);  
  if (!LoRa.begin(433E6)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }
  //LoRa.onReceive(cbk);
  LoRa.receive();
  Serial.println("init ok");

}

void loop() {
  int packetSize = LoRa.parsePacket();
  if (packetSize) { cbk(packetSize);  }
  delay(10);
}
