/** test the influx db connection.
    create a measurement with some device data and insert it into the database table.
    This example is created with the old version of the api.
    Libs
    https://github.com/tobiasschuerg/InfluxDB-Client-for-Arduino


    Credits
    Created by S. Fambach visit Fambach.net if you want.
*/

#include "credentials.h"
#include <WiFi.h>
#include <ESP.h>
#include <InfluxDb.h> // https://github.com/tobiasschuerg/InfluxDB-Client-for-Arduino


#define INFLUXDB_CLIENT_DEBUG
// Single InfluxDB instance
Influxdb influx(INFLUXDB_HOST, INFLUXDB_PORT);

void setup() {
  DEBUG_INIT;

  WiFi.disconnect();

  WiFi.mode(WIFI_STA); // kann auch ein anderer Mode sein
  while (WiFi.status() != WL_CONNECTED) {
    WiFi.begin(mySSID, myPW);
    delay(1000);
  }
  // connected
  DEBUG_PRINT("Wifi Connected ip:");
  DEBUG_PRINTLN(WiFi.localIP());

  // set the target database
  influx.setDb(INFLUXDB_DATABASE);
  // or use a db with auth
  influx.setDbAuth(INFLUXDB_DATABASE, INFLUXDB_USER, INFLUXDB_PASS); // with authentication

}


void loop() {

  InfluxData measurement ("node_data");
  measurement.addField("mac", WiFi.macAddress());
  measurement.addField("ip", WiFi.localIP().toString());


  //Internal RAM
  measurement.addField("heap_size", ESP.getHeapSize()/1000); //total heap size
  measurement.addField( "heap_free", ESP.getFreeHeap()/1000); //available heap
  measurement.addField("heap_minfree", ESP.getMinFreeHeap()/1000); //lowest level of free heap since boot
  measurement.addField("heap_maxalloc", ESP.getMaxAllocHeap()/1000); //largest block of heap that can be allocated at once

  //SPI RAM
  measurement.addField("psram_size", ESP.getPsramSize()/1000);
  measurement.addField( "psram_free", ESP.getFreePsram()/1000);
  measurement.addField( "psram_minfree", ESP.getMinFreePsram()/1000);
  measurement.addField("psram_maxalloc", ESP.getMaxAllocPsram()/1000);

  //chip data
  measurement.addField("chip_model", ESP.getChipModel());
  measurement.addField("chip_revision", ESP.getChipRevision());
  measurement.addField("chip_cores", ESP.getChipCores());
  measurement.addField("cpu_freq", ESP.getCpuFreqMHz());
  measurement.addField("cylce_count", ESP.getCycleCount());
  measurement.addField("sdk_version", ESP.getSdkVersion());

  // flash
  measurement.addField("flash_speed", ESP.getFlashChipSpeed());
  measurement.addField("flash_size", ESP.getFlashChipSize());
  measurement.addField("flash_mode", ESP.getFlashChipMode());

  // sketch things
  measurement.addField("sketch_md5", ESP.getSketchMD5());
  measurement.addField("sketch_size", ESP.getSketchSize());
  measurement.addField("sketch_freespace", ESP.getFreeSketchSpace());

  // write it into db
  influx.write(measurement);
  delay(4000);
}
