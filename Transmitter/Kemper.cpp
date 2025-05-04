#include "Kemper.hpp"
#include "SerialPrintf.hpp"

Kemper::Kemper(RH_ASK &wireless, LedSelector &leds, LedControl &display)
    : col(0), row(0), wireless(wireless), leds(leds), display(display) {}

Kemper::~Kemper() {}

void Kemper::begin(void) {
  leds.begin(); // Initialize the LED selector

  if (!wireless.init())
    SerialPrintf("Wireless init failed\n");
  else
    SerialPrintf("Wireless init succeeded\n");

  this->sendProgramChange();
}

void Kemper::sendProgramChange(void) {
  int program = row * 5 + col;
  int bank = 1; // Assuming bank is always 1 for this example

  SerialPrintf("row: %d, col: %d\n", row, col);
  SerialPrintf("Program Change: %d, Bank: %d\n", program, bank);

  uint8_t msg[2] = {(uint8_t)program, (uint8_t)bank};

  if (this->wireless.send(msg, sizeof(msg))) {
    this->wireless.waitPacketSent();
    SerialPrintf("Sent Program %d, Bank %d\n", msg[0], msg[1]);
    this->leds.select(col); // Select the LED corresponding to the column
  } else {
    SerialPrintf("Failed to send a message...\n");
  }
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
