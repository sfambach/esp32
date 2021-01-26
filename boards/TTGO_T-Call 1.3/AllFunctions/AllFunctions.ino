/**
 * SMS Test with TyniGSM Lib 
 * 
 * Tested on TTGO T-Call 1.3 
 * 
 * Lib: https://github.com/Xinyuan-LilyGO/TTGO-T-Call
 * 
 * For further information see also
 * www.fambach.net
 * 
 * GPLv2
 */

 
const char apn[]      = "TM";
const char gprsUser[] = "";
const char gprsPass[] = "";
const char simPIN[]   = ""; // SIM card PIN code, if any
const char wifiSSID[] = "TEST";
const char wifiPass[] = "TEST123456789";

// TTGO T-Call pin definitions
#define MODEM_RST            5
#define MODEM_PWKEY          4
#define MODEM_POWER_ON       23
#define MODEM_TX             27
#define MODEM_RX             26
#define I2C_SDA              21
#define I2C_SCL              22

// Set serial for debug console (to the Serial Monitor, default speed 115200)
#define SerialMon Serial

// Set serial for AT commands (to the module)
#define SerialAT Serial1


// Select your modem:
#define TINY_GSM_MODEM_SIM800
#define TINY_GSM_RX_BUFFER   1024  // Set RX buffer to 1Kb


// See all AT commands, if wanted
// #define DUMP_AT_COMMANDS

// Define the serial console for debug prints, if needed
//#define TINY_GSM_DEBUG SerialMon

// Range to attempt to autobaud
#define GSM_AUTOBAUD_MIN 9600
#define GSM_AUTOBAUD_MAX 38400

/*
 * Test enabled
 */
//#define TINY_GSM_TEST_GPRS true
//#define TINY_GSM_TEST_WIFI true
//#define TINY_GSM_TEST_CALL true
#define TINY_GSM_TEST_SMS true
//#define TINY_GSM_TEST_USSD true
//#define TINY_GSM_TEST_BATTERY true
// powerdown modem after tests
#define TINY_GSM_POWERDOWN false

// set GSM PIN, if any
#define GSM_PIN ""

// Set phone numbers, if you want to test SMS and Calls
#define SMS_TARGET  "+49***********" // < ---- enter you phone number here
//#define CALL_TARGET "+380xxxxxxxxx"



#include <Wire.h>
#include <TinyGsmClient.h>

#ifdef DUMP_AT_COMMANDS
  #include <StreamDebugger.h>
  StreamDebugger debugger(SerialAT, SerialMon);
  TinyGsm modem(debugger);
#else
  TinyGsm modem(SerialAT);
#endif

void setup() {
  // Set console baud rate
  SerialMon.begin(115200);
  delay(10);

  // Keep power when running from battery
  Wire.begin(I2C_SDA, I2C_SCL);
 // bool   isOk = setPowerBoostKeepOn(1);
 // SerialMon.println(String("IP5306 KeepOn ") + (isOk ? "OK" : "FAIL"));

// Set-up modem reset, enable, power pins
  pinMode(MODEM_PWKEY, OUTPUT);
  pinMode(MODEM_RST, OUTPUT);
  pinMode(MODEM_POWER_ON, OUTPUT);

  digitalWrite(MODEM_PWKEY, LOW);
  digitalWrite(MODEM_RST, HIGH);
  digitalWrite(MODEM_POWER_ON, HIGH);
  

// Set GSM module baud rate and UART pins
  SerialAT.begin(115200, SERIAL_8N1, MODEM_RX, MODEM_TX);
  delay(3000);

  // Restart takes quite some time
  // To skip it, call init() instead of restart()
  SerialMon.println("Initializing modem...");
  modem.restart();
  // Or, use modem.init() if you don't need the complete restart

  String modemInfo = modem.getModemInfo();
  SerialMon.print("Modem: ");
  SerialMon.println(modemInfo);

  // Unlock your SIM card with a PIN if needed
  if (strlen(simPIN) && modem.getSimStatus() != 3 ) {
    modem.simUnlock(simPIN);
  }

   SerialMon.println("Initializing modem... done");
}

