// === === === ENCODER & WHEEL DEFINITION === === ===
#define ENC_COUNT_REV 555
float wheel_radius = 0.0529;

long previousMillisRight = 0;
long currentMillisRight = 0;

long previousMillisLeft = 0;
long currentMillisLeft = 0;

void right_speed_calculate() {
  currentMillisRight = millis();
  rpm_right = (float)(right_wheel_pulse_count * 60 * (1000/(currentMillisRight - previousMillisRight)) / ENC_COUNT_REV);
  ang_velocity_right = rpm_right * rpm_to_radians;
  ang_velocity_right_deg = ang_velocity_right * rad_to_deg;
  linear_velocity_right = ang_velocity_right * wheel_radius;
  right_wheel_pulse_count = 0;
  previousMillisRight = currentMillisRight;
}


void left_speed_calculate() {
  currentMillisLeft = millis();
  rpm_left = (float)(left_wheel_pulse_count * 60 * (1000/(currentMillisLeft - previousMillisLeft)) / ENC_COUNT_REV);
  ang_velocity_left = rpm_left * rpm_to_radians;
  ang_velocity_left_deg = ang_velocity_left * rad_to_deg;
  linear_velocity_left = ang_velocity_left * wheel_radius;
  left_wheel_pulse_count = 0;
  previousMillisLeft = currentMillisLeft;
}


void linearSpeedPrint(){
  Serial.print(linear_velocity_left);
  Serial.print("   ");
  Serial.println(linear_velocity_right);
}