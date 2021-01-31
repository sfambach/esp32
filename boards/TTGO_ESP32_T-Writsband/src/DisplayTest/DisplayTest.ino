/** TTGO wrist band ota test
 *  Created by SteFam http://www.fambach.net
 */

#include <WiFi.h>
#include "config.h"
#include "my_ota.h"
#include "my_wifi.h"
#include "my_display.h"


void setup() {
  Serial.begin(BOUD_RATE);
  Serial.println("Booting");

  setupSTA();
  setupOta();
  setupDisplay();
}

void loop() {
  loopSTA();
  loopOta();
}