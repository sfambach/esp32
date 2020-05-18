/* Default Program from Adafruit
 * Optimized to fit on a webpage ;) 
  */
#include <Wire.h>
#include <Adafruit_MotorShield.h>
 
Adafruit_MotorShield AFMS; // = Adafruit_MotorShield(); 
// motors
Adafruit_DCMotor* motor[]= {AFMS.getMotor(3),AFMS.getMotor(4)};
// directions
int dir[] = {FORWARD,BACKWARD,RELEASE};
String textDir[] = {"FORWARD","BACKWARD","RELEASE"};

void setup() {
  Serial.begin(9600);           // set up Serial library at 9600 bps
  Wire.begin(21,22);
  AFMS.begin(1600, &Wire);
} 

int curDir = 0;
int curMotor = 0;
void loop() {
  Serial.println("test");
  delay(1000);
  uint8_t i;

  String s = "";
  s+= "Motor "; s+= curMotor+1; s+= " test! Direction = "; s+= textDir[curDir];
  Serial.println(s); 
  motor[curMotor]->run(dir[curDir]);
  for (i=0; i<255; i++) {
    motor[curMotor]->setSpeed(i);  
    delay(10);
 }
 curDir++;
 if( curDir > 2){ // switch back to first direction 
  curDir = 0;
  curMotor ++;
  Serial.println("--------------------------------------------------------");
  if(curMotor > 1){ // switch back to first motor
    curMotor = 0;
    
  }
 }
}

