# Power_Meter

Repository for my attempt at reverse-engineering the NA version of the
obiquitous power meter showed in BigClive.com's video about the UK version.

Big Clive's video: https://www.youtube.com/watch?v=fRGKilvExMo

Sorry about the bad quality of the pictures, I did my best.

There is a lot of differences between the two, how many of these are because
of the NA vs UK version (besides the obvious) or just a cost-reduced version 
of the backlight equipped one (or both) is currently debatable. 

My best guess is a cut-down version of the one with backlight. More below.


*** THIS VERSION IS ALSO LIVE REFERENCED - BEWARE!! ***


Differences:

The capacitive dropper capacitor is 1uF instead of 0.47uF (half the voltage, double the capacitor...).
The in-rush limiting resistor stayed at 100 ohms.

The metering chip is a BL6523GX from Shanghai Belling.
Product page: http://www.belling.com.cn/en/product_info.html?id=11
English datasheet: https://s1.dtsheet.com/store/data/001450378.pdf?key=5f9d204f04c58b2e016a2c724a615611&r=1

The mains power detection circuitry is different, there is no transistor on the measuring board.
(More below.)

The ICs on the display board have been replaced by an epoxy blob.
(Educated guess: a microcontroller with LCD display driver but no EEPROM.)

The backlight circuitry is present and working as expected, but no backlight is installed.
The backlight functions only with mains power applied (as the 12v supply is generated from mains).

An i²c EEPROM (P/N 24C02A @ address 0x50) has been added to save/restore data, and it's always powered.
(See EEPROM folder for more on that. Work in progress...)

The cable between the boards have 2 more pins, SCL and SDA for communication with the EEPROM.


More informations:

The microcontroller doesn't check for chip presence, so giving the MCU the "mains is there" signal was sufficient
to trick it. See the serial.txt file for it's output.
Nice part of this is, no mains connected, and no risk of blowing stuff up.

PB0 High enables the circuit. 
~2.7V-2.9V tested with a voltage divider from VCC (1K) / GROUND (10K), probably good up to VCC.
With the signal from the measuring board disconnected.

Cable Pinout (from "top" of the measuring board)

1       SCL   To EEPROM (10k pullup on measuring board)
2       SDA   TO EEPROM (10k pullup on measuring board)
3       PB0   Mains Power Detection (~2.7v)
4       PB1   MCU UART RX (1k resistor in series near BL6523GX)
5       PB2   MCU UART TX (1k resistor in series near BL6523GX)
6       GND   *** LIVE REFERENCED!!! ***
7       VCC   ~3.6V when on battery
8       ~12V


2 pin pads on display board (from board edge)
1       Down button
2       Up button  (1 silkscreen beside it)


3 pin pads on display board (from board edge)
1       VCC
2       GND
3       TBD (only connected to mcu)

Those two together make me thing of a programming port. Who knows...


Buttons common is VCC.

