#define ENC_COUNT_REV 555

float wheel_radius = 0.0529;

double rpm_right = 0;
double rpm_left = 0;

int interval = 100;

double ang_velocity_right = 0;
double ang_velocity_right_deg = 0;
double linear_velocity_right = 0;


double ang_velocity_left = 0;
double ang_velocity_left_deg = 0;
double linear_velocity_left = 0;

volatile long right_wheel_pulse_count = 0;
volatile long left_wheel_pulse_count = 0;

bool Direction_right = true;
bool Direction_left = true;

const float rpm_to_radians = 0.10471975512;
const float rad_to_deg = 57.29578;

const uint16_t PWMA = 25;         // Motor A PWM control     Orange
const uint16_t AIN2 = 17;         // Motor A input 2         Brown
const uint16_t AIN1 = 21;         // Motor A input 1         Green
const uint16_t BIN1 = 22;        // Motor B input 1         Yellow
const uint16_t BIN2 = 23;        // Motor B input 2         Purple
const uint16_t PWMB = 26;

const uint16_t AENCA = 35;        // Encoder A input      Yellow
const uint16_t AENCB = 34;

const uint16_t BENCB = 16;        // Encoder B input      Green
const uint16_t BENCA = 27;     

const uint16_t ANALOG_WRITE_BITS = 8;

int freq = 100000;
int channel_A = 0;
int channel_B = 1;
int resolution = ANALOG_WRITE_BITS;

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

void pinInit(){
  // Set pin states of the encoder
  pinMode(BENCB , INPUT_PULLUP);
  pinMode(BENCA , INPUT);

  pinMode(AENCB , INPUT_PULLUP);
  pinMode(AENCA , INPUT);
 
  // Every time the pin goes high, this is a pulse
  attachInterrupt(digitalPinToInterrupt(BENCB), right_wheel_pulse, RISING);
  attachInterrupt(digitalPinToInterrupt(AENCB), left_wheel_pulse, RISING);

  // Set the mode of the motorCtrl pin
  pinMode(AIN1, OUTPUT);
  pinMode(AIN2, OUTPUT);
  pinMode(PWMA, OUTPUT);
  pinMode(BIN1, OUTPUT);
  pinMode(BIN2, OUTPUT);
  pinMode(PWMB, OUTPUT);

  ledcSetup(channel_A, freq, ANALOG_WRITE_BITS);
  ledcAttachPin(PWMA, channel_A);

  ledcSetup(channel_B, freq, ANALOG_WRITE_BITS);
  ledcAttachPin(PWMB, channel_B);

  digitalWrite(AIN1, LOW);
  digitalWrite(AIN2, LOW);
  digitalWrite(BIN1, LOW);
  digitalWrite(BIN2, LOW);
}

void right_speed_calculate() {
  rpm_right = (float)(right_wheel_pulse_count * 60 * (1000/interval) / ENC_COUNT_REV);
  ang_velocity_right = rpm_right * rpm_to_radians;
  ang_velocity_right_deg = ang_velocity_right * rad_to_deg;
  linear_velocity_right = ang_velocity_right * wheel_radius;
  right_wheel_pulse_count = 0;
}


void left_speed_calculate() {
  rpm_left = (float)(left_wheel_pulse_count * 60 * (1000/interval) / ENC_COUNT_REV);
  ang_velocity_left = rpm_left * rpm_to_radians;
  ang_velocity_left_deg = ang_velocity_left * rad_to_deg;
  linear_velocity_left = ang_velocity_left * wheel_radius;
  left_wheel_pulse_count = 0;
}
void speed(){
  Serial.print("Leftspeed:");
  Serial.println(linear_velocity_left);
  Serial.print("Rightspeed:");
  Serial.println(linear_velocity_right);
}

void setup(){
  Serial.begin(115200);
  pinInit();
  digitalWrite(AIN1, LOW);
  digitalWrite(AIN2, HIGH);
  digitalWrite(BIN1, LOW);
  digitalWrite(BIN2, HIGH);
}
  
void loop(){
  ledcWrite(channel_A,100);
  ledcWrite(channel_B,100);
  right_speed_calculate();
  left_speed_calculate();
  speed();
  delay(1000);
}