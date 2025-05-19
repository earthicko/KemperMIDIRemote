#include "SerialPrintf.hpp"
#include <Arduino.h>
#include <MIDI.h>
#include <WiFi.h>
#include <esp_now.h>

// Use UART2 instead on pins 16/17
MIDI_CREATE_INSTANCE(HardwareSerial, Serial2, midiSw1); // UART 2 (GPIO 16/17)

// REPLACE WITH THE MAC Address of your receiver
uint8_t broadcast_addr[] = {0x88, 0x13, 0xbf, 0x03, 0x3a, 0xfc};
esp_now_peer_info_t peer_info;

// Callback when data is sent
void on_data_sent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  serial_printf("\r\nLast Packet Send Status:\t%s\n",
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

  if (len == 2) {
    serial_printf("Program %d, Bank %d\n", incomingData[0], incomingData[1]);
    midiSw1.sendProgramChange(incomingData[0], incomingData[1]);
  } else {
    serial_printf("Unknown format\n");
  }
}

void setup() {
  // Init Serial Monitor
  Serial.begin(115200);

  // This listens to all MIDI channels
  midiSw1.begin(MIDI_CHANNEL_OMNI);
  // Need to re-initialise serial port with correct pins.
  // MIDI will call Serial2.begin, but with no specific pin
  // settings, so the ESP32 core will use some defaults,
  // which aren't the same as what we need...
  Serial2.begin(31250, SERIAL_8N1, 16, 17);

  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    serial_printf("Error initializing ESP-NOW\n");
    while (1) {
    }
  }

  // Once ESPNow is successfully Init, we will register for Send CB to
  // get the status of Trasnmitted packet
  esp_now_register_send_cb(on_data_sent);

  // Register peer
  memcpy(peer_info.peer_addr, broadcast_addr, 6);
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

void loop() {}