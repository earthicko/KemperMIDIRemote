#include "LedDisplay.hpp"

LedDisplay::LedDisplay(LedControl &lc) : lc(lc) {}

LedDisplay::~LedDisplay() {}

void LedDisplay::begin(void) {
  lc.shutdown(0, false);
  delay(DISPLAY_REFRESH_DELAY);
  lc.setIntensity(0, 7); // 0 ~ 15
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

  // 7 6 5 4 3 2 1 0 (LED idx)
  // 0 1 2 3 4 (String idx)
  // H e l l o (len=5)
  for (int ledIdx = 8 - 1; ledIdx >= 0; ledIdx--) {
    int strIdx = 8 - 1 - ledIdx;
    if (0 <= strIdx && strIdx < value.length()) {
      setChar(ledIdx, value[strIdx]);
    } else {
      clear(ledIdx);
    }
  }
}

void LedDisplay::setChar(int digit, char c) {
  switch (c) {
  case 'A':
    lc.setRow(0, digit, DISPLAY_UPPER_A);
    break;
  case 'C':
    lc.setRow(0, digit, DISPLAY_UPPER_C);
    break;
  case 'E':
    lc.setRow(0, digit, DISPLAY_UPPER_E);
    break;
  case 'F':
    lc.setRow(0, digit, DISPLAY_UPPER_F);
    break;
  case 'G':
    lc.setRow(0, digit, DISPLAY_UPPER_G);
    break;
  case 'H':
    lc.setRow(0, digit, DISPLAY_UPPER_H);
    break;
  case 'I':
    lc.setRow(0, digit, DISPLAY_UPPER_I);
    break;
  case 'J':
    lc.setRow(0, digit, DISPLAY_UPPER_J);
    break;
  case 'L':
    lc.setRow(0, digit, DISPLAY_UPPER_L);
    break;
  case 'O':
    lc.setRow(0, digit, DISPLAY_UPPER_O);
    break;
  case 'P':
    lc.setRow(0, digit, DISPLAY_UPPER_P);
    break;
  case 'S':
    lc.setRow(0, digit, DISPLAY_UPPER_S);
    break;
  case 'U':
    lc.setRow(0, digit, DISPLAY_UPPER_U);
    break;
  case 'Y':
    lc.setRow(0, digit, DISPLAY_UPPER_Y);
    break;
  case 'b':
    lc.setRow(0, digit, DISPLAY_LOWER_B);
    break;
  case 'c':
    lc.setRow(0, digit, DISPLAY_LOWER_C);
    break;
  case 'd':
    lc.setRow(0, digit, DISPLAY_LOWER_D);
    break;
  case 'g':
    lc.setRow(0, digit, DISPLAY_LOWER_G);
    break;
  case 'h':
    lc.setRow(0, digit, DISPLAY_LOWER_H);
    break;
  case 'i':
    lc.setRow(0, digit, DISPLAY_LOWER_I);
    break;
  case 'j':
    lc.setRow(0, digit, DISPLAY_LOWER_J);
    break;
  case 'l':
    lc.setRow(0, digit, DISPLAY_LOWER_L);
    break;
  case 'n':
    lc.setRow(0, digit, DISPLAY_LOWER_N);
    break;
  case 'o':
    lc.setRow(0, digit, DISPLAY_LOWER_O);
    break;
  case 'r':
    lc.setRow(0, digit, DISPLAY_LOWER_R);
    break;
  case 't':
    lc.setRow(0, digit, DISPLAY_LOWER_T);
    break;
  case 'u':
    lc.setRow(0, digit, DISPLAY_LOWER_U);
    break;

  default:
    lc.setChar(0, digit, c, false);
    break;
  }

  delay(DISPLAY_REFRESH_DELAY);
}
