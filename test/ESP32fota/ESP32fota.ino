#include "esp32fota.h"
#include <WiFi.h>

const char *ssid = "NODE_GROUND";
const char *password = "IAmAMeshNetworkPassword";
#define VERSION 4
esp32FOTA esp32FOTA("esp32-heltec-kit-32", VERSION);

void setup()
{
  esp32FOTA.checkURL = "http://pi1/espupdate/update.json";
  Serial.begin(115200);
  delay(1000);
  setup_wifi();
  Serial.print("Version: ");
  Serial.println(VERSION);
}

void setup_wifi()
{
  delay(10);
  Serial.print("Connecting to ");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
}

void loop()
{
  bool updatedNeeded = esp32FOTA.execHTTPcheck();
  if (updatedNeeded)
  {
    Serial.println("updating");
    esp32FOTA.execOTA();
  }
  delay(10000);
}
