#ifndef KEMPER_HPP
#define KEMPER_HPP

#include <MIDI.h>

class Kemper {
public:
  Kemper(midi::MidiInterface<midi::SerialMIDI<HardwareSerial>> &serialMIDI);

  ~Kemper();

  void begin(void);
  void incrementRow(int increment);
  void changeCol(int newCol);

private:
  midi::MidiInterface<midi::SerialMIDI<HardwareSerial>> &serialMIDI;
  int col;
  int row;

  void sendProgramChange(void);
};
#endif
