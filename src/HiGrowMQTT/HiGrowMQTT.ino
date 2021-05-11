/*
  Version2 for the higrow boards
  
  Version 1 was from (c) Claus Kuehnel 2018-03-18 info@ckuehnel.ch
  Version 2 is very different to the Version 1 new MQTT Lib ... 
  
  Visit Fambach.net for additonal informations

  20210421 Support for Lilligo HiGrow added, rebuild mqtt structure, add json mqtt message
  20210430 Lib pubsub client reaplaced by arduino mqtt 
*/

#define DEBUG
#include "debug.h"
#include "WIFI.h"
#include "MQTT.h"
#include <DHT.h>
#include <BH1750.h>
#include "credentials.h"

/* create an instance of PubSubClient client */
uint64_t chipid;

// Initialize sensor.
DHT dht(DHT_PIN, DHTTYPE);

// light sensor
BH1750 lightMeter(OB_BH1750_ADDRESS);  //0x23
bool has_lightSensor = true;


// Temporary variables
static char celsiusTemp[7];
static char humidityTemp[7];
char msg[20];


// Client variables
char linebuf[80];
int charcount = 0;
char deviceid[21];

void goToDeepSleep()
{

  mqttDisconnect();
  digitalWrite(POWER_CTRL, LOW);
  //digitalWrite(dhtpowerpin, LOW);
  wifiDisconnect();

  // Configure the timer to wake us up!
  //esp_sleep_enable_timer_wakeup(DEEPSLEEP_SECONDS * uS_TO_S_FACTOR);
  esp_sleep_enable_timer_wakeup(DEEPSLEEP_SECONDS  * uS_TO_S_FACTOR);

  // Go to sleep! Zzzz
  esp_deep_sleep_start();
}


void setup()
{
  // power optimization
  setCpuFrequencyMhz(80);
  Serial.begin(115200);

  pinMode(POWER_CTRL, OUTPUT);
  digitalWrite(POWER_CTRL, HIGH);

  //! Sensor power control pin , use deteced must set high
  chipid = ESP.getEfuseMac();
  DEBUG_PRINT("Establish WIFI connection");
  wifiConnect();

  mqttConnect();
  dht.begin();
  if (!lightMeter.begin()) {
    DEBUG_PRINTLN ("Could not find a valid BH1750 sensor, check wiring!");
    has_lightSensor = false;
  }

}



uint32_t readSalt()
{
  uint8_t samples = 120;
  uint32_t humi = 0;
  uint16_t array[120];

  for (int i = 0; i < samples; i++)
  {
    array[i] = analogRead(SALT_PIN);
    delay(2);
  }
  std::sort(array, array + samples);
  for (int i = 0; i < samples; i++)
  {
    if (i == 0 || i == samples - 1)
      continue;
    humi += array[i];
  }
  humi /= samples - 2;
  return humi;
}

// READ Soil
uint16_t readSoil()
{
  uint16_t soil = analogRead(SOIL_PIN);
  uint16_t soil_mapped = map(soil, SOIL_MIN, SOIL_MAX, 100, 0);
  return soil_mapped;
}

// READ Battery
float readBattery()
{

#ifdef BAT_ADC

  int vref = 1100;
  uint16_t volt = analogRead(BAT_ADC);
  float result = ((float)volt / 4095.0) * 6.6 * (vref);
  return result;

#else
  return -1;
#endif
}


void sensorsData(char* body)
{

  int32_t salt =  readSalt();

#ifdef SALT_TOPIC
  mqttPublish(SALT_TOPIC, salt);
#endif

  int32_t soil = readSoil();
#ifdef SOIL_TOPIC
  mqttPublish(SOIL_TOPIC, soil);
#endif

  float temp;
  while (isnan (temp = dht.readTemperature()))
  {
    delay(500);
  }
  temp += TEMP_CALIBRATION;
#ifdef TEMP_TOPIC
  mqttPublish(TEMP_TOPIC, temp);
#endif


  float humi;
  while (isnan(humi = dht.readHumidity())) {
    delay(500);
  }

#ifdef HUMI_TOPIC
  mqttPublish(HUMI_TOPIC, humi);
#endif

  int32_t light = -1;
  if (has_lightSensor) {
    light = lightMeter.readLightLevel();
  }
#ifdef LIGHT_TOPIC
  mqttPublish(LIGHT_TOPIC, light);
#endif

  float bat = readBattery();
#ifdef BAT_TOPIC
  mqttPublish(BAT_TOPIC, bat);
#endif

#ifdef JSON
  char buf[255];
  sprintf(buf, JSON_TOPIC_TEMPLATE, temp , humi, soil, salt, light, bat);
  mqttPublish(JSON_TOPIC, buf);
#endif
}

void loop()
{

  char body[1024];
  // digitalWrite(16, LOW); //switched on

  /* if client was disconnected then try to reconnect again */
  mqttLoop();
  sensorsData(body);
  delay(1000);

  goToDeepSleep();    // uncomment for deep sleep
}
