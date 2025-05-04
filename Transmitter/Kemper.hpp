#ifndef KEMPER_HPP
#define KEMPER_HPP

#include "LedSelector.hpp"
#include <LedControl.h>
#include <RH_ASK.h>

class Kemper {
public:
  // Constructor
  Kemper(RH_ASK &wireless, LedSelector &leds, LedControl &display);

  // Destructor
  ~Kemper();

  void begin(void);
  void incrementRow(int increment);
  void changeCol(int newCol);

private:
  RH_ASK &wireless;
  LedSelector &leds;
  LedControl &display;
  int col;
  int row;

  void sendProgramChange(void);
};
#endif
