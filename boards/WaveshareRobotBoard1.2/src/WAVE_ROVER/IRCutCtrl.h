// <<<<<<<<<<=========IR IO CTRL===========>>>>>>>>>>
#define IR_CUT_PIN 5

void IRIO_Init(){
	pinMode(IR_CUT_PIN, OUTPUT);
}


void ioIrCut(int statusInput){
  Serial.println(statusInput);
  if(statusInput == 0){
  	digitalWrite(IR_CUT_PIN, LOW);
  	Serial.println("LOW");
  }
  else if(statusInput == 1){
  	digitalWrite(IR_CUT_PIN, HIGH);
  	Serial.println("HIGH");
  }
}