#ifndef LEDSELECTOR_HPP
#define LEDSELECTOR_HPP

#define LEDSELECTOR_MAX_PINS 16

class LedSelector {
public:
  LedSelector(int numLeds, int *ledPins);
  ~LedSelector();

  void begin(void);
  void select(int ledNum);

private:
  int numLeds;                       // Number of LEDs
  int ledPins[LEDSELECTOR_MAX_PINS]; // Pin numbers for the LEDs
  int currentLed;                    // Currently selected LED
  int previousLed;                   // Previously selected LED
};

#endif
