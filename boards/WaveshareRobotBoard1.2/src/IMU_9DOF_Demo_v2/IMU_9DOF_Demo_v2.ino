#include"IMU.h"

// define GPIOs for IIC.
EulerAngles stAngles;
IMU_ST_SENSOR_DATA_FLOAT stGyroRawData;
IMU_ST_SENSOR_DATA_FLOAT stAccelRawData;
IMU_ST_SENSOR_DATA stMagnRawData;
float temp;
void setup() {

  
  imuInit();
//  Serial.println("/-------------------------------------------------------------/");
  while(1)
  {
    imuDataGet( &stAngles, &stGyroRawData, &stAccelRawData, &stMagnRawData);


    Serial.println();
    Serial.println("/-------------------------------------------------------------/");
    Serial.print("Roll : "); Serial.print(stAngles.roll);
    Serial.print("    Pitch : "); Serial.print(stAngles.pitch);
    Serial.print("    Yaw : "); Serial.print(stAngles.yaw);
    Serial.println();
    Serial.print("Acceleration: X : "); Serial.print(stAccelRawData.X);
    Serial.print("    Acceleration: Y : "); Serial.print(stAccelRawData.Y);
    Serial.print("    Acceleration: Z : "); Serial.print(stAccelRawData.Z);
    Serial.println();
    Serial.print("Gyroscope: X : "); Serial.print(stGyroRawData.X);
    Serial.print("       Gyroscope: Y : "); Serial.print(stGyroRawData.Y);
    Serial.print("       Gyroscope: Z : "); Serial.print(stGyroRawData.Z);
    Serial.println();
    Serial.print("Magnetic: X : "); Serial.print(stMagnRawData.s16X);
    Serial.print("      Magnetic: Y : "); Serial.print(stMagnRawData.s16Y);
    Serial.print("      Magnetic: Z : "); Serial.print(stMagnRawData.s16Z);
    Serial.println();

    delay(100);
  } 

  }

 void loop() {

  }
