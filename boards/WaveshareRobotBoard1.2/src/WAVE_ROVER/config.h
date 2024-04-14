// === === === CONNECTION === === ===
// WIFI_AP settings.
const char* AP_SSID = "WAVE_ROVER";
const char* AP_PWD  = "12345678";

// WIFI_STA settings.
const char* STA_SSID = "NODES";
const char* STA_PWD  = "HappyNodes1234";

// set the baud of uart.
//#define UART_BAUD 1000000
// #define UART_BAUD 921600
#define UART_BAUD 115200

#define HEART_BEAT 3000
long lastCmdTime;

// the MAC address of the device you want to ctrl.
uint8_t broadcastAddress[] = {0x08, 0x3A, 0xF2, 0x93, 0x5F, 0xA8};

// ESP-NOW message struct.
typedef struct struct_message {
  int messageType;
  int messageA;
  int messageB;
  int messageC;
  int messageD;
  int messageE;
} struct_message;

// Create a struct_message called myData
struct_message myData;

// set the default wifi mode here.
// wifiMode:
// 1 as [AP] mode, it will not connect other wifi.
// 2 as [STA] mode(connecting), it will connect to know wifi.
// 3 as [STA] mode(connected)
// 4 as [STA] in the first 20s, if there is none, setting up [AP] mode.
#define DEFAULT_WIFI_MODE 1

String MAC_ADDRESS;
IPAddress IP_ADDRESS;
byte   WIFI_MODE;
int    WIFI_RSSI;

// true: robot won't react to web heartbeat.
bool jsonCtrlMode = true;
String jsonFeedbackWeb;

// === === === INFO === === ===
// MAX_SPEED = 1.32 -> 1.2
// A:left, B:right


// === === === COMMAND DEFINITION === === ===
#define EMERGENCY_STOP 0
#define SPEED_INPUT    1
#define PID_SET        2

#define OLED_SET       3
#define OLED_DEFAULT  -3

#define PWM_SERVO_CTRL 40
#define PWM_SERVO_MID -4

#define BUS_SERVO_CTRL   50
#define BUS_SERVO_SYNC   51
#define BUS_SERVO_MID   -5
#define BUS_SERVO_SCAN   52
#define BUS_SERVO_INFO   53
#define BUS_SERVO_ID_SET 54
#define BUS_SERVO_TORQUE_LOCK  55
#define BUS_SERVO_TORQUE_LIMIT 56
#define BUS_SERVO_MODE   57

#define WIFI_SCAN        60
#define WIFI_TRY_STA     61
#define WIFI_AP_DEFAULT  62
#define WIFI_INFO        65
#define WIFI_OFF         66

#define INA219_INFO    70
#define IMU_INFO       71
#define ENCODER_INFO   73
#define DEVICE_INFO    74

#define IO_IR_CUT      80

#define SET_SPD_RATE   901
#define GET_SPD_RATE   902
#define SPD_RATE_SAVE  903
#define GET_NVS_SPACE  904
#define NVS_CLEAR      905


bool emergencyStopFlag = 0;


// === === === DEBUG MODE SET === === ===
bool debugOutput = true;


// === === === PID === === === 
double KpInputSet = 50;
double KiInputSet = 90;
double KdInputSet = 0;

double setpointA = 0;           // setpoint is rotational speed in Hz
double inputA = 0;              // input is PWM to motors
double outputA = 0;             // output is rotational speed in Hz

double setpointB = 0;           // setpoint is rotational speed in Hz
double inputB = 0;              // input is PWM to motors
double outputB = 0;             // output is rotational speed in Hz

int spdSetWeb = 2;          // webCtrl speed, 0:slow, 1:middle, 2:fast.

float leftRate;
float rightRate;


// true:forward, false:backward
bool direction_A = true;
bool direction_B = true;

// new command input flag, 1:new, 0:none
bool newCommandInput = false;


