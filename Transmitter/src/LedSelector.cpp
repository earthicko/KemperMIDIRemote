#include "LedSelector.hpp"
#include <Arduino.h>


LedSelector::LedSelector(int numLeds, int *ledPins) {
  this->numLeds = numLeds;
  for (int i = 0; i < numLeds; i++) {
    this->ledPins[i] = ledPins[i];
  }
  this->currentLed = -1;
  this->previousLed = -1;
}

LedSelector::~LedSelector() {}

void LedSelector::begin(void) {
  for (int i = 0; i < this->numLeds; i++) {
    pinMode(this->ledPins[i], OUTPUT);
    digitalWrite(this->ledPins[i], LOW);
  }
}

void LedSelector::select(int ledNum) {
  if (ledNum < 0 || ledNum >= this->numLeds) {
    return; // Invalid LED number
  }
  this->previousLed = this->currentLed;
  this->currentLed = ledNum;
  if (this->previousLed != this->currentLed) {
    if (this->previousLed != -1) {
      digitalWrite(this->ledPins[this->previousLed], LOW);
    }
  }
  if (this->currentLed != -1) {
    digitalWrite(this->ledPins[this->currentLed], HIGH); // Turn on new LED
  }
}