void loop() {

  // Restart takes quite some time
  // To skip it, call init() instead of restart()
  DBG("Initializing modem...");
  if (!modem.restart()) {
  // if (!modem.init()) {
    DBG("Failed to restart modem, delaying 10s and retrying");
    delay(3000);
    // restart autobaud in case GSM just rebooted
    TinyGsmAutoBaud(SerialAT,GSM_AUTOBAUD_MIN,GSM_AUTOBAUD_MAX);
    delay(10000);
    return;
  }

  String name = modem.getModemName();
  DBG("Modem Name:", name);

  String modemInfo = modem.getModemInfo();
  DBG("Modem Info:", modemInfo);

#if TINY_GSM_TEST_GPRS
  // Unlock your SIM card with a PIN if needed
  if ( GSM_PIN && modem.getSimStatus() != 3 ) {
    modem.simUnlock(GSM_PIN);
  }
#endif

#if TINY_GSM_TEST_WIFI
  DBG("Setting SSID/password...");
  if (!modem.networkConnect(wifiSSID, wifiPass)) {
    DBG(" fail");
    delay(10000);
    return;
  }
  SerialMon.println(" OK");
#endif

#if TINY_GSM_TEST_GPRS && defined TINY_GSM_MODEM_XBEE
  // The XBee must run the gprsConnect function BEFORE waiting for network!
  modem.gprsConnect(apn, gprsUser, gprsPass);
#endif

  DBG("Waiting for network...");
  if (!modem.waitForNetwork()) {
    delay(10000);
    return;
  }

  if (modem.isNetworkConnected()) {
    DBG("Network connected");
  }
bool res = false;
String imei="";
#if TINY_GSM_TEST_GPRS
  DBG("Connecting to", apn);
  if (!modem.gprsConnect(apn, gprsUser, gprsPass)) {
    delay(10000);
    return;
  }

  res = modem.isGprsConnected();
  DBG("GPRS status:", res ? "connected" : "not connected");

  String ccid = modem.getSimCCID();
  DBG("CCID:", ccid);

  imei = modem.getIMEI();
  DBG("IMEI:", imei);

  String cop = modem.getOperator();
  DBG("Operator:", cop);

  IPAddress local = modem.localIP();
  DBG("Local IP:", local);

  int csq = modem.getSignalQuality();
  DBG("Signal quality:", csq);

  // This is only supported on SIMxxx series
  // String gsmLoc = modem.getGsmLocation();
  // DBG("GSM location:", gsmLoc);

  // This is only supported on SIMxxx series
  // String gsmTime = modem.getGSMDateTime(DATE_TIME);
  // DBG("GSM Time:", gsmTime);
  // String gsmDate = modem.getGSMDateTime(DATE_DATE);
  // DBG("GSM Date:", gsmDate);

  String ussd_balance = modem.sendUSSD("*111#");
  DBG("Balance (USSD):", ussd_balance);

  String ussd_phone_num = modem.sendUSSD("*161#");
  DBG("Phone number (USSD):", ussd_phone_num);
#endif

#if defined(TINY_GSM_MODEM_HAS_GPS)
  modem.enableGPS();
  String gps_raw = modem.getGPSraw();
  modem.disableGPS();
  DBG("GPS raw data:", gps_raw);
#endif

#if TINY_GSM_TEST_SMS && defined(SMS_TARGET)
  res = modem.sendSMS(SMS_TARGET, String("Hello from ") + imei);
  DBG("SMS:", res ? "OK" : "fail");

  // This is only supported on SIMxxx series
  res = modem.sendSMS_UTF16(SMS_TARGET, u"Привіііт!", 9);
  DBG("UTF16 SMS:", res ? "OK" : "fail");
#endif

#if TINY_GSM_TEST_CALL && defined(CALL_TARGET)
  DBG("Calling:", CALL_TARGET);

  // This is NOT supported on M590
  res = modem.callNumber(CALL_TARGET);
  DBG("Call:", res ? "OK" : "fail");

  if (res) {
    delay(1000L);

    // Play DTMF A, duration 1000ms
    modem.dtmfSend('A', 1000);

    // Play DTMF 0..4, default duration (100ms)
    for (char tone='0'; tone<='4'; tone++) {
      modem.dtmfSend(tone);
    }

    delay(5000);

    res = modem.callHangup();
    DBG("Hang up:", res ? "OK" : "fail");
  }
#endif

#if TINY_GSM_TEST_BATTERY
  uint8_t chargeState = -99;
  int8_t percent = -99;
  uint16_t milliVolts = -9999;
  modem.getBattStats(chargeState, percent, milliVolts);
  DBG("Battery charge state:", chargeState);
  DBG("Battery charge 'percent':", percent);
  DBG("Battery voltage:", milliVolts / 1000.0F);

 // float temp = modem.getTemperature();
  //DBG("Chip temperature:", temp);
#endif

#if TINY_GSM_TEST_GPRS
  modem.gprsDisconnect();
  if (!modem.isGprsConnected()) {
    DBG("GPRS disconnected");
  } else {
    DBG("GPRS disconnect: Failed.");
  }
#endif

#if TINY_GSM_TEST_WIFI
  modem.networkDisconnect();
  DBG("WiFi disconnected");
#endif

#if TINY_GSM_POWERDOWN
  // Try to power-off (modem may decide to restart automatically)
  // To turn off modem completely, please use Reset/Enable pins
  modem.poweroff();
  DBG("Poweroff.");
#endif

  // Do nothing forevermore
  while (true) {
    modem.maintain();

  }
}
