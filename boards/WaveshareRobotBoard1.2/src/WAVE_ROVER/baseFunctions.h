// <<<<<<<<<<=========WIFI Placeholder===========>>>>>>>>>>
void wifiScan();


void wifiConnectSTA(char* ssidInput, char* pwdInput);


void wifiAPStart(char* apName, char* pwdInput);


void wifiInfoGet();


void wifiOff();


void setAP();


void setTrySTA(int timeOutSecs);


// <<<<<<<<<<=========Speed Rate Funcs===========>>>>>>>>>>
void spdRateSet(float leftRateInput, float rightRateInput){
  jsonInfoSend.clear();
  jsonFeedbackWeb = "";
  leftRate = leftRateInput;
  rightRate = rightRateInput;
  jsonInfoSend.clear();
  jsonInfoSend["L"] = leftRate;
  jsonInfoSend["R"] = rightRate;
  serializeJson(jsonInfoSend, Serial);
  serializeJson(jsonInfoSend, jsonFeedbackWeb);
  jsonInfoSend.clear();
}


void getSpdRate(){
  jsonInfoSend.clear();
  jsonFeedbackWeb = "";
  jsonInfoSend["leftRateNow"] = leftRate;
  jsonInfoSend["rightRateNow"] = rightRate;
  jsonInfoSend["leftRateSaved"] = preferences.getFloat("leftRate", 1);
  jsonInfoSend["rightRateSaved"] = preferences.getFloat("rightRate",1);
  serializeJson(jsonInfoSend, Serial);
  serializeJson(jsonInfoSend, jsonFeedbackWeb);
  jsonInfoSend.clear();
}


void spdRateSave(){
  jsonInfoSend.clear();
  jsonFeedbackWeb = "";
  preferences.putFloat("leftRate", leftRate);
  preferences.putFloat("rightRate", rightRate);
  Serial.println("spdRateSaved");
  jsonFeedbackWeb = "spdRateSaved";
}


// <<<<<<<<<<=========NVS function===========>>>>>>>>>>
void getFreeNvsSpace(){
  jsonInfoSend.clear();
  jsonFeedbackWeb = "";
  Serial.println(preferences.freeEntries());
  jsonFeedbackWeb = preferences.freeEntries();
}


void nvsClear(){
  jsonInfoSend.clear();
  jsonFeedbackWeb = "";
  nvs_flash_erase();
  nvs_flash_init();
  Serial.println("NVS_CLEAR");
  jsonFeedbackWeb = "NVS_CLEAR";
}


// <<<<<<<<<<=========Info Feedback===========>>>>>>>>>>
void ina219Info(){
  jsonInfoSend.clear();
  jsonFeedbackWeb = "";
  jsonInfoSend["shunt_mV"] = shuntVoltage_mV;
  jsonInfoSend["load_V"] = loadVoltage_V;
  jsonInfoSend["bus_V"] = busVoltage_V;
  jsonInfoSend["current_mA"] = current_mA;
  jsonInfoSend["power_mW"] = power_mW;
  serializeJson(jsonInfoSend, Serial);
  serializeJson(jsonInfoSend, jsonFeedbackWeb);
  jsonInfoSend.clear();
}


void imuInfo(){
  jsonInfoSend.clear();
  jsonFeedbackWeb = "";
  imuDataGet( &stAngles, &stGyroRawData, &stAccelRawData, &stMagnRawData);

  jsonInfoSend["temp"] = QMI8658_readTemp();

  jsonInfoSend["roll"] = stAngles.fRoll;
  jsonInfoSend["pitch"] = stAngles.fPitch;
  jsonInfoSend["yaw"] = stAngles.fYaw;

  jsonInfoSend["acce_X"] = stAccelRawData.s16X;
  jsonInfoSend["acce_Y"] = stAccelRawData.s16Y;
  jsonInfoSend["acce_Z"] = stAccelRawData.s16Z;

  jsonInfoSend["gyro_X"] = stGyroRawData.s16X;
  jsonInfoSend["gyro_Y"] = stGyroRawData.s16Y;
  jsonInfoSend["gyro_Z"] = stGyroRawData.s16Z;

  jsonInfoSend["magn_X"] = stMagnRawData.s16X;
  jsonInfoSend["magn_Y"] = stMagnRawData.s16Y;
  jsonInfoSend["magn_Z"] = stMagnRawData.s16Z;

  serializeJson(jsonInfoSend, Serial);
  serializeJson(jsonInfoSend, jsonFeedbackWeb);
  jsonInfoSend.clear();
}


void encoderInfo(){
  jsonInfoSend.clear();
  jsonFeedbackWeb = "";
  jsonInfoSend["L"] = linear_velocity_left;
  jsonInfoSend["R"] = linear_velocity_right;
  serializeJson(jsonInfoSend, Serial);
  serializeJson(jsonInfoSend, jsonFeedbackWeb);
  jsonInfoSend.clear();
}


void deviceInfo(){
  jsonInfoSend.clear();
  jsonFeedbackWeb = "";
  jsonInfoSend["Kp"] = KpInputSet;
  serializeJson(jsonInfoSend, Serial);
  serializeJson(jsonInfoSend, jsonFeedbackWeb);
  jsonInfoSend.clear();
}



float speedPIControl_A(float inputFBA, float setPointInputA,  float KpInput)
{
  float errorA = setPointInputA - inputFBA;
  float PIDoutputA = outputA + KpInput * errorA; // DT is in miliseconds...

  if(setPointInputA == 0 && PIDoutputA < MIN_PWM){
    PIDoutputA = 0;
  }
  else if(setPointInputA > 0 && PIDoutputA > -MIN_PWM && PIDoutputA < MIN_PWM){
    PIDoutputA = MIN_PWM;
  }
  else if(setPointInputA < 0 && PIDoutputA > -MIN_PWM && PIDoutputA < MIN_PWM){
    PIDoutputA = -MIN_PWM;
  }

  return PIDoutputA;
}


