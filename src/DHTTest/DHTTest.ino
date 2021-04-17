/** DHT Test for ESP8266 and ESP 32

*/

// include adafruit dht lib
#include <DHT.h>

// define the pin for the dht sensor
#define DHT_PIN 16
#define DHT_TEMP_CALIBRATION_VALUE -8

// special pin for higrow board
#define POWER_CTRL          4


// define the type of dht sensor
#define DHT_TYPE DHT11
//#define DHT_TYPE DHT21
//#define DHT_TYPE DHT22

// creat a dht object
DHT dhtSensor(DHT_PIN, DHT_TYPE);


void setup() {

  // init serial interface
  Serial.begin(115200);


  //! Sensor power control pin for higrow, use deteced must set high
  pinMode(POWER_CTRL, OUTPUT);
  digitalWrite(POWER_CTRL, HIGH);
  delay(1000);

  // start dht sensor
  dhtSensor.begin();

}


void loop() {
  // get some values
  // read temperture in Celsius
  float temp = dhtSensor.readTemperature() + DHT_TEMP_CALIBRATION_VALUE ;

  // read humidity in %
  float humi = dhtSensor.readHumidity();

  // ouptu the values
  Serial.printf("Temperatur %2.2f °C \t Humidity %3.0f\%\n", temp, humi);

  // wait a while
  delay(2000);
}
