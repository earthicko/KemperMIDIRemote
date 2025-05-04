// Buttons.cpp
#include "Buttons.hpp"
#include "SerialPrintf.hpp"

Button::Button() {}

Button::~Button() {}

void Button::begin(int pin) {
  SerialPrintf("Button::begin pin: %d\n", pin);
  this->pin = pin;
  this->state = BUTTONS_STATE_LIFTED;
  this->lastDebounceTime = 0;
  this->queueHead = 0;
  this->queueTail = 0;
  for (int i = 0; i < BUTTONS_QUEUESIZE; i++) {
    this->eventQueue[i] = -1; // Initialize event queue
  }
  pinMode(pin, INPUT_PULLUP);
}

void Button::update(void) {
  if (digitalRead(pin) == LOW) { // Pressed state
    switch (this->state) {
    case BUTTONS_STATE_LIFTED:
      // Start debounce for pressed state
      this->lastDebounceTime = millis();
      this->state = BUTTONS_STATE_LIFTED_TO_PRESSED;
      break;
    case BUTTONS_STATE_LIFTED_TO_PRESSED:
      // Debounce in progress
      if ((millis() - this->lastDebounceTime) > BUTTONS_DEBOUNCE_DELAY) {
        // Debounce complete, change state to pressed
        this->state = BUTTONS_STATE_PRESSED;
        this->enqueueEvent(BUTTONS_EVENT_PRESSED);
      }
      break;
    case BUTTONS_STATE_PRESSED_TO_LIFTED:
      // Reset state to pressed
      this->state = BUTTONS_STATE_PRESSED;
      break;
    case BUTTONS_STATE_PRESSED:
      // Do nothing, already pressed
      break;
    default:
      break;
    }
  } else { // Lifted state
    switch (this->state) {
    case BUTTONS_STATE_LIFTED:
      // Do nothing
      break;
    case BUTTONS_STATE_LIFTED_TO_PRESSED:
      // Reset state to lifted
      this->state = BUTTONS_STATE_LIFTED;
      break;
    case BUTTONS_STATE_PRESSED_TO_LIFTED:
      // Debounce in progress
      if ((millis() - this->lastDebounceTime) > BUTTONS_DEBOUNCE_DELAY) {
        // Debounce complete, change state to lifted
        this->state = BUTTONS_STATE_LIFTED;
        this->enqueueEvent(BUTTONS_EVENT_LIFTED);
      }
      break;
    case BUTTONS_STATE_PRESSED:
      // Start debounce for lifted state
      this->lastDebounceTime = millis();
      this->state = BUTTONS_STATE_PRESSED_TO_LIFTED;
      break;
    default:
      break;
    }
  }

#ifdef BUTTONS_DEBUG
  SerialPrintf("Pin %d: State: %d, Debounce Time: %lu\n", pin, this->state,
               this->lastDebounceTime);
#endif
}

int Button::dequeueEvent(void) {
  if (queueHead == queueTail) {
    return -1; // Queue is empty
  }
  int event = eventQueue[queueHead];
  queueHead = (queueHead + 1) % BUTTONS_QUEUESIZE;
#ifdef BUTTONS_DEBUG
  SerialPrintf("Dequeue event: %d\n", event);
#endif
  return event;
}

void Button::enqueueEvent(int event) {
#ifdef BUTTONS_DEBUG
  SerialPrintf("Enqueue event: %d\n", event);
#endif

  int nextTail = (queueTail + 1) % BUTTONS_QUEUESIZE;
  if (nextTail != queueHead) { // Check if queue is not full
    eventQueue[queueTail] = event;
    queueTail = nextTail;
  }
  // else: queue is full, drop the event (or handle overflow if needed)
}

// Constructor
Buttons::Buttons(int numButtons, int *buttonPins) {
  if (numButtons > BUTTONS_MAX_PINS) {
    numButtons = BUTTONS_MAX_PINS; // Limit to max pins
  }
  this->numButtons = numButtons;
  for (int i = 0; i < numButtons; i++) {
    this->buttonPins[i] = buttonPins[i];
  }
}

// Destructor
Buttons::~Buttons() {
  // Cleanup if necessary
}

void Buttons::begin(void) {
  // Initialize all buttons
  SerialPrintf("numButtons: %d\n", numButtons);
  for (int i = 0; i < this->numButtons; i++) {
    SerialPrintf("buttonPins[%d]: %d\n", i, this->buttonPins[i]);
    this->buttons[i].begin(this->buttonPins[i]);
  }
}

void Buttons::update(void) {
  // Update all buttons
  for (int i = 0; i < this->numButtons; i++) {
    this->buttons[i].update();
  }
}

void Buttons::dequeueEvent(int *pinNumReturn, int *eventReturn) {
  for (int i = 0; i < this->numButtons; i++) {
    int event = this->buttons[i].dequeueEvent();
    if (event != -1) {
      *pinNumReturn = this->buttonPins[i];
      *eventReturn = event;
      return; // Event found
    }
  }
  *pinNumReturn = -1; // Button not found
  *eventReturn = -1;  // No event
}