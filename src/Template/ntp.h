#ifndef NTP_H 
#define NTP_H

#ifndef WIFI_ACTIVE
#error WIFI must be active to use NTP
#endif // WIFI_ACTIVE

#include <NTPClient.h>
#include <WiFiUdp.h>
#include <TimeLib.h>  // https://playground.arduino.cc/Code/Time/

WiFiUDP ntpUDP;

// You can specify the time server pool and the offset (in seconds, can be
// changed later with setTimeOffset() ). Additionally you can specify the
// update interval (in milliseconds, can be changed using setUpdateInterval() ).
NTPClient timeClient(ntpUDP, "europe.pool.ntp.org", 3600, 60000);

void setupNTP(){
  timeClient.begin();
}

void loopNTP() {
  timeClient.update();
}

void printDigits(int digits) {
  // add colon character and a leading zero if number < 10 
  Serial.print(":");
  if (digits < 10)
    Serial.print('0');
  Serial.print(digits);
}

void printTimeNTP(){

  if (timeStatus() == timeNotSet) {   // check if the time is successfully updated
    Serial.println("Time not set ...");
    return;
    // if (now() != prevDisplay) {       // update the display only if time has changed
    //  prevDisplay = now();
     //  digitalClockDisplay();          // display the current date and time
    //}
  }

  Serial.print(hour());
  printDigits(minute());
  printDigits(second());
  Serial.print(" ");
  Serial.print(day());
  Serial.print(" ");
  Serial.print(month());
  Serial.print(" ");
  Serial.print(year());
  Serial.println();

}


#endif //NTP_H
