#ifndef NTP_H 
#define NTP_H

#ifndef WIFI_ACTIVE
#error WIFI must be active to use NTP
#endif // WIFI_ACTIVE

#include "credentials.h"
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <TimeLib.h>  // https://playground.arduino.cc/Code/Time/

WiFiUDP ntpUDP;

// You can specify the time server pool and the offset (in seconds, can be
// changed later with setTimeOffset() ). Additionally you can specify the
// update interval (in milliseconds, can be changed using setUpdateInterval() ).
NTPClient* timeClient;//(ntpUDP, NTP_URL, 3600, 60000);

void setupNTP(const char* url){
  timeClient= new NTPClient(ntpUDP, url, 3600, 60000);
  timeClient->begin();
}

void loopNTP() {
  timeClient->update();
}

void printDigits(int digits) {
  // add colon character and a leading zero if number < 10 
  DEBUG_PRINT(":");
  if (digits < 10)
    DEBUG_PRINT('0');
  DEBUG_PRINT(digits);
}

void printTimeNTP(){
  
Serial.println(timeClient->getFormattedTime());
 /* if (!timeClient.isTimeSet()) {   // check if the time is successfully updated
    DEBUG_PRINTLN("Time not set ...");
    return;
    // if (now() != prevDisplay) {       // update the display only if time has changed
    //  prevDisplay = now();
     //  digitalClockDisplay();          // display the current date and time
    //}
  }
*/

setTime(timeClient->getEpochTime());
  DEBUG_PRINT(hour());
  printDigits(minute());
  printDigits(second());
  DEBUG_PRINT(" ");
  DEBUG_PRINT(day());
  DEBUG_PRINT(" ");
  DEBUG_PRINT(month());
  DEBUG_PRINT(" ");
  DEBUG_PRINT(year());
  DEBUG_PRINTLN();

}


#endif //NTP_H
