#include "Arduino.h"

// *************************************************
// SR04 Ultrasonic Distance Sensor
// time to distance conversion is
// divide by 2 (both ways to the obstical and back)
// divide by sonic constant 29.1
//#define TRIGGER_PIN   A0
//#define ECHO_PIN      A1  
//#define TRIGGER_PIN   2
//#define ECHO_PIN      4   
#define TRIGGER_PIN   35
#define ECHO_PIN      34
#define SC_MM         0.17
#define SC_CM         0.017

#define MAX_DUR       300 * 58.2 // ~3m in dur 
#define MIN_DUR       2 * 58.2 // ~2cm in dur
#define TO_SMALL      -1
#define TO_BIG        -2
#define INT_TO_SMALL  -3


/** get duration */
long getDur(int interval){
  if(interval < 1) return -3;
  long dur=0;
  
  for(int i = 0; i < interval; i++ ){
    // init sensor
    digitalWrite(TRIGGER_PIN, LOW);
    delayMicroseconds(10);
    noInterrupts();  
  
    // start 10 us pulse
    digitalWrite(TRIGGER_PIN, HIGH); 
    delayMicroseconds(50);
    digitalWrite(TRIGGER_PIN, LOW);
    
    dur += pulseIn(ECHO_PIN, HIGH); // determine the time
    interrupts();
    
  }

  dur /= interval;
  if(dur > MAX_DUR ) { return TO_BIG;}
  else if (dur < MIN_DUR){ return TO_SMALL;}
  return dur;
}
 
void initDist(){ 
  pinMode(TRIGGER_PIN, OUTPUT); 
  pinMode(ECHO_PIN, INPUT);
}

/** get the distance in centi meter*/
double getDistCM(long dur){ return (dur * SC_CM);}

/** get the distance in milli meter*/
long getDistMM(long dur) { return (dur * SC_MM);}
// *************************************************

void setup(){
  Serial.begin(115200); 
  while(!Serial){delay(1);} // wait until the serial is ok (only for Leonardo)
  initDist();
}

void loop (){
  
  //Serial.print("Current distance is: "); 
  long dur = getDur(5);
  if(dur > 0) Serial.println(getDistCM(dur));
  else if( dur == TO_BIG) Serial.println("TO FAR");
  else if( dur == TO_SMALL) Serial.println("TO NEAR");
  delay(1000); // wait a second
}

