#ifndef MY_LORA_H
#define MY_LORA_H

#include <SPI.h>
#include "LoRa.h" // https://github.com/sandeepmistry/arduino-LoRa
#include "debug.h"
#include "credentials.h"
static int loraPacketSize = 0;
#include "message.h"

QueueHandle_t loraOutboundQueue;
TaskHandle_t loraOutboundTaskHandle;

void loraOutboundTask(void * param) {

  while (true) {
    String msg 
    bool success = xQueueReceive(loraOutboundQueue, (void*) &msg, portMAX_DELAY);

    if (!success) {
      continue;
    }

    // send packet
    LoRa.beginPacket();
    LoRa.print(msg->toSCSeparatedString());
    LoRa.endPacket();
    delete msg;
  }
}

void loraOnReceive(int packetSize) {
  // received a packet
  DEBUG_PRINTLN("Received packet '");

  // read packet
  for (int i = 0; i < packetSize; i++) {
    DEBUG_PRINT((char)LoRa.read());
  }

  // print RSSI of packet
  DEBUG_PRINT("' with RSSI ");
  DEBUG_PRINTLN(LoRa.packetRssi());
}

void loraSetup() {

  SPI.begin(SCK, MISO, MOSI, SS);
  LoRa.setPins(SS, RST, DI0);
  if (!LoRa.begin(BAND)) {
    DEBUG_PRINTLN("Starting LoRa failed!");
    while (1);
  }
  //LoRa.onReceive(cbk);
  LoRa.receive();
  DEBUG_PRINTLN("init ok");

  loraOutboundQueue = xQueueCreate( 10, 1000);

  xTaskCreatePinnedToCore(
    loraOutboundTask,   /* Task function. */
    "DisplayTask",     /* name of task. */
    10000,       /* Stack size of task */
    NULL,        /* parameter of the task */
    1,           /* priority of the task */
    &loraOutboundTaskHandle,      /* Task handle to keep track of created task */
    1);          /* pin task to core 1 */
  delay(500);


  // register the receive callback
  LoRa.onReceive(loraOnReceive);

}


void loraLoop() {
  loraPacketSize = LoRa.parsePacket();
}

void loraAddMessage(String msg) {
  xQueueSend(loraOutboundQueue, msg, portMAX_DELAY);
}




#endif // MY_LORA_H
