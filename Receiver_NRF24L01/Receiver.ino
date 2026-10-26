#include <RF24.h>
#include <SPI.h>

#define pinCE 7
#define pinCSN 8

#define tunnel1 "PIPE2"
#define tunnel2 "PIPE1"
/* ----- NOTEZ L'INVERSION de PIPE1 et PIPE2, entre celui-ci et l'autre montage
 * ! (car chaque tunnel véhicule les infos que dans un seul sens) */

RF24 radio(pinCE, pinCSN);

const byte adresses[][6] = {tunnel1, tunnel2};

bool flagRadioBegin = false;

void setup() {
  Serial.begin(115200);
  while (!Serial) {
  }

  flagRadioBegin = radio.begin();

  if (flagRadioBegin) {
    Serial.println("Receiver begin success");
  } else {
    Serial.println("Receiver begin fail");
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

  int readValue;
  int response;

  radio.startListening();
  Serial.println("Start listening");
  while (!radio.available()) {
    Serial.println("Waiting for response");
    delay(1000);
    // wait
  }

  Serial.println("Got response");
  while (radio.available()) {
    radio.read(&readValue, sizeof(readValue));
    Serial.print("Read: ");
    Serial.println(readValue);
  }
  delay(25);

  response = readValue * 2;

  radio.stopListening();
  Serial.println("Stop listening");
  radio.write(&response, sizeof(response));
  Serial.print("Wrote: ");
  Serial.println(response);
  delay(5);
}