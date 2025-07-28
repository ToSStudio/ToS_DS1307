// Feather M0 --<>-- DS1307

#include <Wire.h>

class ToS_DS1307 {
  public:
    struct TimeDate {
      uint8_t second;
      uint8_t minute;
      uint8_t hour;
      uint8_t day;      // 0 = Sunday, 6 = Saturday
      uint8_t date;
      uint8_t month;
      uint8_t year;     // 00–99
    } now;

    bool begin() {
      Wire.begin();

      // Check if DS1307 responds
      Wire.beginTransmission(0x68);
      Wire.write(0x00); // Start at seconds register
      if (Wire.endTransmission() != 0)
        return false; // No response

      // Read seconds, minutes, hours
      Wire.requestFrom(0x68, 3);
      if (Wire.available() < 3)
        return false;

      uint8_t seconds = Wire.read();
      Wire.read(); // skip minutes
      uint8_t hour = Wire.read();

      // --- Ensure oscillator is running (CH = 0) ---
      if (seconds & 0x80) {
        seconds &= 0x7F;
        Wire.beginTransmission(0x68);
        Wire.write(0x00);       // Seconds register
        Wire.write(seconds);
        Wire.endTransmission();
      }

      // --- Ensure 24-hour mode (bit 6 = 0) ---
      if (hour & 0x40) {
        hour &= 0xBF;
        Wire.beginTransmission(0x68);
        Wire.write(0x02);       // Hour register
        Wire.write(hour);
        Wire.endTransmission();
      }

      // --- Disable square wave output (use OUT as GPIO) ---
      Wire.beginTransmission(0x68);
      Wire.write(0x07); // Control register
      Wire.write(0x00); // Disable SQW
      Wire.endTransmission();

      return true;
    }

    void read() {
      Wire.beginTransmission(0x68); // DS1307 I2C address
      Wire.write(0x00);             // Start at register 0
      Wire.endTransmission();

      Wire.requestFrom(0x68, 7);
      uint8_t reg[7];
      for (int i = 0; i < 7 && Wire.available(); ++i)
        reg[i] = Wire.read();

      now.second      = bcdToDec(reg[0] & 0x7F);
      now.minute      = bcdToDec(reg[1]);
      now.hour        = bcdToDec(reg[2] & 0x3F);
      now.day         = (reg[3] & 0x07) - 1; // 0 = Sunday
      now.date        = bcdToDec(reg[4]);
      now.month       = bcdToDec(reg[5]);
      now.year        = bcdToDec(reg[6]);
    }

    void setTime(uint8_t h, uint8_t m, uint8_t s) {
      Wire.beginTransmission(0x68);
      Wire.write(0x00);
      Wire.write(decToBcd(s));
      Wire.write(decToBcd(m));
      Wire.write(decToBcd(h));
      Wire.endTransmission();
    }

    void setDate(uint8_t date, uint8_t month, uint8_t year, uint8_t day_of_week = 0) {
      Wire.beginTransmission(0x68);  // DS1307 I2C address
      Wire.write(0x04);              // Start at the date register (0x04)
      Wire.write(decToBcd(date));    // Date (1–31)
      Wire.write(decToBcd(month));   // Month (1–12)
      Wire.write(decToBcd(year));    // Year (00–99)
      Wire.write(decToBcd(day_of_week + 1)); // Day of week (1–7, 1 = Sunday)
      Wire.endTransmission();
    }

    void sqwOff() {
      Wire.beginTransmission(0x68);
      Wire.write(0x07);
      Wire.write(0x00); // Control register: disable SQW
      Wire.endTransmission();
    }

    void sqwOn() {
      Wire.beginTransmission(0x68);
      Wire.write(0x07);
      Wire.write(0x80); // Control register: enable SQW (OUT=1)
      Wire.endTransmission();
    }

  private:
    uint8_t bcdToDec(uint8_t val) {
      return (val >> 4) * 10 + (val & 0x0F);
    }

    uint8_t decToBcd(uint8_t val) {
      return ((val / 10) << 4) | (val % 10);
    }
};

ToS_DS1307 rtc;  // initiate

// day-of-week table (1 = Sunday, per DS1307 datasheet)
const char* const dayNames[8] = {
  "", "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"
};

void print2d (uint8_t val) {
  if (val < 10) Serial.print("0");
  Serial.print (val);
}

void setup() {

  Serial.begin(9600);
  delay(2400);
  Serial.print (F("\nFeather M0 and DS1307"));

  if (rtc.begin())  {
    Serial.print (F("\nDS1307 RTC found and ocsillator running"));
  }  else  {
    Serial.print (F("\nDS1307 not connected. Check wiring !"));
  }

  // rtc.setTime (21, 53, 30);    // Optional: set once
  // rtc.setDate (27, 7, 25, 0);  // Set date

}


void loop() {

  static uint8_t lastSec = 255;
  rtc.read();
  if (rtc.now.second != lastSec) {
    lastSec = rtc.now.second;

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

    rtc.sqwOn();
    delay (85);
    rtc.sqwOff();

  }
  delay (10);

}
