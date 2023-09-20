#include <WiFiManager.h>  // https://github.com/tzapu/WiFiManager
WiFiManager wm;
WiFiManagerParameter* customField;

const char* 

String getParam(String name) {
  //read parameter from server, for customhmtl input
  String value;
  if (wm.server->hasArg(name)) {
    value = wm.server->arg(name);
  }
  return value;
}

void saveParamCallback() {
  Serial.println("[CALLBACK] saveParamCallback fired");
  Serial.println("PARAM customfieldid = " + getParam("CUST_ID"));
}

void setupWifiManager() {
  wm.resetSettings();

  // custom field web creation
  customField = new WiFiManagerParameter("CUST_ID", "Custom Field", "/home/esp/", 40);  // custom html input

  wm.addParameter(customField);
  wm.setSaveParamsCallback(saveParamCallback);

  // walk on the dark side ...
  wm.setClass("invert");
  wm.setConfigPortalTimeout(60);  // Auto close cofig portal after 1 minute

  bool res = false;
  // res = wm.autoConnect(); // auto generated AP name from chipid
  res = wm.autoConnect("WifiMan");  // anonymous ap
  //res = wm.autoConnect("WifiMan","letMeIn1234"); // password protected ap

  if (!res) {
    DEBUG_PRINTLN("Failed to connect");
    //ESP.restart();
  } else {
    //if you get here you have connected to the WiFi
    DEBUG_PRINTLN("connected... :)");
  }
}