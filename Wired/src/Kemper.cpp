#include "Kemper.hpp"
#include "SerialPrintf.hpp"

Kemper::Kemper(
    midi::MidiInterface<midi::SerialMIDI<HardwareSerial>> &serialMIDI)
    : col(0), row(0), serialMIDI(serialMIDI) {}

Kemper::~Kemper() {}

void Kemper::begin(void) { this->sendProgramChange(); }

void Kemper::sendProgramChange(void) {
  int program = row * 5 + col;
  int bank = 1; // Assuming bank is always 1 for this example
  serial_printf("row: %d, col: %d\n", row, col);
  serial_printf("Program Change: %d, Bank: %d\n", program, bank);
  serialMIDI.sendProgramChange(program, bank);
}

void Kemper::incrementRow(int increment) {
  if (!(increment == 1 || increment == -1)) {
    return; // Only allow increment or decrement
  }
  row += increment;
  if (row < 0) {
    row = 0;
  }
  sendProgramChange();
}

void Kemper::changeCol(int newCol) {
  if (newCol < 0 || newCol >= 5) {
    return; // Only allow columns 0 to 4
  }
  col = newCol;
  sendProgramChange();
}
