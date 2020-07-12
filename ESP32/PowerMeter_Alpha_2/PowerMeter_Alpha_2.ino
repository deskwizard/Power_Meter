// Arduino IDE version 1.8.12
// ESP32 board files version 1.0.4

// This is a work in progress - The maths used by the original
// device is still undetermined for:
// - Voltage
// - Current
// - Power
// - Wh

// Those that work:
// - Power factor
// - Frequency

#include "defines.h"


bool ledState = LOW;

bool powerState = false;
bool lastPowerState = powerState;

uint32_t dataPacket = 0;
uint8_t currentReadRegisterID = 0;

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);

  Serial.begin(57600);
  Serial.println();
  Serial.println();
  Serial.println("Started");
  Serial.println();
  delay(100);
}

void loop() {

  static bool done_receiving_flag = false;
  uint8_t lowByte = 0;
  uint8_t midByte = 0;
  uint8_t highByte = 0;
  uint8_t sumByte = 0;


  //  // Power state detection
  handleMainsPower();



  uint32_t currentMillis = millis();
  static uint32_t previousReadMillis = 0;

  if (((uint32_t)(currentMillis - previousReadMillis) >= read_interval) && powerState) {

    request_data(currentReadRegisterID);

    previousReadMillis = currentMillis;
  }


  //  if (MySerial.available() == 4) {
  //    // we received the packet, deal with it.
  //
  //    for (uint8_t x = 0; x < 4; x++) {
  //      uint8_t dataByte = MySerial.read();
  //      Serial.print("x: ");
  //      Serial.print(x);
  //      Serial.print("  0x");
  //      Serial.println(dataByte);
  //    }
  //
  //  }


  if (MySerial.available() == 4) {
    // we received the data packet, now deal with it.

    lowByte = MySerial.read();
    midByte = MySerial.read();
    highByte = MySerial.read();
    sumByte = MySerial.read();

    dataPacket = ((uint32_t)highByte << 16) | ((uint32_t)midByte << 8) | (uint32_t)lowByte;

    done_receiving_flag = true;
  }


  if (done_receiving_flag) {

    uint8_t calcSum = ~(read_map[currentReadRegisterID] + lowByte + midByte + highByte) & 0xFF;

    //    Serial.println();
    //    Serial.print("0x");
    //    Serial.print(lowByte, HEX);
    //    Serial.print("  0x");
    //    Serial.print(midByte, HEX);
    //    Serial.print("  0x");
    //    Serial.print(highByte, HEX);
    //    Serial.print("  0x");
    //    Serial.print(sumByte, HEX);
    bool sumOk = false;
    if (sumByte == calcSum) {
      sumOk = true;
    }

    Serial.println();
    Serial.print("0x");
    Serial.print(dataPacket, HEX);
    //    Serial.print(" 0x");
    //    Serial.print(sumByte, HEX);
    //    Serial.print(" 0x");
    //    Serial.print(calcSum, HEX);
    Serial.print(" ");
    Serial.print(sumOk);
    Serial.println();

    do_the_thing(); // Calculate and display values

    currentReadRegisterID++;

    // If we were reading the last register in the array, go back to the beginning
    if (currentReadRegisterID == sizeof(read_map)) {
      currentReadRegisterID = 0;
    }

    done_receiving_flag = false;
  }


  // LED debug flash
  uint32_t currentFlashMillis = millis();
  static uint32_t previousFlashMillis = 0;
  static bool ledState;

  if ((uint32_t)(currentFlashMillis - previousFlashMillis) >= flash_interval) {
    ledState = !ledState;
    digitalWrite(LED_BUILTIN, ledState); // Toggle the LED
    previousFlashMillis = currentFlashMillis;
  }
  // end LED flash


} // loop
