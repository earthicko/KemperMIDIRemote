#include "Buttons.hpp"
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

RH_ASK driver; // Pin 12

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
LedControl lc = LedControl(PIN_DISPLAY_DIN, PIN_DISPLAY_CLK, PIN_DISPLAY_CS, 1);

int col = 0;
int row = 0;

void changeProgramChange(void) {
  SerialPrintf("row: %d, col: %d\n", row, col);
  int program = row * 5 + col;
  int bank = 1; // Assuming bank is always 1 for this example
  SerialPrintf("Program Change: %d, Bank: %d\n", program, bank);

  uint8_t msg[2] = {(uint8_t)program, (uint8_t)bank};
  if (driver.send(msg, sizeof(msg))) {
    driver.waitPacketSent();
    SerialPrintf("Sent Program %d, Bank %d\n", msg[0], msg[1]);
    leds.select(col); // Select the LED corresponding to the column
  } else {
    SerialPrintf("Failed to send a message...\n");
  }
}

void incrementRow(int newRow) {
  row += newRow;
  if (row < 0) {
    row = 0;
  }
  changeProgramChange();
}

void changeCol(int newCol) {
  col = newCol;
  changeProgramChange();
}

void setup() {
  Wire.begin();
  Serial.begin(115200);
  Serial1.begin(31250);

  buttons.begin();
  leds.begin();

  SerialPrintf("Kemper MIDI Test");

  if (!driver.init())
    SerialPrintf("init failed");
  else
    SerialPrintf("init succeeded");

  lc.shutdown(0, false);
  delay(50);
  lc.setIntensity(0, 15); // 0 ~ 15
  delay(50);
  lc.clearDisplay(0);
  delay(50);
  lc.setRow(0, 6, 0x77); // A
  lc.setRow(0, 5, 0x05); // r
  lc.setRow(0, 4, 0x3d); // d
  lc.setRow(0, 3, 0x1c); // u
  lc.setRow(0, 2, 0x10); // i
  lc.setRow(0, 1, 0x15); // n
  lc.setRow(0, 0, 0x1d); // o
  // lc.setDigit(0, 7, 2,
  //             false); // To display the 2 first column on the left (column 7)
  // lc.setDigit(0, 6, 5, true);  // To display a 5 column 6 and decimal point
  // lc.setDigit(0, 5, 4, false); // To display a 4 column 5
  // lc.setRow(0, 4, B1001110);
  // lc.setDigit(0, 2, 3, false); // To show the 5 on 2nd
  // lc.setDigit(0, 1, 7, false); // To Place a 0 in the column 1
  // lc.setRow(0, 0, B0110111);

  SerialPrintf("LED Device: %d\n", lc.getDeviceCount());

  changeProgramChange();
}

void loop() {
  buttons.update();
  for (int i = 0; i < sizeof(buttonIdxToPin) / sizeof(int); i++) {
    int pin = buttonIdxToPin[i];
    int event = buttons.dequeueEvent(pin);
    if (event == BUTTONS_EVENT_PRESSED) {
      switch (pin) {
      case PIN_UP:
        SerialPrintf("UP PRESSED");
        incrementRow(1);
        break;
      case PIN_DOWN:
        SerialPrintf("DOWN PRESSED");
        incrementRow(-1);
        break;
      case PIN_PERFORMANCE_1:
        SerialPrintf("1 PRESSED");
        changeCol(0);
        break;
      case PIN_PERFORMANCE_2:
        SerialPrintf("2 PRESSED");
        changeCol(1);
        break;
      case PIN_PERFORMANCE_3:
        SerialPrintf("3 PRESSED");
        changeCol(2);
        break;
      case PIN_PERFORMANCE_4:
        SerialPrintf("4 PRESSED");
        changeCol(3);
        break;
      case PIN_PERFORMANCE_5:
        SerialPrintf("5 PRESSED");
        changeCol(4);
        break;
      }
    }
  }
}

// This function gets called whenever new serial data arrives
// void serialEvent() {
//  while (Serial.available()) {
//    char inChar = (char)Serial.read();
//    if (inChar == '\n' || inChar == '\r') {
//      stringComplete = true;
//    } else {
//      inputString += inChar;
//    }
//  }
//}
