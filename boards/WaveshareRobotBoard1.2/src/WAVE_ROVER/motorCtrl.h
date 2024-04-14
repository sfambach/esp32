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


void emergencyStop(){
  emergencyStopFlag = 1;
  setpointA = 0;
  setpointB = 0;
  outputA = 0;
  outputB = 0;
  digitalWrite(AIN1, LOW);
  digitalWrite(AIN2, LOW);

  digitalWrite(BIN1, LOW);
  digitalWrite(BIN2, LOW);
}


void leftCtrl(float pwmInputA){
  int pwmIntA = -round(pwmInputA);
  if(pwmIntA > 0){
    digitalWrite(AIN1, LOW);
    digitalWrite(AIN2, HIGH);
    ledcWrite(channel_A, pwmIntA);
  }
  else{
    digitalWrite(AIN1, HIGH);
    digitalWrite(AIN2, LOW);
    ledcWrite(channel_A,-pwmIntA);
  }
}


void rightCtrl(float pwmInputB){
  int pwmIntB = -round(pwmInputB);
  if(pwmIntB > 0){
    digitalWrite(BIN1, LOW);
    digitalWrite(BIN2, HIGH);
    ledcWrite(channel_B, pwmIntB);
  }
  else{
    digitalWrite(BIN1, HIGH);
    digitalWrite(BIN2, LOW);
    ledcWrite(channel_B,-pwmIntB);
  }
}


void speedInput(){
  double speedRightBuffer = jsonCmdReceive["R"].as<double>();
  double speedLeftBuffer  = jsonCmdReceive["L"].as<double>();

  if(speedLeftBuffer > 0){
    direction_A = true;
  }
  else{
    direction_A = false;
  }

  if(speedRightBuffer > 0){
    direction_B = true;
  }
  else{
    direction_B = false;
  }

  rightCtrl(speedRightBuffer);
  leftCtrl(speedLeftBuffer);
}