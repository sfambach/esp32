/***********************************************
* Organ Gampad test, 4 axis one button.
* Licence: AGPL3
* Author: S. Fambach
* Website: http://Fambach.net
************************************************/
#include <Arduino.h>
// allow debug output
#define DEBUG
#ifdef DEBUG
#define DEBUG_PRINTLN(x) Serial.println(x)
#define DEBUG_PRINT(x) Serial.print(x)
#else
#define DEBUG_PRINTLN(x)
#define DEBUG_PRINT(x)
#endif


/******************************************************************************************/
//game controller
#include <BleGamepad.h>

BleGamepad bleGamepad;
BleGamepadConfiguration bleGamepadConfig;

void setupGamepad() {

  bleGamepadConfig.setButtonCount(1);
  bleGamepadConfig.setWhichSpecialButtons(false,false,false,false,false,false,false,false);
  bleGamepadConfig.setWhichSimulationControls(false, false, false, false, false);
  
  bleGamepadConfig.setAxesMin(0);
  bleGamepadConfig.setAxesMax(100);
  
  bleGamepadConfig.setIncludeYAxis(true);
  bleGamepadConfig.setIncludeXAxis(true);
  bleGamepadConfig.setIncludeZAxis(false);
  
  bleGamepadConfig.setIncludeRxAxis(true);
  bleGamepadConfig.setIncludeRyAxis(true);
  bleGamepadConfig.setIncludeRzAxis(false);

  bleGamepadConfig.setIncludeSlider1(false);
  bleGamepadConfig.setIncludeSlider2(false);

  bleGamepadConfig.setAutoReport(false);

  bleGamepadConfig.setFirmwareRevision("2.0.0.0");
  bleGamepadConfig.setHardwareRevision("2.0.0.0");

  bleGamepad.begin(&bleGamepadConfig);
}


/******************************************************************************************/
// Periphery
#include "CustomKeyboard.h"
Poti* xaxis;
Poti* yaxis;
Button* fire;
Poti* poti1;
Poti* poti2;
CustomKeyboard keyboard(6);
void joyCallback(AbstractKeyboardElement* element, int state);

void initKeyboard() {
  //keyboard = new CustomKeyboard(6);
  poti1 = new Poti(25, joyCallback);
  poti1->setThreshold(2);
  keyboard.addElement(poti1);

  poti2 = new Poti(26, joyCallback);
  poti2->setThreshold(2);
  keyboard.addElement(poti2);

  xaxis = new Poti(14, joyCallback);
  xaxis->setThreshold(5);
  keyboard.addElement(xaxis);

  yaxis = new Poti(27, joyCallback);
  yaxis->setThreshold(5);
  keyboard.addElement(yaxis);

  fire = new Button(12, joyCallback);
  keyboard.addElement(fire);

  keyboard.setup();
}

/******************************************************************************************/
// callbacks
void joyCallback(AbstractKeyboardElement* element, int state) {

  if (bleGamepad.isConnected()) {

    bleGamepad.setX(xaxis->getCurrentValue());
    bleGamepad.setY(yaxis->getCurrentValue());
    bleGamepad.setRX(poti1->getCurrentValue());
    bleGamepad.setRY(poti2->getCurrentValue());
    
    if(fire->getCurrentValue()){
      bleGamepad.press(BUTTON_1);
    } else {
      bleGamepad.release(BUTTON_1);
    }

    bleGamepad.sendReport();
  }
  DEBUG_PRINT("Poti 1: ");
  DEBUG_PRINT(poti1->getCurrentValue());

  DEBUG_PRINT(" Poti 2: ");
  DEBUG_PRINT(poti2->getCurrentValue());

  DEBUG_PRINT(" x: ");
  DEBUG_PRINT(xaxis->getCurrentValue());

  DEBUG_PRINT(" y: ");
  DEBUG_PRINT(yaxis->getCurrentValue());

  DEBUG_PRINT(" fire: ");
  DEBUG_PRINTLN((fire->getCurrentValue() ? "ON" : "OFF"));
}



/******************************************************************************************/
void setup() {

  // serial
  Serial.begin(115200);
  DEBUG_PRINTLN("Setup");

  initKeyboard();
  setupGamepad();
}

// run the program
void loop() {
  keyboard.loop();
}
