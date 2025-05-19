#include "Buttons.hpp"
#include "Kemper.hpp"
#include "LedDisplay.hpp"
#include "LedSelector.hpp"
#include "SerialPrintf.hpp"
#include <Arduino.h>
#include <WiFi.h>
#include <esp_now.h>

#define PIN_UP 4
#define PIN_DOWN 16
#define PIN_PERFORMANCE_1 17
#define PIN_PERFORMANCE_2 5
#define PIN_PERFORMANCE_3 18
#define PIN_PERFORMANCE_4 19
#define PIN_PERFORMANCE_5 21
#define PIN_LED_1 27
#define PIN_LED_2 26
#define PIN_LED_3 25
#define PIN_LED_4 33
#define PIN_LED_5 32
#define PIN_DISPLAY_DIN 15
#define PIN_DISPLAY_CS 13
#define PIN_DISPLAY_CLK 14

// REPLACE WITH THE MAC Address of your receiver
uint8_t peer_addr[] = {0x88, 0x13, 0xbf, 0x03, 0x07, 0xe8};
esp_now_peer_info_t peer_info;

int buttonIdxToPin[] = {
    PIN_UP,
    PIN_DOWN,
    PIN_PERFORMANCE_1,
    PIN_PERFORMANCE_2,
    PIN_PERFORMANCE_3,
    PIN_PERFORMANCE_4,
    PIN_PERFORMANCE_5,
};
int ledIdxToPin[] = {
    PIN_LED_1, PIN_LED_2, PIN_LED_3, PIN_LED_4, PIN_LED_5,
};

Buttons buttons = Buttons(sizeof(buttonIdxToPin) / sizeof(int), buttonIdxToPin);
LedSelector leds = LedSelector(sizeof(ledIdxToPin) / sizeof(int), ledIdxToPin);
LedControl lc = LedControl(PIN_DISPLAY_DIN, PIN_DISPLAY_CLK, PIN_DISPLAY_CS, 1);
LedDisplay display = LedDisplay(lc);
Kemper kemper = Kemper(peer_addr, leds, display);

// Callback when data is sent
void on_data_sent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  serial_printf("Last Packet Send Status:\t%s\n",
                status == ESP_NOW_SEND_SUCCESS ? "Success" : "Fail");
}

// Callback when data is received
void on_data_recv(const uint8_t *mac, const uint8_t *incomingData, int len) {
  serial_printf("Bytes received: %d, content: ", len);
  for (int i = 0; i < len; i++) {
    serial_printf("%d", incomingData[i]);
    if (i == len - 1) {
      serial_printf("\n");
    } else {
      serial_printf(" ");
    }
  }
}

void setup() {
  // Init Serial Monitor
  Serial.begin(115200);

  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    serial_printf("Error initializing ESP-NOW\n");
    while (1) {
    }
  }

  buttons.begin();
  kemper.begin();

  // Once ESPNow is successfully Init, we will register for Send CB to
  // get the status of Trasnmitted packet
  esp_now_register_send_cb(on_data_sent);

  // Register peer
  memcpy(peer_info.peer_addr, peer_addr, 6);
  peer_info.channel = 0;
  peer_info.encrypt = false;

  // Add peer
  if (esp_now_add_peer(&peer_info) != ESP_OK) {
    serial_printf("Failed to add peer\n");
    return;
  }
  // Register for a callback function that will be called when data is received
  esp_now_register_recv_cb(esp_now_recv_cb_t(on_data_recv));
}

void loop() {
  buttons.update();

  int pinNum = -1;
  int event = -1;

  buttons.dequeueEvent(&pinNum, &event);
  if (event == BUTTONS_EVENT_PRESSED) {
    switch (pinNum) {
    case PIN_UP:
      serial_printf("UP PRESSED\n");
      kemper.incrementRow(1);
      break;
    case PIN_DOWN:
      serial_printf("DOWN PRESSED\n");
      kemper.incrementRow(-1);
      break;
    case PIN_PERFORMANCE_1:
      serial_printf("1 PRESSED\n");
      kemper.changeCol(0);
      break;
    case PIN_PERFORMANCE_2:
      serial_printf("2 PRESSED\n");
      kemper.changeCol(1);
      break;
    case PIN_PERFORMANCE_3:
      serial_printf("3 PRESSED\n");
      kemper.changeCol(2);
      break;
    case PIN_PERFORMANCE_4:
      serial_printf("4 PRESSED\n");
      kemper.changeCol(3);
      break;
    case PIN_PERFORMANCE_5:
      serial_printf("5 PRESSED\n");
      kemper.changeCol(4);
      break;
    }
  }
}