#include"IMU.h"
#include <Wire.h>

#define S_SCL   33
#define S_SDA   32

int   IMU_Roll = 100;
int   IMU_Pitch = 100;
int   IMU_Yaw = 100;
int   IMU_Temp = 100;

IMU_ST_ANGLES_DATA stAngles;
IMU_ST_SENSOR_DATA stGyroRawData;
IMU_ST_SENSOR_DATA stAccelRawData;
IMU_ST_SENSOR_DATA stMagnRawData;

void getIMU(){
  imuDataGet( &stAngles, &stGyroRawData, &stAccelRawData, &stMagnRawData);
  IMU_Temp = QMI8658_readTemp();
  IMU_Roll  = stAngles.fRoll;
  IMU_Pitch = stAngles.fPitch;
  IMU_Yaw   = stAngles.fYaw;
}

void DOF(){
  Serial.print("R:");
  Serial.println(IMU_Roll);
  Serial.print("P:");
  Serial.println(IMU_Pitch);
  Serial.print("Y:");
  Serial.println(IMU_Yaw);
  Serial.print("T:");
  Serial.println(IMU_Temp);
  Serial.println("--- --- ---");

}

void setup() {
  Wire.begin(S_SDA, S_SCL);
  Serial.begin(115200);
  while(!Serial){}
  Serial.print("R:");
  imuInit();
} 


void loop() {
  getIMU();
  DOF();
  delay(2500);
}
