#include "credentials.h"


void setup(){

  Serial.begin(115200);
  pinMode(PIN_SW1, INPUT_PULLDOWN);

}

void loop(){

  if(digitalRead(PIN_SW1)){
    Serial.println("PRESSED!");
  }
  
}