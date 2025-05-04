#ifndef BUTTONS_HPP
#define BUTTONS_HPP

// #define BUTTONS_DEBUG
#define BUTTONS_MAX_PINS 16
#define BUTTONS_QUEUESIZE 16
#define BUTTONS_DEBOUNCE_DELAY 50 // milliseconds

enum ButtonState {
  BUTTONS_STATE_LIFTED = 0,
  BUTTONS_STATE_PRESSED,
  BUTTONS_STATE_LIFTED_TO_PRESSED,
  BUTTONS_STATE_PRESSED_TO_LIFTED
};

enum ButtonEvent {
  BUTTONS_EVENT_PRESSED = 0,
  BUTTONS_EVENT_LIFTED,
};

class Button {
public:
  // Constructor
  Button();

  // Destructor
  ~Button();

  void begin(int pin);
  void update(void);
  int dequeueEvent(void); // Dequeue a button event

private:
  int pin;                           // Pin number for the button
  int state;                         // State of the button (pressed/lifted)
  unsigned long lastDebounceTime;    // Last time the button state was toggled
  int eventQueue[BUTTONS_QUEUESIZE]; // Queue for button events
  int queueHead;                     // Index to remove event
  int queueTail;                     // Index to insert event

  void enqueueEvent(int event); // Enqueue a button event
};

class Buttons {
public:
  // Constructor
  Buttons(int numButtons, int *buttonPins);

  // Destructor
  ~Buttons();

  void begin(void);
  void update(void);

  int dequeueEvent(int pinNum); // Dequeue a button event

private:
  int numButtons;                   // Number of buttons
  Button buttons[BUTTONS_MAX_PINS]; // Array of button objects
  int buttonPins[BUTTONS_MAX_PINS]; // Pin numbers for the buttons
};

#endif // BUTTONS_HPP