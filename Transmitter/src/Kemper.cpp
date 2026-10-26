#include "Kemper.hpp"
#include "SerialPrintf.hpp"
#include <esp_now.h>

const String Kemper::predefRowNames[KEMPER_MAX_PREDEF_ROW_NAMES] = {
    "1.E_DiSt",
    "2.EbDiSt",
    "3.E_Cln",
    "4.E_bLS",
};

Kemper::Kemper(uint8_t *peerAddr, LedSelector &leds, LedDisplay &display)
    : col(0), row(0), peerAddr(peerAddr), leds(leds), display(display) {}

Kemper::~Kemper() {}

void Kemper::begin(void) {
  leds.begin(); // Initialize the LED selector

  display.begin();

  this->sendProgramChange();
}

void Kemper::sendProgramChange(void) {
  int program = row * 5 + col;
  int bank = 1; // Assuming bank is always 1 for this example
  static char unpredefinedRowNameBuffer[9] = {' ', ' ', ' ', ' ', ' ',
                                              ' ', ' ', ' ', NULL};

  serial_printf("row: %d, col: %d\n", row, col);
  serial_printf("Program Change: %d, Bank: %d\n", program, bank);

  uint8_t msg[2] = {(uint8_t)program, (uint8_t)bank};

  esp_err_t result = esp_now_send(peerAddr, (const uint8_t *)msg, sizeof(msg));

  if (result == ESP_OK) {
    serial_printf("Sent Program %d, Bank %d\n", msg[0], msg[1]);
    this->leds.select(col); // Select the LED corresponding to the column

    if (row < KEMPER_MAX_PREDEF_ROW_NAMES &&
        Kemper::predefRowNames[row] != NULL) {
      serial_printf("Display pre-defined row name %s\n",
                    Kemper::predefRowNames[row]);
      this->display.showString(Kemper::predefRowNames[row]);
    } else {
      unpredefinedRowNameBuffer[0] = '1' + row;
      serial_printf("Display row index %c\n", unpredefinedRowNameBuffer[0]);
      this->display.showString(unpredefinedRowNameBuffer);
    }
  } else {
    serial_printf("Failed to send a message...\n");
    this->display.showString("Error0");
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
