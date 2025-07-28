#include "ToS_DS1307.h"

ToS_DS1307 rtc;

// Optional: day-of-week table (1 = Sunday, per DS1307 datasheet)
const char* const dayNames[8] = {
  "", "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"
};

void setup() {
  Serial.begin(9600);
  while (!Serial);  // for Leonardo/Micro

  if (!rtc.begin()) {
    Serial.println("RTC not found.");
    while (1);
  }

  // Optional: set time manually once
  /*
  rtc.now.hour = 15;
  rtc.now.minute = 42;
  rtc.now.second = 10;
  rtc.now.day = 5;      // Friday
  rtc.now.date = 24;
  rtc.now.month = 7;
  rtc.now.year = 25;    // = 2025
  rtc.write();
  */
}

void loop() {
  rtc.read ();

  // Print day name
  const char* dayStr = (rtc.now.day >= 1 && rtc.now.day <= 7) ? dayNames[rtc.now.day] : "???";

  Serial.print (dayStr);
  Serial.print (" ");

  // Print time HH:MM:SS
  print2d (rtc.now.hour);
  Serial.print (":");
  print2d (rtc.now.minute);
  Serial.print (":");
  print2d (rtc.now.second);

  Serial.print ("  ");

  // Print date DD.MM.YYYY
  print2d (rtc.now.date);
  Serial.print (".");
  print2d (rtc.now.month);
  Serial.print (".");
  Serial.print (2000 + rtc.now.year);

  Serial.println();

  delay (1000);
}

void print2d (uint8_t val) {
  if (val < 10) Serial.print("0");
  Serial.print (val);
}
