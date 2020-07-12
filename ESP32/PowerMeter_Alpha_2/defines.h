#ifndef _defines_h
#define _defines_h

#include <HardwareSerial.h>
#define RX_PIN 16
#define TX_PIN 17
HardwareSerial MySerial(1);

#define LED_BUILTIN 25
#define flash_interval 1000


#define INT_PIN   18
#define PWR_DEBOUNCE_DELAY 50 // milliseconds

#define read_interval 250 // ms

// Commands
#define CMD_READ  0x35
#define CMD_WRITE 0xCA

// Registers in order they are used.

// Registers to write to
#define REG_WRPROT    0x3E
#define REG_MODE      0x14
#define REG_GAIN      0x15
#define REG_WA_CFDIV  0x19
#define REG_SOFT_NRST 0x3F


// Registers to read from
#define REG_IA_RMS  0x05
#define REG_V_RMS   0x07
#define REG_FREQ    0x09
#define REG_A_WATT  0x0A
#define REG_PF      0x08
#define REG_WATTHR  0x0C

// Initialization commands
const uint8_t WPROT_OFF[] =  {CMD_WRITE, REG_WRPROT,    0x55, 0x00, 0x00, 0x6C};
const uint8_t MODE[]      =  {CMD_WRITE, REG_MODE,      0x00, 0x00, 0x10, 0xDB};
const uint8_t GAIN[]      =  {CMD_WRITE, REG_GAIN,      0x04, 0x00, 0x00, 0xE6};
const uint8_t WA_CFDIV[]  =  {CMD_WRITE, REG_WA_CFDIV,  0x08, 0x00, 0x00, 0xDE};
const uint8_t WPROT_ON[]  =  {CMD_WRITE, REG_WRPROT,    0xAA, 0x00, 0x00, 0x17};
const uint8_t SOFT_NRST[] =  {CMD_WRITE, REG_SOFT_NRST, 0x5A, 0x5A, 0x5A, 0xB2};


// Register ID to mapping array (to ease for-loop)

// IDs
#define AMPS    0
#define VOLTS   1
#define FREQ    2
#define WATTS   3
#define PF      4
#define WHATHR  5

//const uint8_t read_map[] = {REG_IA_RMS, REG_V_RMS, REG_FREQ, REG_A_WATT, REG_PF, REG_WATTHR, REG_MODE, REG_GAIN, REG_WA_CFDIV, REG_WRPROT};
const uint8_t read_map[] = {REG_IA_RMS, REG_V_RMS, REG_FREQ, REG_A_WATT, REG_PF, REG_WATTHR};

#endif
