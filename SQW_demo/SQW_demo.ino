// Demo to show the use of the SQW-pin of your DS1307
// Attach the DS1307 module to GND, Vcc, SDA and SCL
// Attach a LED between SQW and GND with a appropriate resistor (e.g. 330 Ω)


#include "ToS_DS1307.h"

ToS_DS1307 rtc;


void setup() {

  Serial.begin(9600);
  while (!Serial);  // for Leonardo/Micro
  Serial.print ("\nTesting the SQW-Pin feature of the DS1307 Module");

  if (!rtc.begin()) {
    Serial.print (F("\nRTC not found."));
    while (1);
  } else {
    Serial.print (F("\nRTC (DS1307) detected and verified running !"));
  }
  
}


void loop() {

  rtc.sqwOn();
  Serial.print ("\nLED on");
  delay (1000);
  rtc.sqwOff();
  Serial.print ("\nLED off");
  delay (1000);

}
