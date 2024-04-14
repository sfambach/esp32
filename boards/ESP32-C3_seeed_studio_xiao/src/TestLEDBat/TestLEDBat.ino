/******************************************************************************
# ESP32-C3 seeed studio xiao
Test Led and battery

##Settings
BAT_MEAS_PIN Battery Measurement pin
BAT_GROUND_PIN Bayttery Ground pin to activate the measurement
LED_PIN Pin for LED connection
SW_PIN Pin for a press button with a 1KOhm pulldown
MEASURE_COUNT how many times should be measured to get an average value


## Credits 
* Licence: AGPL3
* Author:  S. Fambach
* Website: https://www.fambach.net 
******************************************************************************/

/*****************************************************************************/
// Debuging

#define DEBUG
#ifdef DEBUG
#define DEBUG_PRINTLN(x) Serial.println(x)
#define DEBUG_PRINT(x) Serial.print(x)
#else
#define DEBUG_PRINTLN(x)
#define DEBUG_PRINT(x)
#endif

/*****************************************************************************/
// Settings
#define BAT_MEAS_PIN 2//Battery Measurement pin
#define BAT_GROUND_PIN 3 //Battery Ground pin to activate the measurement
#define LED_PIN 9 // Pin for LED connection
#define SW_PIN 10
#define MEASURE_COUNT 10
#define MEASURE_DELAY 5000
#define FIRST_MEASURE_DELAY 200

/*****************************************************************************/
// XXXXXXXX

/*****************************************************************************/
// XXXXXXXX

/*****************************************************************************/
// XXXXXXXX

/*****************************************************************************/
// XXXXXXXX


/*****************************************************************************/
// Main Programm 

void setup() {
  Serial.begin(115200);
  DEBUG_PRINTLN("Setup");
  //pinMode(BAT_MEAS_PIN, INPUT);
  pinMode(BAT_GROUND_PIN,OUTPUT);
  pinMode(LED_PIN,OUTPUT);
  pinMode(SW_PIN,INPUT_PULLDOWN);

  
}


long ts =0;
uint32_t result = 0;

void loop() {

  if( (ts + MEASURE_DELAY) < millis()){
    ts = millis();

    // switch on ground
    digitalWrite(BAT_GROUND_PIN, LOW);
    // wait some time befor doing the first measurement
    delay(FIRST_MEASURE_DELAY); 
    result = 0;
    
    // get average value
    for(int i =0; i < MEASURE_COUNT; i++){
      result += analogRead(BAT_MEAS_PIN);
    }  
    result /=MEASURE_COUNT;

    DEBUG_PRINT("Current is: ");
    DEBUG_PRINTLN(result);

    // switch off ground
    digitalWrite(BAT_GROUND_PIN, HIGH);
  }
  /*if(digitalRead(SW_PIN) == HIGH){
      digitalWrite(LED_PIN, HIGH);
      DEBUG_PRINT("Button pressed, ");
    

  } else {
    digitalWrite(LED_PIN, LOW);
  }
  
  delay(10000);*/
}


/*****************************************************************************/
// Rest of implementations