/*
  HiGrowESP32MQTT
  (c) Claus Kuehnel 2018-03-18 info@ckuehnel.ch

  The ESP32 reads data from HiGrow device and publishes these via MQTT

  based on https://github.com/LilyGO/higrowopen/tree/master/HiGrowEsp32
*/

#include <WiFi.h>
#include "WIFI.h"
#include <PubSubClient.h>
#include "DHT.h"
#include "credentials.h"


/* create an instance of PubSubClient client */
WiFiClient espClient;
PubSubClient client(espClient);

uint64_t chipid;



// Initialize DHT sensor.
DHT dht(DHT_PIN, DHTTYPE);

// Temporary variables
static char celsiusTemp[7];
static char humidityTemp[7];
char msg[20];


// Client variables 
char linebuf[80];
int charcount=0;
char deviceid[21];

void goToDeepSleep()
{
  //digitalWrite(dhtpowerpin, LOW);
  Serial.println("Going to sleep...");
  WiFi.disconnect(true);
  WiFi.mode(WIFI_OFF);
  btStop();

  // Configure the timer to wake us up!
  //esp_sleep_enable_timer_wakeup(DEEPSLEEP_SECONDS * uS_TO_S_FACTOR);
  esp_sleep_enable_timer_wakeup(DEEPSLEEP_SECONDS  * uS_TO_S_FACTOR);

  // Go to sleep! Zzzz
  esp_deep_sleep_start();
  
}


void setup() 
{
  //pinMode(dhtpowerpin, OUTPUT);
  //digitalWrite(dhtpowerpin, HIGH);
 

  // power optimization 
  setCpuFrequencyMhz(80);
  
  Serial.begin(115200);
  delay(2000); // wait for monitor
  dht.begin();
 delay(5000);
  

  chipid = ESP.getEfuseMac();
  /**sprintf(deviceid, "%" PRIu64, chipid);
  Serial.print("DeviceId: ");
  Serial.println(deviceid);*/

  connectWiFi();
  configureMQTT();
}

void loop() 
{
  char body[1024];
 // digitalWrite(16, LOW); //switched on

  /* if client was disconnected then try to reconnect again */
  if (!client.connected()) {
    mqttconnect();
  }

  sensorsData(body);
  delay(500);
  WiFi.disconnect(true);
  Serial.println("Going to Deep Sleep..."); 
  goToDeepSleep();    // uncomment for deep sleep
  
}

void sensorsData(char* body)
{
  //This section reads all sensors
  
  int waterlevel = analogRead(SOIL_PIN);
 // int lightlevel = analogRead(LIGHT_PIN);
  
  waterlevel = map(waterlevel, 0, 4095, 0, 1023);
  waterlevel = constrain(waterlevel, 0, 1023);
  if (!isnan(waterlevel)) 
  {
    snprintf (msg, 20, "%d", waterlevel);
    /* publish the message */
    client.publish(SOIL_TOPIC, msg);
  }
  
 // lightlevel = map(lightlevel, 0, 4095, 0, 1023);
 /* lightlevel = constrain(lightlevel, 0, 1023);
  if (!isnan(lightlevel)) 
  {
    snprintf (msg, 20, "%d", lightlevel);
    /* publish the message 
    client.publish(LIGHT_TOPIC, msg);
  }
  */
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float humidity = dht.readHumidity();
  if (!isnan(humidity)) 
  {
    snprintf (msg, 20, "%5.1f", humidity);
    /* publish the message */
    client.publish(HUMI_TOPIC, msg);
  }
  
  // Read temperature as Celsius (the default)
  float temperature = dht.readTemperature();
  if (!isnan(temperature)) 
  {
    snprintf (msg, 20, "%5.1f", temperature);
    /* publish the message */
    client.publish(TEMP_TOPIC, msg);
  }
 
  Serial.print("DevideId: "); Serial.println(deviceid);
  Serial.print("Temperature: "); Serial.print(temperature); Serial.println(" *C");
  Serial.print("Humidity: "); Serial.print(humidity); Serial.println(" %rF");
  Serial.print("Soil: "); Serial.println(waterlevel);
  //Serial.print("Light: "); Serial.println(lightlevel);
}
