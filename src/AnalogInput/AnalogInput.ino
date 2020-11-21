/*
  Analog Input
  Tested with TTGO-Koala 
  Pin 25 Analog Pin 18 is the current supply
*/

int sensorPin = A18;    // select the input pin for the potentiometer
int ledPin = 5;      // select the pin for the LED
int sensorValue = 0;  // variable to store the value coming from the sensor

void setup() {
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);
  pinMode(sensorPin, INPUT);
}

void loop() {
  // read the value from the sensor:
  sensorValue = analogRead(sensorPin);
  for(int i = 0; i < 5; i++){
    sensorValue = (sensorValue + analogRead(sensorPin))/2;
  }
 
  Serial.print("Analog Value: ");
  Serial.println(sensorValue);
  delay(2000)    ;
}
