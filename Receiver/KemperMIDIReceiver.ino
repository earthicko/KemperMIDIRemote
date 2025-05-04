#include "SerialPrintf.hpp"
#include <MIDI.h>
#include <RH_ASK.h>
#include <SPI.h> // Not actualy used but needed to compile
#include <SoftwareSerial.h>

// MIDI Green: GND
// MIDI Yellow: 5V
// MIDI Black: SwSerial TX (D8)
SoftwareSerial swSerial(10, 8); // RX, TX

MIDI_CREATE_INSTANCE(SoftwareSerial, swSerial, midiSw1);

// D11
RH_ASK driver;

void setup() {
  Serial.begin(9600);
  if (!driver.init()) {
    Serial.println("init failed");
  } else {
    Serial.println("init success!");
  }

  midiSw1.begin(MIDI_CHANNEL_OMNI); // Initialize MIDI
}

void loop() {
  uint8_t buf[2];
  uint8_t buflen = sizeof(buf);
  memset(buf, 0, buflen);
  if (driver.recv(buf, &buflen)) {
    SerialPrintf("Program %d, Bank %d\n", buf[0], buf[1]);
    midiSw1.sendProgramChange(buf[0], buf[1]);
  }
}