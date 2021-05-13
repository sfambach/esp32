#ifndef MY_MQTT_H
#define MY_MQTT_H
#include "debug.h"
#include "credentials.h"
#define MQTT_LOG_ENABLED 1
#include <MqttClient.h> // https://github.com/monstrenyatko/ArduinoMqtt

#define LOG_PRINTFLN(fmt, ...)  logfln(fmt, ##__VA_ARGS__)
#define LOG_SIZE_MAX 128
void logfln(const char *fmt, ...) {
  char buf[LOG_SIZE_MAX];
  va_list ap;
  va_start(ap, fmt);
  vsnprintf(buf, LOG_SIZE_MAX, fmt, ap);
  va_end(ap);
  Serial.println(buf);
}

#ifndef HW_UART_SPEED
  #define HW_UART_SPEED                 115200L
#endif 
#define MQTT_ID 
static MqttClient *mqtt = NULL;

// ============== Object to supply system functions ============================
class System: public MqttClient::System {
public:

  unsigned long millis() const {
    return ::millis();
  }

  void yield(void) {
    ::yield();
  }
};



void receivedCallback(char* topic, byte* payload, unsigned int length) 
{
  DEBUG_PRINT("[MQTT] Message received: ");
  DEBUG_PRINTLN(topic);

  DEBUG_PRINT("[MQTT] payload: ");
  for (int i = 0; i < length; i++) DEBUG_PRINT((char)payload[i]);
  DEBUG_PRINTLN();
}

void mqttSetup()
{
  // Setup MqttClient
  MqttClient::System *mqttSystem = new System;
  MqttClient::Logger *mqttLogger = new MqttClient::LoggerImpl<HardwareSerial>(Serial);
  MqttClient::Network * mqttNetwork = new MqttClient::NetworkClientImpl<WiFiClient>(network, *mqttSystem);
  //// Make 128 bytes send buffer
  MqttClient::Buffer *mqttSendBuffer = new MqttClient::ArrayBuffer<128>();
  //// Make 128 bytes receive buffer
  MqttClient::Buffer *mqttRecvBuffer = new MqttClient::ArrayBuffer<128>();
  //// Allow up to 2 subscriptions simultaneously
  MqttClient::MessageHandlers *mqttMessageHandlers = new MqttClient::MessageHandlersImpl<2>();
  //// Configure client options
  MqttClient::Options mqttOptions;
  ////// Set command timeout to 10 seconds
  mqttOptions.commandTimeoutMs = 10000;
  //// Make client object
  mqtt = new MqttClient(
    mqttOptions, *mqttLogger, *mqttSystem, *mqttNetwork, *mqttSendBuffer,
    *mqttRecvBuffer, *mqttMessageHandlers
  );
}

boolean mqttConnected(){
 return mqtt->isConnected();
}

void mqttConnect() {
  DEBUG_PRINT("[MQTT] Check connection ...");
  /* Loop until reconnected */
  while (!mqttConnected()) {
    DEBUG_PRINT("connecting ...");
    mqttSetup();
    mqttClient.disconnect();
    
    /* connect now */
    if (mqttClient.connect(CLIENTID, BRUSER, BRPWD)) {
      DEBUG_PRINT(mqttClient.state());
      DEBUG_PRINTLN("connected");
      break;
    } else {
      DEBUG_PRINT(" failed, status code =");
      DEBUG_PRINTLN(mqttClient.state());
      DEBUG_PRINTLN("[MQTT] failed try again in 5 seconds");
      /* Wait 5 seconds before retrying */
      delay(5000);
    }
  }

  DEBUG_PRINTLN("[MQTT] Doing things");
}

void mqttPublish(const char* topic, const char* content){
  DEBUG_PRINT("[MQTT] send str   message  \t");
  DEBUG_PRINT("Topic: ");
  DEBUG_PRINT(topic);
  DEBUG_PRINT(" Value: ");
  DEBUG_PRINTLN(content);
  
  mqttClient.publish(topic, content);  
}
void mqttPublish(const char* topic, int32_t content){
  DEBUG_PRINT("[MQTT] send int   message  \t");
  DEBUG_PRINT("Topic: ");
  DEBUG_PRINT(topic);
  DEBUG_PRINT(" Value: ");
  DEBUG_PRINTLN(content);
  mqttClient.publish(topic, String(content).c_str());  
}


void mqttPublish(const char* topic, float content){
  DEBUG_PRINT("[MQTT] send float message \t");
  DEBUG_PRINT("Topic: ");
  DEBUG_PRINT(topic);
  DEBUG_PRINT(" Value: ");
  DEBUG_PRINTLN(content);
  mqttClient.publish(topic, String(content).c_str());  
}


void mqttDisconnect(){
  DEBUG_PRINTLN("[MQTT] Disconnect");
  mqttClient.disconnect();
}

void mqttLoop(){
  mqttConnect();
  mqttClient.loop();
}
#endif // #define MY_MQTT_H
