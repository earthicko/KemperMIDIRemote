#include "LedDisplay.hpp"

LedDisplay::LedDisplay(LedControl &lc) : lc(lc) {}

LedDisplay::~LedDisplay() {}

void LedDisplay::begin(void) {
  lc.shutdown(0, false);
  delay(DISPLAY_REFRESH_DELAY);
  lc.setIntensity(0, 15); // 0 ~ 15
  delay(DISPLAY_REFRESH_DELAY);
  lc.clearDisplay(0);
  delay(DISPLAY_REFRESH_DELAY);
}
void LedDisplay::clear(int digit) { lc.setRow(0, digit, 0b00000000); }

void LedDisplay::setDigit(int digit, byte value) {
  lc.setDigit(0, digit, value, false);
}

void LedDisplay::showInt(int value) {
  if (value < 0) {
    return;
  }

  if (value == 0) {
    setDigit(0, 0);
    for (int i = 1; i < 8; i++) {
      clear(i);
    }
    return;
  }

  for (int i = 0; i < 8; i++) {
    if (value == 0) {
      clear(i);
    } else {
      byte valueInThisDigit = value % 10;
      value = value / 10;
      setDigit(i, valueInThisDigit);
    }
  }
}

void LedDisplay::showString(String value) {
  if (value.length() > 8) {
    return;
  }

  // 4 3 2 1 0 (LED idx)
  // 0 1 2 3 4 (String idx)
  // H e l l o (len=5)
  for (int ledIdx = 0; ledIdx < 8; ledIdx++) {
    int strIdx = value.length() - ledIdx - 1;

    if (0 <= strIdx && strIdx < value.length()) {
      setChar(ledIdx, value[strIdx]);
    } else {
      clear(ledIdx);
    }
  }
}

void LedDisplay::setChar(int digit, char c) {
  switch (c) {
  case 'E':
    lc.setRow(0, 1, DISPLAY_UPPER_E);
    break;
  case 'r':
    lc.setRow(0, 1, DISPLAY_LOWER_R);
    break;
  case 'o':
    lc.setRow(0, 1, DISPLAY_LOWER_O);
    break;

  default:
    break;
  }

  lc.setChar(0, digit, c, false);
  delay(DISPLAY_REFRESH_DELAY);
}
