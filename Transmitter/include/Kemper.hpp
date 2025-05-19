#ifndef KEMPER_HPP
#define KEMPER_HPP

#include "LedDisplay.hpp"
#include "LedSelector.hpp"

class Kemper {
public:
  Kemper(uint8_t *peerAddr, LedSelector &leds, LedDisplay &display);

  ~Kemper();

  void begin(void);
  void incrementRow(int increment);
  void changeCol(int newCol);

private:
  uint8_t *peerAddr;
  LedSelector &leds;
  LedDisplay &display;
  int col;
  int row;

  void sendProgramChange(void);
};
#endif
