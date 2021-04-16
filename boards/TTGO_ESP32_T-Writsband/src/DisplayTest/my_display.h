 #ifndef MY_DISPLAY_H
 #define MY_DISPLAY_H
#include "config.h"

#include <TFT_eSPI.h> 
TFT_eSPI tft = TFT_eSPI();

void setupDisplay(){

    tft.fillScreen(TFT_BLACK);
    tft.setTextColor(TFT_GREEN, TFT_BLACK);
    
    //tft.setTextDatum(TL_DATUM);
    tft.drawString("configure wrist",  0, tft.height() / 2  + 20);
    tft.setTextColor(TFT_GREEN);
    tft.drawString("\"T-Wristband\"",  0, tft.height() / 2 );
    delay(200);

}



void loopDisplay(){

  
}


#endif // MY_DISPLAY_H
