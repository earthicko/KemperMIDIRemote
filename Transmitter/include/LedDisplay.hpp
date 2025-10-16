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

#define DISPLAY_UPPER_A 0b01110111
#define DISPLAY_UPPER_C 0b01001110
#define DISPLAY_UPPER_E 0b01001111
#define DISPLAY_UPPER_F 0b01000111
#define DISPLAY_UPPER_G 0b01011110
#define DISPLAY_UPPER_H 0b00110111
#define DISPLAY_UPPER_I 0b00000110
#define DISPLAY_UPPER_J 0b00111100
#define DISPLAY_UPPER_L 0b00001110
#define DISPLAY_UPPER_O 0b01111110
#define DISPLAY_UPPER_P 0b01100111
#define DISPLAY_UPPER_S 0b01011011
#define DISPLAY_UPPER_U 0b00111110
#define DISPLAY_UPPER_Y 0b00111001

#define DISPLAY_LOWER_B 0b00011111
#define DISPLAY_LOWER_C 0b00001101
#define DISPLAY_LOWER_D 0b00111101
#define DISPLAY_LOWER_G 0b01111011
#define DISPLAY_LOWER_H 0b00010111
#define DISPLAY_LOWER_I 0b00000100
#define DISPLAY_LOWER_J 0b00011000
#define DISPLAY_LOWER_L 0b00000110
#define DISPLAY_LOWER_N 0b00010101
#define DISPLAY_LOWER_O 0b00011101
#define DISPLAY_LOWER_R 0b00000101
#define DISPLAY_LOWER_T 0b00001111
#define DISPLAY_LOWER_U 0b00011100

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
