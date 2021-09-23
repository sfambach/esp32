#ifndef MYGPS_H
#define MYGPS_H

#include <TinyGPS++.h>
#include "credentials.h"

// located in the credentials.h
//// gps Settings
//#define RXD2 34 // 32
//#define TXD2 35 //33
//#define GPS_BOUD 9800 // 4800 // 9800
//#define GPS_SAVE_DELAY 1000 // interval for taking a measurment
//#define GPS_QUEUE_LENGTH 5
//#define GPS_DELAY_IF_NO_DATA 300
//#define MILLIES_READ_TIMEOUT 500
//
//// home location for the distance calculations
//static const double HOME_LAT = 50.135673;
//static const double HOME_LNG = 8.521051;





static boolean gpsConnected = false;

struct GpsData {
  double latitude;
  double logitude;
  double altitude;
  double speed;
  uint32_t time;
  uint32_t date;
  uint32_t sattelites;
  double distanceToHome;
} curGPSData;


TaskHandle_t gpsTaskHandle;
QueueHandle_t gpsQueueHandler;

void gpsTask (void* prams);
TinyGPSPlus gps;


static void gpsSetup() {

  DEBUG_PRINTLN("Open serial port 2");
  Serial2.begin(GPS_BOUD, SERIAL_8N1, RXD2, TXD2);
  
  // create the queue
  DEBUG_PRINTLN("Create queue");
  gpsQueueHandler = xQueueCreate( GPS_QUEUE_LENGTH, sizeof(GpsData));

  // create a task for gps data
  DEBUG_PRINTLN("Create Task");
  xTaskCreatePinnedToCore(
    gpsTask,   /* Task function. */
    "GPSTask",     /* name of task. */
    100000,       /* Stack size of task */
    NULL,        /* parameter of the task */
    1,           /* priority of the task */
    &gpsTaskHandle,      /* Task handle to keep track of created task */
    1);          /* pin task to core 1 */


}

static GpsData gpsGetData  () {
  GpsData data;
  xQueueReceive(gpsQueueHandler, &data, portMAX_DELAY);
  return data;
}

void gpsDisplayData(GpsData data) {

  DEBUG_PRINT("Lat/Long:");
  DEBUG_PRINT(data.latitude);
  DEBUG_PRINT("/");
  DEBUG_PRINT(data.logitude);
  DEBUG_PRINT(data.altitude);
  DEBUG_PRINT(" Speed:");
  DEBUG_PRINT(data.speed);
  DEBUG_PRINT(" Time:");
  DEBUG_PRINT(data.time);
  DEBUG_PRINT(" Date:");
  DEBUG_PRINT(data.date);
  DEBUG_PRINT(" Sat Count:");
  DEBUG_PRINT(data.sattelites);
  DEBUG_PRINT(" Dist to home:");
  DEBUG_PRINTLN(data.distanceToHome) ;
}

void gpsTask (void* prams) {
  unsigned long saveMeasurementTimer = millis();

  while (true) { // run vorever

    if (!Serial2.available()) {
      delay(GPS_DELAY_IF_NO_DATA);
      DEBUG_PRINTLN("No Data");
      continue;
    }

    unsigned long start = millis();

    do
    {
      while (Serial2.available()) {
        int i = Serial2.read();
        gps.encode(i);
      }

    } while (millis() - start < MILLIES_READ_TIMEOUT);

    if (millis() > 5000 && gps.charsProcessed() < 10) {
      gpsConnected = false;
      DEBUG_PRINTLN("No GPS data received: check wiring");
    } else {
      DEBUG_PRINTLN("take mesurement");
      gpsConnected = true;
      if (millis() > saveMeasurementTimer + GPS_SAVE_DELAY) {
        GpsData data;
        data.latitude = gps.location.lat();
        data.logitude = gps.location.lng();
        data.altitude = gps.altitude.meters();
        data.speed = gps.speed.mps();

        // add two houres
        double t = gps.time.value() + TIME_CALIBRATION_H;
        if( t > 24000000){
          t-= 24000000;
        }
        data.time = t;
        
        data.date = gps.date.value();
        data.sattelites = gps.satellites.value();
        data.distanceToHome = gps.distanceBetween(gps.location.lat(), gps.location.lng(), HOME_LAT, HOME_LNG) ;
        

     
        BaseType_t xStatus = xQueueSend(gpsQueueHandler, &data, portMAX_DELAY);
        if( xStatus == pdPASS ) {
          DEBUG_PRINT("Send to queue ");
          gpsDisplayData(data);
        } else {
          DEBUG_PRINTLN("Send to queue failed");
        }
        saveMeasurementTimer = millis(); // refresh timer
      } // end if 
    } // end else 
  } // end while
}








#endif