float speedPIControl_B(float inputFBB, float setPointInputB,  float KpInput)
{
  float errorB = setPointInputB - inputFBB;
  float PIDoutputB = outputB + KpInput * errorB; // DT is in miliseconds...

  if(setPointInputB == 0 && PIDoutputB < MIN_PWM){
    PIDoutputB = 0;
  }
  else if(setPointInputB > 0 && PIDoutputB > -MIN_PWM && PIDoutputB < MIN_PWM){
    PIDoutputB = MIN_PWM;
  }
  else if(setPointInputB < 0 && PIDoutputB > -MIN_PWM && PIDoutputB < MIN_PWM){
    PIDoutputB = -MIN_PWM;
  }

  return PIDoutputB;
}


void pidSet(){
  KpInputSet = jsonCmdReceive["P"].as<double>();
  KiInputSet = jsonCmdReceive["I"].as<double>();
  KdInputSet = jsonCmdReceive["D"].as<double>();

  Serial.print("P:");Serial.print(KpInputSet);
  Serial.print(" I:");Serial.print(KiInputSet);
  Serial.print(" D:");Serial.println(KdInputSet);
}


void setJsonOLED(int lineNumInput, String textInput){
  switch(lineNumInput){
    case 0: screenLine_0 = textInput;break;
    case 1: screenLine_1 = textInput;break;
    case 2: screenLine_2 = textInput;break;
    case 3: screenLine_3 = textInput;break;
  }
  screenDefaultMode = false;
}


void cmdHandler(){
  jsonCtrlMode = true;
  int cmdType = jsonCmdReceive["T"].as<int>();
  switch(cmdType){
    case   EMERGENCY_STOP: emergencyStop();break;
    case      SPEED_INPUT: speedInput();emergencyStopFlag = false;break;
    case          PID_SET: pidSet();break;
    case               -1: emergencyStop();break;
    case         OLED_SET: setJsonOLED(jsonCmdReceive["lineNum"].as<int>(), jsonCmdReceive["Text"]);break;
    case     OLED_DEFAULT: screenDefaultMode = true;break;

    case   PWM_SERVO_CTRL: pwmServoCtrl(jsonCmdReceive["pos"].as<int>());break;
    case    PWM_SERVO_MID: pwmServoCtrl(pwmServoInitPos);break;

    case         BUS_SERVO_CTRL: busServoCtrl(jsonCmdReceive["id"],
                                              jsonCmdReceive["pos"],
                                              jsonCmdReceive["spd"],
                                              jsonCmdReceive["acc"]);break;
    case          BUS_SERVO_MID: busServoMid(jsonCmdReceive["id"]);break;
    case         BUS_SERVO_SCAN: busServoScan(jsonCmdReceive["num"]);break;
    case         BUS_SERVO_INFO: busServoInfo(jsonCmdReceive["id"]);break;
    case       BUS_SERVO_ID_SET: busServoIDSet(jsonCmdReceive["old"], 
                                               jsonCmdReceive["new"]);break;
    case  BUS_SERVO_TORQUE_LOCK: busServoTorqueLock(jsonCmdReceive["id"], 
                                                    jsonCmdReceive["status"]);break;
    case BUS_SERVO_TORQUE_LIMIT: busServoTorqueLimit(jsonCmdReceive["id"], 
                                                     jsonCmdReceive["limit"]);break;
    case         BUS_SERVO_MODE: busServoMode(jsonCmdReceive["id"],
                                              jsonCmdReceive["mode"]);break;

    case              WIFI_SCAN: wifiScan();break;
    case           WIFI_TRY_STA: setTrySTA(20);break;
    case        WIFI_AP_DEFAULT: setAP();break;
    case              WIFI_INFO: wifiInfoGet();break;
    case               WIFI_OFF: wifiOff();break;

    case            INA219_INFO: ina219Info();break;
    case               IMU_INFO: imuInfo();break;
    case           ENCODER_INFO: encoderInfo();break;
    case            DEVICE_INFO: deviceInfo();break;

    case              IO_IR_CUT: ioIrCut(jsonCmdReceive["status"]);break;

    case           SET_SPD_RATE: spdRateSet(jsonCmdReceive["L"],
                                            jsonCmdReceive["R"]);break;
    case           GET_SPD_RATE: getSpdRate();break;
    case          SPD_RATE_SAVE: spdRateSave();break;
    case          GET_NVS_SPACE: getFreeNvsSpace();break;
    case              NVS_CLEAR: nvsClear();break;
  }
}


void serialCtrl(){
  if (Serial.available()){
    DeserializationError err = deserializeJson(jsonCmdReceive, Serial);
    if (err == DeserializationError::Ok){
      cmdHandler();
      newCommandInput = true;
      lastCmdTime = millis();
    }
    else{
      while (Serial.available() > 0){
        Serial.read();
      }
    }
  }
}


void commandThreading(void *pvParameter){
  while(1){
    serialCtrl();
    server.handleClient();
    delay(5);
  }
}


void spdGetThreading(void *pvParameter){
  while(1){
    currentMillis = millis();
    right_speed_calculate();
    left_speed_calculate();
    previousMillis = currentMillis;
    inputA = linear_velocity_left;
    inputB = linear_velocity_right;
    delay(interval);
  }
}