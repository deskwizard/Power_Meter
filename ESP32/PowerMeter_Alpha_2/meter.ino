// https://quadmeup.com/arduino-esp32-and-3-hardware-serial-ports/

void initialize_meter() {

  // Hardware reset sequence
  digitalWrite(TX_PIN, LOW);
  pinMode(TX_PIN, OUTPUT);
  delay(250);

  MySerial.begin(4800, SERIAL_8N1, RX_PIN, TX_PIN); // Enable serial 1 on pins 16(RX) and 17(TX)
  delay(100);

  // Write settings to metering chip
  MySerial.write(WPROT_OFF, sizeof(WPROT_OFF));
  MySerial.write(MODE, sizeof(MODE));
  MySerial.write(GAIN, sizeof(GAIN));
  MySerial.write(WA_CFDIV, sizeof(WA_CFDIV));
  MySerial.write(WPROT_ON, sizeof(WPROT_ON));
  delay(500); // and wait a bit.
}

void handleMainsPower() {

  static uint32_t lastPowerDebounceTime = 0;

  bool reading = digitalRead(INT_PIN);  // read button

  if (reading != lastPowerState) {  // if the powerState has changed
    lastPowerDebounceTime = millis();    // reset the timer
    lastPowerState = reading;
  }

  if ((millis() - lastPowerDebounceTime) >= PWR_DEBOUNCE_DELAY) {  // if PWR_DEBOUNCE_DELAY time has passed
    if (reading != powerState) {  // and if powerState is different
      powerState = reading;  // if its different, set to current state

      if (powerState) {
        Serial.println("On");
        initialize_meter();
      }
      else {
        Serial.println("Off");
        MySerial.end();               // Close serial port before reset on power on
        currentReadRegisterID = 0;    // Reset current read register so we start at the beginning again
      }
    }
  }
  
}


void request_data(uint8_t read_reg_id) {
  currentReadRegisterID = read_reg_id;
  MySerial.write(CMD_READ);
  MySerial.write(read_map[read_reg_id]);
}

void doAmps() {
  Serial.print("A: ");
  Serial.println(dataPacket, 4);
}

void doVolts() {
  Serial.print("V: ");
  Serial.println(dataPacket);
}

void doWatts() {
  Serial.print("W: ");
  Serial.println(dataPacket);
}

void doFreq() {
  //float freq = (87.3906 * 3.579545) / (32.0*dataPacket); // From datasheet
  float freq = ((87.3906 * 3.579545) / (32.0 * dataPacket)) * 1000000; // what works
  Serial.print("Freq: ");
  Serial.print(freq, 2);
  Serial.println(" Hz");
}

void doPF() {
  float pf = mapf(dataPacket, 0x000000, 0x7FFFFF, 0.0, 1.0);
  Serial.print("PF: ");
  Serial.println(pf, 2);
}

void doWH() {
  Serial.print("Wh: ");
  Serial.println(dataPacket);
}

void do_the_thing() {

  switch (currentReadRegisterID) {
    case AMPS:
      doAmps();
      break;
    case VOLTS:
      doVolts();
      break;
    case WATTS:
      doWatts();
      break;
    case FREQ:
      doFreq();
      break;
    case PF:
      doPF();
      break;
    case WHATHR:
      doWH();
      break;
    default:
      Serial.print("Other: ");
      Serial.print(currentReadRegisterID);
      Serial.print(" 0x");
      Serial.println(dataPacket, HEX);
      break;
  }

}

float mapf(float x, float in_min, float in_max, float out_min, float out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
