#include <Preferences.h>
#include <nvs_flash.h>
Preferences preferences;

#include "math.h"
#include <WiFi.h>
#include <WebServer.h>
#include <Wire.h>
#include "WebPage.h"
WebServer server(80);

#include "ArduinoJson.h"
StaticJsonDocument<256> jsonCmdReceive;
StaticJsonDocument<256> jsonInfoSend;
TaskHandle_t serialCtrlHandle;
TaskHandle_t spdGetHandle;

#include "IMU.h"
#include "config.h"
#include "speedGet.h"

#include "IRCutCtrl.h"
#include "motorCtrl.h"
#include "pwmServoCtrl.h"
#include "screenCtrl.h"
#include "powerInfo.h"
#include "busServoCtrl.h"

#include "baseFunctions.h"
#include "connectionFuncs.h"


// EMERGENCY_STOP: {"T":0}
// SPEED_INPUT   : {"T":1,"L":164,"R":164}
// PID_SET       : {"T":2,"P":170,"I":90}

// OLED_SET      : {"T":3,"lineNum":0,"Text":"putYourTextHere"}
// OLED_DEFAULT  : {"T":-3}

// PWM_SERVO_CTRL    : {"T":40,"pos":90,"spd":30}
// PWM_SERVO_MID     : {"T":-4}

// BUS_SERVO_CTRL    : {"T":50,"id":1,"pos":2047,"spd":500,"acc":30}
// BUS_SERVO_MID     : {"T":-5,"id":1}
// BUS_SERVO_SCAN    : {"T":52,"num":20}
// BUS_SERVO_INFO    : {"T":53,"id":1}
// BUS_SERVO_ID_SET  : {"T":54,"old":1,"new":2}
// BUS_SERVO_TORQUE_LOCK  : {"T":55,"id":1,"status":1}
// BUS_SERVO_TORQUE_LIMIT : {"T":56,"id":1,"limit":500}
// BUS_SERVO_MODE    : {"T":57,"id":1,"mode":0}

// WIFI_SCAN       : {"T":60}
// WIFI_TRY_STA    : {"T":61}
// WIFI_AP_DEFAULT : {"T":62}
// WIFI_INFO       : {"T":65}
// WIFI_OFF        : {"T":66}

// INA219_INFO     : {"T":70}
// IMU_INFO        : {"T":71}
// ENCODER_INFO    : {"T":73}
// DEVICE_INFO     : {"T":74}

// IO_IR_CUT       : {"T":80,"status":1}

// SET_SPD_RATE    : {"T":901,"L":1.0,"R":1.0}
// GET_SPD_RATE    : {"T":902}
// SPD_RATE_SAVE   : {"T":903}

// GET_NVS_SPACE   : {"T":904}
// NVS_CLEAR       : {"T":905}


void setup() {
  Wire.begin(S_SDA, S_SCL);
  Serial.begin(UART_BAUD);
  preferences.begin("config", false);
  leftRate = preferences.getFloat("leftRate", 1);
  rightRate = preferences.getFloat("rightRate", 1);

  while(!Serial){}
  pinInit();
  IRIO_Init();
  pwmServoInit();
  pwmServoCtrl(90);
  busServoInit();

  InitScreen();
  allDataUpdate();

  wifiInit();
  imuInit();

  xTaskCreate(&commandThreading, "serialCtrl", 4000, NULL, 5, &serialCtrlHandle);

  webServerSetup();
}


void loop() {
  currentMillis = millis();
  getIMU();

  if(currentMillis - lastCmdTime > HEART_BEAT){
    rightCtrl(0);
    leftCtrl(0);
  }

  if(currentMillis - lastScreenUpdateMillis > screenUpdateInterval){
    lastScreenUpdateMillis = currentMillis;

    InaDataUpdate();
    getWifiStatus();
    allDataUpdate();
  }
  delay(interval);
}