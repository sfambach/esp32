#include "credentials.h"
#include "mygps.h"



void setup(){
    DEBUG_INIT;
    gpsSetup();
}


void loop(){
  //gpsDisplayData( gpsGetData() );
  DEBUG_PRINTLN("Took from QUEU: ");
  int messagesWaiting;
  while(uxQueueMessagesWaiting(gpsQueueHandler)> 0){
      gpsDisplayData(gpsGetData());
  }
  
  
  delay(4000);
}
