#include "Buttons.hpp"
#include "Kemper.hpp"
#include "LedSelector.hpp"
#include "SerialPrintf.hpp"
#include <LedControl.h>
#include <RH_ASK.h>
#include <SPI.h> // Not actually used but needed to compile
#include <Wire.h>

#define PIN_UP 2
#define PIN_DOWN 3
#define PIN_PERFORMANCE_1 4
#define PIN_PERFORMANCE_2 5
#define PIN_PERFORMANCE_3 6
#define PIN_PERFORMANCE_4 7
#define PIN_PERFORMANCE_5 8
#define PIN_LED_1 30
#define PIN_LED_2 31
#define PIN_LED_3 32
#define PIN_LED_4 33
#define PIN_LED_5 34
#define PIN_DISPLAY_DIN 37
#define PIN_DISPLAY_CS 39
#define PIN_DISPLAY_CLK 41

RH_ASK wireless; // Pin 12

int buttonIdxToPin[] = {
    PIN_UP,
    PIN_DOWN,
    PIN_PERFORMANCE_1,
    PIN_PERFORMANCE_2,
    PIN_PERFORMANCE_3,
    PIN_PERFORMANCE_4,
    PIN_PERFORMANCE_5,
};
int ledIdxToPin[] = {
    PIN_LED_1, PIN_LED_2, PIN_LED_3, PIN_LED_4, PIN_LED_5,
};

Buttons buttons = Buttons(sizeof(buttonIdxToPin) / sizeof(int), buttonIdxToPin);
LedSelector leds = LedSelector(sizeof(ledIdxToPin) / sizeof(int), ledIdxToPin);
LedControl display =
    LedControl(PIN_DISPLAY_DIN, PIN_DISPLAY_CLK, PIN_DISPLAY_CS, 1);
Kemper kemper = Kemper(wireless, leds, display);

void setup() {
  Wire.begin();
  Serial.begin(115200);
  Serial1.begin(31250);

  buttons.begin();

  SerialPrintf("Kemper MIDI Transmitter\n");
  kemper.begin();

  display.shutdown(0, false);
  delay(50);
  display.setIntensity(0, 15); // 0 ~ 15
  delay(50);
  display.clearDisplay(0);
  delay(50);
  display.setRow(0, 6, 0x77); // A
  display.setRow(0, 5, 0x05); // r
  display.setRow(0, 4, 0x3d); // d
  display.setRow(0, 3, 0x1c); // u
  display.setRow(0, 2, 0x10); // i
  display.setRow(0, 1, 0x15); // n
  display.setRow(0, 0, 0x1d); // o

  SerialPrintf("LED Device: %d\n", display.getDeviceCount());
}

void loop() {
  buttons.update();

  int pinNum = -1;
  int event = -1;

  buttons.dequeueEvent(&pinNum, &event);
  if (event == BUTTONS_EVENT_PRESSED) {
    switch (pinNum) {
    case PIN_UP:
      SerialPrintf("UP PRESSED\n");
      kemper.incrementRow(1);
      break;
    case PIN_DOWN:
      SerialPrintf("DOWN PRESSED\n");
      kemper.incrementRow(-1);
      break;
    case PIN_PERFORMANCE_1:
      SerialPrintf("1 PRESSED\n");
      kemper.changeCol(0);
      break;
    case PIN_PERFORMANCE_2:
      SerialPrintf("2 PRESSED\n");
      kemper.changeCol(1);
      break;
    case PIN_PERFORMANCE_3:
      SerialPrintf("3 PRESSED\n");
      kemper.changeCol(2);
      break;
    case PIN_PERFORMANCE_4:
      SerialPrintf("4 PRESSED\n");
      kemper.changeCol(3);
      break;
    case PIN_PERFORMANCE_5:
      SerialPrintf("5 PRESSED\n");
      kemper.changeCol(4);
      break;
    }
  }
}
