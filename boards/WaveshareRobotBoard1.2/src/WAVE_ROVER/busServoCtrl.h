// <<<<<<<<<<=========BUS Servo===========>>>>>>>>>>
#include <SCServo.h>
SMS_STS st;

// the uart used to control servos.
// GPIO 18 - S_RXD, GPIO 19 - S_TXD, as default.
#define S_RXD 18
#define S_TXD 19

byte ID_List[253];
s16 loadRead;
s16 speedRead;
byte voltageRead;
int currentRead;
s16 posRead;
s16 modeRead;
s16 temperRead;
int MAX_ID;

s16 stMiddlePos = 2047;



void busServoInit(){
  Serial1.begin(1000000, SERIAL_8N1, S_RXD, S_TXD);
  st.pSerial = &Serial1;
  while(!Serial1) {}
}


void busServoCtrl(u8 idInput, s16 posInput, u16 spdInput, u8 accInput){
  st.WritePosEx(idInput, posInput, spdInput, accInput);
}


void busServoMid(u8 idInput){
  st.WritePosEx(idInput, 2047, 0, 0);
}


void busServoSync(u8 numInput, u8 idInput[], s16 posInput[], u16 spdInput[], u8 accInput[]){
  st.SyncWritePosEx(idInput, numInput, posInput, spdInput, accInput);
}


void busServoScan(u8 maxNumInput){
  int pingStatus;
  int servoNumScan = 0;
  jsonInfoSend.clear();
  jsonFeedbackWeb = "";
  for(int i = 0; i <= maxNumInput; i++){
    pingStatus = st.Ping(i);
    if(pingStatus != -1){
      servoNumScan++;
      Serial.print(i);Serial.print(" ");Serial.println(st.ReadMode(i));
      jsonInfoSend[("servoID_%d_mode:", i)] = st.ReadMode(i);
    }
  }
  jsonInfoSend["ServoNum"] = servoNumScan;
  serializeJson(jsonInfoSend, Serial);
  serializeJson(jsonInfoSend, jsonFeedbackWeb);
}


void busServoInfo(byte idInput){
  if(st.FeedBack(idInput)!=-1){
    jsonInfoSend.clear();
    jsonFeedbackWeb = "";
    jsonInfoSend["pos"]  = st.ReadPos(-1);
    jsonInfoSend["spd"]  = st.ReadSpeed(-1);
    jsonInfoSend["load"] = st.ReadLoad(-1);
    jsonInfoSend["volt"] = st.ReadVoltage(-1);
    jsonInfoSend["curt"] = st.ReadCurrent(-1);
    jsonInfoSend["temp"] = st.ReadTemper(-1);
    jsonInfoSend["mode"] = st.ReadMode(idInput);
    serializeJson(jsonInfoSend, Serial);
    serializeJson(jsonInfoSend, jsonFeedbackWeb);
  }
  else{

  }
}


void busServoIDSet(byte oldInput, byte newInput){
  st.unLockEprom(oldInput);
  st.writeByte(oldInput, SMS_STS_ID, newInput);
  st.LockEprom(newInput);
}


void busServoTorqueLock(byte idInput, int statusInput){
  if(statusInput == 1){
    st.EnableTorque(idInput, 1);
  }
  else if(statusInput == 0){
    st.EnableTorque(idInput, 0);
  }
}


void busServoTorqueLimit(byte idInput, int limitInput){
  if(limitInput > 1000 || limitInput < 0){
    return;
  }
  st.unLockEprom(idInput);
  st.writeWord(idInput, 16, limitInput);
  st.writeWord(idInput, 48, limitInput);
  st.LockEprom(idInput);
}


void busServoMode(byte InputID, byte InputMode){
  if(InputMode == 0){
    st.unLockEprom(InputID);
    st.writeWord(InputID, 11, 4095);
    st.writeByte(InputID, SMS_STS_MODE, InputMode);
    st.LockEprom(InputID);
  }
  else if(InputMode == 3){
    st.unLockEprom(InputID);
    st.writeByte(InputID, SMS_STS_MODE, InputMode);
    st.writeWord(InputID, 11, 0);
    st.LockEprom(InputID);
  }
}