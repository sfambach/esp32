#ifndef CREDENTIALS_H
#define CREDENTIALS_H

#include "debug.h"

// WiFi
const char* mySSID = "NODES";
const char* myPW = "HappyNodes1234";


// influx db settings
#include <InfluxDbClient.h>

#define INFLUXDB_HOST "192.168.1.10"
#define INFLUXDB_PORT 8086
#define INFLUXDB_DATABASE "test_data"

//if used with authentication
#define INFLUXDB_USER "dba"
#define INFLUXDB_PASS "supremo"


#endif // #ifndef CREDENTIALS_H
