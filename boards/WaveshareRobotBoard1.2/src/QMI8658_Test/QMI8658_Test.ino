/******************************************************************************
# WMI8658 Test for WaveShare Robot Board V1.2
# this example uses SensorLib

##Settings
I2C Address in lib
SDA/SCL for Wire

# Libs
https://github.com/lewisxhe/SensorLib
https://github.com/arduino-libraries/MadgwickAHRS
## Credits 
* Licence: AGPL3
* Author:  S. Fambach
Visit http://www.fambach.net if you want
******************************************************************************/
#include <Arduino.h>
/*****************************************************************************/
// Debuging

#define DEBUG
#ifdef DEBUG
  #define DEBUG_PRINTLN(x) Serial.println(x)
  #define DEBUG_PRINT(x) Serial.print(x)
#else
  #define DEBUG_PRINTLN(x)
  #define DEBUG_PRINT(x)
#endif

/*****************************************************************************/
// QMI8658 setup
#include <Wire.h>
#include "SensorQMI8658.hpp"  // https://github.com/lewisxhe/SensorLib
#include <MadgwickAHRS.h>     // MadgwickAHRS from https://github.com/arduino-libraries/MadgwickAHRS

#define QMI_SDA 32
#define QMI_SCL 33
#define QMI_ADDRESS 0x6B

SensorQMI8658 qmi;

IMUdata acc;
IMUdata gyr;

Madgwick filter;

unsigned const long MICROS_PER_READING = (1000000 / 25);
unsigned long microsPrevious;
/*****************************************************************************/
// XXXXXXXX

/*****************************************************************************/
// XXXXXXXX


/*****************************************************************************/
// Main Programm

void setup() {
  Serial.begin(115200);
  while (!Serial)
    ;

  log_v("Setup QMI Sensor");
  bool success = qmi.begin(Wire, QMI_ADDRESS, QMI_SDA, QMI_SCL);

  log_v("Init QMI Sensor: %s", (success ? "Success" : "Failed"));
  if (!success) {
    log_v("Change your setup!");
    exit(1);
  }
  log_v("Device ID: %x", qmi.getChipID());

  qmi.configAccelerometer(
    /*
         * ACC_RANGE_2G
         * ACC_RANGE_4G
         * ACC_RANGE_8G
         * ACC_RANGE_16G
         * */
    SensorQMI8658::ACC_RANGE_2G,
    /*
         * ACC_ODR_1000H
         * ACC_ODR_500Hz
         * ACC_ODR_250Hz
         * ACC_ODR_125Hz
         * ACC_ODR_62_5Hz
         * ACC_ODR_31_25Hz
         * ACC_ODR_LOWPOWER_128Hz
         * ACC_ODR_LOWPOWER_21Hz
         * ACC_ODR_LOWPOWER_11Hz
         * ACC_ODR_LOWPOWER_3H
        * */
    SensorQMI8658::ACC_ODR_1000Hz,
    /*
        *  LPF_MODE_0     //2.66% of ODR
        *  LPF_MODE_1     //3.63% of ODR
        *  LPF_MODE_2     //5.39% of ODR
        *  LPF_MODE_3     //13.37% of ODR
        * */
    SensorQMI8658::LPF_MODE_0,
    // selfTest enable
    true);


  qmi.configGyroscope(
    /*
        * GYR_RANGE_16DPS
        * GYR_RANGE_32DPS
        * GYR_RANGE_64DPS
        * GYR_RANGE_128DPS
        * GYR_RANGE_256DPS
        * GYR_RANGE_512DPS
        * GYR_RANGE_1024DPS
        * */
    SensorQMI8658::GYR_RANGE_256DPS,
    /*
         * GYR_ODR_7174_4Hz
         * GYR_ODR_3587_2Hz
         * GYR_ODR_1793_6Hz
         * GYR_ODR_896_8Hz
         * GYR_ODR_448_4Hz
         * GYR_ODR_224_2Hz
         * GYR_ODR_112_1Hz
         * GYR_ODR_56_05Hz
         * GYR_ODR_28_025H
         * */
    SensorQMI8658::GYR_ODR_896_8Hz,
    /*
        *  LPF_MODE_0     //2.66% of ODR
        *  LPF_MODE_1     //3.63% of ODR
        *  LPF_MODE_2     //5.39% of ODR
        *  LPF_MODE_3     //13.37% of ODR
        * */
    SensorQMI8658::LPF_MODE_3,
    // selfTest enable
    true);

  // start both
  qmi.enableGyroscope();
  qmi.enableAccelerometer();

  // Print register configuration information
  qmi.dumpCtrlRegister();


  // init nonblocking delay
  
  microsPrevious = micros();
}


void loop() {
  float roll, pitch, yaw;

  // check if delay between readings is over
  if (micros() - microsPrevious >= MICROS_PER_READING) {
    if (qmi.getDataReady()) {
      // get data from sensor
      qmi.getAccelerometer(acc.x, acc.y, acc.z);
      qmi.getGyroscope(gyr.x, gyr.y, gyr.z);

      log_v("acc x:%.2f y:%.2f z:%.2f ", acc.x, acc.y, acc.z);
      log_v("gyr x:%.2f y:%.2f z:%.2f ", gyr.x, gyr.y, gyr.z);


      // update filter
      filter.updateIMU(gyr.x, gyr.y, gyr.z, acc.x, acc.y, acc.z);


      // get the filter values
      roll = filter.getRoll();
      pitch = filter.getPitch();
      yaw = filter.getYaw();

      /*  posX -= roll * 2;
  posY += pitch;
  posZ += yaw;
*/

      log_v("filter Raw x:%.2f y:%.2f z:%.2f ", roll, pitch, yaw);
      // log_v("x:%.2f y:%.2f z:%.2f ", roll, pitch, yaw);

      // set a new start time
      microsPrevious = microsPrevious + MICROS_PER_READING;
    }
  }
}

// activate for serial event
/*void serialEvent(){
  DEBUG_PRINTLN("Serial Event");
}*/

/*****************************************************************************/
// Rest of implementations