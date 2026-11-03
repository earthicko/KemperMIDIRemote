#include "Buttons.hpp"
#include "Cfg.hpp"
#include "Kemper.hpp"
#include "SerialPrintf.hpp"
#include <Arduino.h>
#include <MIDI.h>

// ROCKBOARD Midi cable
// Pin 3V3: (Yellow)
// Pin GND: (Green)
// Pin 17: Tx (Black)

// Use UART2 instead on pins 16/17
MIDI_CREATE_INSTANCE(HardwareSerial, Serial2, midiSw1); // UART 2 (GPIO 16/17)

int buttonIdxToPin[] = {
    PIN_UP,
    PIN_DOWN,
    PIN_PERFORMANCE_1,
    PIN_PERFORMANCE_2,
    PIN_PERFORMANCE_3,
    PIN_PERFORMANCE_4,
    PIN_PERFORMANCE_5,
};

Buttons buttons = Buttons(sizeof(buttonIdxToPin) / sizeof(int), buttonIdxToPin);
Kemper kemper = Kemper(midiSw1);

void setup() {
  // Init Serial Monitor
  Serial.begin(115200);

  // This listens to all MIDI channels
  midiSw1.begin(MIDI_CHANNEL_OMNI);
  // Need to re-initialise serial port with correct pins.
  // MIDI will call Serial2.begin, but with no specific pin
  // settings, so the ESP32 core will use some defaults,
  // which aren't the same as what we need...
  Serial2.begin(31250, SERIAL_8N1, 16, 17);

  // sleep(5);
  // midiSw1.sendProgramChange(1, 1);
  // serial_printf("test 1\n");
  // sleep(5);
  // midiSw1.sendProgramChange(0, 1);
  // serial_printf("test 2\n");

  // while (1) {
  //   //
  // }
  buttons.begin();
  kemper.begin();
}

void loop() {
  buttons.update();

  int pinNum = -1;
  int event = -1;

  buttons.dequeueEvent(&pinNum, &event);
  if (event == BUTTONS_EVENT_PRESSED) {
    switch (pinNum) {
    case PIN_UP:
      serial_printf("UP PRESSED\n");
      kemper.incrementRow(1);
      break;
    case PIN_DOWN:
      serial_printf("DOWN PRESSED\n");
      kemper.incrementRow(-1);
      break;
    case PIN_PERFORMANCE_1:
      serial_printf("1 PRESSED\n");
      kemper.changeCol(0);
      break;
    case PIN_PERFORMANCE_2:
      serial_printf("2 PRESSED\n");
      kemper.changeCol(1);
      break;
    case PIN_PERFORMANCE_3:
      serial_printf("3 PRESSED\n");
      kemper.changeCol(2);
      break;
    case PIN_PERFORMANCE_4:
      serial_printf("4 PRESSED\n");
      kemper.changeCol(3);
      break;
    case PIN_PERFORMANCE_5:
      serial_printf("5 PRESSED\n");
      kemper.changeCol(4);
      break;
    }
  }
}