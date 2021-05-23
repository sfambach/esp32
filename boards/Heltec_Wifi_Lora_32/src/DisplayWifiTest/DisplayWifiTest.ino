/*
    This sketch sends data via HTTP GET requests to data.sparkfun.com service.

    You need to get streamId and privateKey at data.sparkfun.com and paste them
    below. Or just customize this script to talk to other HTTP servers.

    Modified by SteFam www.fambach.net

*/

#include <WiFi.h>
#include "console.h"


//OLED pins to ESP32 GPIOs via this connecthin:
//OLED_SDA -- GPIO4
//OLED_SCL -- GPIO15
//OLED_RST -- GPIO16

Console  console(0x3c, 4, 15);

const char* ssid     = "NODE_GROUND";
const char* password = "IAmAMeshNetworkPassword";

const char* host = "192.168.1.21";
const int   port = 2121;

  // Use WiFiClient class to create TCP connections
  WiFiClient client;

void setup()
{
  Serial.begin(115200);
  delay(10);

  console.init();

  // We start by connecting to a WiFi network
  console.println("init network");
  WiFi.begin(ssid, password);
  console.println("Connecting to ");
  
 console.println(String(ssid));
  console.println("MAC: " + WiFi.macAddress());
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    console.print(".");
  }

  console.println("");
  console.println("WiFi connected ");
  console.println(WiFi.localIP().toString());

    console.print("connecting to ");
  console.println(host);
}

int value = 0;

void loop()
{

  if(!client.connected()){
    console.println("Open connection to:");
    console.println(String(host)+":"+String(port));
    if (!client.connect(host, port)) {
      console.println("connection failed");
      delay(1000);
      return;
    }
    console.println("connection succided");
    delay(1000);
  }

  console.println("sending Data");
  client.println("Hello, there I am");
  delay(5000);
  
}
