# ToS_DS1307
A minimalist DS1307 RTC library with SQW and 24h control


---

Why another RTC-Library for Arduino ? This one gives you full control over the SQW-Pin of the DS1307. Use it as an additional GPIO. This can 
make quite a difference on an embedded system such as an ATtiny85 which comes with only five-and-a-half GPIOs. (The "half" GPIO is the Reset-pin which
can read Analog values above the reset-threshold. Some people have added multi-button keyboards using a resistor array attached to the reset-pin. See http://www.technoblogy.com/show?LSE).

Try the standalone demo first. It does not require any library as the DS1307-code is inside a class of the Sketch. It will display date & time on the
serial monitor and flash the LED on SQW every second. In setup() you can set time & date.


## 🧠 Highlights

- begin() will verify the chip is present, the oscillator is running and time-format is set to 24 hrs
- SQW-Pin can be used like a digital Pin of your arduino. Flash a LED with it. (See examples)


## 🛠️ Hardware Setup

- Arduino board (any)
- DS1307 with Xtal and backup battery
- I²C pull-up resistors (4.7kΩ recommended)
- Optional: USB-to-serial adapter or programmer for ATtiny85
