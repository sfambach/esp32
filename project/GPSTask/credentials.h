#ifndef CREDENTIALS_H
#define CREDENTIALS_H

#include "debug.h"

// gps Settings
#define RXD2 34 // 32
#define TXD2 35 //33
#define GPS_BOUD 9800 // 4800 // 9800
#define GPS_SAVE_DELAY 1000 // interval for taking a measurment
#define GPS_QUEUE_LENGTH 5
#define GPS_DELAY_IF_NO_DATA 300
#define MILLIES_READ_TIMEOUT 500
#define TIME_CALIBRATION_H 2000000

// home location for the distance calculations
static const double HOME_LAT = 50.135673;
static const double HOME_LNG = 8.521051;




#endif // #ifndef CREDENTIALS_H
