#ifndef MESSAGE_H
#define MESSAGE_H
#include "credentials.h"

/* message type */
class Message {

  public:
    String _time = "";
    String _date = "";
    String _lat  = "";
    String _lng  = "";
    String _mac  = "";

    char* toJSON() {
      char buf2[100];
      sprintf(buf2, JSON_TOPIC_TEMPLATE, _time, _date, _lat , _lng, _mac);
      DEBUG_PRINTLN(buf2);
      return buf2;
    }

    String toSCSeparatedString() {
      return _time + ";" + _date + ";" + _lat + ";" + _lng + ";" + _mac + ";";
    }

    void parseSCString(String message) {

      String buf   = "";
      int index = 0;
      DEBUG_PRINT("RAW: ");
      DEBUG_PRINTLN(message);

      bool success = false;

      for ( int i = 0 ; i < message.length(); i++) {

        if (message.charAt(i) == ';') {

          switch (index) {
            case 0 : _time = String(buf); break;
            case 1 : _date = String(buf); break;
            case 2 : _lat = String(buf); break;
            case 3 : _lng = String(buf); break;
            case 4 : _mac = String(buf); break;
          }
          buf = "";

          if (index == 4) {
            success = true;
            break;
          }

          index ++;

        } else {
          buf += message.charAt(i);
        }
      }

      // if parse was ok and mac address is not the own address
      // Send the data
      if (success && !_mac.equals(WiFi.macAddress())) {
        return msg;
      } else {
        return NULL;
      }
    }


    virtual ~Message() {

    }
};



#endif // #define MESSAGE_H
