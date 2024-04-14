#ifndef OTA_H__
#define OTA_H__


#include <WiFiUdp.h>
#include <ArduinoOTA.h>

void setupOTA() {

  // Port defaults to 8266 / 3232
#ifdef ESP8266
  //ArduinoOTA.setPort(8266);
#elif defined ESP32
  //ArduinoOTA.setPort(3232);
#endif
  // Hostname defaults to esp8266-[ChipID]
  #ifdef OTA_NAME
  ArduinoOTA.setHostname(OTA_NAME);
  #endif // OTA_NAME

  // No authentication by default
  // ArduinoOTA.setPassword("admin");

  // Password can be set with it's md5 value as well
  // MD5(admin) = 21232f297a57a5a743894a0e4a801fc3
  // ArduinoOTA.setPasswordHash("ef84180df45cf2f22993e3a03dc71a27");

  ArduinoOTA.onStart([]() {
    String type;
    if (ArduinoOTA.getCommand() == U_FLASH) {
      type = "sketch";
    } else {  // U_FS
      type = "filesystem";
    }

    // NOTE: if updating FS this would be the place to unmount FS using FS.end()
    Serial.println("Start updating " + type);
  });
  ArduinoOTA.onEnd([]() {
    log_v("\nEnd");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    log_v("Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) {
      log_e("Auth Failed");
    } else if (error == OTA_BEGIN_ERROR) {
      log_e("Begin Failed");
    } else if (error == OTA_CONNECT_ERROR) {
      log_e("Connect Failed");
    } else if (error == OTA_RECEIVE_ERROR) {
      log_e("Receive Failed");
    } else if (error == OTA_END_ERROR) {
      log_e("End Failed");
    }
  });
  ArduinoOTA.begin();
  log_v("Ready IP address: %s",WiFi.localIP());
  
  
}






#endif //OTA_H__