/** Debuging *****************************************************************/
#ifndef DEBUG_H
#define DEBUG_H

#include "credentials.h"


#ifdef DEBUG_ACTIVE
#define DEBUG_PRINTLN(x) Serial.println(x)
#define DEBUG_PRINT(x) Serial.print(x)
#else
#define DEBUG_PRINTLN(x)
#define DEBUG_PRINT(x)
#endif


#endif // DEBUG_H