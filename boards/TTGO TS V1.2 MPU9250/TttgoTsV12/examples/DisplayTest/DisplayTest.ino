/****************************************************
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
#include "ttgotsv12.h"
TtgoTsV12 board;

/***************************************************/

/***************************************************/
// network setup
//#include "secrets.h"
#include <WiFi.h>
#include <HTTPClient.h>

String url;


void setup(void) {
  Serial.begin(115200);
  Serial.print(F("Setup Sketch"));
  board.init();
}

void loop() {
 board.loop();

  if( board.getSwitch(board.SW1)){
   board.beep();
   board.paintText("Switch ON", ST7735_WHITE);
  }
 
}