// === === === MOTOR CTRL === === ===
const uint16_t ANALOG_WRITE_BITS = 8;
const uint16_t MAX_PWM = pow(2, ANALOG_WRITE_BITS)-1;
const uint16_t MIN_PWM = MAX_PWM/4;


// === === === MOTOR PIN DEFINITION === === ===
const uint16_t PWMA = 25;         // Motor A PWM control  Orange
const uint16_t AIN2 = 17;         // Motor A input 2      Brown
const uint16_t AIN1 = 21;         // Motor A input 1      Green
const uint16_t BIN1 = 22;        // Motor B input 1       Yellow
const uint16_t BIN2 = 23;        // Motor B input 2       Purple
const uint16_t PWMB = 26;        // Motor B PWM control   White

const uint16_t AENCA = 35;        // Encoder A input      Yellow
const uint16_t AENCB = 34;

const uint16_t BENCB = 16;        // Encoder B input      Green
const uint16_t BENCA = 27;

int freq = 100000;
int channel_A = 5;
int channel_B = 6;


// === === === ROBOT IIC INTERFACE === === ===
#define S_SCL   33
#define S_SDA   32

float INA219_DATA_V = -1;
int   IMU_Roll = 100;
int   IMU_Pitch = 100;
int   IMU_Yaw = 100;

int   IMU_Temp = 100;

IMU_ST_ANGLES_DATA stAngles;
IMU_ST_SENSOR_DATA stGyroRawData;
IMU_ST_SENSOR_DATA stAccelRawData;
IMU_ST_SENSOR_DATA stMagnRawData;

// s16 acce_X;
// s16 acce_Y;
// s16 acce_Z;

// s16 gyro_X;
// s16 gyro_Y;
// s16 gyro_Z;

// s16 magn_X;
// s16 magn_Y;
// s16 magn_Z;


// === === === CONNECTION MIDDLEWARE === === ===
double leftSetPoint;
double rightSetPoint;
double accelerationSet;

bool leftStopFlag;
bool rightStopFlag;


// === === === SPEED CHECK === === ===
bool Direction_right = true;
bool Direction_left = true;

volatile long right_wheel_pulse_count = 0;
volatile long left_wheel_pulse_count = 0;

int interval = 100;

long previousMillis = 0;
long currentMillis = 0;

double rpm_right = 0;
double rpm_left = 0;

double ang_velocity_right = 0;
double ang_velocity_right_deg = 0;
double linear_velocity_right = 0;

double ang_velocity_left = 0;
double ang_velocity_left_deg = 0;
double linear_velocity_left = 0;

const float rpm_to_radians = 0.10471975512;
const float rad_to_deg = 57.29578;


void IRAM_ATTR right_wheel_pulse() {
  Direction_right = digitalRead(BENCA);
  if(Direction_right){
    right_wheel_pulse_count++;
  }
  else{
    right_wheel_pulse_count--;
  }
}


void IRAM_ATTR left_wheel_pulse() {
  Direction_left = digitalRead(AENCA);
  if(Direction_left){
    left_wheel_pulse_count++;
  }
  else{
    left_wheel_pulse_count--;
  }
}


// <<<<<<<<<<=========9DOF-IMU===========>>>>>>>>>>
void getIMU(){
  imuDataGet( &stAngles, &stGyroRawData, &stAccelRawData, &stMagnRawData);
  IMU_Temp = QMI8658_readTemp();
  IMU_Roll  = stAngles.fRoll;
  IMU_Pitch = stAngles.fPitch;
  IMU_Yaw   = stAngles.fYaw;

  // acce_X = stAccelRawData.s16X;
  // acce_Y = stAccelRawData.s16Y;
  // acce_Z = stAccelRawData.s16Z;

  // gyro_X = stGyroRawData.s16X;
  // gyro_Y = stGyroRawData.s16Y;
  // gyro_Z = stGyroRawData.s16Z;

  // magn_X = stMagnRawData.s16X;
  // magn_Y = stMagnRawData.s16Y;
  // magn_Z = stMagnRawData.s16Z;
}