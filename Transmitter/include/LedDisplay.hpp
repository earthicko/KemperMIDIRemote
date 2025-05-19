#ifndef LEDDISPLAY_HPP
#define LEDDISPLAY_HPP

#include <LedControl.h>

#define DISPLAY_REFRESH_DELAY 50

/*
     A
    ---
F |     | B
  |  G  |
    --- |
E |     | C
  |  D  |
    ---   . DP

DP A B C D E F G
*/

#define DISPLAY_UPPER_E 0b01001111
#define DISPLAY_LOWER_R 0b00000101
#define DISPLAY_LOWER_O 0b00011101

class LedDisplay {
public:
  // Constructor
  LedDisplay(LedControl &lc);

  // Destructor
  ~LedDisplay();

  void begin(void);
  void clear(int digit);
  void showInt(int value);
  void showString(String value);
  void setDigit(int digit, byte value);
  void setChar(int digit, char c);

private:
  LedControl &lc; // Reference to the LedControl object
};

#endif
