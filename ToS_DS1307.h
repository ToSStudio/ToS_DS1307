#ifndef TOS_DS1307_H
#define TOS_DS1307_H

#include <Wire.h>

class ToS_DS1307 {
public:
    struct Time {
        uint8_t second, minute, hour, day, date, month, year;
    } now;

    bool begin();                // Initializes RTC and enables oscillator
    void read();                 // Reads current time into now
    void write();                // Writes now back to RTC
    void sqwOn();                // Sets SQW pin high (OUT=1)
    void sqwOff();              // Sets SQW pin low (OUT=0)

private:
    uint8_t bcd2bin(uint8_t b) { return (b >> 4) * 10 + (b & 0x0F); }
    uint8_t bin2bcd(uint8_t b) { return ((b / 10) << 4) | (b % 10); }

    static const uint8_t addr = 0x68;
};

inline bool ToS_DS1307::begin() {
    Wire.beginTransmission(addr);
    if (Wire.endTransmission() != 0)
        return false;

    Wire.beginTransmission(addr);
    Wire.write(0x00);           // seconds register
    Wire.endTransmission();

    Wire.requestFrom(addr, (uint8_t)1);
    if (Wire.available()) {
        uint8_t sec = Wire.read();
        if (sec & 0x80) {
            sec &= 0x7F;
            Wire.beginTransmission(addr);
            Wire.write(0x00);
            Wire.write(sec);
            Wire.endTransmission();
        }
    }

    // Set 24h mode
    Wire.beginTransmission(addr);
    Wire.write(0x02);
    Wire.endTransmission();
    Wire.requestFrom(addr, (uint8_t)1);
    if (Wire.available()) {
        uint8_t hour = Wire.read();
        if (hour & 0x40) {
            hour &= ~0x40;
            Wire.beginTransmission(addr);
            Wire.write(0x02);
            Wire.write(hour);
            Wire.endTransmission();
        }
    }

    return true;
}

inline void ToS_DS1307::read() {
    Wire.beginTransmission(addr);
    Wire.write(0x00);
    Wire.endTransmission();

    Wire.requestFrom(addr, (uint8_t)7);
    if (Wire.available() >= 7) {
        now.second = bcd2bin(Wire.read() & 0x7F);
        now.minute = bcd2bin(Wire.read());
        now.hour   = bcd2bin(Wire.read() & 0x3F);
        now.day    = bcd2bin(Wire.read());
        now.date   = bcd2bin(Wire.read());
        now.month  = bcd2bin(Wire.read());
        now.year   = bcd2bin(Wire.read());
    }
}

inline void ToS_DS1307::write() {
    Wire.beginTransmission(addr);
    Wire.write(0x00);
    Wire.write(bin2bcd(now.second));
    Wire.write(bin2bcd(now.minute));
    Wire.write(bin2bcd(now.hour & 0x3F));
    Wire.write(bin2bcd(now.day));
    Wire.write(bin2bcd(now.date));
    Wire.write(bin2bcd(now.month));
    Wire.write(bin2bcd(now.year));
    Wire.endTransmission();
}

inline void ToS_DS1307::sqwOn() {
    Wire.beginTransmission(addr);
    Wire.write(0x07);
    Wire.write(0b10000000); // OUT=1, SQWE=0
    Wire.endTransmission();
}

inline void ToS_DS1307::sqwOff() {
    Wire.beginTransmission(addr);
    Wire.write(0x07);
    Wire.write(0b00000000); // OUT=0, SQWE=0
    Wire.endTransmission();
}

#endif // TOS_DS1307_H
