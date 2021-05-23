#include <Arduino.h>
#include <Wire.h> 

void setup() {
        Serial.begin (115200);
        while (!Serial) {}
        
}  // end of setup

void loop() {

        Serial.println ();
        Serial.println ("Scan the wire bus ...");
        byte count = 0;

        //Wire.begin(D6,D5);
        Wire.begin(21,22);
        for (byte i = 8; i < 120; i++)
        {
                Wire.beginTransmission (i);
                if (Wire.endTransmission () == 0)
                {
                        Serial.print ("Found i2c Device Address: ");
                        Serial.print (i, DEC);
                        Serial.print (" (0x");
                        Serial.print (i, HEX);
                        Serial.println (")");
                        count++;
                        delay (1);
                } // end of good response
        } // end of for loop
        Serial.println ("Done.");
        Serial.print ("Found ");
        Serial.print (count, DEC);
        Serial.println (" device(s).");

        delay(5000);
}

