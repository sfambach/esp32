#ifndef CREDENTIALS_H
#define CREDENTIALS_H

#include "debug.h"

struct SSID_DATA{
  char* ssid;
  char* pw;
};

// WiFi
const int SSID_SIZE = 1;
SSID_DATA ssids[SSID_SIZE] ={ {.ssid="NODES", .pw="HappyNodes1234"}};

const char * hostname = "Mein ESP";


#endif // #ifndef CREDENTIALS_H
