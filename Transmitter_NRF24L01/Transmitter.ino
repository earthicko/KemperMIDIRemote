#include <RF24.h>
#include <SPI.h>

#define pinCE 7
#define pinCSN 8

#define tunnel1 "PIPE1"
#define tunnel2 "PIPE2"

RF24 radio(pinCE, pinCSN);

const byte adresses[][6] = {tunnel1, tunnel2};

bool flagRadioBegin = false;

void setup() {
  Serial.begin(115200);
  // while (!Serial) {
  // }

  flagRadioBegin = radio.begin();

  if (flagRadioBegin) {
    Serial.println("Transmitter begin success");
  } else {
    Serial.println("Transmitter begin fail");
  }
  radio.openWritingPipe(adresses[0]);
  radio.openReadingPipe(1, adresses[1]);
  radio.setPALevel(RF24_PA_MIN);

  delay(2000);
}

void loop() {
  if (!flagRadioBegin) {
    return;
  }

  static int currentTime;
  static int response;

  radio.stopListening();
  Serial.println("Stop listening");
  currentTime = millis();
  radio.write(&currentTime, sizeof(currentTime));
  Serial.print("Wrote: ");
  Serial.println(currentTime);
  delay(5);

  radio.startListening();
  Serial.println("Start listening");
  while (!radio.available()) {
    Serial.println("Waiting for response");
    delay(1000);
    // wait
  }
  Serial.println("Got response");
  while (radio.available()) {
    radio.read(&response, sizeof(response));
    Serial.print("Read: ");
    Serial.println(response);
  }
  delay(25);
}