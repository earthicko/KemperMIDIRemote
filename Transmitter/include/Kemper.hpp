#ifndef KEMPER_HPP
#define KEMPER_HPP

#include "LedDisplay.hpp"
#include "LedSelector.hpp"

#define KEMPER_MAX_PREDEF_ROW_NAMES 16

class Kemper {
public:
  Kemper(uint8_t *peerAddr, LedSelector &leds, LedDisplay &display);

  ~Kemper();

  void begin(void);
  void incrementRow(int increment);
  void changeCol(int newCol);

private:
  static const String predefRowNames[KEMPER_MAX_PREDEF_ROW_NAMES];
  uint8_t *peerAddr;
  LedSelector &leds;
  LedDisplay &display;
  int col;
  int row;

  void sendProgramChange(void);
};
#endif
